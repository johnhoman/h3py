from h3py import H3Index, GeoCoord
import numpy as np
import pytest
from glob import glob
from os.path import join, exists
from math import radians
import sys


def test_get_set_lat_lon():
    """Verify the getters and setters from the c
    end are working
    """
    ray = zip(np.random.random(100) * 180,
              np.random.random(100) * 180)
    for lat, lon in ray:
        coordinates = GeoCoord(lat, lon)
        coordinates.lat = lat - 5
        coordinates.lon = lon - 5
        assert coordinates.lat == lat - 5
        assert coordinates.lon == lon - 5

def test_to_h3():
    """convert lat/lon to H3 and verify"""

    test_path = 'h3/tests/inputfiles'
    if not exists(test_path):
        assert exists(test_path), "Missing test files"

    target = join(test_path, 'rand*centers.txt')
    log = open('output.log', 'w+')

    def assert_expected(h1: H3Index, g1: GeoCoord):
        assert isinstance(h1, H3Index)
        assert isinstance(g1, GeoCoord)
        h2 = g1.to_h3(h1.get_resolution())
        print('h2 string', h2.to_string())
        print('h1 string', h1.to_string())
        assert h1 == h2, "got unexpected GeoCoord.to_h3 output"

    for filename in glob(target):
        with open(filename, 'rt') as test_file:
            test_values = [line.split() for line in test_file]
        for h3_str, lat_degrees, lon_degrees in test_values:
            print('h3 str', h3_str)
            h3 = H3Index.from_string(h3_str)
            print('degrees', lat_degrees, lon_degrees)
            lat = radians(float(lat_degrees))
            lon = radians(float(lon_degrees))
            coord = GeoCoord(lat, lon)
            print('radians', lat, lon)
            assert_expected(h3, coord)

    log.close()
