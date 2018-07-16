import pytest
from h3py import H3Index


def test_get_resolution():
    # assert H3Index(0xcafe).get_resolution() == 1
    # assert H3Index(0xabcdef).get_resolution() == 1
    pass

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
