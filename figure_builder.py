#! /usr/bin/env python

from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import *
import matplotlib.pyplot as plt
import matplotlib.colors as clr
import matplotlib as mpl
import numpy as np
from itertools import product, combinations
import math


# World Coordinates
camera_position=np.array([[0],[0],[10]])

# Camera Intrinsics
camera_axis=(1,0,0)
camera_rotation=25

focal_length=2

focal_plane_width=2
focal_plane_height=2

 
##################################
#     Plane-Line Intersection    #
##################################
def plane_line_intersection( p1, p2, N, p3 ):
    
    u = np.dot(np.transpose(N), p3-p1)[0,0]/np.dot(np.transpose(N),p2-p1)[0,0];
    
    return (p1 + u*(p2-p1));
    

#############################################################
#            Quaternion to Rotation Matrix Class            #
#############################################################
class Quaternion:

    rMatrix = np.eye(4,4)
    
    def __init__( self, angle, axis ):
        
        # Make sure we have a unit axis
        uaxis = axis / np.linalg.norm(axis)
		
        # convert angle to radians
        theta = angle * math.pi / 180.0

        # Create quaternion
        r = math.cos(theta/2.0)
        i = np.array([
            math.sin(theta/2.0)*uaxis[0],
            math.sin(theta/2.0)*uaxis[1],
            math.sin(theta/2.0)*uaxis[2]])	
	
        self.rMatrix = np.mat([[ 1 - 2*i[1]*i[1] - 2*i[2]*i[2],     2*i[0]*i[1] + 2*  r *i[2],     2*i[0]*i[2] - 2*  r *i[1]],
	   	                    [     2*i[0]*i[1] - 2* r  *i[2], 1 - 2*i[0]*i[0] - 2*i[2]*i[2],     2*i[1]*i[2] + 2*  r *i[0]],
                               [     2*i[0]*i[2] + 2* r  *i[1],     2*i[1]*i[2] - 2* r  *i[0], 1 - 2*i[0]*i[0] - 2*i[1]*i[1]]])
		


rMatrix = Quaternion(camera_rotation, camera_axis).rMatrix

# Compute the focal vector
focal_vector   =  rMatrix * np.array([[0],[0],[-focal_length]])
focal_plane_tl = (rMatrix * np.array([[-focal_plane_width/2.0],[ focal_plane_height/2.0],[-focal_length]])) + camera_position
focal_plane_tr = (rMatrix * np.array([[ focal_plane_width/2.0],[ focal_plane_height/2.0],[-focal_length]])) + camera_position
focal_plane_bl = (rMatrix * np.array([[-focal_plane_width/2.0],[-focal_plane_height/2.0],[-focal_length]])) + camera_position
focal_plane_br = (rMatrix * np.array([[ focal_plane_width/2.0],[-focal_plane_height/2.0],[-focal_length]])) + camera_position


# Compute the point where the principle axis strikes the surface
gnd_pnt = plane_line_intersection( camera_position, camera_position + focal_vector, np.array([[0],[0],[1]]), np.array([[0],[0],[0]]) )
gnd_tl  = plane_line_intersection( camera_position, focal_plane_tl, np.array([[0],[0],[1]]), np.array([[0],[0],[0]]))
gnd_tr  = plane_line_intersection( camera_position, focal_plane_tr, np.array([[0],[0],[1]]), np.array([[0],[0],[0]]))
gnd_bl  = plane_line_intersection( camera_position, focal_plane_bl, np.array([[0],[0],[1]]), np.array([[0],[0],[0]]))
gnd_br  = plane_line_intersection( camera_position, focal_plane_br, np.array([[0],[0],[1]]), np.array([[0],[0],[0]]))

# Compute the range of values we need to span
maxx = max( gnd_tl[0,0], gnd_tr[0,0], gnd_bl[0,0], gnd_br[0,0])
minx = min( gnd_tl[0,0], gnd_tr[0,0], gnd_bl[0,0], gnd_br[0,0])
maxy = max( gnd_tl[1,0], gnd_tr[1,0], gnd_bl[1,0], gnd_br[1,0])
miny = min( gnd_tl[1,0], gnd_tr[1,0], gnd_bl[1,0], gnd_br[1,0])
maxz = camera_position[2,0]
minz = -1

span = max( maxx-minx, maxy-miny, maxz-minz ) + 2

tlx = (maxx + minx)/2.0 - span/2.0
tly = (maxy + miny)/2.0 - span/2.0
tlz = (maxz + minz)/2.0 - span/2.0


# Create figure structures
fig = plt.figure()
ax = Axes3D(fig)

 
# Draw the ground surface 
ran = [-6, 6]

x = [tlx, tlx+span, tlx+span, tlx]
y = [tly, tly, tly+span, tly+span]
z = [0,0,0,0]
verts = [zip(x, y,z)]
ax.add_collection3d(Poly3DCollection(verts, facecolors=['#D0F5A9'], label='Surface', alpha=0.8))

###################################################
#           Draw the original camera              #
###################################################
# Draw the camera origin
ax.plot( [camera_position[0,0]], 
         [camera_position[1,0]], 
         [camera_position[2,0]], 
         c='b', marker='o', label='Camera Origin')

# Draw the Principle Point
ax.plot( [camera_position[0,0] + focal_vector[0,0]], 
         [camera_position[1,0] + focal_vector[1,0]], 
         [camera_position[2,0] + focal_vector[2,0]], 
         c='#01A9DB', marker='o', label='Principle Point')
         
# Draw the principle axis
prin_axis_x = [camera_position[0,0], gnd_pnt[0,0]]
prin_axis_y = [camera_position[1,0], gnd_pnt[1,0]]
prin_axis_z = [camera_position[2,0], gnd_pnt[2,0]]
verts = [zip( prin_axis_x, prin_axis_y, prin_axis_z)]
ax.plot( prin_axis_x, prin_axis_y, prin_axis_z, label='Principle Axis', linewidth=1)

# Draw the focal plane
# Draw the Principle Point
focal_points_x = [focal_plane_tl[0,0], focal_plane_tr[0,0], focal_plane_br[0,0], focal_plane_bl[0,0], focal_plane_tl[0,0], camera_position[0,0], gnd_tl[0,0], camera_position[0,0], gnd_tr[0,0], camera_position[0,0], gnd_bl[0,0], camera_position[0,0], gnd_br[0,0]]
focal_points_y = [focal_plane_tl[1,0], focal_plane_tr[1,0], focal_plane_br[1,0], focal_plane_bl[1,0], focal_plane_tl[1,0], camera_position[1,0], gnd_tl[1,0], camera_position[1,0], gnd_tr[1,0], camera_position[1,0], gnd_bl[1,0], camera_position[1,0], gnd_br[1,0]]
focal_points_z = [focal_plane_tl[2,0], focal_plane_tr[2,0], focal_plane_br[2,0], focal_plane_bl[2,0], focal_plane_tl[2,0], camera_position[2,0], gnd_tl[2,0], camera_position[2,0], gnd_tr[2,0], camera_position[2,0], gnd_bl[2,0], camera_position[2,0], gnd_br[2,0]] 
ax.plot( focal_points_x,
         focal_points_y,
         focal_points_z,
         c='#01A9DB', marker='o', label='Focal Plane')

ax.add_collection3d(Poly3DCollection([zip(focal_points_x[0:5], focal_points_y[0:5], focal_points_z[0:5])], label='Focal Plane', facecolors=['#D0A9F5'], alpha=0.8))


       
# Set the limits
ax.legend()
ax.set_xlim3d(tlx, tlx+span)
ax.set_ylim3d(tly, tly+span)
ax.set_zlim3d(tlz, tlz+span)

ax.set_aspect('equal')

plt.show()
