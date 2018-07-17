#include <Python.h>
#include <h3api.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>

#ifndef PY_GEO_BOUNDARY_H
#define PY_GEO_BOUNDARY_H

#ifdef __cplusplus
extern "C" {
#endif

PyAPI_DATA(PyTypeObject) PyGeoBoundary_Type;
#define PyGeoBoundary_Check(x) (Py_TYPE(x) == &PyGeoBoundary_Type)

/* wrapping the original object seems stupid  */
typedef struct {
    PyObject_HEAD
    GeoBoundary *ob_val;
} PyGeoBoundaryObject;

static PyObject *
PyGeoBoundary_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

static int
PyGeoBoundary_init(PyGeoBoundaryObject *self, PyObject *args, PyObject *kwds);

static void
PyGeoBoundary_dealloc(PyGeoBoundaryObject *self);

static PyObject *
PyGeoBoundary_getverts(PyGeoBoundaryObject *self);

static PyObject *
PyGeoBoundary_repr(PyGeoBoundaryObject *self);

#ifdef __cplusplus
}
#endif

#endif /* PY_GEO_BOUNDARY_H */
