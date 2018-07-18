#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <stddef.h>
#include <stdio.h>

#ifndef Py_GEO_COORD_H
#define Py_GEO_COORD_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_DATA(PyTypeObject) PyGeoCoord_Type;
#define PyGeoCoord_Check(x) (Py_TYPE(x) == &PyGeoCoord_Type)
#define PyGeoCoord_Lat(x) (x)->ob_gval->lat
#define PyGeoCoord_Lon(x) (x)->ob_gval->lon
#define PyGeoCoord_AsGeoCoord(ob) ((ob)->ob_gval)

typedef struct {
    PyObject_HEAD
    GeoCoord *ob_gval;
} PyGeoCoordObject;

PyObject *
PyGeoCoord_to_h3(PyGeoCoordObject *self, PyObject *args);

static PyObject *
PyGeoCoord_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

static int
PyGeoCoord_init(PyGeoCoordObject *self, PyObject *args, PyObject *kwds);

static void
PyGeoCoord_dealloc(PyGeoCoordObject *self);

static PyObject *
PyGeoCoord_getlat(PyGeoCoordObject *self, void *closure);

static int
PyGeoCoord_setlat(PyGeoCoordObject *self, PyObject *value, void *closure);

static PyObject *
PyGeoCoord_getlon(PyGeoCoordObject *self, void *closure);

static int
PyGeoCoord_setlon(PyGeoCoordObject *self, PyObject *value, void *closer);


#ifdef __cplusplus
}
#endif

#endif
