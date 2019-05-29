#ifndef STAB_DEMO_CV_VIDEOSTAB_FAST_MARCHING_HPP
#define STAB_DEMO_CV_VIDEOSTAB_FAST_MARCHING_HPP

// C++ Libraries
#include <algorithm>
#include <cmath>
#include <queue>

// OpenCV Libraries
#include <opencv2/core.hpp>


class FastMarchingMethod
{
    public:

        FastMarchingMethod() : inf_(1e6f), size_(0)
        {}

        /** @brief Template method that runs the Fast Marching Method.

        @param mask Image mask. 0 value indicates that the pixel value must be inpainted, 255 indicates
        that the pixel value is known, other values aren't acceptable.
        @param inpaint Inpainting functor that overloads void operator ()(int x, int y).
        @return Inpainting functor.
         */
        template<typename Inpaint>
        Inpaint run(const cv::Mat &mask, Inpaint inpaint);

        /**
        @return Distance map that's created during working of the method.
        */
        cv::Mat distanceMap() const
        { return dist_; }

    private:
        enum
        {
            INSIDE = 0, BAND = 1, KNOWN = 255
        };

        struct DXY
        {
            float dist;
            int x, y;

            DXY() : dist(0), x(0), y(0)
            {}

            DXY(float _dist, int _x, int _y) : dist(_dist), x(_x), y(_y)
            {}

            bool operator<(const DXY &dxy) const
            { return dist < dxy.dist; }
        };

        float solve(int x1, int y1, int x2, int y2) const;

        int &indexOf(const DXY &dxy)
        { return index_(dxy.y, dxy.x); }

        void heapUp(int idx);

        void heapDown(int idx);

        void heapAdd(const DXY &dxy);

        void heapRemoveMin();

        float inf_;

        cv::Mat_<uchar> flag_; // flag map
        cv::Mat_<float> dist_; // distance map

        cv::Mat_<int> index_; // index of point in the narrow band
        std::vector<DXY> narrowBand_; // narrow band heap
        int size_; // narrow band size
};


#include "Fast_Marching_Inl.hpp"

#endif
