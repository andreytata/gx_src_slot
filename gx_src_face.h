#ifndef GX_SRC_FACE_H
#define GX_SRC_FACE_H

#include <map>
#include <string>
#include <memory>

#include <QtCore/QObject>
#include <QtCore/QMetaMethod>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QWebSocket>


namespace gx
{
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

/// CLASS Interface. Interface-class-instance life time independed from Session-
/// -class-instance life-time. And connected to concrete Session-class-instance
/// using slot-path string. Interface-class-instance role is proxy between some
/// 'gxvm' instance and some Session class like instance. Interface class parse
/// input delivered by on_text_message/on_binary_message/on_disconnected events,
/// and accomulate data from connected gx::gxvm-instance before sending data to
/// self output => connected Session class or any other class with equal slots.
///
/// CONSEQUENTLY: Is dublicate gx::job derived gx::vip class. But gx::vip class
/// has no ability to accomulate logging info, after each command or methods be
/// executed on gxvm-instance-side. gxvm-command execution change all data in
/// vm-register-variables. Is may be any value setted by last command, but only
/// Interface-class-instance or some other connected-to-vm-events-sender has all
/// knowelege about file or source line generated last commands set. For example,
/// some vm-error-template pointer setted by last vm-command overwrite previous
/// pointer value. but all components like 'C' __file__, __line__,  exists at
/// object called command in this concrete vm-instance. Depend type of this proxy
/// object errors can be logged, readed or sended to some console or debug file,
/// but vm-instance has no info about all this additional enviroment variables.
///
///
class Interface: public QObject
{
    Q_OBJECT

public:

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

    void cget(const QJsonObject& input , QJsonObject& output);

    void cset(const QJsonObject& input , QJsonObject& output);

    void edit(const QJsonObject& input , QJsonObject& output);

    void href(const QJsonObject& input , QJsonObject& output);

    void wref(const QJsonObject& input , QJsonObject& output);

    void self(const QJsonObject& input , QJsonObject& output);

    void on_remote_opened(QWebSocket* socket);

    void on_remote_string(QString message, QWebSocket* socket);

    void on_remote_binary(QByteArray message, QWebSocket* socket);

    void on_remote_closed(QWebSocket* socket);

    /// All remote callable slots has unified signature (void)(QWebSocket*,QString&),
    /// And has name starts with ["get_"|"set_"|"new_"|"add_"|"del_"] prefix
    ///
/*
    void new_sock_test(QWebSocket* socket, const QString& arguments_line);  //

    void get_shared_vm(QWebSocket* socket, const QString& shared_vm_path);  // query auto interface from shared variable

    void get_interface(QWebSocket* socket, const QString& arguments_line);  // send JSON with current interface to socket

    void add_interface(QWebSocket* socket, const QString& interface_name);  // try to extend interface use named-methods-set

    void del_interface(QWebSocket* socket, const QString& interface_name);  // try to remove methods set from

    void set_attribute(QWebSocket* socket, const QString& arguments_line);  // fom --help

    // remote client inspect shared variables dict ( name, type, path )
    void gxvm_set(QWebSocket* socket, const QString& arguments_line);  // ? set, set_path, set_vars
*/
//    void sess_allow( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_allow"}));}
//    void sess_set  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_set"}));}
//    void sess_get  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_get"}));}
//    void sess_new  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_new"}));}
//    void sess_del  ( QWebSocket *ws, const QString&){if(ws)ws->sendTextMessage(JSON({"meth":"sess_del"}));}

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


#endif // GX_SRC_FACE_H
