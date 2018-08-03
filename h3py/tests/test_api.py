import pandas as pd
import numpy as np
from h3py import GeoCoord

test_coords = [
    {'lat': 32.806671, 'lon': -86.791130, 'state': "Alabama"},
    {'lat': 61.370716, 'lon': -152.40441, 'state': "Alaska"},
    {'lat': 33.729759, 'lon': -111.43122, 'state': "Arizona"},
    {'lat': 34.969704, 'lon': -92.373123, 'state': "Arkansas"},
    {'lat': 36.116203, 'lon': -119.68156, 'state': "California"},
    {'lat': 39.059811, 'lon': -105.31110, 'state': "Colorado"},
    {'lat': 41.597782, 'lon': -72.755371, 'state': "Connecticut"},
    {'lat': 39.318523, 'lon': -75.507141, 'state': "Delaware"},
    {'lat': 38.897438, 'lon': -77.026817, 'state': "DistrictOfColumbia"},
    {'lat': 27.766279, 'lon': -81.686783, 'state': "Florida"},
    {'lat': 33.040619, 'lon': -83.643074, 'state': "Georgia"},
    {'lat': 21.094318, 'lon': -157.49833, 'state': "Hawaii"},
    {'lat': 44.240459, 'lon': -114.47882, 'state': "Idaho"},
    {'lat': 40.349457, 'lon': -88.986137, 'state': "Illinois"},
    {'lat': 39.849426, 'lon': -86.258278, 'state': "Indiana"},
    {'lat': 42.011539, 'lon': -93.210526, 'state': "Iowa"},
    {'lat': 38.526600, 'lon': -96.726486, 'state': "Kansas"},
    {'lat': 37.668140, 'lon': -84.670067, 'state': "Kentucky"},
    {'lat': 31.169546, 'lon': -91.867805, 'state': "Louisiana"},
    {'lat': 44.693947, 'lon': -69.381927, 'state': "Maine"},
    {'lat': 39.063946, 'lon': -76.802101, 'state': "Maryland"},
    {'lat': 42.230171, 'lon': -71.530106, 'state': "Massachusetts"},
    {'lat': 43.326618, 'lon': -84.536095, 'state': "Michigan"},
    {'lat': 45.694454, 'lon': -93.900192, 'state': "Minnesota"},
    {'lat': 32.741646, 'lon': -89.678696, 'state': "Mississippi"},
    {'lat': 38.456085, 'lon': -92.288368, 'state': "Missouri"},
    {'lat': 46.921925, 'lon': -110.45435, 'state': "Montana"},
    {'lat': 41.125370, 'lon': -98.268082, 'state': "Nebraska"},
    {'lat': 38.313515, 'lon': -117.05537, 'state': "Nevada"},
    {'lat': 43.452492, 'lon': -71.563896, 'state': "NewHampshire"},
    {'lat': 40.298904, 'lon': -74.521011, 'state': "NewJersey"},
    {'lat': 34.840515, 'lon': -106.24848, 'state': "NewMexico"},
    {'lat': 42.165726, 'lon': -74.948051, 'state': "NewYork"},
    {'lat': 35.630066, 'lon': -79.806419, 'state': "NorthCarolina"},
    {'lat': 47.528912, 'lon': -99.784012, 'state': "NorthDakota"},
    {'lat': 40.388783, 'lon': -82.764915, 'state': "Ohio"},
    {'lat': 35.565342, 'lon': -96.928917, 'state': "Oklahoma"},
    {'lat': 44.572021, 'lon': -122.07093, 'state': "Oregon"},
    {'lat': 40.590752, 'lon': -77.209755, 'state': "Pennsylvania"},
    {'lat': 41.680893, 'lon': -71.511780, 'state': "RhodeIsland"},
    {'lat': 33.856892, 'lon': -80.945007, 'state': "SouthCarolina"},
    {'lat': 44.299782, 'lon': -99.438828, 'state': "SouthDakota"},
    {'lat': 35.747845, 'lon': -86.692345, 'state': "Tennesse"},
    {'lat': 31.054487, 'lon': -97.563461, 'state': "Texas"},
    {'lat': 40.150032, 'lon': -111.86243, 'state': "Utah"},
    {'lat': 44.045876, 'lon': -72.710686, 'state': "Vermont"},
    {'lat': 37.769337, 'lon': -78.169968, 'state': "Virginia"},
    {'lat': 47.400902, 'lon': -121.49049, 'state': "Washington"},
    {'lat': 38.491226, 'lon': -80.954453, 'state': "WestVirginia"},
    {'lat': 44.268543, 'lon': -89.616508, 'state': "Wisconsin"},
    {'lat': 42.755966, 'lon': -107.30249, 'state': "Wyoming"},
]

def test_tag_frame_state():

    test_df = pd.DataFrame(test_coords, dtype=np.float32)

    def tag(row, res):
        h3 = GeoCoord.from_degrees(row['lat'], row['lon']).to_h3(res)
        row['h3'] = int(h3)

        return row

    assert test_df.apply(lambda x: tag(x, 5), axis=1).groupby('h3').ngroups == len(test_coords)
    assert test_df.apply(lambda x: tag(x, 4), axis=1).groupby('h3').ngroups == 50
    assert test_df.apply(lambda x: tag(x, 3), axis=1).groupby('h3').ngroups == 49
    assert test_df.apply(lambda x: tag(x, 2), axis=1).groupby('h3').ngroups == 43
    assert test_df.apply(lambda x: tag(x, 1), axis=1).groupby('h3').ngroups == 21
    assert test_df.apply(lambda x: tag(x, 0), axis=1).groupby('h3').ngroups == 7
