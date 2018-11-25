import pytest
from h3py import k_ring, GeoCoord, H3Index, geo_to_h3, k_ring_distances, set_h3_index


def test_k_ring_counts():
    lat = 0.659966917655
    lon = 2 * 3.14159 - 2.1364398519396
    sf = GeoCoord(lat, lon)
    sfhex0 = geo_to_h3(sf, 9)

    for i in range(16):
        k, dist = k_ring_distances(sfhex0, i)
        assert all(h3.is_valid() for h3 in k)
        length = 1
        for j in range(i, -1, -1):
            length += 6 * j
        assert len(k) == length


def test_k_ring0():
    lat = 0.659966917655
    lon = 2 * 3.14159 - 2.1364398519396
    sf = GeoCoord(lat, lon)
    sfhex0 = geo_to_h3(sf, 0)
    expectedk1 = [
        H3Index(0x8029fffffffffff),
        H3Index(0x801dfffffffffff),
        H3Index(0x8013fffffffffff),
        H3Index(0x8027fffffffffff),
        H3Index(0x8049fffffffffff),
        H3Index(0x8051fffffffffff),
        H3Index(0x8037fffffffffff)
    ]

    k1, k1dist = k_ring_distances(sfhex0, 1)

    assert all(i.is_valid() for i in k1), "Indexes not populated"

    for i in range(7):
        in_list = 0
        for j in range(7):
            if k1[i] == expectedk1[j]:
                assert k1dist[i] == (0 if k1[i] == sfhex0 else 1)
                in_list += 1

        assert in_list == 1, "index not found in expected set"


def test_k_ring0_polar_pentagon():

    polar = H3Index.set(0, 4, 0)
    expectedk2 = [
        H3Index(0x8009fffffffffff),
        H3Index(0x8007fffffffffff),
        H3Index(0x8001fffffffffff),
        H3Index(0x8011fffffffffff),
        H3Index(0x801ffffffffffff),
        H3Index(0x8019fffffffffff),
        H3Index(0)
    ]

    k2, k2dist = k_ring_distances(polar, 1)

    k2present = 0
    for i in range(7):
        if k2[i] != H3Index(0):
            k2present += 1
            in_list = 0
            for j in range(7):
                if k2[i] == expectedk2[j]:
                    assert k2dist[i] == (0 if k2[i] == polar else 1), \
                        "distance is not as expected"
                    in_list += 1

            assert in_list == 1, "index not found in expected set"

    assert k2present == 6, \
       "pentagon does not have 6 neighbors, it has %d" % k2present

@pytest.mark.skip(reason="can't set polar h3index yet")
def test_k_ring1_polar_pentagon():
    polar = None
