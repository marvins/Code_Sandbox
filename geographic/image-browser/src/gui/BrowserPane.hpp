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

        virtual ~BrowserPane();

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


        /**
         * @brief Respond to Messages from html
        */
        void Handle_HTML_Response( const QString& response );

        void Handle_New_Connection();
        void Handle_Close_Connection();

        void Handle_Text_Message( const QString& message );
        void Handle_Binary_Message( QByteArray message );

        void Socket_Disconnected();

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

        void Configure_Networking();


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

        /// Socket server
        QWebSocketServer* m_socket_server;

        /// List of Connections
        QList<QWebSocket*> m_socket_clients;
};

#endif
