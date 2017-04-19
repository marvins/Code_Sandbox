/**
 * @file   BrowserPane.hpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#ifndef __SRC_GUI_BROWSERPANE_HPP__
#define __SRC_GUI_BROWSERPANE_HPP__

#include <QGridLayout>
#include <QToolButton>
#include <QVBoxLayout>
#include <QtWebEngine/QtWebEngine>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QWidget>

#include <src/core/DataContainer.hpp>
#include <src/core/MessagingService.hpp>
#include <src/core/WebSocketClientWrapper.hpp>


extern DataContainer    settings;
extern MessagingService message_service;

class Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged)
public:
    explicit Document(QObject *parent = nullptr) : QObject(parent) {}

    void setText(const QString &text);

signals:
    void textChanged(const QString &text);

private:
    QString m_text;
};

/**
 * @class BrowserPane
*/
class BrowserPane : public QWidget{

    Q_OBJECT

    public:

        /**
         * Default Constructor
         */
        BrowserPane( QWidget* parent = NULL );


        /**
         *  Set the url of the browser pane
         */
        void setUrl( std::string const& url );


        /**
         * @brief Event Filter
        */
        bool eventFilter(QObject *target, QEvent *event);

    public slots:

        /**
         * @brief Reload the Browser Overlays on the HTML Map.
        */
        void reloadBrowserOverlays();

    private:

        //-----------------------------//
        //-     Private Functions     -//
        //-----------------------------//
        void build_toolbar_widget();

        /**
         * @brief Parse Latitude Result
         */
        void Parse_Latitude( const QString& result );
        void Parse_Longitude( const QString& result );

        //-----------------------------//
        //-     Private Variables     -//
        //-----------------------------//

        /// Main Layout
        QGridLayout*  mainLayout;

        ///  Web View
        QWebEngineView*  webView;

        /// Toolbar Widget
        QWidget*      toolbarWidget;

        /// Toolbar Layout
        QVBoxLayout*  toolbarLayout;

        /// Toolbar Look At Button
        QToolButton*  toolbarLookAtButton;

        /// Toolbar Export Button
        QToolButton*  toolbarExportButton;

        /// content
        Document* m_content;

        /// Client server
        WebSocketClientWrapper* m_client_wrapper;

        /// Socket server
        QWebSocketServer* m_socket_server;

        /// channel
        QWebChannel* m_web_channel;
};

#endif
