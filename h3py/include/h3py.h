
#include <Python.h>
#include <h3api.h>
#include <stdio.h>
#include <pygeocoord.h>
#include <pyh3index.h>
#include <pygeoboundary.h>


#ifndef Py_H3API_H
#define Py_H3API_H


#ifdef __cpluscplus
extern "C" {
#endif


typedef struct _geocoord {
    PyObject_HEAD
    GeoCoord ob_val;
} geocoord_object;

typedef struct _h3index {
    PyObject_HEAD
    H3Index ob_val;
} h3index_object;

typedef struct _geoboundary {
    PyObject_HEAD
    GeoBoundary ob_val;
} geoboundary_object;

static PyTypeObject geocoord_type;
static PyTypeObject h3index_type;
static PyTypeObject geoboundary_type;

static h3index_object *geotoh3(geocoord_object *, PyLongObject *);
static geocoord_object *h3togeo(h3index_object *);
static geoboundary_object *h3togeoboundary(h3index_object *);
static PyObject *maxkringsize(long);
static PyObject *hexrange(h3index_object *, long);
static PyObject *hexrangedistances(h3index_object *, long);
static PyObject *hexranges(PyObject *, long);
static PyObject *


#define PyH3_PARSE_ERROR(funcname) \
    PyErr_SetString(PyExc_RuntimeError, \
        "Could not parse arguments for function '" funcname "'.")


/* API-Functions  */
PyObject *set_h3_index(PyObject *self, PyObject *args);
static PyObject *_geoToH3(PyObject *self, PyObject *args);
static PyObject *_h3ToGeo(PyObject *self, PyObject *args);
static PyObject *_h3ToGeoBoundary(PyObject *self, PyObject *args);
static PyObject *_maxKringSize(PyObject *self, PyObject *args);
static PyObject *_hexRange(PyObject *self, PyObject *args);
static PyObject *_hexRangeDistances(PyObject *self, PyObject *args);
static PyObject *_hexRanges(PyObject *self, PyObject *args);
static PyObject *_kRing(PyObject *self, PyObject *args);
static PyObject *_kRingDistances(PyObject *self, PyObject *args);
static PyObject *_hexRing(PyObject *self, PyObject *args);
static PyObject *_maxPolyfillSize(PyObject *self, PyObject *args);
static PyObject *_polyfill(PyObject *self, PyObject *args);
static PyObject *_h3SetToLinkedGeo(PyObject *self, PyObject *args);
static PyObject *_destroyLinkedPolygon(PyObject *self, PyObject *args);
static PyObject *_degsToRads(PyObject *self, PyObject *args);
static PyObject *_radsToDegs(PyObject *self, PyObject *args);
static PyObject *_hexAreaKm2(PyObject *self, PyObject *args);
static PyObject *_hexAreaM2(PyObject *self, PyObject *args);
static PyObject *_edgeLengthKm(PyObject *self, PyObject *args);
static PyObject *_edgeLengthM(PyObject *self, PyObject *args);
static PyObject *_numHexagons(PyObject *self, PyObject *args);
static PyObject *_h3GetResolution(PyObject *self, PyObject *args);
static PyObject *_h3GetBaseCell(PyObject *self, PyObject *args);
static PyObject *_stringToH3(PyObject *self, PyObject *args);
static PyObject *_h3ToString(PyObject *self, PyObject *args);
static PyObject *_h3IsValid(PyObject *self, PyObject *args);
static PyObject *_h3ToParent(PyObject *self, PyObject *args);
static PyObject *_maxH3ToChildrenSize(PyObject *self, PyObject *args);
static PyObject *_h3ToChildren(PyObject *self, PyObject *args);
static PyObject *_compact(PyObject *self, PyObject *args);
static PyObject *_maxUncompactSize(PyObject *self, PyObject *args);
static PyObject *_uncompact(PyObject *self, PyObject *args);
static PyObject *_h3isResClassIII(PyObject *self, PyObject *args);
static PyObject *_h3IsPentagon(PyObject *self, PyObject *args);
static PyObject *_h3IndexesAreNeighbors(PyObject *self, PyObject *args);
static PyObject *_getH3UnidirectionalEdge(PyObject *self, PyObject *args);
static PyObject *_h3UnidirectionalEdgeIsValid(PyObject *self, PyObject *args);
static PyObject *_getOriginH3IndexFromUnidirectionalEdge(PyObject *self, PyObject *args);
static PyObject *_getDestinationH3IndexFromUnidirectionalEdge(PyObject *self, PyObject *args);
static PyObject *_getH3IndexesFromUnidirectionalEdge(PyObject *self, PyObject *args);
static PyObject *_getH3UnidirectionalEdgesFromHexagon(PyObject *self, PyObject *args);
static PyObject *_getH3UnidirectionalEdgeBoundary(PyObject *self, PyObject *args);

#ifdef __cpluscplus
}
#endif

#endif /* def Py_H3 */
