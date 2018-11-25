#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <stdio.h>

#define rad2deg(x) (x * M_PI / 180)
#define deg2rad(x) (x / M_PI * 180)
#define CHECK_REF(x) \
    printf("ref_count " #x " :%ld, (line %d)\n", Py_REFCNT(x), __LINE__)
/* GeoCoord */
static PyTypeObject geocoord_type;
static PyTypeObject geoboundary_type;
static PyTypeObject geoboundaryiter_type;
// static PyTypeObject geofence_type;


typedef struct {
    PyObject_HEAD
    GeoCoord ob_val;
    int deg; /* 1 - units in degrees, 0 - units in radians */
} geocoord_object;

typedef struct {
    PyObject_HEAD
    GeoBoundary ob_val;
} geoboundary_object;

typedef struct {
    PyObject_HEAD
    Py_ssize_t it_index;
    geoboundary_object *it_seq;
} geoboundaryiter_object;

typedef struct {
    PyObject_HEAD
    Geofence ob_val;
} geofence_object;

typedef struct {
    PyObject_HEAD
    geofence_object *it_seq;
} geofenceiter_object;

typedef struct {
    PyObject_HEAD
    GeoPolygon ob_val;
} geopolygon_object;


/* GeoCoord methods forward declare */
static geocoord_object *geocoord_new_impl(PyTypeObject *, PyObject *, PyObject *, PyObject *);
static PyObject *geocoord_new(PyTypeObject *, PyObject *, PyObject *);

/* GeoBoundary forward declare */
static geoboundary_object *geoboundary_new_impl(PyTypeObject *, PyObject *);
static PyObject *geoboundary_new(PyTypeObject *, PyObject *, PyObject *);
static PyObject *geoboundary_iter(PyObject *ob);
static void geoboundary_dealloc(geoboundary_object *ob);

/*  geoboundary_iter forward declare */
static PyObject *geoboundaryiter_len(geoboundaryiter_object *it);
static PyObject *geoboundaryiter_reduce(geoboundaryiter_object *it);
static PyObject *geoboundaryiter_setstate(geoboundaryiter_object *it, PyObject *state);
static void geoboundaryiter_dealloc(geoboundaryiter_object *it);
static int geoboundaryiter_traverse(geoboundaryiter_object *it, visitproc visit, void *arg);
static PyObject *geoboundaryiter_next(geoboundaryiter_object *it);

/* geofence */
/* geopolygon */


PyDoc_STRVAR(geocoord_toh3_doc,
"geocoord_toh3(g, res) - get the h3 index corresponding \n\
    to the location specified by GeoCoord object `g` at \n\
    special resolution 'res'. \n\
\n\
    Parameters\n\
    ----------\n\
    g: GeoCoord\n\
        GeoCoord object with members lat, lon in radians\n\
    res: int\n\
        h3 resolution in the inclusive range 0->15\n\
\n\
    Returns\n\
    -------\n\
    int:\n\
        h3 index");

static PyObject *geocoord_toh3(PyObject *, PyObject *);
static PyObject *geocoord_toh3_impl(geocoord_object *, PyObject *);

PyDoc_STRVAR(h3to_geocoord_doc,
"h3to_geocoord(h3, deg=False) - get the latitude, longitude coordinate \n\
    center point in radians from index `h3`.\n\
\n\
    Parameters\n\
    ----------\n\
    h3: int\n\
        valid h3 index\n\
    deg: bool, default False\n\
        return the GeoCoord object with coordiantes in degrees if\n\
        `deg=True` otherwise, return coordiantes in radians\n\
\n\
    Returns\n\
    -------\n\
    GeoCoord:\n\
        GeoCoord object with coordinate location described by h3 index `h3`\n");

static PyObject *h3to_geocoord(PyObject *, PyObject *, PyObject *);
static geocoord_object *h3to_geocoord_impl(PyObject *, PyObject *);

PyDoc_STRVAR(h3to_geoboundary_doc,
"h3to_geoboundary(h3) - give the cell boundary in lat/lon\n\
    coordinates for the cell h3\n\
\n\
    Parameters\n\
    ----------\n\
    h3: int\n\
        valid h3 index\n\
\n\
    Returns\n\
    -------\n\
    GeoBoundary:\n\
        GeoBoundary object holding at most 10 GeoCoord objects\n\
\n\
    Examples\n\
    --------\n\
    >>> from h3py import h3to_geoboundary, GeoCoord\n\
    >>> from pprint import pprint\n\
    >>> h3 = GeoCoord(75.0, 33.8, deg=True).toh3(1)\n\
    >>> boundary = h3to_geoboundary(h3)\n\
    >>> it = iter(boundary)\n\
    >>> pprint(list(it))\n\
    [<h3py.GeoCoord object at 0x7f0f57c3dd10>,\n\
     <h3py.GeoCoord object at 0x7f0f57c3dd30>,\n\
     <h3py.GeoCoord object at 0x7f0f57c3dd50>,\n\
     <h3py.GeoCoord object at 0x7f0f57c3dd70>,\n\
     <h3py.GeoCoord object at 0x7f0f57c3ddf0>,\n\
     <h3py.GeoCoord object at 0x7f0f57c3dd90>]\n\
");

static PyObject *h3to_geoboundary(PyObject *, PyObject *, PyObject *);

PyDoc_STRVAR(maxkringsize_doc,
"max_kring_size(k) - maximum number of indicies that result from the kring\n\
    algorithm with the given k `k`\n\
\n\
    Parameters\n\
    ----------\n\
    k: int\n\
        number of rings\n\
\n\
    Returns\n\
    -------\n\
    kring_size: int\n\
        number of hexagons in kring\n\
\n\
    Examples\n\
    --------\n\
    >>> from _h3py import maxkringsize\n\
    >>> maxkringsize(3)\n\
    >>> \n\
");

static PyObject *
maxkringsize(PyObject *m, PyObject *args, PyObject *kwds);

PyDoc_STRVAR(hexrange_doc,
"hex_range(origin, k) - get the hexagon neighbors in all directions,\n\
    assuming no pentagons\n\
\n\
    Parameters\n\
    ----------\n\
    origin: int (H3Index)\n\
        center H3Index\n\
    k: int\n\
        number of rings to return from origin\n\
\n\
    Returns\n\
    -------\n\
    H3Indexes: [int]\n\
        H3 indexes\n\
\n\
    Examples\n\
    --------\n\
    >>> from h3py import GeoCoord, hex_range_distances\n\
    >>> h3 = GeoCoord(75.0, -103.2, deg=True).toh3(2)\n\
    >>> hex_range(h3, 1)\n\
    [588367363720609791,\n\
     588366813964795903,\n\
     588368463232237567,\n\
     588940759034494975,\n\
     588873688825200639,\n\
     588875887848456191,\n\
     588369562743865343]\n\
    >>> \n\
\n");


static PyObject *
hexrange(PyObject *m, PyObject *args, PyObject *kwds);

PyDoc_STRVAR(hexrangedistances_doc,
"hex_range_distances(origin, k) - produces indexes within k distances of the\n\
    origin index. Output behavior is undefined when one of the indexes returned\n\
    by the function is a pentagon or is in the pentagon distortion area.\n\
    \n\
    k-ring 0 is defined as the origin index, k-ring 1 is defined as k-ring 0\n\
    and all neighboring indexes, and so on.\n\
    \n\
    Output is placed in a list and is in order of increasing distance from\n\
    the origin.\n\
    \n\
    Parameters\n\
    ----------\n\
    origin: int (H3Index)\n\
        center H3Index\n\
    k: int\n\
        number of rings to return from origin\n\
    \n\
    Returns\n\
    -------\n\
    H3Indexes, distances: [(int, int)]\n\
        H3 indicies and their corresponding distances from the origin\n\
    \n\
    Examples\n\
    --------\n\
    >>> from h3py import GeoCoord, hex_range_distances\n\
    >>> h3 = GeoCoord(75.0, -103.2, deg=True).toh3(2)\n\
    >>> hex_range_distances(h3, 1)\n\
    [(588367363720609791, 0),\n\
     (588366813964795903, 1),\n\
     (588368463232237567, 1),\n\
     (588940759034494975, 1),\n\
     (588873688825200639, 1),\n\
     (588875887848456191, 1),\n\
     (588369562743865343, 1)]\n\
    >>> \n\
\n");

static PyObject *
hexrangedistances(PyObject *m, PyObject *args, PyObject *kwds);

PyDoc_STRVAR(hexranges_doc,
"hex_ranges(origins, k) - collections of hex rings sorted by ring for all\n\
    given hexagons.\n\
\n");

static PyObject *
hexranges(PyObject *m, PyObject *args, PyObject *kwds);

PyDoc_STRVAR(kring_doc,
"kring(origin, k) - kring produces indices within k distance of the origin\n\
    index.\n\
\n\
    k-ring 0 is defined as the origin index, k-ring 1 is defined as\n\
    k-ring 0 and all neighboring indices, and so on.\n\
\n\
    The output list is in no particular order. Elemented of the output list\n\
    may be left zero, as can happen when crossing a pentagon.\n\
\n\
    Parameters\n\
    ----------\n\
    origin: int\n\
    k: int\n\
\n");

static PyObject *
kring(PyObject *m, PyObject *args, PyObject *self);


PyDoc_STRVAR(kring_distances_doc,
"kring_distances(origin, k) - hexagon neighbors in all directions,\n\
    reporting distance from origin\n\
\n\
    Parameters\n\
    ----------\n\
");

static PyObject *
kring_distances(PyObject *m, PyObject *args, PyObject *kwds);

PyDoc_STRVAR(hex_area_doc,
"hex_ares(res, units) - hexagon area in square meters or kilometers.\n\
\n\
    Parameters\n\
    ----------\n\
    res: int\n\
        h3 resolution\n\
    units: {'km2', 'm2'}\n\
        m2 - return units in square meters\n\
        km2 - return units in square kilometers\n\
\n\
    Returns\n\
    -------\n\
    float:\n\
        hexagon area returned with specified units\n\
\n");

static PyObject *
hexarea(PyObject *m, PyObject *args, PyObject *kwds);

static PyMemberDef geocoord_members[] = { {NULL} };
static PyMethodDef geocoord_methods[] = {
    {"toh3", (PyCFunction)geocoord_toh3, METH_VARARGS, geocoord_toh3_doc},
    {"from_h3", (PyCFunction)h3to_geocoord, METH_STATIC, h3to_geocoord_doc},
    {NULL}
};

static PyMethodDef h3py_methods[] = {
    {"geocoord_toh3", (PyCFunction)geocoord_toh3,
      METH_VARARGS, geocoord_toh3_doc},
    {"h3to_geocoord", (PyCFunction)h3to_geocoord,
      METH_VARARGS | METH_KEYWORDS, h3to_geocoord_doc},
    {"h3to_geoboundary", (PyCFunction)h3to_geoboundary,
      METH_VARARGS | METH_KEYWORDS, h3to_geoboundary_doc},
    {"max_kring_size", (PyCFunction)maxkringsize,
      METH_VARARGS | METH_KEYWORDS, maxkringsize_doc},
    {"hex_range", (PyCFunction)hexrange,
      METH_VARARGS | METH_KEYWORDS, hexrange_doc},
    {"hex_range_distances", (PyCFunction)hexrangedistances,
      METH_VARARGS | METH_KEYWORDS, hexrangedistances_doc},
    {"hex_ranges", (PyCFunction)hexranges,
      METH_VARARGS | METH_KEYWORDS, hexranges_doc},
    {"kring", (PyCFunction)kring,
      METH_VARARGS | METH_KEYWORDS, kring_doc},
    {"kring_distances", (PyCFunction)kring_distances,
      METH_VARARGS | METH_KEYWORDS, kring_distances_doc},
    {"hex_area", (PyCFunction)hexarea,
      METH_VARARGS | METH_KEYWORDS, hex_area_doc},
    {NULL}
};

static geocoord_object *
geocoord_new_impl(PyTypeObject *tp, PyObject *la, PyObject *lo, PyObject *deg)
{
    GeoCoord coord;

    if (PyFloat_Check(la) && PyFloat_Check(lo)) {
        coord.lat = PyFloat_AsDouble(la);
        coord.lon = PyFloat_AsDouble(lo);
    } else {
        PyErr_Format(PyExc_TypeError,
          "'GeoCoord' requires signature 'GeoCoord(float, float)'\n\
                but recieved 'GeoCoord(%s, %s)'",
           Py_TYPE(la)->tp_name, Py_TYPE(lo)->tp_name);
        return NULL;
    }
    geocoord_object *self = (geocoord_object *)tp->tp_alloc(tp, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_MemoryError, "GeoCoord()");
        return NULL;
    }
    if (PyBool_Check(deg) && deg == Py_False) {
        self->deg = 0;
        self->ob_val.lat = coord.lat;
        self->ob_val.lon = coord.lon;
    } else if (PyBool_Check(deg) && deg == Py_True) {
        self->deg = 1;
        self->ob_val.lat = deg2rad(coord.lat);
        self->ob_val.lon = deg2rad(coord.lon);
    } else {
        PyErr_Format(PyExc_TypeError,
            "keyword argument 'deg' must be type bool not %s",
            Py_TYPE(deg)->tp_name);
        return NULL;
    }
    /* TODO: remove calcuation and do comparison in radians */
    if (rad2deg(self->ob_val.lat) > 90 || rad2deg(self->ob_val.lat < -90)) {
        PyErr_Format(PyExc_ValueError, "invalid latitude '%S'", la);
        goto invalid;
    } else if (rad2deg(self->ob_val.lon) < -180 ||
               rad2deg(self->ob_val.lon) > 180) {
        PyErr_Format(PyExc_ValueError, "invalid longitude '%S'", lo);
        goto invalid;
    }

    return self;

    invalid:
        Py_DECREF((PyObject *)self);
        return NULL;
}

static PyObject *
geocoord_new(PyTypeObject *tp, PyObject *args, PyObject *kwds)
{
    static char *kwargs[] = {"lat", "lon", "deg", NULL};

    PyObject *la = NULL, *lo = NULL, *deg = NULL;
    geocoord_object *coord;
    int err;

    err = !PyArg_ParseTupleAndKeywords(args, kwds, "OO|O:GeoCoord", kwargs,
                                       &la, &lo, &deg);
    if (err) {
        return NULL;
    } else {
        if (deg == NULL)
            deg = Py_False; /* for now, assume radians */

        Py_INCREF(la);
        Py_INCREF(lo);
        Py_INCREF(deg);
    }
    coord = geocoord_new_impl(tp, la, lo, deg);
    if (coord == NULL)
        return NULL;

    Py_DECREF(la);
    Py_DECREF(lo);
    Py_DECREF(deg);
    return (PyObject *)coord;
}

static PyObject *
geocoord_toh3_impl(geocoord_object *ob, PyObject *res)
{
    H3Index h3;
    int resolution;


    if (!PyLong_Check(res)) {
        PyErr_Format(PyExc_TypeError,
            "expected type 'int' but recieved '%s'", Py_TYPE(res)->tp_name);
        return NULL;
    } else {
        resolution = (int)PyLong_AsLong(res);
        h3 = H3_EXPORT(geoToH3)(&ob->ob_val, resolution);
        return PyLong_FromLongLong(h3);
    }
}

static PyObject *
geocoord_toh3(PyObject *self, PyObject *args)
{
    geocoord_object *coord = NULL;
    PyObject *res = NULL, *ob;
    int ok;

    if (PyModule_Check(self)) { /* called as a module function */
        ok = PyArg_ParseTuple(args, "O!O:geocoord_toh3",
                              &geocoord_type, &coord, &res);

    } else if (Py_TYPE(self) == &geocoord_type) {
        ok = PyArg_ParseTuple(args, "O:GeoCoord.toh3", &res);
        coord = (geocoord_object *)self;
    } else {
        PyErr_Format(PyExc_NotImplementedError,
            "unexpected input argument of type '%s'", Py_TYPE(self)->tp_name);
        return NULL;
    }

    if (!ok || coord == NULL || res == NULL) {
        Py_XDECREF(coord);
        Py_XDECREF(res);
        PyErr_SetString(PyExc_RuntimeError,
            "error parsing args for function 'geocoord_toh3'");
        return NULL;
    } else {
        Py_INCREF(coord);
        Py_INCREF(res);
    }

    ob = geocoord_toh3_impl(coord, res);
    if (ob == NULL)
        return NULL;

    Py_DECREF(coord);
    Py_DECREF(res);
    return ob;
}



static void geocoord_dealloc(geocoord_object *);
static void
geocoord_dealloc(geocoord_object *g) { Py_TYPE(g)->tp_free((PyObject *)g); }


PyDoc_STRVAR(geocoord_doc,
"GeoCoord(lat, lon, deg=False) - GeoCoord object with latitude and \n\
    longitude arguments `lat`, `lon` in radians. Input arguments can \n\
    be in degree units if optional keyword argument `deg=True` as passed. \n\
\n\
    Parameters \n\
    ---------- \n\
    lat: float \n\
        latitude coordinate in radians. Degrees if `deg` is True \n\
    lon: float \n\
        longitude coordinate in radians. Degrees if `deg` is True\n\
    deg: bool, default False \n\
        if True, GeoCoord will use units of degrees, radians will be used \n\
        otherwise. \n\
\n\
    Examples \n\
    -------- \n\
    >>> from h3py import GeoCoord \n\
    >>> coord = GeoCoord(0.710599509, -1.291647896)\n\
    >>> coord = GeoCoord(40.71435280, -74.00597310, deg=True)\n\
    >>> \n");

static PyTypeObject geocoord_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "h3py.GeoCoord",
    sizeof(geocoord_object),
    0,
    (destructor)geocoord_dealloc,               /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    geocoord_doc,                               /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    geocoord_methods,                           /* tp_methods */
    geocoord_members,                           /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    geocoord_new,                               /* tp_new */
};



static struct PyModuleDef moduledef = {
     PyModuleDef_HEAD_INIT,
     "_h3py",
     NULL,
     -1,
     h3py_methods
};

PyMODINIT_FUNC
PyInit__h3py(void)
{
    PyObject *module = PyModule_Create(&moduledef);

    if (PyType_Ready(&geocoord_type) < 0 ||
        PyType_Ready(&geoboundary_type) < 0 ||
        PyType_Ready(&geoboundaryiter_type) < 0) {

        return NULL;
    }

    Py_INCREF(&geocoord_type);
    if(PyModule_AddObject(module, "GeoCoord", (PyObject *)&geocoord_type) ||
       PyModule_AddObject(module, "GeoBoundary", (PyObject *)&geoboundary_type)) {
        return NULL;
    }
    return module;
}

static geocoord_object *
h3to_geocoord_impl(PyObject *index, PyObject *deg)
{
    H3Index h3 = 0;
    GeoCoord coord;
    PyObject *la = NULL, *lo = NULL;

    if (PyLong_Check(index)) {
        h3 = (H3Index)PyLong_AsLong(index);
        /* TODO: check valid */
        if (!H3_EXPORT(h3IsValid)(h3)) {
            PyErr_SetString(PyExc_ValueError,
                "h3 index is invalid, cannot return GeoCoord");
            return NULL;
        } else {
            H3_EXPORT(h3ToGeo)(h3, &coord);
            if (deg == Py_True) {
                coord.lat = rad2deg(coord.lat);
                coord.lon = rad2deg(coord.lon);
            }
            la = PyFloat_FromDouble(coord.lat);
            lo = PyFloat_FromDouble(coord.lon);
        }
    } else {
        PyErr_Format(PyExc_TypeError,
            "'h3to_geocoord' expected type 'int' but received '%s' instead.",
            Py_TYPE(index)->tp_name);
        return NULL;
    }

    if (la == NULL || lo == NULL) {
        PyErr_Format(PyExc_RuntimeError,
            "unknown exception occured in file %s line (%d)", __FILE__, __LINE__);
        return NULL;
    } else {
        return geocoord_new_impl(&geocoord_type,   la, lo, deg);
    }
}

static PyObject *
h3to_geocoord(PyObject *m, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"h3", "deg", NULL};

    PyObject *h3 = NULL, *deg = NULL;
    int ok;

    ok = PyArg_ParseTupleAndKeywords(args, kwds, "O|O:h3to_geocoord",
                                     kwlist, &h3, &deg);
    if (!ok) {
        PyErr_SetString(PyExc_RuntimeError,
            "error occured parsing arguments for function 'h3to_geocoord'");
        return NULL;
    } else {
        if (deg == NULL) {
            deg = Py_False;
            Py_INCREF(deg);
        }

        return (PyObject *)h3to_geocoord_impl(h3, deg);
    }
}


static geoboundary_object *
geoboundary_new_impl(PyTypeObject *tp, PyObject *seq)
{
    geoboundary_object *self;
    geocoord_object *item;
    PyObject *it, *iteritem, *(*next)(PyObject *), *type;
    GeoBoundary gb;

    it = PyObject_GetIter(seq);
    if (it == NULL)
        return NULL;

    next = *Py_TYPE(it)->tp_iternext;
    type = (PyObject *)tp;
    int i;
    for (i = 0;;++i) {
        iteritem = next(it);
        if (iteritem == NULL)
            break;
        if (!PyObject_IsInstance(iteritem, type)) {
            PyErr_Format(PyExc_TypeError,
                "invalid input type '%s' found in input sequence",
                Py_TYPE(iteritem)->tp_name);
            return NULL;
        }
        item = (geocoord_object *)iteritem;
        if (i >= 10) {
            PyErr_SetString(PyExc_OverflowError,
                "iterable contains too many values. GeoBoundary is 10 items max");
            return NULL;
        }
        gb.verts[i].lat = item->ob_val.lat;
        gb.verts[i].lon = item->ob_val.lon;
    }
    gb.numVerts = i + 1;

    self = (geoboundary_object *)tp->tp_alloc(tp, 0);
    if (self == NULL) {
        PyErr_SetString(PyExc_MemoryError, "");
        return NULL;
    }
    self->ob_val = gb;
    return self;

}

static PyObject *
geoboundary_new(PyTypeObject *tp, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"verts", NULL};

    geoboundary_object *self;
    PyObject *seq = NULL;
    int ok;

    ok = PyArg_ParseTupleAndKeywords(args, kwds, "O:GeoBoundary.__new__",
                                     kwlist, &seq);
    if (!ok) {
        return NULL;
    }

    self = geoboundary_new_impl(tp, seq);
    if (self == NULL)
        return NULL;

    return (PyObject *)self;
}
static void geoboundary_dealloc(geoboundary_object *self);

static void
geoboundary_dealloc(geoboundary_object *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyMemberDef geoboundary_members[] = {
    {NULL}
};

static PyMethodDef geoboundary_methods[] = {
    {NULL}
};

PyDoc_STRVAR(geoboundary_doc,
"GeoBoundary(geocoords) - GeoBoundary object with latitude and \n\
    Parameters \n\
    ---------- \n\
\n\
    Examples \n\
    -------- \n\
    >>> from h3py import GeoCoord, h3to_geoboundary \n\
    >>> \n");

static PyTypeObject geoboundary_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "h3py.GeoBoundary",
    sizeof(geoboundary_object),
    0,
    (destructor)geoboundary_dealloc,            /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    geoboundary_doc,                            /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    geoboundary_iter,                           /* tp_iter */
    0,                                          /* tp_iternext */
    geoboundary_methods,                        /* tp_methods */
    geoboundary_members,                        /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    geoboundary_new,                            /* tp_new */
};

/********************** geoboundary_iterator ********************************/

static void
geoboundaryiter_dealloc(geoboundaryiter_object *it)
{
    _PyObject_GC_UNTRACK(it);
    Py_XDECREF(it->it_seq);
    PyObject_GC_Del(it);
}

static int
geoboundaryiter_traverse(geoboundaryiter_object *it, visitproc visit, void *arg)
{
    Py_VISIT(it->it_seq);
    return 0;
}

static PyObject *
geoboundaryiter_next(geoboundaryiter_object *it)
{
    PyObject *la, *lo, *item;
    GeoCoord coord;
    geoboundary_object *ob;

    assert(it != NULL);
    ob = it->it_seq;
    if (ob == NULL)
        return NULL;
    assert(Py_TYPE(ob) == &geoboundary_type);

    if (it->it_index < ob->ob_val.numVerts) {
        coord = ob->ob_val.verts[it->it_index];
        la = PyFloat_FromDouble(coord.lat);
        lo = PyFloat_FromDouble(coord.lon);
        item = (PyObject *)geocoord_new_impl(&geocoord_type, la, lo, Py_False);

        ++it->it_index;
        Py_INCREF(item);
        Py_DECREF(la);
        Py_DECREF(lo);
        return item;
    }

    it->it_seq = NULL;
    Py_DECREF(ob);
    return NULL;
}

static PyObject *
geoboundaryiter_len(geoboundaryiter_object *it)
{
    Py_ssize_t len = 0;
    if (it->it_seq)
        len = PyList_GET_SIZE(it->it_seq) - it->it_index;
    return PyLong_FromSsize_t(len);
}

static PyObject *
geoboundaryiter_reduce(geoboundaryiter_object *it)
{
    if (it->it_seq)
        return Py_BuildValue("N(O)n", _PyObject_GetBuiltin("iter"),
                             it->it_seq, it->it_index);
    else
        return Py_BuildValue("N(())", _PyObject_GetBuiltin("iter"));
}

static PyObject *
geoboundaryiter_setstate(geoboundaryiter_object *it, PyObject *state)
{
    Py_ssize_t index = PyLong_AsSsize_t(state);
    if (index == -1 && PyErr_Occurred())
        return NULL;
    if (it->it_seq != NULL) {
        int size = it->it_seq->ob_val.numVerts;
        if (index < 0)
            index = 0;
        else if (index > size)
            index = size; /* exhausted iterator */
        it->it_index = index;
    }
    Py_RETURN_NONE;
}

PyDoc_STRVAR(gbiter_length_hint_doc,
    "Private method returning an estimate of len(list(it)).");
PyDoc_STRVAR(gbiter_reduce_doc, "Return state information for pickling.");
PyDoc_STRVAR(gbiter_setstate_doc, "Set state information for unpickling.");

static PyMethodDef geoboundaryiter_methods[] = {
    {"__length_hint__", (PyCFunction)geoboundaryiter_len, METH_NOARGS,
     gbiter_length_hint_doc},
    {"__reduce__", (PyCFunction)geoboundaryiter_reduce, METH_NOARGS,
     gbiter_reduce_doc},
    {"__setstate__", (PyCFunction)geoboundaryiter_setstate, METH_O,
     gbiter_setstate_doc},
    {NULL}
};

static PyTypeObject geoboundaryiter_type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "geoboundary_iterator",
    sizeof(geoboundaryiter_object),
    0,
    (destructor)geoboundaryiter_dealloc,
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,    /* tp_flags */
    0,                                          /* tp_doc */
    (traverseproc)geoboundaryiter_traverse,     /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    PyObject_SelfIter,                          /* tp_iter */
    (iternextfunc)geoboundaryiter_next,         /* tp_iternext */
    geoboundaryiter_methods,                    /* tp_methods */
    0,
};

static PyObject *
geoboundary_iter(PyObject *ob)
{
    geoboundaryiter_object *it;

    if (Py_TYPE(ob) != &geoboundary_type) {
        PyErr_BadInternalCall();
        return NULL;
    }
    it = PyObject_GC_New(geoboundaryiter_object, &geoboundaryiter_type);
    if (it == NULL)
        return NULL;
    it->it_index = 0;
    Py_INCREF(ob);
    it->it_seq = (geoboundary_object *)ob;
    _PyObject_GC_TRACK(it);
    return (PyObject *)it;
}
/************************** Geofence ****************************************/

// static PyTypeObject geofence_type = {
//     PyVarObject_HEAD_INIT(&PyType_Type, 0)
//     "h3py.Geofence",
//     sizeof(geofence_object),
//     0,
//     (destructor)geofence_dealloc,               /* tp_dealloc */
//     0,                                          /* tp_print */
//     0,                                          /* tp_getattr */
//     0,                                          /* tp_setattr */
//     0,                                          /* tp_reserved */
//     0,                                          /* tp_repr */
//     0,                                          /* tp_as_number */
//     0,                                          /* tp_as_sequence */
//     0,                                          /* tp_as_mapping */
//     0,                                          /* tp_hash */
//     0,                                          /* tp_call */
//     0,                                          /* tp_str */
//     0,                                          /* tp_getattro */
//     0,                                          /* tp_setattro */
//     0,                                          /* tp_as_buffer */
//     Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
//     geofence_doc,                               /* tp_doc */
//     0,                                          /* tp_traverse */
//     0,                                          /* tp_clear */
//     0,                                          /* tp_richcompare */
//     0,                                          /* tp_weaklistoffset */
//     0,                                          /* tp_iter */
//     0,                                          /* tp_iternext */
//     geofence_methods,                           /* tp_methods */
//     geofence_members,                           /* tp_members */
//     0,                                          /* tp_getset */
//     0,                                          /* tp_base */
//     0,                                          /* tp_dict */
//     0,                                          /* tp_descr_get */
//     0,                                          /* tp_descr_set */
//     0,                                          /* tp_dictoffset */
//     0,                                          /* tp_init */
//     0,                                          /* tp_alloc */
//     0,                                          /* tp_new */
// };


/**************************** api functions ********************************/


static geoboundary_object *
h3to_geoboundary_impl(PyObject *index)
{
    H3Index h3;
    GeoBoundary gp;
    geoboundary_object *self;

    if (PyLong_Check(index)) {
        h3 = PyLong_AsLong(index);
    } else {
        PyErr_Format(PyExc_TypeError,
            "expected type 'int' but recieved '%s'",
            Py_TYPE(index)->tp_name);
        return NULL;
    }

    H3_EXPORT(h3ToGeoBoundary)(h3, &gp);
    self = (geoboundary_object *)PyObject_New(geoboundary_object, &geoboundary_type);
    int i;
    for (i = 0; i < gp.numVerts; ++i) {
        self->ob_val.verts[i].lat = gp.verts[i].lat;
        self->ob_val.verts[i].lon = gp.verts[i].lon;
    }
    self->ob_val.numVerts = gp.numVerts;
    return self;
}

static PyObject *
h3to_geoboundary(PyObject *m, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"h3", NULL};

    PyObject *h3 = NULL;
    geoboundary_object *self;

    if (!PyModule_Check(m)) {
        PyErr_BadInternalCall();
        return NULL;
    } else {
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &h3)) {
            return NULL;
        } else {
            assert(h3 != NULL);
        }
    }

    self = h3to_geoboundary_impl(h3);
    if (self == NULL)
        return NULL;

    return (PyObject *)self;
}

static PyObject *
maxkringsize_impl(PyObject *k)
{
    PyObject *size = NULL;
    int kring_size = 0, kin = 0;

    if (PyLong_Check(k)) {
        kin = (int)PyLong_AsLong(k);
    } else {
        PyErr_Format(PyExc_TypeError,
            "'maxkringsize' expected argument type 'int' but received '%s'",
            Py_TYPE(k)->tp_name);
        return NULL;
    }

    kring_size = H3_EXPORT(maxKringSize)(kin);
    size = PyLong_FromLong((long)kring_size);
    if (size == NULL) {
        PyErr_NoMemory();
        return NULL;
    }

    return size;
}

static PyObject *
maxkringsize(PyObject *m, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"k", NULL};

    PyObject *k = NULL, *size;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O:max_kring_size",
                                     kwlist, &k)) {
        return NULL;
    } else {
        if (k == NULL) {
            PyErr_Occurred();
            return NULL;
        }
    }

    size = maxkringsize_impl(k);
    if (size == NULL)
        return NULL;

    return size;
}

static int
hexrange_parseargs(PyObject *m, PyObject *args, PyObject *kwds, PyObject **origin, PyObject **k)
{
    static char *kwlist[] = {"origin", "k", NULL};

    if (!PyModule_Check(m)) {
        PyErr_BadInternalCall();
        return 0;
    } else {
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, origin, k)) {
            return 0;
        } else {
            return 1;
        }
    }
    PyErr_BadInternalCall();
    return 0;
}

static PyObject *hexrangedistances_impl(PyObject *, PyObject *);


static int
check_neighbors_inputs(PyObject *origin, PyObject *k)
{
    if (!PyLong_Check(origin)) {
        PyErr_Format(PyExc_TypeError,
            "expected argument 'origin' (pos 1) to be type 'int'"
             " but got '%s'", Py_TYPE(origin)->tp_name);
        return 0;
    } else if (!PyLong_Check(k)) {
        PyErr_Format(PyExc_TypeError,
            "expected argments 'k' (pos 2) to be type 'int' "
            "but got '%s'", Py_TYPE(k)->tp_name);
        return 0;
    }
    return 1;
}

static PyObject *
hexrange_impl(PyObject *origin, PyObject *k)
{
    PyObject *ranges, *out, *tup, *item;
    Py_ssize_t i;

    ranges = hexrangedistances_impl(origin, k);
    if (ranges == NULL)
        return NULL;
    out = PyList_New(PyList_Size(ranges));
    for (i = 0; i < PyList_Size(ranges); ++i) {
        tup = PyList_GetItem(ranges, i);
        if (!PyTuple_Check(tup)) {
            PyErr_BadInternalCall();
            return NULL;
        }
        item = PyTuple_GetItem(tup, 0);
        Py_INCREF(item);
        PyList_SetItem(out, i, item);
    }
    Py_DECREF(ranges);

    return out;
}

static PyObject *
hexrange(PyObject *m, PyObject *args, PyObject *kwds)
{

    PyObject *origin, *k, *ob;
    if (!hexrange_parseargs(m, args, kwds, &origin, &k)) {
        return NULL;
    } else {
        ob = hexrange_impl(origin, k);
        if (ob == NULL)
            return NULL;

        return ob;
    }

    PyErr_BadInternalCall();
    return NULL;
}

static PyObject *
hexrangedistances_impl(PyObject *origin, PyObject *k)
{
    PyObject *max_size, *item;
    PyObject *out;
    int cmax_size = 0, ck = 0;
    H3Index corigin;

    max_size = maxkringsize_impl(k);
    if (!check_neighbors_inputs(origin, k)) {
        return NULL;
    } else if (!PyLong_Check(max_size)) {
        PyErr_BadInternalCall();
        return NULL;
    } else {
        cmax_size = PyLong_AsLong(max_size);
        ck = PyLong_AsLong(k);
        corigin = PyLong_AsLong(origin);

        if (!H3_EXPORT(h3IsValid)(corigin)) {
            PyErr_Format(PyExc_ValueError,
                "invalid index '%S'", origin);
            return NULL;
        }

        H3Index cout[cmax_size];
        int cdistances[cmax_size];

        H3_EXPORT(kRingDistances)(corigin, ck, cout, cdistances);

        out = PyList_New(cmax_size);

        Py_ssize_t i;
        for (i = 0; i < PyList_Size(out); ++i) {
            item = Py_BuildValue("(Ki)", cout[i], cdistances[i]);
            PyList_SET_ITEM(out, i, item);
        }

        return out;
    }
    PyErr_BadInternalCall();
    return NULL;
}

static PyObject *
hexrangedistances(PyObject *m, PyObject *args, PyObject *kwds)
{

    PyObject *origin, *k;
    if (!hexrange_parseargs(m, args, kwds, &origin, &k)) {
        return NULL;
    } else {
        return hexrangedistances_impl(origin, k);
    }

    PyErr_BadInternalCall();
    return NULL;
}


static PyObject *
hexranges_impl(PyObject *origins, PyObject *k)
{
    /* it's easier to just implemented this myself than wrap their
    hexranges code */
    PyObject *origin, *out, *ranges, *it, *item, *(* next)(PyObject *);
    Py_ssize_t i, j, num_elements;

    if (!PyList_CheckExact(origins)) {
        PyErr_Format(PyExc_TypeError,
            "hex_ranges excepted argument type 'list' (pos 1) but received '%s'",
            Py_TYPE(origins)->tp_name);
        return NULL;
    } else if (!PyLong_Check(k)){
        PyErr_Format(PyExc_TypeError,
            "argument 'k' (pos 2) expected type 'int' but received '%s'",
            Py_TYPE(k)->tp_name);
        return NULL;
    } else {
        num_elements = PyLong_AsSsize_t(maxkringsize_impl(k));
        out = PyList_New(PyList_Size(origins) * num_elements);
        it = PyObject_GetIter(origins);
        if (it == NULL)
            return NULL;
        next = Py_TYPE(it)->tp_iternext;

        for (i = 0;; ++i) {
            origin = next(it);
            if (origin == NULL)
                break;
            if (!PyLong_Check(origin)) {
                PyErr_Format(PyExc_TypeError,
                    "sequence must only contain type 'int', not '%s'",
                    Py_TYPE(origin)->tp_name);
                return NULL;
            }
            ranges = hexrange_impl(origin, k);
            for (j = 0; j < PyList_Size(ranges); ++j) {
                item = PyList_GetItem(ranges, j);
                if (item == NULL) {
                    PyErr_Occurred();
                    return NULL;
                }
                PyList_SET_ITEM(out, i * num_elements + j, item);
                Py_INCREF(item);
            }
            Py_DECREF(ranges);
        }
        return out;
    }

    PyErr_BadInternalCall();
    return NULL;
}


static PyObject *
hexranges(PyObject *m, PyObject *args, PyObject *kwds)
{
    PyObject *origins, *k;
    if (!hexrange_parseargs(m, args, kwds, &origins, &k)) {
        return NULL;
    } else {
        return hexranges_impl(origins, k);
    }

    PyErr_BadInternalCall();
    return NULL;
}


static PyObject *
kring_impl(PyObject *origin, PyObject *k)
{
    PyObject *kring_size, *ring, *item;
    int kring_size_c, i;
    Py_ssize_t j;

    if (!check_neighbors_inputs(origin, k)) {
        return NULL;
    } else {
        kring_size = maxkringsize_impl(k);
        ring = PyList_New(PyLong_AsSsize_t(kring_size));
        kring_size_c = (int)PyLong_AsLong(kring_size);

        H3Index out[kring_size_c];
        H3_EXPORT(kRing)(PyLong_AsLong(origin), PyLong_AsLong(k), out);

        for (i = 0, j = 0; i < kring_size_c; ++i, ++j) {
            item = PyLong_FromLongLong(out[i]);
            PyList_SET_ITEM(ring, j, item);
        }
        return ring;
    }

    PyErr_BadInternalCall();
    return NULL;
}

static PyObject *
kring(PyObject *m, PyObject *args, PyObject *kwds)
{
    PyObject *origin, *k;
    if (!hexrange_parseargs(m, args, kwds, &origin, &k)) {
        return NULL;
    } else {
        return kring_impl(origin, k);
    }
}

static PyObject *
kring_distances_impl(PyObject *origin, PyObject *k)
{
    PyObject *ranges, *tup, *max_kring;
    Py_ssize_t i;
    int j;
    size_t size;

    max_kring = maxkringsize_impl(k);
    if (!check_neighbors_inputs(origin, k)) {
        return NULL;
    } else if (max_kring == NULL) {
        return NULL;
    } else {
        size = (size_t)PyLong_AsLong(max_kring);
        H3Index out[size];
        int distances[size];

        H3_EXPORT(kRingDistances)(PyLong_AsLong(origin), PyLong_AsLong(k),
                                  out, distances);

        ranges = PyList_New(PyLong_AsSsize_t(max_kring));
        for (i = 0, j = 0; i < PyLong_AsSsize_t(max_kring); ++i, ++j) {
            tup = Py_BuildValue("(Ki)", out[i], distances[i]);
            PyList_SetItem(ranges, j, tup);
        }

        return ranges;
    }
    PyErr_BadInternalCall();
    return NULL;
}

static PyObject *
kring_distances(PyObject *m, PyObject *args, PyObject *kwds)
{
    PyObject *origin, *k;
    if (!hexrange_parseargs(m, args, kwds, &origin, &k)) {
        return NULL;
    } else {
        return kring_distances_impl(origin, k);
    }
}

static PyObject *
hexring_impl(PyObject *origin, PyObject *k)
{
    PyObject *item, *num_items, *ring;
    Py_ssize_t idx;

    if (!check_neighbors_inputs(origin, k)) {
        return NULL;
    } else {
        num_items = maxkringsize_impl(k);
        if (num_items == NULL)
            return NULL;

        H3Index out[PyLong_AsLong(num_items)];
        H3_EXPORT(hexRing)(PyLong_AsLong(origin), PyLong_AsLong(k), out);

        ring = PyList_New(PyLong_AsSsize_t(num_items));
        for (idx = 0; idx < PyList_Size(ring); ++idx) {
            item = PyLong_FromLong(*(out + (int)idx));
            PyList_SET_ITEM(ring, idx, item);
        }
        return ring;
    }
    PyErr_BadInternalCall();
    return NULL;
}

static PyObject *
hexring(PyObject *m, PyObject *args, PyObject * kwds)
{
    PyObject *origin, *k;
    if (!hexrange_parseargs(m, args, kwds, &origin, &k)) {
        return NULL;
    } else {
        return hexring_impl(origin, k);
    }
}

static int
check_res(PyObject **res, const char *func, int pos)
{
    PyObject *ob = *res;
    if (PyLong_Check(ob)) {
        return 1;
    }
    PyErr_Format(PyExc_TypeError,
        "%s expected type 'int' (pos %d) but recieved '%s'",
        func, pos, Py_TYPE(ob)->tp_name);
    return 0;
}

static PyObject *
hexarea_impl(PyObject *res, PyObject *u)
{
    double area = -1;
    PyObject *b;
    const char *units;

    if (!check_res(&res, "hex_area()", 1)) {
        return NULL;
    } else if (!PyUnicode_Check(u)) {
        PyErr_Format(PyExc_TypeError,
            "%s expected type 'str' (pos 2) but recieved '%s'",
            "hex_area", Py_TYPE(u)->tp_name);
        return NULL;
    } else {
        // Inputs okay
        b = PyUnicode_AsASCIIString(u);
        if (b == NULL)
            return PyErr_Occurred();
        units = PyBytes_AsString(b);
        if (units == NULL)
            return PyErr_Occurred();

        if (strcmp(units, "km2") == 0)
            area = H3_EXPORT(hexAreaKm2)(PyLong_AsLong(res));
        else if (strcmp(units, "m2") == 0)
            area = H3_EXPORT(hexAreaM2)(PyLong_AsLong(res));
        else {
            PyErr_Format(PyExc_ValueError,
                "%s expected one of {'km2', 'm2'} (pos 2) but recieved '%s'",
                "hex_area()", units);
            Py_DECREF(b);
            return NULL;
        }
        Py_DECREF(b);

        if (area < 0) {
            PyErr_Format(PyExc_RuntimeError,
                "internal error: %s in hex_area() (line %d)", __FILE__, __LINE__);
            return NULL;
        } else {
            return PyFloat_FromDouble(area);
        }

    }

    PyErr_BadInternalCall();
    return NULL;
}

static PyObject *
hexarea(PyObject *m, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"res", "units", NULL};
    PyObject *u, *res;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, &res, &u)) {
        return NULL;
    } else {
        return hexarea_impl(res, u);
    }
}

static PyObject *
edgelength_impl(PyObject *res, PyObject *u)
{
    PyObject *b;
    const char *s;
    double edge_len = -1;

    if (!check_res(&res, "edge_length", 1)) {
        return NULL; /* error string set in check_res */
    } else if (!PyUnicode_Check(u)) {
        PyErr_Format(PyExc_TypeError,
            "h3py.edge_length(): expected type 'str' (pos 2) but recieved '%s'",
            Py_TYPE(u)->tp_name);
        return NULL;
    } else { /* input types okay */
        b = PyUnicode_AsASCIIString(u);
        if (b == NULL)
            return PyErr_Occurred();
        s = PyBytes_AsString(b);
        if (s == NULL)
            return PyErr_Occured();
        if (strcmp(s, "km") == 0) {
            edge_len = H3_EXPORT(edgeLengthKm)(PyLong_AsLong(res));
        } else if (strcmp(s, "m") == 0) {
            edge_len = H3_EXPORT(edgeLengthM)(PyLong_AsLong(res));
        } else {
            PyErr_Format(PyExc_ValueError,
                "expected one of {'km', 'm'} (pos 2) but recieved '%s'", s);
            Py_DECREF(b);
            return NULL;
        }
        Py_DECREF(b);
        if (edge_len < 0) {
            PyErr_Format(PyExc_RuntimeError,
                "internal error: %s in edge_length() (line %d)", __FILE__, __LINE__);
            return NULL;
        } else {
            return PyFloat_FromDouble(edge_len);
        }
    }
    PyErr_BadInternalCall();
    return NULL;
}

static PyObject *
edgelength(PyObject *m, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"res", "units", NULL};

    PyObject *u, *res;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, &res, &u)) {
        return NULL;
    } else {
        return edgelength_impl(res, u);
    }
}
