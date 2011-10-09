#!/usr/bin/env python
#coding=utf-8

"""
python setup.py build  -c mingw32
python setup.py install --skip-build
"""

from os.path import join
from distutils.core import setup, Extension

setup (name = 'dwmri',
       version = '1.0',
       description = 'A Python package for diffusion MRI.',
       author = 'Bing Jian',
       author_email = 'bing.jian@gmail.com',
       url = 'http://diffusion-mri.googlecode.com/',
       long_description = '''
       This is a Python package for diffusion MRI.
       ''',
       package_dir={'dwmri': ''},
       packages=['dwmri'],
      )
