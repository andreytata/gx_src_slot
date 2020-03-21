#include "gx_src_serv.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QJsonDocument>
#include <QJsonObject>

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


void Session::cdir(const QJsonObject& input, QJsonObject& output)
{
    if( !mp_interface )
    {
        output["echo"] = input;
        output["fail"] = "Session::dir Interface instance not connected";
        return;
    }

    if( !mp_socket )
    {
        output["echo"] = input;
        output["fail"] = "Session::dir Session socket is nullptr";
        return;
    }

    if( mp_interface != mp_inspected )
    {
        inspect_connected();
    }

    QJsonObject echo;

    for( auto& m : mm_interface )
    {
        echo[ m.first.c_str() ] = m.second.first.methodSignature().data();
    }

    output["echo"] = echo;

    return;
}

void Session::inspect_connected()
{
    mm_interface.clear();

    int dir_index = this->metaObject()->indexOfMethod("cdir(QJsonObject,QJsonObject&)");

    QMetaMethod dir_method = this->metaObject()->method(dir_index);

    mm_interface["cdir"] = std::pair<QMetaMethod, QObject*>(dir_method, this);

    int methods_count = mp_interface->metaObject()->methodCount();

    for( int i = 0; i < methods_count; ++i )
    {
       QMetaMethod method = mp_interface->metaObject()->method(i);

       QString name = method.name();

       if( method.methodSignature().endsWith("(QJsonObject,QJsonObject&)") )
       {
           mm_interface[name.toStdString()] = std::pair<QMetaMethod, QObject*>(method, mp_interface);
       }
    }
    mp_inspected = mp_interface;  // prevent rescan
}


void Session::on_remote_string(QString message)
{   
    if ( message.startsWith("{") )
    {
        qDebug() << "protocol_02";
        if ( mp_interface )
        {
            if (mp_socket)
            {
                if( mp_interface != mp_inspected )
                {
                    inspect_connected();
                }

                QJsonParseError parse_error;

                QJsonDocument doc = QJsonDocument::fromJson(message.toLocal8Bit(), &parse_error);

                QString last_error;

                QJsonObject input, output;

                if( QJsonParseError::NoError != parse_error.error )
                {
                    last_error = parse_error.errorString();   // qDebug() << "  <<SESS>> input text message are not JSON" << last_error;

                    output["fail"] = QString("SESS: text input not JSON") + last_error;

                    QJsonDocument echo(output);

                    mp_socket->sendTextMessage( echo.toJson() );

                    return;
                }

                if( ! doc.isObject() )
                {
                    output["fail"] = QString("SESS: query is not some json object");

                    QJsonDocument echo(output);

                    mp_socket->sendTextMessage( echo.toJson() );

                    return;
                }

                input = doc.object();

                QJsonValue meta_val = input["meta"];

                if( meta_val.isUndefined() )
                {
                    output["fail"] = QString("SESS: query has no field 'meta'");

                    QJsonDocument echo(output);

                    mp_socket->sendTextMessage( echo.toJson() );

                    return;
                }

                if( ! meta_val.isString() )
                {
                    output["fail"] = QString("SESS: field 'meta' must be string");

                    QJsonDocument echo(output);

                    mp_socket->sendTextMessage( echo.toJson() );

                    return;
                }

                QString meta_name(meta_val.toString());

                qDebug() << "META NAME IS " << meta_name;

                std::map<std::string, std::pair<QMetaMethod, QObject*> >::iterator                        
                        method_iter = mm_interface.find(meta_name.toStdString());

                if( method_iter == mm_interface.end() )
                {
                    output["fail"] = QString("SESS: interface has no method ") + meta_name + "(QJsonObject,QJsonObject&)";

                    QJsonDocument echo(output);

                    mp_socket->sendTextMessage( echo.toJson() );

                    return;
                }

                QMetaMethod method = (*method_iter).second.first;

                QObject* object    = (*method_iter).second.second;

                qDebug() << "  SESS PREPARE CALL" << method.methodSignature() << object;

                method.invoke( object, Qt::DirectConnection,
                               Q_ARG(const QJsonObject&, input  ),
                               Q_ARG(      QJsonObject&, output ));

                qDebug() << doc;

                for(auto i: mm_interface)
                {
                    QString name(i.first.c_str());
                    qDebug() << name << i.second.first.name() << i.second.second;
                }

                QJsonDocument echo(output);

                mp_socket->sendTextMessage( echo.toJson() );
            }
        }
        else
        {
            if (mp_socket)
            {
                mp_socket->sendTextMessage(JSON({"fail":"has no interface connected for protocol_02"}));
            }
        }
    }
    else
    {
        mp_interface->on_remote_string(message, mp_socket);
    }
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
