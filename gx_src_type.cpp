#include <QDebug>

#include <gx_src_user.h>

#include <gx_src_type.h>

/// STRATEGY HERE:
/// 1. slot-type object can create and manage node-derived value in slot
///
/// 2. slot-type object detect and set node-type object in concrete slot
///
/// 3. node-type object manage node values.
///
/// 4. slot-type is node-type for all builtin types.
///
/// EVENTS SENDED TO SLOTS CONTAIN
///  { slot, slot, slot }
///  { "QString recv_path", "QString data_path", "QString echo_path" }
///  { "QString path", "uint reciver_uid", "QString data", "uint sender_uid" }

/// DATA IS SOME JSON, LIKE
/// { "__type__" : "__type__"                 // builtin type name, or __type__
/// , "__base__" : "user_defined_type_name"   //
/// , "__diff__" : { "self"       : "base_type_name"
///                , "some_array" : "float buff 4 5 6 8"   // new uv1f(4*5*6*8); // => std::vector<float> data[4*5*6*8]
///                , "some_uint"  : "uint"
///                , "some_slot"  : "user_defined_type_channel:list"
///                }
/// , "some_array": [ 0, 0, 0, 1 ]
/// , "some_uint" : 0
/// , "some_slot" : [
///
/// ]
/// }


const char* gx::none_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'none'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::none);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}


const char* gx::dict_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'dict'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::dict);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}


const char* gx::list_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'list'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::list);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}

const char* gx::gxvm_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'gxvm'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gxvm);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}

const char* gx::fail_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'fail'" << QString( p_slot->get_path() );

    // TYPE FIXED BY gx::root::error(..), is shared fail_type_object instance
    // p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    // MEMORY ALLOCATED WITH gx::root::error(..)
    // p_slot->sp_node = std::shared_ptr<node>(new gx::fail);

    // p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}


const char* gx::dict::set_from_dict( const QJsonObject& src ) noexcept
{
    if ( src.isEmpty() ) return nullptr;  // Set from empty dict? - Ignore

    // INITIALLY, 'slot-node-type' IS 'slot-type', BUT MAY BE CHANGED LATER.
    // HERE MUST BE USED ACTUAL NODE'S TYPE-DEFS-DICT. PAIRS NAME:TYPE_NAME.

    auto slot_type_defs = sp_type->get_type_defs();  // std::map< std::string, std::string >

    qDebug() << "||>TYPEDEFS_AT" << (void*)slot_type_defs.get();

    for( auto key: src.keys() )
    {
        qDebug() << key << src[key];

        QJsonValue json_value = src[key];

        gx::type::href slot_type_object;

        const char*    slot_type_name = "?";

        // slot-type-name not exists in slot-type-defs, so detect slot-type-name from json value type

        if      ( json_value.isArray()     )  { qDebug() << "array";     slot_type_name = "list"; }
        else if ( json_value.isObject()    )  { qDebug() << "object";    slot_type_name = "dict"; }
        else if ( json_value.isBool ()     )  { qDebug() << "bool";      slot_type_name = "bool"; }
        else if ( json_value.isDouble()    )  { qDebug() << "double";    slot_type_name = "real"; }
        else if ( json_value.isNull()      )  { qDebug() << "null";      slot_type_name = "none"; }
        else if ( json_value.isString()    )  { qDebug() << "string";    slot_type_name = "qstr"; }
        else if ( json_value.isUndefined() )  { qDebug() << "undefined"; slot_type_name = "none"; }
        else                                  { qDebug() << "unknown";   slot_type_name = "none"; }

        qDebug() << "slot_type_name is" << QString(slot_type_name);

        // The target slot may be already created
        //
        // qDebug() << "*****THIS SLOT PATH" << mp_slot->get_path();

        std::string slot_item_path(mp_slot->get_path());

        slot_item_path += "/";  // Depend Semantic:
                                //   heap://slot_name/slot_attr_name/slot_attr_sub_name/method?arguments
                                // or:
                                //   window.context.prgrams.StingrayPBS.vertex_shader(get)

        slot_item_path += key.toStdString();  // auto-generated path to next item;

        uint item_hash = gx::root::hash( slot_item_path.c_str() );

        auto exists = gx::root::globals().find(item_hash);

        gx::slot::href item_slot;

        if( gx::root::globals().end() != exists )
        {
            item_slot = exists->second.lock();
            qDebug() << "dict item exists" << (void*) item_slot.get();
        }
        else
        {
            item_slot = gx::root::slot_make(slot_item_path.c_str(), item_hash);

            auto sp_type = gx::root::type_find(slot_type_name);

            const char* error = nullptr;

            error = item_slot->set_type(sp_type);

            if(error)
            {
                qDebug() << "*****DICT ITEM LOAD SET TYPE ERROR:" << error;
            }

            qDebug() << "dict item create" << (void*) item_slot.get();
        }

        mo_dict[key.toStdString()] = item_slot;  // store created or existed slot in this dict;

        item_slot->set_data(json_value);  // recursive call set_data

        gx::root::show( item_slot.get() );
    }

    return nullptr;
}

const char* gx::list::set_from_list( const QJsonArray& src ) noexcept
{
    if ( src.isEmpty() ) return nullptr;  // Set from empty dict? - Ignore

    return "NOT IMPLEMENTED list from list";
}

const char* gx::real_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'real'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::real);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}


const char* gx::bool_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'bool'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::Bool);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}


const char* gx::qstr_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'qstr'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::qstr);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}


const char* gx::xuv2f_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'uv2f'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::xuv2f);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}

const char* gx::xuv3f_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'uv3f'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::xuv3f);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}

const char* gx::xum4f_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'um4f'" << QString( p_slot->get_path() );

    p_slot->sp_type = gx::root::type_find( get_type_name().c_str() );

    p_slot->sp_node = std::shared_ptr<node>(new gx::xum4f);

    p_slot->sp_node->sp_type = p_slot->sp_type;

    p_slot->sp_node->mp_slot = p_slot;

    return nullptr;
}

const char* gx::xuv2f::set_from_list(const QJsonArray & list) noexcept
{
    qDebug() << "    <<UV2F>> SET" << list;

    uint offset = 0;

    for ( auto i : list )
    {
        if ( i.isDouble() )
        {
            // qDebug() << "    <<UV2F>> SET" << offset << i;

            buff[offset] = float(i.toDouble());
        }
        else
        {
            qDebug() << "    <<UV2F>> ERROR SET" << offset << i;
        }

        if( ++offset >= buff.size() )
        {
            break;
        }
    }

    return nullptr;
}

const char* gx::xuv3f::set_from_list(const QJsonArray & list) noexcept
{
    qDebug() << "    <<UV3F>> SET" << list;

    uint offset = 0;

    for ( auto i : list )
    {
        if ( i.isDouble() )
        {
            // qDebug() << "    <<UV3F>> SET" << offset << i;

            buff[offset] = float(i.toDouble());
        }
        else
        {
            qDebug() << "    <<UV3F>> ERROR SET" << offset << i;
        }

        if( ++offset >= buff.size() )
        {
            break;
        }
    }

    return nullptr;
}

const char* gx::xum4f::set_from_list(const QJsonArray & list) noexcept
{
    qDebug() << "    <<UM4F>> SET" << list;

    uint offset = 0;

    for ( auto i : list )
    {
        if ( i.isDouble() )
        {
            // qDebug() << "    <<UM4F>> SET" << offset << i;

            buff[offset] = float(i.toDouble());
        }
        else
        {
            qDebug() << "    <<UM4F>> ERROR SET" << offset << i;
        }

        if( ++offset >= buff.size() )
        {
            break;
        }
    }

    return nullptr;
}


void gx::root::dump(const char* path, QJsonObject& obj) noexcept
{
    uint hash = gx::root::hash(path);

    gx::slot::href sp_slot;

    auto exists = gx::root::globals().find(hash);

    if ( gx::root::globals().end() != exists )
    {
        sp_slot = exists->second.lock();
    }
    else
    {
        sp_slot = nullptr;
    }

    obj["path"] = path;

    gx::root::dump( sp_slot.get(), obj);
}


void gx::root::dump(gx::slot* s, QJsonObject& obj) noexcept
{
    if ( nullptr == s )
    {
        obj["slot_heap"] = QString::number( uint( (void*)s ) , 16 );
        return;
    }

    obj["slot_heap"] = "slot at " + QString::number( uint( (void*)s ) , 16 );
    obj["slot_path"] = s->path.c_str();

    return;

    if ( nullptr == s->sp_type )

        qDebug() << "  type is nullptr";

    else

        qDebug() << "  type" << (void*)s->sp_type.get() << QString(s->sp_type->get_type_name().c_str());

    qDebug()     << "  node" << (void*)s->sp_node.get();

    if( nullptr == s->sp_node) return;

    gx::node* p_node = s->sp_node.get();

    qDebug() << "    type"   << (void*) p_node->sp_type.get();

    if( p_node->get_xunfa() )
    {
        qDebug() << "      xunfa" << (void*) p_node->get_xunfa();

        static struct : gx::xunfa::proc
        {
            void on(gx::xuv2f* o)
            {
                qDebug() << "        uv2f" << (void*)o << "["
                << o->buff[0] << ","
                << o->buff[1] << "]"
                   ;
            }

            void on(gx::xuv3f* o)
            {
                qDebug() << "        uv3f" << (void*)o << "["
                << o->buff[0] << ","
                << o->buff[1] << ","
                << o->buff[2] << "]"
                   ;
            }

            void on(gx::xum4f* o)
            {
                qDebug() << "        um4f" << (void*)o << ":";

                qDebug() << "          |"
                         << o->buff [0] << "," << o->buff [1] << "," << o->buff [2] << "," << o->buff [3] << "|";
                qDebug() << "          |"
                         << o->buff [4] << "," << o->buff [5] << "," << o->buff [6] << "," << o->buff [7] << "|";
                qDebug() << "          |"
                         << o->buff [8] << "," << o->buff [9] << "," << o->buff[10] << "," << o->buff[11] << "|";
                qDebug() << "          |"
                         << o->buff[12] << "," << o->buff[13] << "," << o->buff[14] << "," << o->buff[15] << "|";
            }
        }
        u_show;
        p_node->get_xunfa()->on(&u_show);
    }

    // NEXT ADDITIONAL 'VTXA' INTERFACE:
    // THIS NODE-DERIVED OBJECT CAN BE COMPUTED/LOADED AND SENDED AS OpenGL vertex buffer object
    // to GPU memory as VERTEX BUFFER OBJECT. BINDED AND USED AS VERTEX-ATTRIBUTE GLSL VARIABLE.
    if ( p_node->get_xvtxa() )
    {
        qDebug() << "   xvtxa" << (void*) p_node->get_xvtxa();
    }

    if ( p_node->get_vtxa() )
    {
        qDebug() << "    vtxa" << (void*) p_node->get_vtxa();
    }

    if ( p_node->get_unfa() )
    {
        qDebug() << "    unfa" << (void*) p_node->get_unfa();
    }

    // NEXT ADDITIONAL 'PROG' INTERFACE:
    // THIS NODE-DERIVED OBJECT CAN BE COMPILED AS GLSL PROGRAM, LINKED, AND BINDED.
    // AFTER COMPILE OBJECT CAN INIT SELF OWN VARIABLES DICT ( DICT INTERFACE SUPPORTED )
    if ( p_node->get_prog() )
    {
        qDebug() << "    prog" << (void*) p_node->get_prog();
    }

    // NEXT ADDITIONAL 'DICT' INTERFACE
    if ( p_node->get_dict() )
    {
        qDebug() << "    dict" << (void*) p_node->get_dict();
    }

    // NEXT ADDITIONAL 'LIST' INTERFACE
    if ( p_node->get_list() )
    {
        qDebug() << "    list" << (void*) p_node->get_list();
    }

    if ( p_node->get_json() )
    {
        qDebug() << "    json" << (void*) p_node->get_json();

        static struct : gx::json::proc
        {
            void on(gx::none*o) { qDebug() << "      none" << (void*)o;
                                }
            void on(gx::real*o) { qDebug() << "      real" << (void*)o <<  o->get_real(); }

            void on(gx::qstr*o) { qDebug() << "      qstr" << (void*)o << *o->get_qstr(); }

            void on(gx::Bool*o) { qDebug() << "      bool" << (void*)o <<  o->get_bool(); }
        }
        separator;

        p_node->get_json()->on(&separator);
    }

    if ( p_node->get_proc() )
    {
        qDebug() << "    fail" << QString(p_node->get_proc()->mo_error.c_str());

        if( p_node->get_proc()->mo_path.size() )
        {
            qDebug() << "    path" << QString(p_node->get_proc()->mo_path.c_str());
        }
    }
}


void gx::root::show(gx::slot* s)  noexcept
{
    if ( nullptr == s )
    {
        qDebug()     << "slot"   << (void*)s;
        return;
    }

    qDebug()     << "slot"   << (void*)s << s->path.c_str();

    if ( nullptr == s->sp_type )

        qDebug() << "  type is nullptr";

    else

        qDebug() << "  type" << (void*)s->sp_type.get() << QString(s->sp_type->get_type_name().c_str());

    qDebug()     << "  node" << (void*)s->sp_node.get();

    if( nullptr == s->sp_node) return;

    gx::node* p_node = s->sp_node.get();

    qDebug() << "    type"   << (void*) p_node->sp_type.get();

    if( p_node->get_xunfa() )
    {
        qDebug() << "      xunfa" << (void*) p_node->get_xunfa();

        static struct : gx::xunfa::proc
        {
            void on(gx::xuv2f* o)
            {
                qDebug() << "        uv2f" << (void*)o << "["
                << o->buff[0] << ","
                << o->buff[1] << "]"
                   ;
            }

            void on(gx::xuv3f* o)
            {
                qDebug() << "        uv3f" << (void*)o << "["
                << o->buff[0] << ","
                << o->buff[1] << ","
                << o->buff[2] << "]"
                   ;
            }

            void on(gx::xum4f* o)
            {
                qDebug() << "        um4f" << (void*)o << ":";

                qDebug() << "          |"
                         << o->buff [0] << "," << o->buff [1] << "," << o->buff [2] << "," << o->buff [3] << "|";
                qDebug() << "          |"
                         << o->buff [4] << "," << o->buff [5] << "," << o->buff [6] << "," << o->buff [7] << "|";
                qDebug() << "          |"
                         << o->buff [8] << "," << o->buff [9] << "," << o->buff[10] << "," << o->buff[11] << "|";
                qDebug() << "          |"
                         << o->buff[12] << "," << o->buff[13] << "," << o->buff[14] << "," << o->buff[15] << "|";
            }
        }
        u_show;
        p_node->get_xunfa()->on(&u_show);
    }

    // NEXT ADDITIONAL 'VTXA' INTERFACE:
    // THIS NODE-DERIVED OBJECT CAN BE COMPUTED/LOADED AND SENDED AS OpenGL vertex buffer object
    // to GPU memory as VERTEX BUFFER OBJECT. BINDED AND USED AS VERTEX-ATTRIBUTE GLSL VARIABLE.
    if ( p_node->get_xvtxa() )
    {
        qDebug() << "   xvtxa" << (void*) p_node->get_xvtxa();
    }

    if ( p_node->get_vtxa() )
    {
        qDebug() << "    vtxa" << (void*) p_node->get_vtxa();
    }

    if ( p_node->get_unfa() )
    {
        qDebug() << "    unfa" << (void*) p_node->get_unfa();
    }

    // NEXT ADDITIONAL 'PROG' INTERFACE:
    // THIS NODE-DERIVED OBJECT CAN BE COMPILED AS GLSL PROGRAM, LINKED, AND BINDED.
    // AFTER COMPILE OBJECT CAN INIT SELF OWN VARIABLES DICT ( DICT INTERFACE SUPPORTED )
    if ( p_node->get_prog() )
    {
        qDebug() << "    prog" << (void*) p_node->get_prog();
    }

    // NEXT ADDITIONAL 'DICT' INTERFACE
    if ( p_node->get_dict() )
    {
        qDebug() << "    dict" << (void*) p_node->get_dict();
    }

    // NEXT ADDITIONAL 'LIST' INTERFACE
    if ( p_node->get_list() )
    {
        qDebug() << "    list" << (void*) p_node->get_list();
    }

    if ( p_node->get_json() )
    {
        qDebug() << "    json" << (void*) p_node->get_json();

        static struct : gx::json::proc
        {
            void on(gx::none*o) { qDebug() << "      none" << (void*)o;
                                }
            void on(gx::real*o) { qDebug() << "      real" << (void*)o <<  o->get_real(); }

            void on(gx::qstr*o) { qDebug() << "      qstr" << (void*)o << *o->get_qstr(); }

            void on(gx::Bool*o) { qDebug() << "      bool" << (void*)o <<  o->get_bool(); }
        }
        separator;

        p_node->get_json()->on(&separator);
    }

    if ( p_node->get_proc() )
    {
        qDebug() << "    fail" << QString(p_node->get_proc()->mo_error.c_str());

        if( p_node->get_proc()->mo_path.size() )
        {
            qDebug() << "    path" << QString(p_node->get_proc()->mo_path.c_str());
        }
    }
}
