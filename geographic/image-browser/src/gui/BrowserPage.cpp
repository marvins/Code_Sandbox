/**
 * @file    BrowserPage.cpp
 * @author  Marvin Smith
 * @date    4/19/2017
*/
#include "BrowserPage.hpp"

// C++ Libraries
#include <iostream>

BrowserPage::BrowserPage( QWidget* parent )
 : QWebEnginePage(parent)
{
    std::cout << "Inside Constructor (BrowserPage)" << std::endl;

}

bool BrowserPage::certificateError( const QWebEngineCertificateError& certificate_error )
{
    std::cout << "Inside Certificate Error" << std::endl;
    return true;
}
