#include <Python.h>
#include "structmember.h"
#include <h3api.h>
#include <stddef.h>
#include <stdio.h>

#include "pygeocoord.h"
#include "pyh3index.h"

#ifndef Py_H3_H
#define Py_H3_H
#ifdef __cpluscplus
extern "C" {
#endif

/* fwd declare */


static struct PyModuleDef moduledef = {
     PyModuleDef_HEAD_INIT,
     "h3py",
     NULL,
     -1,
     NULL
};

PyMODINIT_FUNC
PyInit_h3py(void)
{
    PyObject *module = PyModule_Create(&moduledef);

    if (PyType_Ready(&PyGeoCoord_Type) < 0
        || PyType_Ready(&PyH3Index_Type) < 0){
        return NULL;
    }

    Py_INCREF(&PyGeoCoord_Type);
    Py_INCREF(&PyH3Index_Type);
    if(PyModule_AddObject(module, "GeoCoord", (PyObject *)&PyGeoCoord_Type)
       || PyModule_AddObject(module, "H3Index", (PyObject *)&PyH3Index_Type)){
        return NULL;
    }
    return module;
}

#ifdef __cpluscplus
}
#endif

#endif /* H3 def */
