import pytest
from h3py import H3Index, GeoCoord, GeoBoundary, h3ToGeo
from glob import glob
import os
from math import isclose

def test_get_resolution():
    pass


def geo_almost_equal_threshold(p1, p2, threshold):
    lat = abs(p1.lat - p2.lat) < threshold
    lon = abs(p1.lon - p2.lon) < threshold
    return lat and lon


def assert_expected(h1, g1):
    epsilon = 0.000001 * 0.017453292519943295769236907684886127111
    g2 = h1.to_coord()
    assert geo_almost_equal_threshold(g1, g2, epsilon)


def test_to_geo():
    """bc*r*centers.txt"""
    test_files = glob(os.path.join('tests', 'inputfiles', 'bc*centers.txt'))
    for file in test_files:
        with open(file, 'rt') as test:
            for line in test:
                h3str, latdegs, londegs = line.split()
                h3 = H3Index.from_string(h3str)
                coord = GeoCoord.from_degrees(float(latdegs), float(londegs))
                assert_expected(h3, coord)


def test_to_boundary():
    "bc*cells.txt"
    test_files = glob(os.path.join('tests', 'inputfiles', 'bc*cells.txt'))
    for file in test_files:
        test = open(file, 'rt')
        it = iter(test)
        while True:
            try:
                h3str = next(it)
            except StopIteration:
                test.close()
                break
            h3 = H3Index.from_string(h3str)
            assert next(it).startswith('{'), file
            coordinates = []
            while True:
                line = next(it)
                if line.startswith('}'):
                    break
                lat, lon = map(float, line.split())
                coordinates.append(GeoCoord.from_degrees(lat, lon))
            b1 = h3.to_boundary()
            b2 = GeoBoundary(coordinates)
            assert b1 == b2


def test_to_string():
    """test index to_string method"""
    index = H3Index(0xcafe)
    assert index.to_string() == "cafe", \
        'H3Index.to_string() failed to produce base 16 results'
    with pytest.raises(ValueError):
        H3Index(0xffffffffffffffff).to_string()
    assert H3Index(0xabcdef).to_string() == "abcdef"


def test_from_string():
    """test H3Index.from_string method"""

    with pytest.raises(ValueError):
        H3Index.from_string("")
    with pytest.raises(ValueError):
        H3Index.from_string("**")
    with pytest.raises(ValueError):
        H3Index.from_string("ffffffffffffffff")
