#include <gx_src_user.h>

#include <QJsonObject>

#include <QJsonDocument>

#include <QJsonArray>

// command interpreter, created w/o sockets, but each query can have
// real pointer to socket or nullptr. Methods calculation independed
// are, exist socket or not exists. But, if concrete instance of the
// command interpreter has self-own connected debug-or-logger-socket,
// interpreter has abilitity accomulate debug info and execute step-
// by-step each incoming query. So meta method called, but socket on
// query side has no result, until own-debug socket has no command
// step and continue. So, if debug socket implanded in to concrete
// gxvm, no one query have one socket or not, not finished until
// logging finished and permitions for next step be received
// from implanted debug socket.

/// Each command line interpreter can create dummy-shared-slot with
/// "unicue name" used for connect one interpreter to other, for use
/// shared one as self co-process variable. Is only for ability read
/// data from co-process depend state of co-process. Recomended way
/// is:  process PA must do something in loop w/o pause. And need
/// known-type-data from UNKNOWN process PB in UNKNOWN state. PA read
/// PB state each loop, but depend PB-state use pb-data or some other
/// default-on-PB-in-wait-state-data or default-on-PB-in-fail-state-data.

/// Each commandline interpreter must support additional methods set.
/// Example: For each unknown user (from incoming connection) auto-created
/// instance of session interpreter. sess. Each "sess" can be extended to
/// "sudo" interpreter with administartor abilites. Used realization with
/// base, IUnknown interface with support QueryInterface protocol. Is not
/// really good idea, but easy to undestand. Each commandline session use
/// query interface to change self functions set. Used only one extender.
/// If need more then one, co-process used. So create co-process:
/// > new_gxvm name interface_name heap:/textures/my%20new%20texture2d
/// > new_call name get_interface
/// > new_call name set_interface extended interface name
/// > name.query_interface("texture2d")
// reserve => declare
// declare => created  // some other process right now build this node
// created => started  // node created, wait exec to first calculation
// invalid => started  // node changed, wait exec to re-calculate data
// started => success | failure | invalid
// success => invalid
// failure => invalid
void gx::user::on_remote_string(QString message, QWebSocket *socket)
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


void gx::user::on_remote_binary(QByteArray message, QWebSocket *socket)
{
    qDebug()<<"SESS BIN" << (void*)this << (void*)socket<<message;
    if (socket) socket->sendBinaryMessage(message);
}


void gx::user::on_remote_closed(QWebSocket *socket)
{
    qDebug()<<"SESS DEL" << (void*)this << (void*) socket;
}

void gx::user::on_remote_opened(QWebSocket *socket)
{
    qDebug()<<"SESS NEW" << (void*)this << (void*) socket;
}

void gx::user::set_attribute( QWebSocket *socket, const QString& arguments_line )
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

void gx::user::build_or_rebuild_interface()
{
    // Re-Insert all "Interface" methods to mm_self_interface

    mm_self_interface.clear();

    int methods_count = this->metaObject()->methodCount();

    for( int i = 0; i < methods_count; ++i )
    {
       QMetaMethod method = this->metaObject()->method(i);

       QString name = method.name();

       static QRegExp method_name_parser("^(gxvm_|new_|get_|set_|del_|add_)\\w+$");


       int pos = method_name_parser.indexIn(name);

       if ( pos == 0 )
       {
           mm_self_interface[name.toStdString()] = std::pair<QMetaMethod, QObject*>(method, this);
       }
    }
}


void gx::user::new_sock_test(QWebSocket *socket, const QString &arguments_line)
{
    if( socket )
    {
        QJsonObject echo;
        echo["proc"] = "new_sock_test";
        echo["args"] = arguments_line;
        socket->sendTextMessage(QJsonDocument(echo).toJson());
    }
}


void gx::user::get_interface(QWebSocket* socket, const QString&)
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

void gx::user::add_interface(QWebSocket* socket, const QString& interface_name)
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

void gx::user::del_interface(QWebSocket* socket, const QString& interface_name)
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

void gx::user::get_attribute(QWebSocket *socket, const QString &arguments_line)
{
    QJsonObject echo;

    echo["proc"] = "get_attribute";

    echo["args"] = arguments_line;

    if( socket )
    {
        echo["echo"] = QString("SUCCESS: ") + arguments_line + " removed";
        socket->sendTextMessage(QJsonDocument(echo).toJson());
    }
}

// static
gx::slot::href gx::user::new_gxvm(const char* path)
{
        gx::slot::href test_slot = gx::root::slot_make(path);

        auto sp_type = gx::root::type_find("gxvm");

        const char* error = nullptr;

        error = test_slot->set_type( sp_type );

        if(error)
        {
            qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
            qDebug() << "      " << error;
            return nullptr;
        }

//        error = test_slot->set_data( RAW_JSON( [ 11, 12, 13, 14
//                                               , 21, 22, 23, 24
//                                               , 31, 32, 33, 34
//                                               , 41, 42, 43, 44 ] ) );

//        if(error)
//        {
//            qDebug() << "ERROR:" << test_slot->get_path();
//            qDebug() << "      " << error;
//            return nullptr;
//        }

        gx::root::show(test_slot.get());

        return test_slot;
}
