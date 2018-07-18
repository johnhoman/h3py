#include <Python.h>
#include <h3api.h>
#include <stdio.h>

#include <pygeocoord.h>
#include <pyh3index.h>
#include <pygeoboundary.h>

#ifndef Py_H3_H
#define Py_H3_H
#ifdef __cpluscplus
extern "C" {
#endif


static PyObject *
_geoToH3(PyObject *self, PyObject *args);

static PyObject *
_h3ToGeo(PyObject *self, PyObject *args);

static PyObject *
_h3ToGeoBoundary(PyObject *self, PyObject *args);

static PyObject *
_maxKringSize(PyObject *self, PyObject *args);

static PyObject *
_hexRange(PyObject *self, PyObject *args);

static PyObject *
_hexRangeDistances(PyObject *self, PyObject *args);

static PyObject *
_hexRanges(PyObject *self, PyObject *args);

static PyObject *
_kRing(PyObject *self, PyObject *args);

static PyObject *
_kRingDistances(PyObject *self, PyObject *args);

static PyObject *
_hexRing(PyObject *self, PyObject *args);

static PyObject *
_maxPolyfillSize(PyObject *self, PyObject *args);

static PyObject *
_polyfill(PyObject *self, PyObject *args);

static PyObject *
_h3SetToLinkedGeo(PyObject *self, PyObject *args);

static PyObject *
_destroyLinkedPolygon(PyObject *self, PyObject *args);

static PyObject *
_degsToRads(PyObject *self, PyObject *args);

static PyObject *
_radsToDegs(PyObject *self, PyObject *args);

static PyObject *
_hexAreaKm2(PyObject *self, PyObject *args);

static PyObject *
_hexAreaM2(PyObject *self, PyObject *args);

static PyObject *
_edgeLengthKm(PyObject *self, PyObject *args);

static PyObject *
_edgeLengthM(PyObject *self, PyObject *args);

static PyObject *
_numHexagons(PyObject *self, PyObject *args);

static PyObject *
_h3GetResolution(PyObject *self, PyObject *args);

static PyObject *
_h3GetBaseCell(PyObject *self, PyObject *args);

static PyObject *
_stringToH3(PyObject *self, PyObject *args);

static PyObject *
_h3ToString(PyObject *self, PyObject *args);

static PyObject *
_h3IsValid(PyObject *self, PyObject *args);

static PyObject *
_h3ToParent(PyObject *self, PyObject *args);

static PyObject *
_maxH3ToChildrenSize(PyObject *self, PyObject *args);

static PyObject *
_h3ToChildren(PyObject *self, PyObject *args);

static PyObject *
_compact(PyObject *self, PyObject *args);

static PyObject *
_maxUncompactSize(PyObject *self, PyObject *args);

static PyObject *
_uncompact(PyObject *self, PyObject *args);

static PyObject *
_h3isResClassIII(PyObject *self, PyObject *args);

static PyObject *
_h3IsPentagon(PyObject *self, PyObject *args);

static PyObject *
_h3IndexesAreNeighbors(PyObject *self, PyObject *args);

static PyObject *
_getH3UnidirectionalEdge(PyObject *self, PyObject *args);

static PyObject *
_h3UnidirectionalEdgeIsValid(PyObject *self, PyObject *args);

static PyObject *
_getOriginH3IndexFromUnidirectionalEdge(PyObject *self, PyObject *args);

static PyObject *
_getDestinationH3IndexFromUnidirectionalEdge(PyObject *self, PyObject *args);

static PyObject *
_getH3IndexesFromUnidirectionalEdge(PyObject *self, PyObject *args);

static PyObject *
_getH3UnidirectionalEdgesFromHexagon(PyObject *self, PyObject *args);

static PyObject *
_getH3UnidirectionalEdgeBoundary(PyObject *self, PyObject *args);

#ifdef __cpluscplus
}
#endif

#endif /* def Py_H3 */
