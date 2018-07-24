#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <stddef.h>
#include <stdio.h>


#ifndef Py_H3_INDEX_H
#define Py_H3_INDEX_H


#ifdef __cplusplus
extern "C" {
#endif


PyAPI_DATA(PyTypeObject) PyH3Index_Type;
#define PyH3Index_Check(x) (Py_TYPE(x) == &PyH3Index_Type)
#define PyH3Index_AsLongLong(ob) (PyLong_FromLongLong((ob)->ob_val))
#define PyH3Index_AS_H3Index(ob) ((ob)->ob_val)

typedef struct {
    PyObject_HEAD
    H3Index ob_val;
} PyH3IndexObject;

static PyObject *PyH3Index_get_resolution(PyH3IndexObject *self);
static PyObject *PyH3Index_get_base_cell(PyH3IndexObject *self);
static PyObject *PyH3Index_from_string(PyObject *ob, PyObject *args);
static PyObject *PyH3Index_to_string(PyH3IndexObject *self, PyObject *args);
static PyObject *PyH3Index_to_boundary(PyH3IndexObject *self);
static PyObject *PyH3Index_is_valid(PyH3IndexObject *self);
PyObject *PyH3Index_to_geocoord(PyH3IndexObject *self);
static PyObject *PyH3Index_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int PyH3Index_init(PyH3IndexObject *self, PyObject *args, PyObject *kwds);
static void PyH3Index_dealloc(PyH3IndexObject *self);
static PyObject *PyH3Index_getindex(PyH3IndexObject *self, void *closure);
static int PyH3Index_setindex(PyH3IndexObject *self, PyObject *value, void *closure);

#ifdef __cplusplus
}
#endif

#endif
