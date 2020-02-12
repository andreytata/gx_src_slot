/*
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "echoserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("QtWebSockets example: echoserver");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for echoserver [default: 1234]."),
            QCoreApplication::translate("main", "port"), QLatin1String("1234"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    EchoServer *server = new EchoServer(port, debug);
    QObject::connect(server, &EchoServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
*/

#include <iostream>

#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include <gx_src_es20.h>
#endif
#include <gx_src_serv.h>
#include <gx_src_test.h>

#include <gx_src_user.h>

const char* cmd_new = RAW_JSON(
{      "__type__": "new"                                         // * commands are only "set", "get" and "new"
,      "__isas__": "heap://name/name/name/prototype_name"        // * path to prototype, from where object cloned
,    "transform" : [ "um4f", [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1] ] // owerride type of the 'attr1', even prototype has one
,"sample_vector3": [ "uv3f", [1, 2, 3] ]                         // name: [ typename, value ]
,      "__defs__": [ "defs",
                { "m_float_buff":"uv1f:4:4"   // uv1f o(size=4*4)
                , "m_quaternion":"uv4f"       // uv4f o(size=1)
                , "m_uint_array":"uint:2"     // uint o[2] = { -1, 34 };
                }]
});


int test_static_slot_type()  // primitive type set_json / get_json
{
    static gx::slot::href test_static_slot = gx::root::slot_make("heap://static/static_slot");

    gx::root::root_info();  // test_static_slot->"

    auto sp_type = gx::root::type_find("uv2f");

    const char* error = nullptr;

    error = test_static_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_static_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
    }

    error = test_static_slot->set_data( RAW_JSON( [ 1.0, -2, 3.14 ] ) );               // test json list of numbers

//    error = test_static_slot->set_data( RAW_JSON( { "x":11, "y":12 } ) );        // test json dict

//    error = test_static_slot->set_data( RAW_JSON( "Hello ASCII world!" ) );      // test json string

//    error = test_static_slot->set_data( RAW_JSON( "1234567890" ) );              // test json string

    if(error)
    {
        qDebug() << "ERROR:" << test_static_slot->get_path();
        qDebug() << "      " << error;
    }

    gx::root::show(test_static_slot.get());

    return 0;
}

/// SETTER. Each field, except "__type__" field, contain JSON data,
/// w/o additional TYPE-INFO. A "__type__" field has value "setter".
/// SHARED HASHABLE. It is effective when MULTIPLE OBJECTS ANIMATED
/// FROM SINGLETONED SETTERS LIST. SO EACH OBJECT HAS SELF OWN STEP
/// (CURRENT POSITION) IN SETTERS LIST. The "__type__" field can't
/// be changed (because object's instance created from "__type__").
const char* setter_test = RAW_JSON(
{"__type__":"__type__"       // this object's type is type_defs
,"__name__":"um4f_extended"  // this type type_name
,"__base__":"um4f"           // base type,

,"__defs__":
{"m_float_buff":"uv1f:4:4"   // uv1f o(size=4*4)
,"m_quaternion":"uv4f"       // uv4f o(size=1)
,"m_uint_array":"uint:2"     // uint o[2] = { -1, 34 };
}

,"__init__":
{"m_float_buff":[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]  // initial value
,"m_uint_array":[-1,34]     // => values list
}

});

/// SOME SLOT ????.
/// INIT SLOT-TYPE
/// EXEC SLOT-TYPE->__INIT__(SLOT ????)              // compute type name from SLOT and THIS SLOT TYPE
///      READ ???? FROM SLOT & DETECT NODE-TYPE
///      MAKE NODE, SET NODE-TYPE;
///      EXEC NODE-TYPE->__INIT__(SLOT ????, NODE*)  // compute created NODE's initial values
/// SWITCH SLOT's MODE TO 'MAIN'
///
/// Create dict-like-node, contain std::map<std::string, std::shared_ptr<gx::slot> >
/// Apply type from __defs__ to each slot in this dict-like-node;
/// Execute __init__

int test_root_type(const char* type_name )
{
    qDebug() << "TYPE test, type_name is:" << QString(type_name);

    auto type_object = gx::root::type_find( type_name );

    if( nullptr == type_object )
    {
        qDebug() << "TEST TYPE FAILURE:\n   has no object named" << QString(type_name);

        return 1;
    }

    return 0;
}

int test_root_slot(const char* path)
{
    qDebug() << "TEST BEGIN" << QString(path);

    std::string head(path);
    std::list<gx::slot::href> PAR_pool = {
        gx::root::slot_make((head+"0").c_str()),
        gx::root::slot_make((head+"1").c_str()),
        gx::root::slot_make((head+"2").c_str()),
        gx::root::slot_make((head+"3").c_str()),
        gx::root::slot_make((head+"4").c_str()),
        gx::root::slot_make((head+"5").c_str()),
    };

    qDebug()<< "  " << PAR_pool.size();

    auto href_1 = gx::root::slot_make(path);

    auto mode_0 = gx::root::slot_mode(path);

    static struct : gx::slot::mode::proc
    {
        void on(gx::slot::mode::init*) { mode_name = "init"; }
        void on(gx::slot::mode::main*) { mode_name = "main"; }
        void on(gx::slot::mode::free*) { mode_name = "free"; }
        void on(gx::slot::mode::none*) { mode_name = "none"; }
        const char* mode_name;
    }   mode_proc;

    href_1->get_mode()->on(&mode_proc);
    qDebug() << "href_1 mode" << mode_proc.mode_name;

    mode_0->on(&mode_proc);
    qDebug() << "mode_0 name" << mode_proc.mode_name;

    qDebug() << "TEST END  " << QString(path);

    return 0;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "|>----- MAIN-INIT BEGIN --------------<|";

    gx::slot::href test_fail = test_fail_type("ERROR: sample error description");
    qDebug() << "|>---- test_FAIL_type ----" << ( test_fail ? "Success" : "Failure !!!" );
    if ( test_fail )
    {
        qDebug() << "<*><*><*><*><*><*><*>Test FAIL ";
        gx::root::show( test_fail.get() );
    }
    // gx::slot::href test_fail = gx::root::error("test error object");

    gx::root::show(test_fail.get());

    // return 0;

    gx::slot::href test_qstr = test_qstr_type("heap://tests/test_qstr");
    qDebug() << "|>---- test_bool_type ----" << ( test_qstr ? "Success" : "Failure !!!" );
    if ( test_qstr )
    {
        qDebug() << "<*><*><*><*><*><*><*>Test qstr ";
        gx::root::show( test_qstr.get() );
    }

    gx::slot::href test_bool = test_bool_type("heap://tests/test_bool");
    qDebug() << "|>---- test_bool_type ----" << ( test_bool ? "Success" : "Failure !!!" );
    if ( test_bool )
    {
        qDebug() << "<*><*><*><*><*><*><*>Test bool ";
        gx::root::show( test_bool.get() );
    }

    gx::slot::href test_real = test_real_type("heap://tests/test_real");
    qDebug() << "|>---- test_real_type ----" << ( test_real ? "Success" : "Failure !!!" );
    if ( test_real )
    {
        qDebug() << "<*><*><*><*><*><*><*>Test real ";
        gx::root::show( test_real.get() );
    }

    gx::slot::href test_none = test_none_type("heap://tests/test_none");
    qDebug() << "|>---- test_none_type ----" << ( test_none ? "Success" : "Failure !!!" );

    gx::slot::href test_uv2f = test_uv2f_type("heap://tests/test_uv2f");
    qDebug() << "|>---- test_uv2f_type ----" << ( test_uv2f ? "Success" : "Failure !!!" );

    gx::slot::href test_uv3f = test_uv3f_type("heap://tests/test_uv3f");
    qDebug() << "|>---- test_uv3f_type ----" << ( test_uv3f ? "Success" : "Failure !!!" );

    gx::slot::href test_um4f = test_um4f_type("heap://tests/test_um4f");
    qDebug() << "|>---- test_um4f_type ----" << ( test_um4f ? "Success" : "Failure !!!" );

    gx::slot::href test_dict = test_dict_type("heap://tests/test_dict");
    qDebug() << "|>---- test_dict_type ----" << ( test_dict ? "Success" : "Failure !!!" );

    gx::slot::href test_list = test_list_type("heap://tests/test_list");
    qDebug() << "|>---- test_list_type ----" << ( test_list ? "Success" : "Failure !!!" );

    //gx::user root_gxvm( NULL, false );

    gx::slot::href gxvm = gx::user::new_gxvm("/main");
    qDebug() << "|>---- test_GXVM_type ----" << ( gxvm ? "Success" : "Failure !!!" );

    qDebug() << "|>----- MAIN-LOOP READY --------------<|";

    gx::root::root_info();

    qDebug() << "|>----- MAIN SURFACE-FORMAT-SETUP ----<|";

    QSurfaceFormat format;

    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    qDebug() << "|>----- MAIN APP-NAME-VERSION-SETUP --<|";

    app.setApplicationName("BMB client");

    app.setApplicationVersion("0.2.1");

    qDebug() << "|>----- MAIN GLSL WINDOW CREATE ------<|";

    #ifndef QT_NO_OPENGL
        gx::es20 gl_widget;  // can be replaced with gx::gap_generated widget
        gl_widget.show();
    #else
        QLabel note("OpenGL Support required");
        note.show();
    #endif

    qDebug() << "|>----- MAIN RUN-WEBSOCKET-SERVER -----<|";

    // Create websocket-server to remote Python/WebApp manage 'heap://' contents,
    // support remote tests and webbrowser based contents introspection

    EchoServer *server = new EchoServer(4680, true);

    QObject::connect(server, &EchoServer::closed, &app, &QCoreApplication::quit);

    qDebug() << "|>----- MAIN-LOOP BEGIN ---------------<|";

    int result = app.exec();

    qDebug() << "|>----- MAIN-LOOP FINAL ---------------<|"; //DONELOOP END";

    return result;
}
