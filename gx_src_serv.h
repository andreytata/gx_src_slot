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

namespace gx {

struct slot;

struct attr
{
    attr(const std::shared_ptr<gx::slot>&sp)
        :sp_slot(sp)
        ,wp_slot(sp)
    {}
    attr(const attr& src)
        :sp_slot(src.sp_slot)
        ,wp_slot(src.sp_slot)
    {}
    std::shared_ptr <gx::slot> sp_slot;
    std::weak_ptr   <gx::slot> wp_slot;
};

}

class Interface: public QObject
{
    Q_OBJECT

public:

    // PROTOCOL SUPPORT MULTIPLE ACCESS LEVELS
    // EACH NEXT ACCESS LEVEL OVERWRITE PREVIOUS
    // 'SESS' 'VIEW' 'USER' 'EDIT' 'SUDO' etc..
    // Each actor have multiple permissions to self variables: like
    // SUDO to self session, but USER of self session variable, and
    // EDIT to self other variable. Some variables are shared slots.
    //
    // all metamethods started with permission prefix, can be
    // added to gxvm session interface on "build_or_rebuild_interface"
    // query interface exists in all levels of permissions,

    // WAIT - is not level of permission, is gxvm state, used to decline
    // some step, edit or view commands, before w-conditions acomplished.
    QStringList allow { "gxvm_", "new_", "get_", "set_", "del_", "add_" };
    QString permission = "sess_";  // basic set of commands
                       // view_    // view access only, w/o change values alloved
                       // user_    // like view, but support callbacks insert/remove commands
                       // edit_    // like user, but support set value commands set
                       // sudo_    // full access to each gxvm commands ( not safe )

    explicit Interface(QObject *parent, bool debug = true)
        :QObject(parent)
        ,debug(debug)
    {
        if(debug)
        {
            qDebug()<<"++Interface" << (void*) this;
        }
        build_or_rebuild_interface();
    }

    virtual ~Interface()
    {
        if(debug)
        {
            qDebug()<<"--Interface" << (void*) this;
        }
    }

public Q_SLOTS:

    void on_remote_opened(QWebSocket* socket);

    void on_remote_string(QString message, QWebSocket* socket);

    void on_remote_binary(QByteArray message, QWebSocket* socket);

    void on_remote_closed(QWebSocket* socket);

    /// All remote callable slots has unified signature (void)(QWebSocket*,QString&),
    /// And has name starts with ["get_"|"set_"|"new_"|"add_"|"del_"] prefix
    ///
    void new_sock_test(QWebSocket* socket, const QString& arguments_line);  //

    void get_shared_vm(QWebSocket* socket, const QString& shared_vm_path);  // query auto interface from shared variable

    void get_interface(QWebSocket* socket, const QString& arguments_line);  // send JSON with current interface to socket

    void add_interface(QWebSocket* socket, const QString& interface_name);  // try to extend interface use named-methods-set

    void del_interface(QWebSocket* socket, const QString& interface_name);  // try to remove methods set from

    void set_attribute(QWebSocket* socket, const QString& arguments_line);  // fom --help

    // remote client inspect shared variables dict ( name, type, path )
    void gxvm_set(QWebSocket* socket, const QString& arguments_line);  // ? set, set_path, set_vars

    void sess_allow( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_allow"}));}
    void sess_set  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_set"}));}
    void sess_get  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_get"}));}
    void sess_new  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_new"}));}
    void sess_del  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_del"}));}

    // void gxvm_get(QWebSocket* socket, const QString& arguments_line);  // ? get_self, get_path, get_vars

    //
    void set(QJsonObject& echo, const QString& name, const QString& type, const QString& path);
//    void get_path(QJsonObject& echo, const QString& name, const QString& type, const QString& path);
//    void get_self(QJsonObject& echo, const QString& name, const QString& type, const QString& path);
//    void self(QJsonObject& echo, const QString& name)
//    void dump(QJsonObject& echo, const QString& type, const QString& path);

private:

    void build_or_rebuild_interface();

    bool debug;

    // variables, is weakref or hardref objects stored at this gxvm side
    std::map<std::string, gx::attr> vars;

    // interface extentions set
    std::map<std::string, QObject*> mm_self_interfaces;  // interface name => invocated QObject

    // this and self - summary interface
    std::map<std::string, std::pair<QMetaMethod, QObject*> > mm_self_interface;  // method name => invocation data
};


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

public Q_SLOTS:

    void on_remote_string(QString message);

    void on_remote_binary(QByteArray message);

private:

    bool debug;

    Interface* mp_interface = nullptr;

    QWebSocket* mp_socket = nullptr;
};

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
