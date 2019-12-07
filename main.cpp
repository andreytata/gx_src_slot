#include <QApplication>
#include <QMainWindow>

#include <gx_src_slot.h>

static gx::slot::href test_static_slot = gx::root::slot_make("heap://static/static_slot");

int test_static_slot_type()
{
    gx::root::root_info();  // test_static_slot->"

    auto sp_type = gx::root::type_find("uv3f");

    test_static_slot->set_type(sp_type);

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
//    QMainWindow wnd;
//    wnd.show();

    qDebug() << "|>--------- MAIN BEGIN -------------:)";

//    qDebug() << "\ntest_root_slot" << ((0 == test_root_slot("heap://static/static_slot")) ? "SUCCESS\n\n" : "FAILURE\n\n");

//    qDebug() << "\ntest_root_type" << ((0 == test_root_type("uv3f")) ? "SUCCESS\n\n" : "FAILURE\n\n");
//    qDebug() << "\ntest_root_type" << ((0 == test_root_type("type_name")) ? "SUCCESS\n\n" : "FAILURE\n\n");

test_static_slot_type();
    qDebug() << "|>--------- MAIN END ----------------:(";

    return 0;

    gx::slot::href temp;

    auto path = "heap://main_loop//test_slot";

    uint hash = gx::root::hash(path);

    // gx::slot::href test_slot
    gx::root::smap::iterator exists = gx::root::globals().find(hash);

    if( exists == gx::root::globals().end() )
    {
        temp = gx::root::slot_make(path, hash);
    }

    qDebug() << "|>------------";
    gx::root::root_info();

    qDebug() << "|>MAIN LOOP BEGIN";
    int result = 0; //app.exec();

    qDebug() << "|>MAIN LOOP END";

    return result;
}
