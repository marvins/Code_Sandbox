#ifndef __MATHSSTUFF_H__
#define __MATHSSTUFF_H__

#include "DataTypes.h"

inline double RAD_TO_DEG(double x);
inline double SQ(double x);
inline double BoundAngle(double x);

inline double Pixel2Yaw(int x, int width);
inline double Pixel2Pitch(int y, int height);
inline double Yaw2Pixel(double yaw, int width);
inline double Pitch2Pixel(double pitch, int height);
inline void Pixel2Angles(double x, double y, double focal, double cx, double cy, double *yaw, double *pitch);
inline bool Angles2Pixel(double yaw, double pitch, double roll, double focal, double cx, double cy, double *x, double *y);

void FindBestRotation(const vector <MatchPair> &matches, const PanoImage &pano1, const PanoImage &pano2, double *yaw, double *pitch, double *roll);

// Inline functions have to be declared in the header
/******************************************************************************/
double RAD_TO_DEG(double x)
{
    return x*180.0/M_PI;
}
/******************************************************************************/
double SQ(double x)
{
    return x*x;
}
/******************************************************************************/
// Force angle to lie between -180 and 180 degrees
double BoundAngle(double x)
{
    if(x < -M_PI) {
        x += 2*M_PI;
    }

    if(x > M_PI) {
        x -= 2*M_PI;
    }

    return x;
}
/******************************************************************************/
double Pixel2Yaw(int x, int width)
{
    // 0 degrees at x = 0
    return 2.0*M_PI*x/width;
}
/******************************************************************************/
double Pixel2Pitch(int y, int height)
{
    // -90 degrees at y = 0
    return -M_PI_2 + M_PI*y/height;
}
/******************************************************************************/
double Yaw2Pixel(double yaw, int width)
{
    return yaw*width/(2.0*M_PI);
}
/******************************************************************************/
double Pitch2Pixel(double pitch, int height)
{
    return (pitch + M_PI_2) / (M_PI/height);
}
/******************************************************************************/
void Pixel2Angles(double x, double y, double focal, double cx, double cy, double *yaw, double *pitch)
{

    double px = x - cx;
    double py = y - cy;

    *yaw = atan2(px,focal);

    double r = sqrt(focal*focal + px*px);

    *pitch = atan(py/r);
}
/******************************************************************************/
bool Angles2Pixel(double yaw, double pitch, double roll, double focal, double cx, double cy, double *x, double *y)
{
    // Given 3 angles we want to find where this ray intersects on the rectilinear image

    // Out of bounds for rectilinear images  180 degrees (-90, 90) FOV maximum
    if(fabs(yaw) > M_PI_2) {
        return false;
    }

    if(fabs(pitch) > M_PI_2) {
        return false;
    }

    // Apply the roll to the yaw/pitch
    roll = -roll;
    double yaw2 = yaw*cos(roll) - pitch*sin(roll);
    double pitch2 = yaw*sin(roll) + pitch*cos(roll);

    double px = focal*tan(yaw2);
    double r = sqrt(focal*focal + px*px);
    double py = r*tan(pitch2);

    *x = cx + px;
    *y = cy + py;

    return true;
}

#endif
