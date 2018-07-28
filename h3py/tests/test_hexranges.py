from h3py import GeoCoord, geo_to_h3, hex_ranges, H3Index, hex_range
import pytest

lat = 0.65996691765
lon = 2 * 3.14159 - 2.1364398519396
sf = GeoCoord(lat, lon)
sfhex = geo_to_h3(sf, 9)


k1 = [
    H3Index(0x89283080ddbffff),
    H3Index(0x89283080c37ffff),
    H3Index(0x89283080c27ffff),
    H3Index(0x89283080d53ffff),
    H3Index(0x89283080dcfffff),
    H3Index(0x89283080dc3ffff)
]


def test_identity_kring():
    k0 = hex_ranges([sfhex], 0)
    assert k0[0] == sfhex, "failed to generate identity k-ring"
    k0 = hex_range(sfhex, 0)
    assert k0[0] == sfhex


def test_ring1of1():
    allkrings = hex_ranges(k1, 1)

    for i, kring in enumerate(allkrings):
        assert kring, "failed to create Kring"
        if i % 7 == 0:
            index = i // 7
            assert k1[index] == kring, \
                "The beginning of the segment is the incorrect hexagon."

    for h3 in k1:
        krings = hex_range(h3, 1)
        assert krings[0] == h3


def test_ring2of1():
    allkrings = hex_ranges(k1, 2)

    for k, kring in enumerate(allkrings):
        assert kring != 0, "index is not populated"

        if k % (1 + 6 + 12) == 0:
            index = k // (1 + 6 + 12)
            assert k1[index] == kring, \
                "The beginning of the segment is the incorrect hexagon"
