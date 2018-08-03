#include <Python.h>
#include <h3api.h>

typedef struct _geocoord {
    PyObject_HEAD
    GeoCoord ob_val;
} GeoCoordObject;

typedef struct _h3index {
    PyObject_HEAD
    H3Index ob_val;
} H3IndexObject;

typedef struct _geoboundary {
    PyObject_HEAD
    GeoBoundary ob_val;
} GeoBoundaryObject;

static PyTypeObject GeoCoord_Type;
static PyTypeObject H3Index_Type;
static PyTypeObject GeoBoundary_Type;
