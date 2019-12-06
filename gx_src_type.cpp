#include <QDebug>

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

void gx::uv2f_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'uv2f'" << QString( p_slot->get_path() );
    p_slot->sp_node = std::shared_ptr<node>(new gx::uv2f);
}

void gx::uv3f_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'uv3f'" << QString( p_slot->get_path() );
    p_slot->sp_node = std::shared_ptr<node>(new gx::uv3f);
}

void gx::um4f_type_object::__init__(gx::slot* p_slot) noexcept
{
    qDebug() << "|><<SLOT>> SET_TYPE 'um4f'" << QString( p_slot->get_path() );
    p_slot->sp_node = std::shared_ptr<node>(new gx::um4f);
}
