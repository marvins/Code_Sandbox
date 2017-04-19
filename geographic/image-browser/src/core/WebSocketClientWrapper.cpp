#include "WebSocketClientWrapper.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include <iostream>

/*!
    Construct the transport object and wrap the given socket.

    The socket is also set as the parent of the transport object.
*/
WebSocketTransport::WebSocketTransport(QWebSocket *socket)
: QWebChannelAbstractTransport(socket)
, m_socket(socket)
{
    connect(socket, &QWebSocket::textMessageReceived,
            this, &WebSocketTransport::textMessageReceived);
    connect(socket, &QWebSocket::disconnected,
            this, &WebSocketTransport::deleteLater);
}

/*!
    Destroys the WebSocketTransport.
*/
WebSocketTransport::~WebSocketTransport()
{
    m_socket->deleteLater();
}

/*!
    Serialize the JSON message and send it as a text message via the WebSocket to the client.
*/
void WebSocketTransport::sendMessage(const QJsonObject &message)
{
    QJsonDocument doc(message);
    m_socket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    std::cout << "Sending Message" << std::endl;
}

/*!
    Deserialize the stringified JSON messageData and emit messageReceived.
*/
void WebSocketTransport::textMessageReceived(const QString &messageData)
{
    QJsonParseError error;
    QJsonDocument message = QJsonDocument::fromJson(messageData.toUtf8(), &error);
    if (error.error) {
        qWarning() << "Failed to parse text message as JSON object:" << messageData
                   << "Error is:" << error.errorString();
        return;
    } else if (!message.isObject()) {
        qWarning() << "Received JSON message that is not an object: " << messageData;
        return;
    }

    std::cout << "Received Message Data: " << messageData.toStdString() << std::endl;
    emit messageReceived(message.object(), this);
}

/*!
    Construct the client wrapper with the given parent.

    All clients connecting to the QWebSocketServer will be automatically wrapped
    in WebSocketTransport objects.
*/
WebSocketClientWrapper::WebSocketClientWrapper(QWebSocketServer *server, QObject *parent)
    : QObject(parent)
    , m_server(server)
{
    connect(server, &QWebSocketServer::newConnection,
            this, &WebSocketClientWrapper::handleNewConnection);
}

/*!
    Wrap an incoming WebSocket connection in a WebSocketTransport object.
*/
void WebSocketClientWrapper::handleNewConnection()
{
    emit clientConnected(new WebSocketTransport(m_server->nextPendingConnection()));
}
