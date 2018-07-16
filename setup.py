from distutils.core import setup, Extension
from glob import glob
from os.path import abspath, join
import os

UBER_H3 = ('h3')


h3py_ext = Extension('h3py',
                      sources=[
                          'src/h3py.c',
                          'src/pygeocoord.c',
                          'src/pyh3index.c'
                      ],
                      include_dirs=[
                          'include',
                          join(UBER_H3, 'src', 'h3lib', 'include')
                      ],
                      library_dirs=[join(UBER_H3, 'lib')],
                      libraries=['h3'],
                      extra_compile_args=['-Wno-unused-function'],
                      undef_macros=['NDEBUG'])

setup(name='h3py', ext_modules=[h3py_ext])
