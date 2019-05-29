/**
 * @file    Outlier_Rejection.cpp
 */
#include "Outlier_Rejection.hpp"


void NullOutlierRejector::process( Size /*frameSize*/, InputArray points0, InputArray points1, OutputArray mask)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(points0.type() == points1.type());
    CV_Assert(points0.getMat().checkVector(2) == points1.getMat().checkVector(2));

    int npoints = points0.getMat().checkVector(2);
    mask.create(1, npoints, CV_8U);
    Mat mask_ = mask.getMat();
    mask_.setTo(1);
}

TranslationBasedLocalOutlierRejector::TranslationBasedLocalOutlierRejector()
{
    setCellSize(Size(50, 50));
    setRansacParams(RansacParams::default2dMotion(MM_TRANSLATION));
}


void TranslationBasedLocalOutlierRejector::process(
        Size frameSize, InputArray points0, InputArray points1, OutputArray mask)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(points0.type() == points1.type());
    CV_Assert(points0.getMat().checkVector(2) == points1.getMat().checkVector(2));

    int npoints = points0.getMat().checkVector(2);

    const Point2f* points0_ = points0.getMat().ptr<Point2f>();
    const Point2f* points1_ = points1.getMat().ptr<Point2f>();

    mask.create(1, npoints, CV_8U);
    uchar* mask_ = mask.getMat().ptr<uchar>();

    Size ncells((frameSize.width + cellSize_.width - 1) / cellSize_.width,
                (frameSize.height + cellSize_.height - 1) / cellSize_.height);

    // fill grid cells

    grid_.assign(ncells.area(), Cell());

    for (int i = 0; i < npoints; ++i)
    {
        int cx = std::min(cvRound(points0_[i].x / cellSize_.width), ncells.width - 1);
        int cy = std::min(cvRound(points0_[i].y / cellSize_.height), ncells.height - 1);
        grid_[cy * ncells.width + cx].push_back(i);
    }

    // process each cell

    RNG rng(0);
    int niters = ransacParams_.niters();
    std::vector<int> inliers;

    for (size_t ci = 0; ci < grid_.size(); ++ci)
    {
        // estimate translation model at the current cell using RANSAC

        float x1, y1;
        const Cell &cell = grid_[ci];
        int ninliers, ninliersMax = 0;
        float dxBest = 0.f, dyBest = 0.f;

        // find the best hypothesis

        if (!cell.empty())
        {
            for (int iter = 0; iter < niters; ++iter)
            {
                int idx = cell[static_cast<unsigned>(rng) % cell.size()];
                float dx = points1_[idx].x - points0_[idx].x;
                float dy = points1_[idx].y - points0_[idx].y;

                ninliers = 0;
                for (size_t i = 0; i < cell.size(); ++i)
                {
                    x1 = points0_[cell[i]].x + dx;
                    y1 = points0_[cell[i]].y + dy;
                    if (sqr(x1 - points1_[cell[i]].x) + sqr(y1 - points1_[cell[i]].y) <
                        sqr(ransacParams_.thresh))
                    {
                        ninliers++;
                    }
                }

                if (ninliers > ninliersMax)
                {
                    ninliersMax = ninliers;
                    dxBest = dx;
                    dyBest = dy;
                }
            }
        }

        // get the best hypothesis inliers

        ninliers = 0;
        inliers.resize(ninliersMax);
        for (size_t i = 0; i < cell.size(); ++i)
        {
            x1 = points0_[cell[i]].x + dxBest;
            y1 = points0_[cell[i]].y + dyBest;
            if (sqr(x1 - points1_[cell[i]].x) + sqr(y1 - points1_[cell[i]].y) <
                sqr(ransacParams_.thresh))
            {
                inliers[ninliers++] = cell[i];
            }
        }

        // refine the best hypothesis

        dxBest = dyBest = 0.f;
        for (size_t i = 0; i < inliers.size(); ++i)
        {
            dxBest += points1_[inliers[i]].x - points0_[inliers[i]].x;
            dyBest += points1_[inliers[i]].y - points0_[inliers[i]].y;
        }
        if (!inliers.empty())
        {
            dxBest /= inliers.size();
            dyBest /= inliers.size();
        }

        // set mask elements for refined model inliers

        for (size_t i = 0; i < cell.size(); ++i)
        {
            x1 = points0_[cell[i]].x + dxBest;
            y1 = points0_[cell[i]].y + dyBest;
            if (sqr(x1 - points1_[cell[i]].x) + sqr(y1 - points1_[cell[i]].y) <
                sqr(ransacParams_.thresh))
            {
                mask_[cell[i]] = 1;
            }
            else
            {
                mask_[cell[i]] = 0;
            }
        }
    }
}

} // namespace videostab
} // namespace cv
