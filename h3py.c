#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <stddef.h>
#include <stdio.h>


#ifndef Py_H3_H
#define Py_H3_H
#ifdef __cpluscplus
extern "C" {
#endif


PyAPI_DATA(PyTypeObject) PyGeoCoord_Type;

#define PyGeoCoord_Check(x) (Py_TYPE(x) == &PyGeoCoord_Type)
#define PyGeoCoord_Lat(x) (x)->ob_gval->lat
#define PyGeoCoord_Lon(x) (x)->ob_gval->lon

typedef struct {
    PyObject_HEAD
    GeoCoord *ob_gval;
} PyGeoCoordObject;

/* forward declaration */
static PyObject *
geo_to_h3(PyGeoCoordObject *self, PyObject *args)
{
    GeoCoord *ob_gval;
    H3Index index;
    int res;

    if (!PyArg_ParseTuple(args, "i", &res)) {
        // TODO: handle error case
        return NULL;
    }

    ob_gval = self->ob_gval;
    index = H3_EXPORT(geoToH3)(ob_gval, res);

    return PyLong_FromLong((long)index);
}


static PyObject *
PyGeoCoord_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyGeoCoordObject *self;
    GeoCoord *ob_gval;

    if (type != &PyGeoCoord_Type) {
        PyErr_SetString(PyExc_TypeError,
            "Incorrect input type");
        goto error;
    }

    self = (PyGeoCoordObject *)type->tp_alloc(type, 0);

    ob_gval = (GeoCoord *)PyMem_RawMalloc(sizeof(GeoCoord));
    if (ob_gval == NULL) {
        PyErr_SetString(PyExc_MemoryError,
            "Could not allocate memory for h3py.GeoCoord.");
        goto error;
    }
    ob_gval->lat = 0.0;
    ob_gval->lon = 0.0;
    self->ob_gval = ob_gval;

    return (PyObject *)self;


    error: {
        return NULL;
    }
}

static int
PyGeoCoord_init(PyGeoCoordObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"lat", "lon", NULL};
    double lat, lon;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "dd", kwlist, &lat, &lon)) {
        return -1;
    }

    PyGeoCoord_Lat(self) = lat;
    PyGeoCoord_Lon(self) = lon;
    // PyGeoCoord_Lat(self) = lat;
    // PyGeoCoord_Lon(self) = lon;
    return 0;
}

void PyGeoCoord_dealloc(PyGeoCoordObject *self)
{
    GeoCoord *ob_gval;

    ob_gval = self->ob_gval;
    PyMem_RawFree(ob_gval);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
PyGeoCoord_getlat(PyGeoCoordObject *self, void *closure)
{
    PyObject *ob;
    ob = PyFloat_FromDouble(self->ob_gval->lat);
    Py_INCREF(ob);
    return ob;
}

static int
PyGeoCoord_setlat(PyGeoCoordObject *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Invalid argument 'value'");
    }

    if (PyFloat_Check(value)) {
        PyGeoCoord_Lat(self) = PyFloat_AsDouble(value);
        return 0;
    }
    else if (PyLong_Check(value)) {
        PyGeoCoord_Lat(self) = (double)PyLong_AsLong(value);
        return 0;
    }

    PyErr_SetString(PyExc_TypeError,
        "Cannot assign non numeric input argument 'value'.");
    return -1;
}

static PyObject *
PyGeoCoord_getlon(PyGeoCoordObject *self, void *closure)
{
    PyObject *ob;
    ob = PyFloat_FromDouble(PyGeoCoord_Lon(self));
    Py_INCREF(ob);
    return ob;
}

static int
PyGeoCoord_setlon(PyGeoCoordObject *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Invalid argument 'value'");
    }

    if (PyFloat_Check(value)) {
        PyGeoCoord_Lon(self) = PyFloat_AsDouble(value);
        return 0;
    }
    else if (PyLong_Check(value)) {
        PyGeoCoord_Lon(self) = (double)PyLong_AsLong(value);
        return 0;
    }

    PyErr_SetString(PyExc_TypeError,
        "Cannot assign non numeric input argument 'value'.");
    return -1;
}


static PyGetSetDef PyGeoCoord_getseters[] = {
     {"lat", (getter)PyGeoCoord_getlat, (setter)PyGeoCoord_setlat,
      "GeoCoord.latitude", NULL},
     {"lon", (getter)PyGeoCoord_getlon, (setter)PyGeoCoord_setlon,
      "GeoCoord.longitude", NULL},
     {NULL}
};


static PyMemberDef PyGeoCoord_members[] = {
     {NULL}
};

static PyMethodDef PyGeoCoord_methods[] = {
     {"geo_to_h3", (PyCFunction)geo_to_h3, METH_VARARGS, ""},
     {NULL}
};

PyTypeObject PyGeoCoord_Type = {
     PyVarObject_HEAD_INIT(NULL, 0)
     "h3py.GeoCoord",
     sizeof(PyGeoCoordObject),
     0,
     (destructor)PyGeoCoord_dealloc,             /* tp_dealloc */
     0,                                          /* tp_print */
     0,                                          /* tp_getattr */
     0,                                          /* tp_setattr */
     0,                                          /* tp_reserved */
     0,                                          /* tp_repr */
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
     "GeoCoord object",                          /* tp_doc */
     0,                                          /* tp_traverse */
     0,                                          /* tp_clear */
     0,                                          /* tp_richcompare */
     0,                                          /* tp_weaklistoffset */
     0,                                          /* tp_iter */
     0,                                          /* tp_iternext */
     PyGeoCoord_methods,                         /* tp_methods */
     PyGeoCoord_members,                         /* tp_members */
     PyGeoCoord_getseters,                       /* tp_getset */
     0,                                          /* tp_base */
     0,                                          /* tp_dict */
     0,                                          /* tp_descr_get */
     0,                                          /* tp_descr_set */
     0,                                          /* tp_dictoffset */
     (initproc)PyGeoCoord_init,                  /* tp_init */
     0,                                          /* tp_alloc */
     PyGeoCoord_new,                             /* tp_new */
};


static PyObject *
h3_get_resolution(PyObject *self, PyObject *args)
{

}

static PyObject *
string_to_h3(PyObject *self, PyObject *args)
{

}


static struct PyModuleDef moduledef = {
     PyModuleDef_HEAD_INIT,
     "h3py",
     NULL,
     -1,
     NULL
};

PyMODINIT_FUNC
PyInit_h3py(void)
{
    PyObject *module = PyModule_Create(&moduledef);

    if (PyType_Ready(&PyGeoCoord_Type) < 0){
        return NULL;
    }

    Py_INCREF(&PyGeoCoord_Type);
    if(PyModule_AddObject(module, "GeoCoord", (PyObject *)&PyGeoCoord_Type)){
        return NULL;
    }
    return module;
}

#ifdef __cpluscplus
}
#endif

#endif /* H3 def */
