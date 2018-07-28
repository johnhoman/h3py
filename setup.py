from distutils.core import setup, Extension
from glob import glob
from os.path import abspath, join
import os

UBER_H3 = ('h3py/h3')


h3py_ext = Extension('_h3py',
                      sources=[
                          'h3py/src/h3py.c',
                          'h3py/src/pygeocoord.c',
                          'h3py/src/pyh3index.c',
                          'h3py/src/pygeoboundary.c'
                      ],
                      include_dirs=[
                          'h3py/include',
                          join(UBER_H3, 'src', 'h3lib', 'include')
                      ],
                      library_dirs=[join(UBER_H3, 'lib')],
                      libraries=['h3'],
                      extra_compile_args=['-Wno-unused-function'],
                      undef_macros=['NDEBUG'])

setup(name='_h3py',
      ext_modules=[h3py_ext],
      packages=['h3py'])
