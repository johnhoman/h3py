#include <Python.h>
#include <h3api.h>
#include <stddef.h>
/*
I don't know what TODO with this file. I can't find any code
that actually creates a Geofence, so where is it used?
*/

PyAPI_DATA(PyTypeObject) PyGeofence_Type;
#define PyGeofence_AsGeofence(ob) ((ob)->ob_val)
#define PyGeofence_Verts(ob) (PyGeofence_AsGeofence((ob))->numVerts)
#define PyGeofence_GetHoles(ob) (PyGeofence_AsGeofence((ob))->holes)
#define PyGeofence_GetHole(ob, i) (PyGeofence_GetHoles(ob)[(i)])
#define PyGeofence_Allocate(type) (PyGeofenceObject *)(type)->tp_alloc(type, 0)


typedef struct {
    PyObject_HEAD
    Geofence *ob_val;
} PyGeofenceObject;

static PyObject *
PyGeofence_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

static int
PyGeofence_init(PyGeofenceObject *self, PyObject *args, PyObject *kwds);

static PyObject *
PyGeofence_append(PyGeofenceObject *self, PyObject *args);
