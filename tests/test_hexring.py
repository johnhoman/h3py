import pytest
from h3py import GeoCoord, H3Index, hex_ring, set_h3_index

lat_rad, lon_rad = 0.659966917655, 2 * 3.14159 - 2.1364398519396
sf = GeoCoord(lat_rad, lon_rad)
sfhex = sf.to_h3(9)

def test_ring1():

    expectedk1 = [
        H3Index(0x89283080ddbffff),
        H3Index(0x89283080c37ffff),
        H3Index(0x89283080c27ffff),
        H3Index(0x89283080d53ffff),
        H3Index(0x89283080dcfffff),
        H3Index(0x89283080dc3ffff)
    ]

    k1 = hex_ring(sfhex, 1)

    for i in range(6):
        assert k1[i] != 0, "index is not populated"
        in_list = 0
        for j in range(6):
            if k1[i] == expectedk1[j]:
                in_list += 1

        assert in_list == 1, "index not found in expected set"


def test_ring2():

    expectedk2 = [
        H3Index(0x89283080ca7ffff),
        H3Index(0x89283080cafffff),
        H3Index(0x89283080c33ffff),
        H3Index(0x89283080c23ffff),
        H3Index(0x89283080c2fffff),
        H3Index(0x89283080d5bffff),
        H3Index(0x89283080d43ffff),
        H3Index(0x89283080d57ffff),
        H3Index(0x89283080d1bffff),
        H3Index(0x89283080dc7ffff),
        H3Index(0x89283080dd7ffff),
        H3Index(0x89283080dd3ffff)
    ]

    k2 = hex_ring(sfhex, 2)

    for i in range(12):
        assert k2[i] !=  0, "index is not populated"
        inlist = 0
        for j in range(12):
            if k2[i] == expectedk2[j]:
                inlist += 1
        assert inlist == 1, "index not found in expected set"

@pytest.fixture
def near_pentagon():
    return H3Index(0x837405fffffffff)


def test_near_pentagon_ring_1(near_pentagon):
    with pytest.raises(RuntimeError):
        hex_ring(near_pentagon, 1)


def test_near_pentagon_ring_2(near_pentagon):
    with pytest.raises(RuntimeError):
        hex_ring(near_pentagon, 2)


def test_on_pentagon(near_pentagon):
    set_h3_index(near_pentagon, 0, 4, 0)

    with pytest.raises(RuntimeError):
        hex_ring(near_pentagon, 2)
