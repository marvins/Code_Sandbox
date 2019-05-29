#ifndef STAB_DEMO_CV_VIDEOSTAB_DEBLURRING_HPP
#define STAB_DEMO_CV_VIDEOSTAB_DEBLURRING_HPP

// C++ Libraries
#include <vector>

// OpenCV Libraries
#include <opencv2/core.hpp>

/**
 * @brief Compute the Blurriness of a Frame
 * @param frame
 * @return
 */
float calcBlurriness( const cv::Mat& frame );


class DeblurerBase
{
    public:


        DeblurerBase() = default;

        virtual ~DeblurerBase() = default;

        virtual void setRadius(int val){ radius_ = val; }

        virtual int radius() const{ return radius_; }

        virtual void deblur( int idx, cv::Mat &frame) = 0;


        // data from stabilizer

        virtual void setFrames(const std::vector<cv::Mat> &val){ frames_ = &val; }

        virtual const std::vector<cv::Mat> &frames() const{ return *frames_; }

        virtual void setMotions(const std::vector<cv::Mat> &val){ motions_ = &val; }

        virtual const std::vector<cv::Mat> &motions() const{ return *motions_; }

        virtual void setBlurrinessRates(const std::vector<float> &val){ blurrinessRates_ = &val; }

        virtual const std::vector<float> &blurrinessRates() const{ return *blurrinessRates_; }

    protected:

        int radius_ { 0 };
        const std::vector<cv::Mat> *frames_ { 0 };
        const std::vector<cv::Mat> *motions_ { 0 };
        const std::vector<float> *blurrinessRates_ { 0 };
};

class NullDeblurer : public DeblurerBase
{
    public:

        void deblur(int /*idx*/, cv::Mat &/*frame*/) override{}
};

class WeightingDeblurer : public DeblurerBase
{
    public:
        WeightingDeblurer();

        void setSensitivity(float val){ sensitivity_ = val; }

        float sensitivity() const{ return sensitivity_; }

        void deblur(int idx, cv::Mat &frame) override;

    private:

        float sensitivity_;
        cv::Mat_<float> bSum_, gSum_, rSum_, wSum_;
};

#endif
