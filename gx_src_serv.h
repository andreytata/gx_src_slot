#ifndef GX_SRC_SERV_H
#define GX_SRC_SERV_H

#include <map>
#include <string>
#include <memory>

#include <QtCore/QObject>
#include <QtCore/QMetaMethod>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QWebSocket>
#include <QDebug>

#include <gx_src_face.h>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
// QT_FORWARD_DECLARE_CLASS(QWebSocket)

/// Interface created on query-interface and not removed by disconnection.
/// Instance of interface-derived contain session's variables, states and
/// pointers to controlled async threads, so can't be deleted immediately.
/// Instances are sharable storable and restorable. Session can be stored
/// and restored by application's global timer, without server connection.
/// Session definition is long-time job story, saved & restored each time
/// application started/finished. So, web-socket activation of interfaces
/// is only one more method to activate stored session. This abstract obj
/// has connection-time life, and used only to connect more complicated
/// interface-derived to this connection-life-time interface-derived
///
/// Base interface created w/o any objects or permissions. Anonimoust are.
/// IUnknownGuest. Session Interface derived from IUnknownGuest. Is only
/// one difference, socket used as input/output device in this case, are
/// QWebSocket created by WebSocketServer. IUnknownGuest can use
/// syslog mode => read file.txt   => write file.txt.log.txt
/// stdout mode => read stdin      => write stdout,
/// silent mode => read nulldevice => write nulldevice,
/// socket mode => read websocket  => write websocket,

#define JSON(...) #__VA_ARGS__

/// This, Interface instance, support protocol and methods used for
/// find, query and connect to some exists commandline session. And
/// result of query depend real-time detected states an permissions.
/// After query target interface detected (or created), this object
/// rebuild self meta-object-interface to extend self own interface
/// with meta-metods from connected interface. Is not proxy pattern,
/// used query-interface-pattern. This instance can be disconnected
/// and connected to other gxvm instance.

/// SESS <-1-> Interface <-1-> SLOT
/// Interface -> proxy & mediator, has on_socket_lost(QWebSocket*)
/// Interface -> proxy & mediator, has on_socket_repl(QWebSocket*)

/// CLASS Session. Session-class-instance created for each incoming connection.
/// Contain pointer to Interface-class-instance (initially nullptr). Interface-
/// -class-instance can be created separately from Session-class-instance. And
/// can be connected to concrete Session-class-instance using slot-path string.
/// Session class can be extended with traditional network security abilities.
/// But all data sended from Session-class-instance to Interface-class-instance
/// are decoded as JSON text or binary object. Session class instance also send
/// to connected Interface-class-instance signal on self lost connection.
///
/// CONSEQUENTLY: minimal
///    session::get_path(QString) => SLOT-INFO-JSON
///    session::set_path(QString) => SUCCESS-FAILURE-JSON
///
/// SESS send to interface query, and can extract partial or full ansver from
/// Interface side. But extract is some next session call.
/// State "Final"JSON echo object
class Session : public QObject
{
    Q_OBJECT
public:

    explicit Session(QWebSocket *parent, bool debug = false)
        :QObject((QObject*)parent)
        ,debug(debug)
        ,mp_interface(new Interface(nullptr))  // new Unknown_Interface created
        ,mp_socket(parent)
    {
        if(debug)
        {
            qDebug()<<"++Session" << (void*) this;
        }
        mp_interface->on_remote_opened(mp_socket);
    }

    virtual ~Session()
    {
        if(debug)
        {
            qDebug()<<"--Session" << (void*) this;
        }
        mp_interface->on_remote_closed(mp_socket);  // query delete delegated to Interface instance
    }

    void inspect_connected();

public Q_SLOTS:

    void on_remote_string(QString message);

    void on_remote_binary(QByteArray message);

    void cdir(const QJsonObject&, QJsonObject&);

private:

    bool debug;

    std::map<std::string, std::pair<QMetaMethod, QObject*> > mm_interface;
    // [name.toStdString()] = std::pair<QMetaMethod, QObject*>(method, this);

    Interface* mp_inspected = (Interface*)(-1);
    Interface* mp_interface = nullptr;  // mp_interface->on_string_msg(const QString)
                                        // mp_interface->on_binary_msg(const QByteArray)
                                        // mp_interface->on_disconnect(this)
                                        // mp_interface->on_recuperate(this)

    QWebSocket* mp_socket = nullptr;
};


/// EchoServer class. Websocket server listener and Session's creator/deleter
///
///
class EchoServer : public QObject
{
    Q_OBJECT
public:

    explicit EchoServer(quint16 port, bool debug = false, QObject *parent = nullptr);

    ~EchoServer();

Q_SIGNALS:

    void closed();

private Q_SLOTS:

    void onNewConnection();

    void socketDisconnected();

private:

    QWebSocketServer *m_pWebSocketServer;

    QList<QWebSocket *> m_clients;

    bool m_debug;
};

#endif // GX_SRC_SERV_H
