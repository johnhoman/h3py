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

// TODO: Capitolize tail end of macro names
#define PyGeoBoundary_Check(x) (Py_TYPE(x) == &PyGeoBoundary_Type)
#define PyGeoBoundary_GetVert(ob, i) ((ob)->ob_val->verts[(i)])
#define PyGeoBoundary_GetVerts(ob) ((ob)->ob_val->verts)
#define PyGeoBoundary_GetGeoBoundary(x) ((x)->ob_val)
#define PyGeoBoundary_GetNumberVerts(x) ((x)->ob_val->numVerts)
#define PyGeoBoundary_AsPyObject(x) ((PyObject *)x)
#define PyGeoBoundary_Allocate(T) ((PyGeoBoundaryObject *)(T)->tp_alloc((T), 0))
#define PyGeoBoundary_AS_GeoBoundary(ob) ((ob)->ob_val)


/* wrapping the original object seems stupid  */
typedef struct {
    PyObject_HEAD
    GeoBoundary *ob_val;
} PyGeoBoundaryObject;

/* public constructor wrapper */
PyGeoBoundaryObject *
PyGeoBoundary_New(void);

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

static PyObject *
PyGeoBoundary_richcompare(PyObject *self, PyObject *other, int op);

#ifdef __cplusplus
}
#endif

#endif /* PY_GEO_BOUNDARY_H */
