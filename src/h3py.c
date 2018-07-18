#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <stddef.h>
#include <stdio.h>

#include <h3py.h>
#include <pygeocoord.h>
#include <pyh3index.h>
#include <pygeoboundary.h>


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
        PyErr_SetString(PyExc_RuntimeError,
            "Cound not parse args for function 'h3py.h3ToGeo'.");
        return NULL;
    }

    if (!PyH3Index_Check(h3)) {
        PyErr_SetString(PyExc_RuntimeError,
            "Could not parse args for function 'h3py.geoToH3'.");
        return NULL;
    }

    return PyH3Index_to_geocoord(h3);
}

static PyObject *
_h3ToGeoBoundary(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_maxKringSize(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_hexRange(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_hexRangeDistances(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_hexRanges(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
_kRing(PyObject *self, PyObject *args){
    Py_INCREF(Py_None);
    return Py_None;
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
