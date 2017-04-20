/**
 * @file    BrowserPage.hpp
 * @author  Marvin Smith
 * @date    4/19/2017
*/
#ifndef BROWSER_PAGE_HPP
#define BROWSER_PAGE_HPP

// Qt Libraries
#include <QtWebEngineWidgets/QWebEngineView>


/**
 * @class BrowserPage
*/
class BrowserPage : public QWebEnginePage
{
    public:

        /**
         * @brief Constructor
        */
        BrowserPage( QWidget* parent = nullptr );

    protected:

        /**
         * @brief Certificate Error Method.
        */
        virtual bool certificateError( const QWebEngineCertificateError& certificate_error );


}; // End of BrowserPage Class

#endif
