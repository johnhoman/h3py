#include <Python.h>
#include <pygeoboundary.h>
#include <pygeocoord.h>


static PyObject *
PyGeoBoundary_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyGeoBoundaryObject *self;
    GeoBoundary *ob_val;

    if (type != &PyGeoBoundary_Type) {
        PyErr_SetString(PyExc_TypeError, "Incorrect input type.");
        return NULL;
    }

    self = (PyGeoBoundaryObject *)type->tp_alloc(type, 0);

    ob_val = (GeoBoundary *)PyMem_RawMalloc(sizeof(GeoBoundary));

    if (ob_val == NULL) {
        PyErr_SetString(PyExc_MemoryError,
            "Failed to allocate memory for h3py.GeoBoundary");
        return NULL;
    }
    ob_val->numVerts = 0;
    self->ob_val = ob_val;

    return (PyObject *)self;
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
        elem = PyList_GetItem(verts, 0);
        if (!PyGeoCoord_Check(elem)) {
            PyErr_SetString(PyExc_TypeError,
                "GeoBoundary verts must be of type 'h3py.GeoCoord'.");
            return -1;
        } else {
            py_elem = (PyGeoCoordObject *)elem;
            ob_val = (GeoCoord *)py_elem->ob_gval;
        }
        self->ob_val->verts[index] = *ob_val;
    }
    self->ob_val->numVerts = num_verts;
    return 0;
}

static void
PyGeoBoundary_dealloc(PyGeoBoundaryObject *self)
{
    PyMem_RawFree(self->ob_val);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
PyGeoBoundary_getverts(PyGeoBoundaryObject *self, void *closure)
{
    PyObject *ob_verts;
    GeoBoundary *ob_val;
    int n_verts;

    ob_val = self->ob_val;
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
    return PyObject_Repr(PyGeoBoundary_getverts(self, NULL));
}

static PyGetSetDef PyGeoBoundary_getseters[] = {
    {"verts", (getter)PyGeoBoundary_getverts, 0,
     "GeoBoundary vertices", NULL},
    {NULL}
};

static PyMemberDef PyGeoBoundary_members[] = {
     {NULL}
};

static PyMethodDef PyGeoBoundary_methods[] = {
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
     0,                                          /* tp_richcompare */
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
