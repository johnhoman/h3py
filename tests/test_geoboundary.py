import pytest
from h3py import GeoBoundary, GeoCoord
import numpy as np


def test_GeoBoundary_init_error_on_int():
    coord_values = zip(np.random.random(5) * np.pi,
                       np.random.random(5) * np.pi)
    geo_coords = [GeoCoord(lat, lon) for lat, lon in coord_values] + [5]
    with pytest.raises(TypeError):
        GeoBoundary(geo_coords)

def test_GeoBoundary_init_error_on_float():
    coord_values = zip(np.random.random(6) * np.pi,
                       np.random.random(6) * np.pi)
    geo_coords = [5.4] + [GeoCoord(lat, lon) for lat, lon in coord_values]
    with pytest.raises(TypeError):
        GeoBoundary(geo_coords)

def test_GeoBoundary_init_error_on_str():
    coord_values = zip(np.random.random(3) * np.pi,
                       np.random.random(3) * np.pi)
    geo_coords = [GeoCoord(lat, lon) for lat, lon in coord_values]
    geo_coords.insert(4, 'mittens')

    with pytest.raises(TypeError):
        GeoBoundary(geo_coords)

def test_GeoBoundary_init_error_overflow():
    coord_values = zip(np.random.random(11) * np.pi,
                       np.random.random(11) * np.pi)
    geo_coords = [GeoCoord(lat, lon) for lat, lon in coord_values]
    with pytest.raises(ValueError):
        GeoBoundary(geo_coords)

def test_GeoBoundary_raises_tuple():
    coord_values = zip(np.random.random(5) * np.pi,
                       np.random.random(5) * np.pi)
    geo_coords = tuple(GeoCoord(lat, lon) for lat, lon in coord_values)
    with pytest.raises(TypeError):
        GeoBoundary(geo_coords)


def test_GeoBoundary_raises_generator():
    coord_values = zip(np.random.random(5) * np.pi,
                       np.random.random(5) * np.pi)
    with pytest.raises(TypeError):
        GeoBoundary(GeoCoord(lat, lon) for lat, lon in coord_values)
