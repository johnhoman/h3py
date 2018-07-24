#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <stddef.h>
#include <stdio.h>

#include <h3py.h>
#include <pygeocoord.h>
#include <pyh3index.h>
#include <pygeoboundary.h>


static H3Index *
_init_h3indexset(H3Index *in, size_t size)
{
    size_t i;
    for (i = 0; i < size; ++i) {
        in[i] = 0;
    }
    return in;
}


static PyObject *
_PyList_FromH3IndexSet(H3Index *h3in, size_t size)
{
    PyObject *h3out;
    PyH3IndexObject *h3;
    h3out = PyList_New((Py_ssize_t)size);
    size_t i;
    for (i = 0; i < size; ++i) {
        if (h3in[i] == 0) {
            Py_INCREF(Py_None);
            PyList_SET_ITEM(h3out, (Py_ssize_t)i, Py_None);
        } else {
            h3 = PyObject_New(PyH3IndexObject, &PyH3Index_Type);
            PyH3Index_AS_H3Index(h3) = h3in[i];
            PyList_SET_ITEM(h3out, (Py_ssize_t)i, (PyObject *)h3);
        }
    }
    return h3out;
}

static int
_validate_h3_list_input(PyObject *h3set)
{
    PyObject *ob;

    if (PyList_Check(h3set)) {
        Py_ssize_t i;
        for (i = 0; i < PyList_Size(h3set); ++i) {
            ob = PyList_GetItem(h3set, i);
            if (!PyH3Index_Check(ob)) {
                return -1;
            }
        }
    } else {
        return -1;
    }

    return 0;
}

static void
_h3index_fromlist(H3Index *h3setin, PyObject *h3set)
{
    PyH3IndexObject *h3;
    Py_ssize_t i;
    for (i = 0; i < PyList_Size(h3set); ++i) {
        h3 = (PyH3IndexObject *)PyList_GetItem(h3set, i);
        h3setin[i] = PyH3Index_AS_H3Index(h3);
    }
}


static PyObject *
_geoToH3(PyObject *self, PyObject *args){

    PyGeoCoordObject *g;
    PyObject *res;

    if (!PyArg_ParseTuple(args, "OO", &g, &res)) {
        PyErr_SetString(PyExc_RuntimeError,
            "Could not parse args for function 'h3py.geoToH3'");
        return NULL;
    }

    if (!PyGeoCoord_Check(g)) {
        PyErr_SetString(PyExc_TypeError,
            "Function 'geoToH3' expected positional "
            "argument 1 to be of type 'GeoCoord'.");
        return NULL;
    }
    return PyGeoCoord_to_h3(g, Py_BuildValue("(O)", res));
}


static PyObject *
_h3ToGeo(PyObject *self, PyObject *args){

    PyH3IndexObject *h3;

    if (!PyArg_ParseTuple(args, "O", &h3)) {
        PyErr_SetString(PyExc_RuntimeError, "h3py.h3ToGeo");
        return NULL;
    }

    if (!PyH3Index_Check(h3)) {
        PyErr_SetString(PyExc_RuntimeError, "h3py.h3ToGeo");
        return NULL;
    }

    return PyH3Index_to_geocoord(h3);
}

static PyObject *
_h3ToGeoBoundary(PyObject *self, PyObject *args){
    PyH3IndexObject *pyh3;
    PyGeoBoundaryObject *pygp;
    GeoBoundary *gp;
    H3Index h3;

    if (!PyArg_ParseTuple(args, "O!", &PyH3Index_Type ,&pyh3)) {
        PyErr_SetString(PyExc_RuntimeError,
            "Could not parse args for function 'h3ToGeoBoundary'.");
        return NULL;
    }
    Py_INCREF(pyh3);

    h3 = PyH3Index_AS_H3Index(pyh3);
    pygp = PyGeoBoundary_New();
    if (pygp == NULL) {
        PyErr_SetString(PyExc_SystemError,
            "Could not allocate GeoBoundary object.");
        Py_DECREF(pyh3);
        return NULL;
    }

    gp = PyGeoBoundary_AS_GeoBoundary(pygp);
    H3_EXPORT(h3ToGeoBoundary)(h3, gp);

    Py_DECREF(pyh3);
    return (PyObject *)pygp;
}

static PyObject *
_maxKringSize(PyObject *self, PyObject *args)
{
    int k, size;

    if (!PyArg_ParseTuple(args, "i", &k)) {
        PyErr_SetString(PyExc_RuntimeError,
            "Could not parse arguments for function 'maxKringSize'.");
        return NULL;
    }

    size = H3_EXPORT(maxKringSize)(k);
    return PyLong_FromLong((long)size);
}


static PyObject *
_hexRange(PyObject *self, PyObject *args){
    int sz_alloc = 0, k = 0, err = 0;
    PyH3IndexObject *origin;

    if (!PyArg_ParseTuple(args, "O!i", &PyH3Index_Type, &origin, &k)) {
        PyH3_PARSE_ERROR("hexRange");
        return NULL;
    }
    Py_INCREF(origin);
    sz_alloc = H3_EXPORT(maxKringSize)(k);

    H3Index out[sz_alloc];
    int i;
    for (i = 0; i < sz_alloc; ++i) {
        out[i] = 0;
    }
    err = H3_EXPORT(hexRange)(PyH3Index_AS_H3Index(origin), k, out);
    if (err != 0) {
        PyErr_Format(PyExc_RuntimeError,
            "Unknown error occured getting hex range for index '%llu'",
            PyH3Index_AS_H3Index(origin));
    }

    PyObject *range;
    PyH3IndexObject *h3;
    range = PyList_New(sz_alloc);

    for (i = 0; i < sz_alloc; ++i) {
        if (out[i] == 0) {
            Py_INCREF(Py_None);
            PyList_SET_ITEM(range, (Py_ssize_t)i, Py_None);
        } else {
            h3 = PyObject_New(PyH3IndexObject, &PyH3Index_Type);
            if (h3 == NULL) {
                PyErr_SetString(PyExc_MemoryError,
                    "Could not allocate memory for 'h3py.H3Index'.");
                return NULL;
            }
            PyH3Index_AS_H3Index(h3) = out[i];
            PyList_SET_ITEM(range, (Py_ssize_t)i, (PyObject *)h3);
        }
    }

    Py_DECREF(origin);
    return range;
}


static PyObject *
_hexRangeDistances(PyObject *self, PyObject *args){
    int sz_alloc = 0, k = 0, sz_out = 0;
    PyH3IndexObject *origin;

    if (!PyArg_ParseTuple(args, "O!i", &PyH3Index_Type, &origin, &k)) {
        PyH3_PARSE_ERROR("hexRangeDistances");
        return NULL;
    }
    Py_INCREF(origin);
    sz_alloc = H3_EXPORT(maxKringSize)(k);

    H3Index out[sz_alloc];
    int distances[sz_alloc];

    H3Index h3_origin = PyH3Index_AS_H3Index(origin);
    sz_out = H3_EXPORT(hexRangeDistances)(h3_origin, k, out, distances);

    PyObject *range, *dists, *distance;
    PyH3IndexObject *h3;

    range = PyList_New(sz_out);
    dists = PyList_New(sz_out);

    if (range == NULL || dists == NULL) {
        PyErr_SetString(PyExc_MemoryError,
            "Could not allocated memory for return values.");
        return NULL;
    }

    size_t ii;
    for (ii = 0; ii < (size_t)sz_out; ++ii) {
        h3 = PyObject_New(PyH3IndexObject, &PyH3Index_Type);
        if (h3 == NULL) {
            PyErr_SetString(PyExc_MemoryError,
                "Could not allocate H3Index.");
            return NULL;
        }
        PyH3Index_AS_H3Index(h3) = out[ii];
        PyList_SET_ITEM(range, (Py_ssize_t)ii, (PyObject *)h3);

        distance = PyLong_FromLong(distances[ii]);
        if (distance == NULL) {
            PyErr_SetString(PyExc_MemoryError,
                "Could not allocated memory for 'int'.");
            return NULL;
        }
        PyList_SET_ITEM(dists, (Py_ssize_t)ii, distance);
    }

    Py_DECREF(origin);
    return Py_BuildValue("(OO)", range, dists);
}

static PyObject *
_hexRanges(PyObject *self, PyObject *args){
    PyObject *h3set;
    int length = 0, k = 0, sz_alloc = 0, err = 0;

    if (!PyArg_ParseTuple(args, "Oi", &h3set, &k)) {
        PyErr_SetString(PyExc_RuntimeError,
            "Error parse arguments for function 'h3py.hexRanges'.");
        return NULL;
    }

    Py_INCREF(h3set);
    if (_validate_h3_list_input(h3set) < 0) {
        PyErr_SetString(PyExc_TypeError,
            "Positional argument number 1 must be type 'list' "
            "and contain only type 'h3py.H3Index'.");
        return NULL;
    }

    length = (int)PyList_Size(h3set);
    sz_alloc = length * H3_EXPORT(maxKringSize)(k);
    H3Index h3setin[length];
    _h3index_fromlist(h3setin, h3set);

    /* initialize output array  */
    H3Index out[sz_alloc];
    assert(_init_h3indexset(out, sz_alloc) == out);

    err = H3_EXPORT(hexRanges)(h3setin, length, k, out);
    if (err != 0) {
        PyErr_SetString(PyExc_RuntimeError,
            "Unknown error occured. Cannot get hex ranges.");
    }

    Py_DECREF(h3set);
    return _PyList_FromH3IndexSet(out, sz_alloc);
}

static PyObject *
_kRing(PyObject *self, PyObject *args){
    PyObject *origin;
    int k = 0, size = 0;

    if (!PyArg_ParseTuple(args, "Oi", &origin, &k)) {
        PyErr_SetString(PyExc_RuntimeError,
            "Could not parse args for function 'h3py.kRing'.");
        return NULL;
    }

    if (!PyH3Index_Check(origin)) {
        PyErr_SetString(PyExc_TypeError,
            "Positional argument 1 must be of type 'h3py.H3Index'.");
        return NULL;
    }
    Py_INCREF(origin);
    size = H3_EXPORT(maxKringSize)(k);

    H3Index out[size];
    _init_h3indexset(out, size);

    H3_EXPORT(kRing)(PyH3Index_AS_H3Index((PyH3IndexObject *)origin), k, out);

    Py_DECREF(origin);
    return _PyList_FromH3IndexSet(out, size);
}

static PyObject *
_kRingDistances(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_hexRing(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_maxPolyfillSize(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_polyfill(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3SetToLinkedGeo(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_destroyLinkedPolygon(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_degsToRads(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_radsToDegs(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_hexAreaKm2(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_hexAreaM2(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_edgeLengthKm(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_edgeLengthM(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_numHexagons(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3GetResolution(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3GetBaseCell(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_stringToH3(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3ToString(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3IsValid(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3ToParent(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_maxH3ToChildrenSize(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3ToChildren(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_compact(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_maxUncompactSize(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_uncompact(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3isResClassIII(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3IsPentagon(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3IndexesAreNeighbors(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_getH3UnidirectionalEdge(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_h3UnidirectionalEdgeIsValid(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_getOriginH3IndexFromUnidirectionalEdge(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_getDestinationH3IndexFromUnidirectionalEdge(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_getH3IndexesFromUnidirectionalEdge(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_getH3UnidirectionalEdgesFromHexagon(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_getH3UnidirectionalEdgeBoundary(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}


static PyMethodDef h3py_methods[] = {
    {"geoToH3",
        (PyCFunction)_geoToH3,
        METH_VARARGS,
        "find the H3 index of the resolution res cell containing the lat/lon g"},
    {"h3ToGeo",
        (PyCFunction)_h3ToGeo,
        METH_VARARGS,
        "find the lat/lon center point g of the cell h3"},
    {"h3ToGeoBoundary",
        (PyCFunction)_h3ToGeoBoundary,
        METH_VARARGS,
        "give the cell boundary in lat/lon coordinates for the cell h3"},
    {"maxKringSize",
        (PyCFunction)_maxKringSize,
        METH_VARARGS,
        "maximum number of hexagons in k-ring"},
    {"hexRange",
        (PyCFunction)_hexRange,
        METH_VARARGS,
        "hexagons neighbors in all directions, assuming no pentagons"},
    {"hexRangeDistances",
        (PyCFunction)_hexRangeDistances,
        METH_VARARGS,
        "hexagons neighbors in all directions, assuming no pentagons, "
        "reporting distance from origin"},
    {"hexRanges",
        (PyCFunction)_hexRanges,
        METH_VARARGS,
        "collection of hex rings sorted by ring for all given hexagons"},
    {"kRing",
        (PyCFunction)_kRing,
        METH_VARARGS,
        "hexagon neighbors in all directions"},
    {"kRingDistances",
        (PyCFunction)_kRingDistances,
        METH_VARARGS,
        "hexagon neighbors in all directions, reporting distance from origin"},
    {"hexRing",
        (PyCFunction)_hexRing,
        METH_VARARGS,
        "hollow hexagon ring at some origin"},
    {"maxPolyfillSize",
        (PyCFunction)_maxPolyfillSize,
        METH_VARARGS,
        "maximum number of hexagons in the geofence"},
    {"polyfill",
        (PyCFunction)_polyfill,
        METH_VARARGS,
        "hexagons within the given geofence"},
    {"h3SetToLinkedGeo",
        (PyCFunction)_h3SetToLinkedGeo,
        METH_VARARGS,
        "Create a LinkedGeoPolygon from a set of contiguous hexagons"},
    {"destroyLinkedPolygon",
        (PyCFunction)_destroyLinkedPolygon,
        METH_VARARGS,
        "Free all memory created for a LinkedGeoPolygon"},
    {"degsToRads",
        (PyCFunction)_degsToRads,
        METH_VARARGS,
        "converts degrees to radians"},
    {"radsToDegs",
        (PyCFunction)_radsToDegs,
        METH_VARARGS,
        "converts radians to degrees"},
    {"hexAreaKm2",
        (PyCFunction)_hexAreaKm2,
        METH_VARARGS,
        "hexagon area in square kilometers"},
    {"hexAreaM2",
        (PyCFunction)_hexAreaM2,
        METH_VARARGS,
        "hexagon area in square meters"},
    {"edgeLengthKm",
        (PyCFunction)_edgeLengthKm,
        METH_VARARGS,
        "hexagon edge length in kilometers"},
    {"edgeLengthM",
        (PyCFunction)_edgeLengthM,
        METH_VARARGS,
        "hexagon edge length in meters"},
    {"numHexagons",
        (PyCFunction)_numHexagons,
        METH_VARARGS,
        "number of hexagons for a given resolution"},
    {"h3GetResolution",
        (PyCFunction)_h3GetResolution,
        METH_VARARGS,
        "returns the resolution of the provided hexagon"},
    {"h3GetBaseCell",
        (PyCFunction)_h3GetBaseCell,
        METH_VARARGS,
        "returns the base cell of the provided hexagon"},
    {"stringToH3",
        (PyCFunction)_stringToH3,
        METH_VARARGS,
        "converts the canonical string format to H3Index format"},
    {"h3ToString",
        (PyCFunction)_h3ToString,
        METH_VARARGS,
        "converts an H3Index to a canonical string"},
    {"h3IsValid",
        (PyCFunction)_h3IsValid,
        METH_VARARGS,
        "confirms if an H3Index is valid"},
    {"h3ToParent",
        (PyCFunction)_h3ToParent,
        METH_VARARGS,
        "returns the parent (or grandparent, etc) hexagon of the given hexagon"},
    {"maxH3ToChildrenSize",
        (PyCFunction)_maxH3ToChildrenSize,
        METH_VARARGS,
        "determines the maximum number of children (or grandchildren, etc) "
        "that could be returned for the given hexagon"},
    {"h3ToChildren",
        (PyCFunction)_h3ToChildren,
        METH_VARARGS,
        "provides the children (or grandchildren, etc) of the given hexagon"},
    {"compact",
        (PyCFunction)_compact,
        METH_VARARGS,
        "compacts the given set of hexagons as best as possible"},
    {"maxUncompactSize",
        (PyCFunction)_maxUncompactSize,
        METH_VARARGS,
        "determines the maximum number of hexagons that could be uncompacted "
        "from the compacted set"},
    {"uncompact",
        (PyCFunction)_uncompact,
        METH_VARARGS,
        "uncompacts the compacted hexagon set"},
    {"h3isResClassIII",
        (PyCFunction)_h3isResClassIII,
        METH_VARARGS,
        "determines if a hexagon is Class III (or Class II)"},
    {"h3IsPentagon",
        (PyCFunction)_h3IsPentagon,
        METH_VARARGS,
        "determines if a hexagon is actually a pentagon"},
    {"h3IndexesAreNeighbors",
        (PyCFunction)_h3IndexesAreNeighbors,
        METH_VARARGS,
        "returns whether or not the provided hexagons border"},
    {"getH3UnidirectionalEdge",
        (PyCFunction)_getH3UnidirectionalEdge,
        METH_VARARGS,
        "returns the unidirectional edge H3Index for the specified origin and "
        "destination "},
    {"h3UnidirectionalEdgeIsValid",
        (PyCFunction)_h3UnidirectionalEdgeIsValid,
        METH_VARARGS,
        "returns whether the H3Index is a valid unidirectional edge"},
    {"getOriginH3IndexFromUnidirectionalEdge",
        (PyCFunction)_getOriginH3IndexFromUnidirectionalEdge,
        METH_VARARGS,
        "Returns the origin hexagon H3Index from the unidirectional edge H3Index"},
    {"getDestinationH3IndexFromUnidirectionalEdge",
        (PyCFunction)_getDestinationH3IndexFromUnidirectionalEdge,
        METH_VARARGS,
        "Returns the destination hexagon H3Index from the unidirectional edge H3Index"},
    {"getH3IndexesFromUnidirectionalEdge",
        (PyCFunction)_getH3IndexesFromUnidirectionalEdge,
        METH_VARARGS,
        "Returns the origin and destination hexagons from the unidirectional edge H3Index"},
    {"getH3UnidirectionalEdgesFromHexagon",
        (PyCFunction)_getH3UnidirectionalEdgesFromHexagon,
        METH_VARARGS,
        "Returns the 6 (or 5 for pentagons) edges associated with the H3Index"},
    {"getH3UnidirectionalEdgeBoundary",
        (PyCFunction)_getH3UnidirectionalEdgeBoundary,
        METH_VARARGS,
        "Returns the GeoBoundary containing the coordinates of the edge"},
    {NULL}

};

static struct PyModuleDef moduledef = {
     PyModuleDef_HEAD_INIT,
     "h3py",
     NULL,
     -1,
     h3py_methods
};

PyMODINIT_FUNC
PyInit_h3py(void)
{
    PyObject *module = PyModule_Create(&moduledef);

    if (PyType_Ready(&PyGeoCoord_Type) < 0
            || PyType_Ready(&PyH3Index_Type) < 0
            || PyType_Ready(&PyGeoBoundary_Type) < 0){

        return NULL;
    }

    Py_INCREF(&PyGeoCoord_Type);
    Py_INCREF(&PyH3Index_Type);
    Py_INCREF(&PyGeoBoundary_Type);
    if(PyModule_AddObject(module, "GeoCoord", (PyObject *)&PyGeoCoord_Type)
            || PyModule_AddObject(module, "H3Index", (PyObject *)&PyH3Index_Type)
            || PyModule_AddObject(module, "GeoBoundary", (PyObject *)&PyGeoBoundary_Type)){
        return NULL;
    }
    return module;
}
