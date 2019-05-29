#ifndef STAB_DEMO_CV_VIDEOSTAB_LOG_HPP
#define STAB_DEMO_CV_VIDEOSTAB_LOG_HPP

#include "opencv2/core.hpp"

class ILog
{
    public:
        virtual ~ILog() = default;

        virtual void print(const char *format, ...) = 0;
};

class NullLog : public ILog
{
    public:
        virtual void print(const char * /*format*/, ...) override {}
};

class LogToStdout : public ILog
{
    public:

        virtual void print(const char *format, ...) override;
};


#endif
