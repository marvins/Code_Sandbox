#     File:    Projections.py
#     Author:  Marvin Smith
#     Date:    11/30/2015
#


#  Python Libraries
import math, logging


class Projector_Base(object):
    
    def __init__(self, projector_args):

        #  Set args
        self.projector_args = projector_args

        self.deg2rad = math.pi / 180.0
        self.rad2deg = 180.0 / math.pi

    def Transform_Forward( self, point ):
        
        return point


class Projector_Cassini(Projector_Base):
    
    def __init__(self, projector_args):
        
        #  Create Parent
        Projector_Base.__init__(self, projector_args)

    def Transform_Forward( self, point ):
        
        output = [0,0]

        output[0] = math.asin( math.cos(point[1] * self.deg2rad) * math.sin(point[0]*self.deg2rad))
        output[1] = math.atan2( math.tan(point[1] * self.deg2rad),
                                math.cos(point[0] * self.deg2rad))

        return output


def Get_Projector( projection_def, projection_args = None):

    projector = None

    #  Check def
    if projection_def == 'geographic_direct':
        projector = Projector_Base(projection_args)

    elif projection_def == 'cassini':
        projector = Projector_Cassini(projection_args)

    else:
        logging.warning('No projector found for def (' + projection_def + ')')

    return projector

