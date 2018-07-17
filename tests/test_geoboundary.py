import pytest
from h3py import GeoBoundary, GeoCoord
import numpy as np

@pytest.fixture
def coord_values():
    coord_values = zip(np.random.random(5) * np.pi,
                       np.random.random(5) * np.pi)
    return list(coord_values)


def test_GeoBoundary_init_error_on_int(coord_values):
    geo_coords = [GeoCoord(lat, lon) for lat, lon in coord_values] + [5]
    with pytest.raises(TypeError):
        GeoBoundary(geo_coords)

def test_GeoBoundary_init_error_on_float(coord_values):
    geo_coords = [5.4] + [GeoCoord(lat, lon) for lat, lon in coord_values]
    with pytest.raises(TypeError):
        GeoBoundary(geo_coords)

def test_GeoBoundary_init_error_on_str(coord_values):
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

def test_GeoBoundary_raises_tuple(coord_values):
    geo_coords = tuple(GeoCoord(lat, lon) for lat, lon in coord_values)
    with pytest.raises(TypeError):
        GeoBoundary(geo_coords)


def test_GeoBoundary_raises_generator(coord_values):
    with pytest.raises(TypeError):
        GeoBoundary(GeoCoord(lat, lon) for lat, lon in coord_values)
