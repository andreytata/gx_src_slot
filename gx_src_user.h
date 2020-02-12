#ifndef GX_SRC_USER_H
#define GX_SRC_USER_H

#include <QObject>

#include <QMetaMethod>

#include <QWebSocket>

#include <gx_src_slot.h>

namespace gx
{

class node;

class user: public QObject  //, public gx::node
{
    Q_OBJECT
public:

    explicit user(QObject *parent, bool debug = true)
        :QObject(parent)
        ,debug(debug)
    {
        if(debug)
        {
            qDebug()<<"++USER" << (void*) this;
        }
        build_or_rebuild_interface();
    }

    virtual ~user()
    {
        if(debug)
        {
            qDebug()<<"--USER" << (void*) this;
        }
    }

    static gx::slot::href new_gxvm(const char *path);

public Q_SLOTS:

    void on_remote_opened(QWebSocket* socket);

    void on_remote_string(QString message, QWebSocket* socket);

    void on_remote_binary(QByteArray message, QWebSocket* socket);

    void on_remote_closed(QWebSocket* socket);

    /// All remote callable slots has unified signature (void)(QWebSocket*,QString&),
    /// And has name starts with ["get_"|"set_"|"new_"|"add_"|"del_"] prefix
    ///
    void new_sock_test(QWebSocket* socket, const QString& arguments_line);  //

    void get_interface(QWebSocket* socket, const QString& arguments_line);  // send JSON with current interface to socket

    void add_interface(QWebSocket* socket, const QString& interface_name);  // try to extend interface use named-methods-set

    void del_interface(QWebSocket* socket, const QString& interface_name);  // try to remove methods set from

    void set_attribute(QWebSocket* socket, const QString& arguments_line);

    void get_attribute(QWebSocket* socket, const QString& arguments_line);

private:

    void build_or_rebuild_interface();

    bool debug;

    gx::node* curr = nullptr;

    // interface extentions set
    std::map<std::string, QObject*> mm_self_interfaces;  // interface name => invocated QObject

    // this and self - summary interface
    std::map<std::string, std::pair<QMetaMethod, QObject*> > mm_self_interface;  // method name => invocation data
};


}

#endif // GX_SRC_USER_H
