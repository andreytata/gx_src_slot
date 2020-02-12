#ifndef GX_SRC_TEST_H
#define GX_SRC_TEST_H

#include <gx_src_slot.h>

gx::slot::href find_or_make(const char* path, const char* type)
{
    gx::type::href sp_type = gx::root::type_find(type);

    if( nullptr == sp_type )
    {
        qDebug() << "ERROR: is not builtin type" << QString(type);
        return nullptr;
    }

    gx::slot::href sp_slot = nullptr;

    uint hash = gx::root::hash(path);

    auto exists = gx::root::globals().find(hash);

    if ( gx::root::globals().end() != exists )
    {
        sp_slot = exists->second.lock();

        if ( sp_slot->sp_type->get_type_name() != std::string(type) )
        {
            qDebug() << "ERROR: slot" << QString(path)
                     << "has type"    << QString(sp_slot->sp_type->get_type_name().c_str())
                     << "not"         << QString(type) ;

            return nullptr;
        }

        return sp_slot;
    }

    sp_slot = gx::root::slot_make( path, hash );

    const char* error = nullptr;

    error = sp_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << sp_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();

        qDebug() << "      " << error;

        return nullptr;
    }

    gx::root::show(sp_slot.get());

    return sp_slot;
}

gx::slot::href test_none_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("none");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}

gx::slot::href test_bool_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("bool");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}


gx::slot::href test_qstr_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("qstr");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}


gx::slot::href test_fail_type(const char* error_description)
{
    // Slot-with-fail newer inserted in gx::root::globals() map.
    // After lost refs, slot-with-fail must be deleted. No hash.
    // Because next try of SET/GET/NEW can be done successfully.
    //
    // Create node with type "fail" is error. Is return failure.
    // Not new gx::root::globals object. Only href to temporary
    // slot created by gx::root::fail("can't create object with
    // type 'fail'"). With path field "target path".
    //
    gx::slot::href test_slot = gx::root::error(error_description);

    return test_slot;
}


gx::slot::href test_real_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("real");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}


gx::slot::href test_dict_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("dict");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    error = test_slot->set_data(RAW_JSON({"x":[11, 12, 13, 14],
                                          "y":[21, 22, 23, 24],
                                          "z":[31, 32, 33, 34],
                                          "w":[41, 42, 43, 44]}));

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}


gx::slot::href test_list_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("list");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    error = test_slot->set_data(RAW_JSON( [ "x", [11, 12, 13, 14] ,
                                            "y", {} ,
                                            "z", 31 ,
                                            "w", null ] ) );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}


gx::slot::href test_um4f_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("um4f");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    error = test_slot->set_data( RAW_JSON( [ 11, 12, 13, 14
                                           , 21, 22, 23, 24
                                           , 31, 32, 33, 34
                                           , 41, 42, 43, 44 ] ) );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}


gx::slot::href test_uv3f_type( const char* path)
{
    static gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("uv3f");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    error = test_slot->set_data( RAW_JSON( [ 1.0, -2, 3.14 ] ) );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return test_slot;
}


gx::slot::href test_uv2f_type(const char* path)
{
    gx::slot::href test_slot = gx::root::slot_make(path);

    auto sp_type = gx::root::type_find("uv2f");

    const char* error = nullptr;

    error = test_slot->set_type( sp_type );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path() << "SET TYPE" << sp_type->get_type_name().c_str();
        qDebug() << "      " << error;
        return nullptr;
    }

    error = test_slot->set_data( RAW_JSON( [ -1.0, -2.14 ] ) );

    if(error)
    {
        qDebug() << "ERROR:" << test_slot->get_path();
        qDebug() << "      " << error;
        return nullptr;
    }

    gx::root::show(test_slot.get());

    return 0;
}


#endif // GX_SRC_TEST_H
