#include <QWebSocket>
#include <QDebug>
#include <QMetaMethod>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRegExp>
#include <QRegularExpressionMatch>
#include <QString>

#include <gx_src_serv.h>

#include <gx_src_slot.h>

// Interface is only set of MethaMethods parsed in current interface object.
// Building started from base interface, on create or after each changes in
// Interface chain. Frist interface's chain has fixed type "class IUnknown".
// Each additional interface inserted in concrete instance, of the IUnknown,
// as child. So 'has_interface' 'set_interface' and 'get_interface' is some
// basic operations set in IUnknown interface.
/** # ----/ Example /----:
class Texture2D:
    @staticmethod
    def rules(): return """
?unknown:
    set_interface texture_2d
    set_source %%node_path%%
?created:
    new_connect this.finished self.on_finished
    run_invalidate %%node_name%%
?started:
    new_connect this.finished self.on_finished
?failure:
    on_failure_%%node_name%%
?success:
    on_success_%%node_name%%
"""
>>> sess.def_node("texture1", "texture2d", "heap://hello/from/big/bodun.jpeg")
**/
// > set_interface           => "echo":"texture2d.2018.01.20.11:09:35 autodetected"
// > get_interface texture2d => "echo":"texture2d.2018.01.20.11:09:35"
// > get_interface         => "echo":"IUnknown"
// > interface             => "echo":"IUnknown"
// has_interface texture2d => "echo":true
// BTW, 'has_' prefix not realy
// exists. Get w/o parameters used as 'has_' - so return current interface's
// name. "name" - key to detect compatibility with this installed interface
// and old (overwrited) interface.

/// Rebuild Interface Step Used After Each Set Interface
/// Some interfaces connected to shared data nodes.
/// Weakref, Hardref, Iterable And Procedure Variables:
/// -- All variables started from "wr_" are weakref to node (dict), at least empty node (dist)
/// -- All variables started from "wh_" are weakref to iterable with hardrefs "ih_"
/// -- All variables started from "ww_" are weakref to iterable with weakrefs "iw_"
/// -- All variables started from "ih_" are hardref to iterable with hardrefs
/// -- All variables started from "iw_" are hardref to iterable with weakrefs
/// -- Only node(dict) can be inserted in iterable, newer iterable
///
/// -- All variables started from "fn_" are hardref and callable ( builtin method or node(dict) with method __call__ )
/// -- All variables started from "ls_" are iterable list of weakrefs
/// -- All variables started from "lw_" are array
/// -- All other variables are hardref to node
// > file sess.__weakref_variable_name file://path/to/file.ext
// > file sess.href_variable_name
// all commands now goto get_interface["name"]
// |sess.var_name> get_vars | get_interface | help get_face
// |sess> set this.var_name sess.var2_name free

/// is must try to find existed IFile node use hash("file://path/to/file.ext")
/// is like
/// > find variable_name file://path/to/file.ext
/// ? failure => create new w/o started
/// ? unknown => key exist, but interface not exist or IUnknown
/// ? started => exists find existed
///
/// > find file://path/to/file.ext" is blockind hash key creator
///
/// get_find
/// And Lazy Loaded "file", share resource  used So,
void Interface::build_or_rebuild_interface()
{
    // Re-Insert all "Interface" methods to mm_self_interface

    mm_self_interface.clear();

    int methods_count = this->metaObject()->methodCount();

    for( int i = 0; i < methods_count; ++i )
    {
       QMetaMethod method = this->metaObject()->method(i);

       QString name = method.name();

       //const QString old_template = "^(gxvm_|new_|get_|set_|del_|add_)\\w+$";
       QString allow_template = "^((-_-))\\w+$";
       QString allowed = allow.join("|");
       QString method_name_expression = allow_template.replace("(-_-)", allowed );
       QRegExp method_name_parser(method_name_expression);

       int pos = method_name_parser.indexIn(name);

       if ( pos == 0 )
       {
           mm_self_interface[name.toStdString()] = std::pair<QMetaMethod, QObject*>(method, this);
       }
    }
}


void Interface::new_sock_test(QWebSocket *socket, const QString &arguments_line)
{
    if( socket )
    {
        QJsonObject echo;
        echo["proc"] = "new_sock_test";
        echo["args"] = arguments_line;
        socket->sendTextMessage(QJsonDocument(echo).toJson());
    }
}


void Interface::set(QJsonObject& echo, const QString& name, const QString& type, const QString& path)
{
    std::string name_std = name.toStdString();

    std::string type_std = type.toStdString();

    std::string path_std = path.toHtmlEscaped().toStdString();

    qDebug() << "set" << name << type << path;

    struct : gx::slot::mode::proc
    {
        void on(gx::slot::mode::init*)  // new slot created, possible has no data or type components
        {
            p_echo->insert("echo", "slot exists or created, but possible has no data or type" );

            auto sp_type = gx::root::type_find(p_type_name);

            // DETECT TYPE TO EXISTS SLOT ERROR
            if ( nullptr == sp_type.get() )
            {
                QString error_qstr = QString("get-type <") + p_type_name + QString("> unknown type");
                qDebug() << "ERROR:" << error_qstr;
                (*p_echo)["fail"] = error_qstr;
                return;
            }

            p_fail = sp_slot->set_type( sp_type );

            // DETECT SET TYPE TO EXISTS SLOT ERROR
            if(p_fail)
            {
                QString error_qstr = QString("set-type<") + p_type_name + QString("> to path <") + p_slot_path + p_fail;
                qDebug() << "ERROR:" << error_qstr;
                p_echo->insert("fail", error_qstr);
            }
            p_echo->insert("fail", "not implemented SET gx::slot::mode::init");


            p_gxvm->vars.insert({std::string(ba_name.data()), gx::attr(sp_slot)});  // store created variable in dict
        }

        void on(gx::slot::mode::main*)
        {
            p_echo->insert("echo", "exists slot, with type and data exists" );
            p_echo->insert("fail", "not implemented SET gx::slot::mode::main");
        }

        void on(gx::slot::mode::free*)
        {
            p_echo->insert("echo", "(simulated-state) mean in garbage" );
            p_echo->insert("fail", "not implemented SET gx::slot::mode::free");
        }

        void on(gx::slot::mode::none*){}  // (simulated-state) mean not in memory

        void operator()()
        {
            sp_slot = gx::root::slot_make(p_slot_path);

            sp_slot->get_mode()->on(this);

        }

        Interface*      p_gxvm      = nullptr;  // target vm pointer
        QJsonObject*    p_echo      = nullptr;  //
        QByteArray      ba_name      = nullptr;  // target vm variable name
        gx::type::href  sp_type      = nullptr;
        gx::slot::href  sp_slot      = nullptr;
        const char*     p_fail      = nullptr;
        const char*     p_type_name = nullptr;  // queryed type name
        const char*     p_slot_path = nullptr;  // shared path or ""
    }
    react_depend_slot_mode;
    react_depend_slot_mode.p_gxvm      = this;
    react_depend_slot_mode.p_echo      = &echo;
    react_depend_slot_mode.ba_name     = name.toLatin1();
    react_depend_slot_mode.p_type_name = type_std.c_str();
    react_depend_slot_mode.p_slot_path = path_std.c_str();
    react_depend_slot_mode();
}


void Interface::gxvm_set(QWebSocket *socket, const QString &arguments_line)
{
    // TODO: is impossible before gxvm detected and connected to this interface
    // but temporary create implementation here, later must be moved to IClient.
    // So, on connection established, at path "remote/host/port" created gxvm node
    // instanceIUnknownClient.
    // ?WHY: unautorized remote client has no permissions to inspect, or create,
    // or change variables in this
    // variables, or calculate client can't create or inspect variables

    QJsonObject echo;

    const QString expression = "^\\s*([_a-zA-Z][_a-zA-Z0-9]*)\\s+([_a-zA-Z][_a-zA-Z0-9]*)(.*)$";

    QRegExp arguments_parser( expression );

    int pos = arguments_parser.indexIn( arguments_line );

    if ( pos == 0 )
    {
        QString name = arguments_parser.cap(1);
        QString type = arguments_parser.cap(2);
        QString path = arguments_parser.cap(3).trimmed();

        set( echo, name, type, path );

        if(socket)
        {
            echo["proc"] = "set";
            echo["name"] = name;
            echo["type"] = type;
            echo["path"] = path;
            socket->sendTextMessage(QJsonDocument(echo).toJson());
        }
    }
    else
    {
        if(socket)
        {
            echo["proc"] = "set";
            echo["args"] = arguments_line;
            echo["fail"] = "syntax error:" + expression;
            socket->sendTextMessage(QJsonDocument(echo).toJson());
        }
    }
}



void Interface::get_shared_vm(QWebSocket *socket, const QString &arguments_line)
{
    if( socket )
    {
        QJsonObject echo;
        echo["proc"] = "get_shared_vm";
        echo["args"] = arguments_line;
        socket->sendTextMessage(QJsonDocument(echo).toJson());
    }
}


void Interface::get_interface(QWebSocket* socket, const QString&)
{
    // send JSON with current interface to socket
    QJsonObject interface;

    for ( auto& pair : mm_self_interface )
    {
        std::string  name = pair.first;

        QObject* base = pair.second.second; //subjpointer

        QString hexptr = QString("0x%1").arg(uint((void*)base), 8, 16, QLatin1Char( '0' ));

        interface[name.c_str()] = QJsonValue(hexptr);
    }

    if( socket )
    {
        socket->sendTextMessage(QJsonDocument(interface).toJson());
    }
}

//void Interface::unknownviewguest invader

void Interface::add_interface(QWebSocket* socket, const QString& interface_name)
{
    auto exists = mm_self_interfaces.find(interface_name.toStdString());

    QJsonObject echo;

    echo["proc"] = "add_interface";

    echo["args"] = interface_name;

    if ( exists!= mm_self_interfaces.end() )
    {
        if(socket)
        {
            echo["echo"] = "already exists";
            socket->sendTextMessage(QJsonDocument(echo).toJson());
        }
        return;
    }

    // else, IF NOT interface name exists in interfaces:

    QObject* plugin = nullptr; // TODO: add QObject* from registry use interface name

    if ( nullptr == plugin )
    {
        if(socket)
        {
            echo["fail"] = QString("FAILURE: ") + interface_name + " not found in registry";
            socket->sendTextMessage(QJsonDocument(echo).toJson());
        }
        return;
    }

    // else, queryed interface exists in registry and not in mm_self_interfaces
    mm_self_interfaces[interface_name.toStdString()] = plugin;

    build_or_rebuild_interface();

    if(socket)
    {
        echo["echo"] = QString("SUCCESS: ") + interface_name + " appended";
        socket->sendTextMessage(QJsonDocument(echo).toJson());
    }
}

void Interface::del_interface(QWebSocket* socket, const QString& interface_name)
{
    QJsonObject echo;

    echo["proc"] = "del_interface";

    echo["args"] = interface_name;

    auto exists = mm_self_interfaces.find(interface_name.toStdString());

    if ( exists == mm_self_interfaces.end() )
    {
        if( socket )
        {
            echo["echo"] = QString("SUCCESS: ") + interface_name + " not exists";
            socket->sendTextMessage(QJsonDocument(echo).toJson());
        }
        return;
    }

    mm_self_interfaces.erase(exists);

    build_or_rebuild_interface();

    if( socket )
    {
        echo["echo"] = QString("SUCCESS: ") + interface_name + " removed";
        socket->sendTextMessage(QJsonDocument(echo).toJson());
    }
}


void Interface::set_attribute( QWebSocket *socket, const QString& arguments_line )
{
    // std::string path_std = arguments_line.toStdString();

    // const char* path = path_std.c_str();

    QJsonObject echo;

    echo["proc"] = "set_attribute";

    echo["args"] = arguments_line;

    if( socket )
    {
        socket->sendTextMessage(QJsonDocument(echo).toJson());
    }
}


void Interface::get(const QJsonObject& i, QJsonObject& o)
{
    QJsonValue v_args = i["args"];

    if( v_args.isUndefined() )
    {
        o["fail"] = "Interface::get ERROR: 'args' undefined";
        return;
    }

    if( ! v_args.isObject() )
    {
        o["fail"] = "Interface::get ERROR: 'args' is not object";
        return;
    }

    QJsonObject args = v_args.toObject();

    QJsonValue v_path = args["path"];

    if( v_path.isUndefined() )
    {
        o["fail"] = "Interface::get ERROR: args.path undefined";
        return;
    }

    if( !v_path.isString() )
    {
        o["fail"] = "Interface::get ERROR: args.path is not string";
        return;
    }

    QString path = v_path.toString();

    QJsonObject obj;
    gx::root::dump( path.toStdString().c_str(), obj);  // dump from path to output json
    o["echo_path"] = path;
    o["echo_echo"] = "hello";
    o["echo_dump"] = obj;
}


// command line interpreter, interface contain only methods started from
// get_
// set_
// add_
// new_
// del_
// But all get_ slots have shortcuts
// get_xxx ==> xxx
// In method "build_or_rebuild_interface" all shortcuts must be detected and
// added to interface dictionary.
void Interface::on_remote_string(QString message, QWebSocket *socket)
{
    qDebug()<<"SESS TXT" << (void*)this << (void*)socket<<message;

    if (socket)
    {
        // QRegExp message_parser("^(\\w+)(.*)$");
        // remote callable slot expression
        static QRegExp message_parser("^((gxvm_|new_|get_|set_|del_|add_)\\w+)(.*)$");
        QString meth = "";
        QString args = "";
        QString ext  = "";

        int pos = message_parser.indexIn(message);

        if ( pos == 0 )
        {
             meth = message_parser.cap(1);  // echo_interface
             args = message_parser.cap(3);  // formatted_json | compressed_json
             ext  = "!!!!!";

             auto exists = mm_self_interface.find(meth.toStdString());

             if ( exists != mm_self_interface.end() )
             {
                 //socket->sendTextMessage( QString("> ") + meth + "(" + args + ")" );
                 QMetaMethod mptr = exists->second.first;
                 QObject*    optr = exists->second.second;
                 mptr.invoke ( optr
                             , Qt::DirectConnection
                             , Q_ARG( QWebSocket* , socket )
                             , Q_ARG( QString     , args.trimmed() )
                             );
             }
             else
             {
                 if( socket )
                 {
                     QJsonObject echo;
                     echo["meth"] = meth;
                     echo["args"] = args;
                     echo["fail"] = "unknow method";
                     socket->sendTextMessage(QJsonDocument(echo).toJson());
                 }
             }
        }
        else
        {
            if( socket )
            {
                QJsonObject echo;
                echo["meth"] = "syntax parser";
                echo["args"] = message + ext;
                echo["fail"] = "syntax error";
                socket->sendTextMessage(QJsonDocument(echo).toJson());
            }
        }
    }
}


void Interface::on_remote_binary(QByteArray message, QWebSocket *socket)
{
    qDebug()<<"SESS BIN" << (void*)this << (void*)socket<<message;
    if (socket) socket->sendBinaryMessage(message);
}


void Interface::on_remote_closed(QWebSocket *socket)
{
    qDebug()<<"SESS DEL" << (void*)this << (void*) socket;
}


void Interface::on_remote_opened(QWebSocket *socket)
{
    qDebug()<<"SESS NEW" << (void*)this << (void*) socket;
}
