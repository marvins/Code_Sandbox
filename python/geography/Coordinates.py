
import math

class CoordinateGeographic:

    def __init__(self, lat_deg, lon_deg, alt_met):
        self.lat_rad = lat_deg * math.pi / 180.0
        self.lon_rad = lon_deg * math.pi / 180.0
        self.alt_met = alt_met

    def Get_Latitude_Degrees(self):
        return self.lat_rad * 180.0 / math.pi
    
    def Get_Longitude_Degrees(self):
        return self.lon_rad * 180.0 / math.pi


class CoordinateCartesian:
    
    def __init__( self, x, y, z):
        self.x = x
        self.y = y
        self.z = z


class CoordinateECEF(CoordinateCartesian):
    
    def __init__(self, x, y, z):
        CoordinateCartesian.__init__(self, x, y, z)

