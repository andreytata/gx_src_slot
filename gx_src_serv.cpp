#include "gx_src_serv.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

EchoServer::EchoServer(quint16 port, bool debug, QObject *parent)
    : QObject(parent)
    , m_pWebSocketServer( new QWebSocketServer(
            QStringLiteral("Echo Server"), QWebSocketServer::NonSecureMode, this ))
    , m_debug(debug)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        if (m_debug) qDebug() << "Echoserver listening on port" << port;

        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                              this, &EchoServer::onNewConnection);

        connect(m_pWebSocketServer, &QWebSocketServer::closed,
                              this, &EchoServer::closed);
    }
}

EchoServer::~EchoServer()
{
    m_pWebSocketServer->close();

    qDeleteAll(m_clients.begin(), m_clients.end());
}

void EchoServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::disconnected, this, &EchoServer::socketDisconnected);

    auto pParser = new Session(pSocket);

    connect(pSocket, &QWebSocket::textMessageReceived, pParser, &Session::on_remote_string);

    connect(pSocket, &QWebSocket::binaryMessageReceived, pParser, &Session::on_remote_binary);

    m_clients << pSocket;
}


void Session::on_remote_string(QString message)
{
      mp_interface->on_remote_string(message, mp_socket);
}


void Session::on_remote_binary(QByteArray message)
{
//    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

//    if (pClient == mp_socket)

//    if (debug) qDebug() << "Binary Message received:" << message;

//    if (pClient)
//    {
//        pClient->sendBinaryMessage(message);
//    }
//    if (mp_socket) mp_socket->sendBinaryMessage(message`);

      mp_interface->on_remote_binary(message, mp_socket);
}

/*
void EchoServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (m_debug){

        qDebug() << "Message received:" << (void*) pClient << message;

        qDebug() << pClient->children();
    }

    if (pClient)
    {
        pClient->sendTextMessage(message);
    }
}

void EchoServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (m_debug) qDebug() << "Binary Message received:" << message;

    if (pClient)
    {
        pClient->sendBinaryMessage(message);
    }
}
*/

void EchoServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    // if (m_debug) qDebug() << "SOCK !--" << pClient;

    if (pClient)
    {
        m_clients.removeAll(pClient);

        pClient->deleteLater();
    }
}
