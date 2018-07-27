#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <h3Index.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include <pyh3index.h>
#include <pygeocoord.h>
#include <pygeoboundary.h>


static PyObject *
PyH3Index_get_resolution(PyH3IndexObject *self)
{
    H3Index h3;
    int resolution;
    h3 = self->ob_val;

    resolution = H3_EXPORT(h3GetResolution)(h3);
    return PyLong_FromLong((long)resolution);
}

static PyObject *
PyH3Index_get_base_cell(PyH3IndexObject *self)
{
    H3Index h3;
    int base_cell;
    h3 = self->ob_val;

    base_cell = H3_EXPORT(h3GetBaseCell)(h3);
    return PyLong_FromLong((long)base_cell);
}

static PyObject *
PyH3Index_to_boundary(PyH3IndexObject *self)
{
    H3Index h3;
    GeoBoundary *gp;
    PyGeoBoundaryObject *boundary;

    boundary = PyGeoBoundary_New();
    assert(boundary != NULL);

    /* TODO: NULL check */
    gp = PyGeoBoundary_AS_GeoBoundary(boundary);
    h3 = PyH3Index_AS_H3Index(self);
    H3_EXPORT(h3ToGeoBoundary)(h3, gp);

    return (PyObject *)boundary;
}


static PyObject *
PyH3Index_from_string(PyObject *unused, PyObject *args)
{
    PyObject *u_str, *arglist;
    H3Index h3;
    const char *str;

    if(!PyArg_ParseTuple(args, "O", &u_str)) {
        PyErr_SetString(PyExc_ValueError,
            "Error parsing argument. Acceptable argument type is 'str'.");
        return NULL;
    }

    if (!PyUnicode_Check(u_str)) {
        PyErr_SetString(PyExc_TypeError,
            "Expected input argument of type 'str'.");
        return NULL;
    }
    str = PyUnicode_AsUTF8(u_str);
    h3 = H3_EXPORT(stringToH3)(str);

    arglist = Py_BuildValue("(l)", (long)h3);
    PyObject *ob = PyObject_CallObject((PyObject *)&PyH3Index_Type, arglist);
    if (ob == NULL) {
        PyErr_Format(PyExc_ValueError,
                     "Cannot create h3py.H3Index from string '%s'.", str);
        return NULL;
    }
    return ob;

}

static PyObject *
PyH3Index_to_string(PyH3IndexObject *self, PyObject *args)
{
    char buff[17];
    H3_EXPORT(h3ToString)(self->ob_val, buff, 17);

    return PyUnicode_FromString((const char *)buff);
}

static PyObject *
PyH3Index_is_valid(PyH3IndexObject *self)
{
    H3Index h3;
    int is_valid;

    h3 = self->ob_val;
    is_valid = H3_EXPORT(h3IsValid)(h3);

    return PyBool_FromLong((long)is_valid);
}

PyObject *
PyH3Index_to_geocoord(PyH3IndexObject *self)
{
    PyGeoCoordObject *coord;
    PyObject *arglist;
    GeoCoord *g;
    H3Index h3;


    arglist = Py_BuildValue("dd", 0.0, 0.0);
    coord = (PyGeoCoordObject *)PyObject_CallObject((PyObject *)&PyGeoCoord_Type,
                                                     arglist);
    g = coord->ob_gval; /* weak reference */
    h3 = self->ob_val;
    H3_EXPORT(h3ToGeo)(h3, g); /* should mutate PyObject */

    Py_DECREF(arglist);
    return (PyObject *)coord;

}

/*
static PyObject *
PyH3Index_hex_ring(PyH3IndexObject *self, PyObject *args, PyObject *kwds)
{
    H3Index origin;
    int k;
    PyObject *h3_out;

    Py_INCREF(Py_None);
    return Py_None;
}
*/

static PyObject *
PyH3Index_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyH3IndexObject *self;
    H3Index ob_val;

    if (type != &PyH3Index_Type) {
        PyErr_SetString(PyExc_TypeError,
            "Incorrect input type. Expected H3Index.");
        return NULL;
    }

    self = (PyH3IndexObject *)type->tp_alloc(type, 0);
    ob_val = 0;

    self->ob_val = ob_val;
    return (PyObject *)self;
}

static int
PyH3Index_init(PyH3IndexObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"index", NULL};
    PyObject *index = NULL;
    long h3_index = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &index)) {
        return -1;
    }

    if (PyLong_Check(index)) {
        h3_index = PyLong_AsLong(index);
    }
    else if (PyH3Index_Check(index)) {
        h3_index = (long)((PyH3IndexObject *)index)->ob_val;
    }
    else {
        PyErr_SetString(PyExc_TypeError,
            "Input argument 'index' must be an integer");
    }

    if (h3_index <= 0) {
        PyErr_SetString(PyExc_ValueError,
            "Invalid h3 value. Index must be > 0.");
        return -1;
    }

    self->ob_val = (H3Index)h3_index;
    return 0;
}

static void
PyH3Index_dealloc(PyH3IndexObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
PyH3Index_richcompare(PyObject *self, PyObject *other, int op)
{
    H3Index h3_self, h3_other;
    PyObject *result = NULL;

    if (!PyH3Index_Check(self)) {
        // noop?
        PyErr_SetString(PyExc_TypeError,
            "Input argument 'self' must of of type h3py.H3Index.");
        return NULL;
    }
    else {
        h3_self = ((PyH3IndexObject *)self)->ob_val;
    }

    if (PyLong_Check(other)) {
        h3_other = (H3Index)PyLong_AsLong(other);
    }
    else if (PyH3Index_Check(other)) {
        h3_other = ((PyH3IndexObject *)other)->ob_val;
    }
    else {
        PyErr_SetString(PyExc_TypeError,
            "rhs argument must be one of {int, h3py.H3Index}.");
        return NULL;
    }

    switch (op) {
      case Py_EQ:
          result =  h3_self == h3_other ? Py_True : Py_False;
          break;
      case Py_NE:
          result =  h3_self == h3_other ? Py_False : Py_True;
          break;
      case Py_GT:
      case Py_GE:
      case Py_LT:
      case Py_LE:
          PyErr_SetString(PyExc_NotImplementedError,
              "Operators {<=, <, >, >=} not implemented for type h3py.H3Index.");
          break;
    }
    Py_XINCREF(result);
    return result;
}

static PyObject *
PyH3Index_repr(PyH3IndexObject *self)
{
    return PyUnicode_FromFormat("H3Index: %ld", self->ob_val);
}

/* not used */
static PyObject *
PyH3Index_getindex(PyH3IndexObject *self, void *closure)
{
    long index;
    if (!PyH3Index_Check(self)) {
        PyErr_SetString(PyExc_TypeError,
            "Invalid type. Expected 'H3Index'.");
        return NULL;
    }

    index = (long)self->ob_val;
    return PyLong_FromLong(index);
}

/* not used */
static int
PyH3Index_setindex(PyH3IndexObject *self, PyObject *value, void *closure)
{
    if (PyLong_Check(value)) {
        self->ob_val = (H3Index)PyLong_AsLong(value);
        return 0;
    }
    else if (PyH3Index_Check(value)) {
        self->ob_val = ((PyH3IndexObject *)value)->ob_val;
        return 0;
    }
    else {
        PyErr_SetString(PyExc_TypeError,
            "Invalid argument 'value'. Valid types are {int, H3Index}.");
        return -1;
    }
}

static PyMemberDef PyH3Index_members[] = {
    {"value", T_ULONGLONG, offsetof(PyH3IndexObject, ob_val), READONLY,
     "integer representation of H3Index object"},
    {NULL}
};

static PyMethodDef PyH3Index_methods[] = {
    {"get_resolution", (PyCFunction)PyH3Index_get_resolution,
    METH_NOARGS, "returns the H3 resolution of an H3 index."},
    {"base_cell", (PyCFunction)PyH3Index_get_base_cell,
    METH_NOARGS, ""},
    {"from_string", (PyCFunction)PyH3Index_from_string,
    METH_VARARGS | METH_STATIC, ""},
    {"to_string", (PyCFunction)PyH3Index_to_string,
    METH_VARARGS, ""},
    {"to_boundary", (PyCFunction)PyH3Index_to_boundary,
    METH_VARARGS, ""},
    {"to_coord", (PyCFunction)PyH3Index_to_geocoord,
    METH_NOARGS, "find the lat/lon center point g of the cell h3."},
    {"is_valid", (PyCFunction)PyH3Index_is_valid,
    METH_NOARGS, "returns whether or not an H3 index is valid."},
    {NULL}
};

PyTypeObject PyH3Index_Type = {
     PyVarObject_HEAD_INIT(NULL, 0)
     "h3py.H3Index",
     sizeof(PyH3IndexObject),
     0,
     (destructor)PyH3Index_dealloc,              /* tp_dealloc */
     0,                                          /* tp_print */
     0,                                          /* tp_getattr */
     0,                                          /* tp_setattr */
     0,                                          /* tp_reserved */
     (reprfunc)PyH3Index_repr,                   /* tp_repr */
     0,                                          /* tp_as_number */
     0,                                          /* tp_as_sequence */
     0,                                          /* tp_as_mapping */
     0,                                          /* tp_hash */
     0,                                          /* tp_call */
     (reprfunc)PyH3Index_repr,                   /* tp_str */
     PyObject_GenericGetAttr,                    /* tp_getattro */
     0,                                          /* tp_setattro */
     0,                                          /* tp_as_buffer */
     Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
     "H3Index object",                           /* tp_doc */
     0,                                          /* tp_traverse */
     0,                                          /* tp_clear */
     PyH3Index_richcompare,                      /* tp_richcompare */
     0,                                          /* tp_weaklistoffset */
     0,                                          /* tp_iter */
     0,                                          /* tp_iternext */
     PyH3Index_methods,                          /* tp_methods */
     PyH3Index_members,                          /* tp_members */
     0,                                          /* tp_getset */
     0,                                          /* tp_base */
     0,                                          /* tp_dict */
     0,                                          /* tp_descr_get */
     0,                                          /* tp_descr_set */
     0,                                          /* tp_dictoffset */
     (initproc)PyH3Index_init,                   /* tp_init */
     0,                                          /* tp_alloc */
     PyH3Index_new,                              /* tp_new */
};
