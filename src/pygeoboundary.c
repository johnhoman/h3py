#include <Python.h>
#include <pygeoboundary.h>
#include <pygeocoord.h>
#include <h3api.h>
#include <constants.h>
#include <geoCoord.h>


PyGeoBoundaryObject *
PyGeoBoundary_New(void)
{
    return (PyGeoBoundaryObject *)PyGeoBoundary_new(&PyGeoBoundary_Type, NULL, NULL);
}

static PyObject *
PyGeoBoundary_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyGeoBoundaryObject *self;
    GeoBoundary *ob_val;

    if (type != &PyGeoBoundary_Type) {
        PyErr_SetString(PyExc_TypeError, "Incorrect input type.");
        return NULL;
    }

    self = PyGeoBoundary_Allocate(type);
    ob_val = PyMem_RawMalloc(sizeof(GeoBoundary));

    if (ob_val == NULL) {
        PyErr_SetString(PyExc_MemoryError,
            "Failed to allocate memory for h3py.GeoBoundary");
        return NULL;
    }
    ob_val->numVerts = 0;
    PyGeoBoundary_GetGeoBoundary(self) = ob_val;

    return PyGeoBoundary_AsPyObject(self);
}

static int
PyGeoBoundary_init(PyGeoBoundaryObject *self, PyObject *args, PyObject *kwds)
{
    PyObject *verts = NULL;
    int num_verts = 0;

    char *kwlist[] = {"verts", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &verts)) {
        PyErr_SetString(PyExc_TypeError,
            "Count not parse arguments for h3py.GeoBoundary.");
        return -1;
    }

    if (!PyList_Check(verts)) {
        PyObject *type = PyObject_Repr(PyObject_Type(verts));
        PyErr_SetString(PyExc_TypeError,
            PyUnicode_AsUTF8(PyUnicode_FromFormat(
                                 "Expected input argument of type 'list'"
                                 " but instead got type '%S'.", type)));
        return -1;
    }

    num_verts = (int)PyList_Size(verts);
    if (num_verts > MAX_CELL_BNDRY_VERTS || num_verts < 1) {
        PyErr_SetString(PyExc_ValueError,
            "Input argument 'verts' has at most 10 elements");
        return -1;
    }

    PyObject *elem;
    PyGeoCoordObject *py_elem;
    GeoCoord *ob_val;
    Py_ssize_t index;
    for (index = 0; index < (Py_ssize_t)num_verts; ++index) {
        elem = PyList_GetItem(verts, index);
        if (!PyGeoCoord_Check(elem)) {
            PyErr_SetString(PyExc_TypeError,
                "GeoBoundary verts must be of type 'h3py.GeoCoord'.");
            return -1;
        }
        else {
            py_elem = (PyGeoCoordObject *)elem;
            ob_val = (GeoCoord *)py_elem->ob_gval;
        }
        PyGeoBoundary_GetVert(self, index) = *ob_val;
    }
    PyGeoBoundary_GetNumberVerts(self) = num_verts;
    return 0;
}

static void
PyGeoBoundary_dealloc(PyGeoBoundaryObject *self)
{
    PyMem_RawFree(self->ob_val);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
PyGeoBoundary_getverts(PyGeoBoundaryObject *self)
{
    PyObject *ob_verts;
    GeoBoundary *ob_val;
    int n_verts;

    ob_val = PyGeoBoundary_GetGeoBoundary(self);
    n_verts = ob_val->numVerts;
    ob_verts = PyList_New((Py_ssize_t)n_verts);

    PyObject *ob, *args;
    GeoCoord coord;
    Py_ssize_t index;
    for (index = 0; index < n_verts; ++index) {
        coord = ob_val->verts[index];
        args = Py_BuildValue("(dd)", coord.lat, coord.lon);
        if (args == NULL)
            goto Fail;
        ob = PyObject_CallObject((PyObject *)&PyGeoCoord_Type, args);
        if (ob == NULL)
            goto Fail;
        if (PyList_SetItem(ob_verts, index, ob) < 0)
            goto Fail;
    }

    return ob_verts;

    Fail: {
        PyErr_SetString(PyExc_NotImplementedError,
            "Unknown error occured in function GeoBoundary.verts(inst)");
        return NULL;
    }

}

static PyObject *
PyGeoBoundary_repr(PyGeoBoundaryObject *self)
{
    PyObject *pprint, *pformat, *repr, *verts, *coord;
    pprint = PyImport_ImportModuleNoBlock("pprint");
    Py_INCREF(pprint);
    pformat = PyObject_GetAttrString(pprint, "pformat");
    Py_INCREF(pprint);
    if (!PyCallable_Check(pformat)) {
        PyErr_BadInternalCall();
        return NULL;
    }

    verts = PyGeoBoundary_getverts(self);
    repr = PyObject_Call(pformat, Py_BuildValue("(O)", verts), NULL);
    Py_ssize_t i;
    for (i = 0; i < PyList_Size(verts); ++i) {
        coord = PyList_GetItem(verts, i);
        Py_DECREF(coord);
    }
    Py_DECREF(verts);
    Py_DECREF(pprint);
    Py_DECREF(pformat);
    return repr;
}

static PyObject *
PyGeoBoundary___eq__(PyObject *self, PyObject *other)
{
    PyObject *result;
    GeoBoundary *bound_self, *bound_other;

    if (!PyGeoBoundary_Check(self) || !PyGeoBoundary_Check(other)) {
        result = Py_False;
    }
    bound_self = PyGeoBoundary_AS_GeoBoundary((PyGeoBoundaryObject *)self);
    bound_other = PyGeoBoundary_AS_GeoBoundary((PyGeoBoundaryObject *)other);

    result = Py_True;
    if (bound_self->numVerts != bound_other->numVerts) {
        result = Py_False;
    }

    int i;
    GeoCoord geo_self, geo_other;
    for (i = 0; i < bound_self->numVerts; ++i) {
        geo_self = bound_self->verts[i];
        geo_other = bound_other->verts[i];
        if (!geoAlmostEqual(&geo_self, &geo_other)) {
            result = Py_False;
        }
    }

    Py_INCREF(result);
    return result;

}

static PyObject *
PyGeoBoundary_richcompare(PyObject *self, PyObject *other, int op)
{

  PyObject *result;
  switch (op) {
    case Py_EQ:
        result =  PyGeoBoundary___eq__(self, other) ? Py_True: Py_False;
        break;
    case Py_NE:
        result =  PyGeoBoundary___eq__(self, other) ? Py_False: Py_True;
        break;
    case Py_GT:
    case Py_GE:
    case Py_LT:
    case Py_LE:
        result = Py_False;
        break;
  }
  Py_XINCREF(result);
  return result;
}

static PyGetSetDef PyGeoBoundary_getseters[] = {
    {NULL}
};

static PyMemberDef PyGeoBoundary_members[] = {
     {NULL}
};

static PyMethodDef PyGeoBoundary_methods[] = {
     {"verts", (PyCFunction)PyGeoBoundary_getverts, METH_NOARGS,
               "Returns list of GeoCoord objects"},
     {NULL}
};

PyTypeObject PyGeoBoundary_Type = {
     PyVarObject_HEAD_INIT(NULL, 0)
     "h3py.GeoBoundary",
     sizeof(PyGeoBoundaryObject),
     0,
     (destructor)PyGeoBoundary_dealloc,          /* tp_dealloc */
     0,                                          /* tp_print */
     0,                                          /* tp_getattr */
     0,                                          /* tp_setattr */
     0,                                          /* tp_reserved */
     (reprfunc)PyGeoBoundary_repr,               /* tp_repr */
     0,                                          /* tp_as_number */
     0,                                          /* tp_as_sequence */
     0,                                          /* tp_as_mapping */
     0,                                          /* tp_hash */
     0,                                          /* tp_call */
     0,                                          /* tp_str */
     PyObject_GenericGetAttr,                    /* tp_getattro */
     0,                                          /* tp_setattro */
     0,                                          /* tp_as_buffer */
     Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
     "GeoObject object",                         /* tp_doc */
     0,                                          /* tp_traverse */
     0,                                          /* tp_clear */
     PyGeoBoundary_richcompare,                  /* tp_richcompare */
     0,                                          /* tp_weaklistoffset */
     0,                                          /* tp_iter */
     0,                                          /* tp_iternext */
     PyGeoBoundary_methods,                      /* tp_methods */
     PyGeoBoundary_members,                      /* tp_members */
     PyGeoBoundary_getseters,                    /* tp_getset */
     0,                                          /* tp_base */
     0,                                          /* tp_dict */
     0,                                          /* tp_descr_get */
     0,                                          /* tp_descr_set */
     0,                                          /* tp_dictoffset */
     (initproc)PyGeoBoundary_init,               /* tp_init */
     0,                                          /* tp_alloc */
     PyGeoBoundary_new,                          /* tp_new */
};
