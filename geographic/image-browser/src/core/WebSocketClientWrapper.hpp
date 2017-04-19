/**
 * @file    WebSocketClientWrapper
*/
#pragma once

#include <QtWebChannel/QWebChannelAbstractTransport>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

class WebSocketTransport : public QWebChannelAbstractTransport
{
    Q_OBJECT
public:
    explicit WebSocketTransport(QWebSocket *socket);
    virtual ~WebSocketTransport();

    void sendMessage(const QJsonObject &message) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void textMessageReceived(const QString &message);

private:
    QWebSocket *m_socket;
};

class WebSocketClientWrapper : public QObject
{
    Q_OBJECT

    public:
        WebSocketClientWrapper(QWebSocketServer *server, QObject *parent = 0);

    Q_SIGNALS:
        void clientConnected(WebSocketTransport* client);

    private Q_SLOTS:
        void handleNewConnection();

    private:
        QWebSocketServer *m_server;
};
