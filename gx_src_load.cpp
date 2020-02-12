#include <gx_src_slot.h>

/// LOAD NODE DATA FROM JSON VALUE
///
const char* gx::type::__load__(gx::slot* self, const QJsonValue& data) noexcept
{
    // IS DEFAULT DATA PARSER, EACH TYPE-DERIVED OBJECT CAN OVERLOAD THIS METHOD.

    // ALL slot, slot-type, node and node-type ASSERTS REMOVED TO gx::slot::set_data.
    // IF THIS METHOD CALLED:
    //   1. nullptr != self                       // slot->set_data(data)
    //   2. nullptr != self->sp_type              // slot-type defined and called, so
    //   3. nullptr != self->sp_node              // node created by slot-type
    //   4. nullptr != self->sp_node->sp_type     // node-type created and stored

    // PARSE DATA ONLY
    if      ( data.isArray  () ) { return self->sp_node->set_from_list( data.toArray  () ); }

    else if ( data.isObject () ) { return self->sp_node->set_from_dict( data.toObject () ); }

    else if ( data.isString () ) { return self->sp_node->set_from_qstr( data.toString () ); }

    else if ( data.isDouble () ) { return self->sp_node->set_from_real( data.toDouble () ); }

    else if ( data.isBool   () ) { return self->sp_node->set_from_bool( data.toBool   () ); }

    else if ( data.isNull   () ) { return self->sp_node->set_from_null(); }

    return "unsupported load from 'unknown' json type";

}
