/**
 * @file  Frame_Source.cpp"
 */
#include "Frame_Source.hpp"

// Project Libraries
#include "Ring_Buffer.hpp"

// OpenCV Libraries
#include <opencv2/videoio.hpp>

class VideoFileSourceImpl : public IFrameSource
{
    public:

        VideoFileSourceImpl(const cv::String &path, bool volatileFrame)
            : path_(path), volatileFrame_(volatileFrame)
        { reset(); }

        void reset() override
        {
            vc.release();
            vc.open(path_);
            if (!vc.isOpened())
                CV_Error(0, "can't open file: " + path_);
        }

        cv::Mat nextFrame() override
        {
            cv::Mat frame;
            vc >> frame;
            return volatileFrame_ ? frame : frame.clone();
        }

        int width()
        {
            return static_cast<int>(vc.get(cv::CAP_PROP_FRAME_WIDTH));
        }

        int height()
        {
            return static_cast<int>(vc.get(cv::CAP_PROP_FRAME_HEIGHT));
        }

        int count()
        {
            return static_cast<int>(vc.get(cv::CAP_PROP_FRAME_COUNT));
        }

        double fps()
        {
            return vc.get(cv::CAP_PROP_FPS);
        }

    private:

        cv::String path_;
        bool volatileFrame_;
        cv::VideoCapture vc;
};


VideoFileSource::VideoFileSource(const cv::String &path, bool volatileFrame)
    : impl(new VideoFileSourceImpl(path, volatileFrame)) {}

void VideoFileSource::reset() { impl->reset(); }
cv::Mat VideoFileSource::nextFrame() { return impl->nextFrame(); }

int VideoFileSource::width() { return ((VideoFileSourceImpl*)impl.get())->width(); }
int VideoFileSource::height() { return ((VideoFileSourceImpl*)impl.get())->height(); }
int VideoFileSource::count() { return ((VideoFileSourceImpl*)impl.get())->count(); }
double VideoFileSource::fps() { return ((VideoFileSourceImpl*)impl.get())->fps(); }

