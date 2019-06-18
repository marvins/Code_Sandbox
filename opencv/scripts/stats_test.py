#!/usr/bin/env python3

import math
import numpy as np

def RotationZ( angle ):

    return np.array([[math.cos(angle), -math.sin(angle), 0.0],
                     [math.sin(angle),  math.cos(angle), 0.0],
                     [            0.0,              0.0, 1.0]])


def Build_Weights( radius, stddev ):

    weights = {}
    sum = 0
    for x in range(-radius, radius+1):
        value = math.exp(-x*x / (stddev*stddev))
        sum += value
        weights[x] = value
    for x in range(-radius, radius+1):
        weights[x] = weights[x]/sum

    return weights


def Build_Motions( radius, stddev ):

    ran = 2*radius + 1.0
    motions = {}
    for x in range(-radius, radius+1):
        motions[x] = RotationZ( math.pi/(2 * ran))

    return motions

def Get_Motion( fromIdx, toIdx, motions ):

    m = np.identity(3)

    if toIdx > fromIdx:
        for x in range(fromIdx, toIdx):
            m = np.dot(motions[x],m)
    elif fromIdx > toIdx:
        for x in range(toIdx, fromIdx):
            m = np.dot(motions[x],m)
            m = np.linalg.pinv(m)

    return m

def Compute_Stab_Matrix( weights, motions, id ):

    res = np.zeros((3,3))
    sum = 0.0
    for x in range(-radius, radius+1):
        res += np.dot(weights[x - id],Get_Motion( id, x, motions))
        sum += weights[x - id]

    print('Res:\n', res,'\nSum:' + str(sum))
    return res/sum

radius = 5
stddev = math.sqrt(radius)

#  Create list of weights
weights = Build_Weights( radius, stddev )
print('Weights: ' + str(weights))

#  Create Motions
motions = Build_Motions( radius, stddev )

#  Compute Stab Matrix
stab_motion = Compute_Stab_Matrix( weights, motions, 0 )

print('Stab Motion\n', stab_motion)

print('STab Val: ' + str(stab_motion[0,0]))
stab_angle = math.acos(stab_motion[0,0]) * 180 / math.pi

print('Angle: ' + str(stab_angle))