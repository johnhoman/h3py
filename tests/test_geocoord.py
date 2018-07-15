import unittest
import h3py
import numpy as np
import pytest


def test_get_set_lat_lon():
    """Verify the getters and setters from the c
    end are working
    """
    ray = zip(np.random.random(100) * 180,
              np.random.random(100) * 180)
    for lat, lon in ray:
        coordinates = h3py.GeoCoord(lat, lon)
        coordinates.lat = lat - 5
        coordinates.lon = lon - 5
        assert coordinates.lat == lat - 5
        assert coordinates.lon == lon - 5
