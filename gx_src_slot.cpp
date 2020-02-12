#include <gx_src_slot.h>

#include <gx_src_type.h>

#include <QHash>


gx::slot::slot()             { qDebug() << "++<< SLOT >>" << gx::root::hex_hash( 0 )          << (void*) this; }

gx::slot::~slot()            { qDebug() << "--<< SLOT >>" << gx::root::hex_hash( this->hash ) << (void*) this; }

struct gx::slot::mode::init : gx::slot::mode { void on(gx::slot::mode::proc* __) { __->on(this); } };

struct gx::slot::mode::main : gx::slot::mode { void on(gx::slot::mode::proc* __) { __->on(this); } };

struct gx::slot::mode::none : gx::slot::mode { void on(gx::slot::mode::proc* __) { __->on(this); } };

struct gx::slot::mode::free : gx::slot::mode { void on(gx::slot::mode::proc* __) { __->on(this); } };

gx::slot::mode* gx::slot::mode_init() noexcept { static gx::slot::mode::init __; return &__; }

gx::slot::mode* gx::slot::mode_main() noexcept { static gx::slot::mode::main __; return &__; }

gx::slot::mode* gx::slot::mode_none() noexcept { static gx::slot::mode::none __; return &__; }

gx::slot::mode* gx::slot::mode_free() noexcept { static gx::slot::mode::free __; return &__; }


struct __globals__
{
    static __globals__* get();

    gx::root::smap& allocated_slots() noexcept { return mo_allocated_slots; }

    gx::root::gmap& garbaged_slots()  noexcept { return mo_garbaged_slots; }

    gx::root::tmap& type_objects()    noexcept { return mo_type_objects; }

    std::shared_ptr<gx::fail_type_object> get_fail_type_object() noexcept
    {
        return mo_fail_type_object;
    }

private:

    gx::root::gmap mo_garbaged_slots;

    gx::root::smap mo_allocated_slots;

    gx::root::tmap mo_type_objects;

    std::shared_ptr<gx::fail_type_object> mo_fail_type_object = std::make_shared<gx::fail_type_object>();

    __globals__()  // on first usage
    {
        qDebug() << "++GLOBALS(on first call)";

        // REGISTER all builtin type-objects, except 'fail'

        __globals__::type_objects()["uv2f"] = std::shared_ptr<gx::type>(new gx::xuv2f_type_object);

        __globals__::type_objects()["uv3f"] = std::shared_ptr<gx::type>(new gx::xuv3f_type_object);

        __globals__::type_objects()["um4f"] = std::shared_ptr<gx::type>(new gx::xum4f_type_object);

        __globals__::type_objects()["dict"] = std::shared_ptr<gx::type>(new gx::dict_type_object);

        __globals__::type_objects()["none"] = std::shared_ptr<gx::type>(new gx::none_type_object);

        __globals__::type_objects()["real"] = std::shared_ptr<gx::type>(new gx::real_type_object);

        __globals__::type_objects()["bool"] = std::shared_ptr<gx::type>(new gx::bool_type_object);

        __globals__::type_objects()["qstr"] = std::shared_ptr<gx::type>(new gx::qstr_type_object);

        __globals__::type_objects()["list"] = std::shared_ptr<gx::type>(new gx::list_type_object);

        __globals__::type_objects()["gxvm"] = std::shared_ptr<gx::type>(new gx::gxvm_type_object);
    }

    ~__globals__() // on exit application
    {
        qDebug() << "--GLOBALS";

        gx::root::root_info();

        for( auto& pair : allocated_slots() )
        {
            gx::slot::href slot = pair.second.lock();
            if( slot )
            if( slot->sp_node )
                slot->sp_node->sig_exit(slot.get());
        }

        std::list<uint> garb_list;

        for( auto& pair: garbaged_slots() )
        {
            garb_list.push_back(pair.first);
        }

        for(auto& hash: garb_list)
        {
            gx::root::slot_kill(hash);
        }
    }
};

__globals__* __globals__::get()
{
    static volatile struct destroyer
    {
        destroyer():subj(new __globals__){}
        ~destroyer()
        {
            qDebug() << ">>>>>>>>>>>>>CATCH EXIT HERE";

            auto ggg = gx::root::globals();

            auto gbg = gx::root::garbage();

            gx::root::root_info();

            delete subj;
        }
        __globals__ * subj;
    } auto_destroyer;

    return auto_destroyer.subj;
}

gx::root::smap& gx::root::globals() noexcept { return __globals__::get()->allocated_slots(); }

gx::root::gmap& gx::root::garbage() noexcept { return __globals__::get()->garbaged_slots();  }

gx::root::tmap& gx::root::types()   noexcept { return __globals__::get()->type_objects();    }

gx::slot::href  gx::root::error ( const char* error_c_str ) noexcept
{
    // TODO:
    // HERE EASY TO ADD MULTIPLE fail-based types, each fail is some analog of
    // exception class, depended parsed 'error_s_str' for example:
    // sp_slot->sp_node = std::shared_ptr<gx::node>(new gx::fail);
    // sp_slot->sp_node = std::shared_ptr<gx::node>(new gx::fail_set_error);
    // sp_slot->sp_node = std::shared_ptr<gx::node>(new gx::fail_get_error);
    // sp_slot->sp_node = std::shared_ptr<gx::node>(new gx::fail_new_error);
    // e.t.c.

    gx::proc* p_fail = new gx::proc;

    p_fail->mo_error = error_c_str;

    gx::slot::href sp_slot = std::shared_ptr<gx::slot> ( new gx::slot, gx::root::error_slot_free );

    sp_slot->hash = 0;  // unhashable

    sp_slot->mp_mode = gx::slot::mode_main();

    sp_slot->sp_type = __globals__::get()->get_fail_type_object();

    sp_slot->sp_node = std::shared_ptr<gx::node>(p_fail);

    sp_slot->sp_node->sp_type = sp_slot->sp_type;

    sp_slot->sp_type->__init__(sp_slot.get());

    return sp_slot;
}


void gx::root::slot_kill( const uint& hash ) noexcept
{
    auto exists = garbage().find( hash );

    if( exists != garbage().end() )
    {
        delete exists->second;
        garbage().erase(hash);
    }
    else
    {
        qDebug() << "FAILURE: can't delete 'slot'" << hex_hash(hash);
    }
}


QString gx::root::hex_hash(const uint& number) noexcept
{
    const QString _uint_max = QString::number(uint(-1), 16).fill(QString("0")[0]);

    auto _hex_hash = QString::number(number, 16).toUpper();

    auto out_repr = _uint_max + _hex_hash;

    return out_repr.replace( 0, _hex_hash.size(), "" );
}


void gx::root::root_info()
{
    qDebug() << "|> GLOBAL SCOPE";

    for ( auto& pair : gx::root::globals() )
    {
        gx::slot::href sp_slot = pair.second.lock();
        if( sp_slot )
        {
            qDebug() << " " << hex_hash(pair.first) << hex_hash(sp_slot->get_hash()) << QString(sp_slot->get_path());
        }
        else
        {
            qDebug() << " " << hex_hash(pair.first) << "LOCK FAILURE";
        }
    }

    qDebug() << "   ----";

    qDebug() << "|> GARBAGED SLOTS";

    for( auto& pair : gx::root::garbage() )
    {
        gx::slot* p_slot = pair.second;

        QString hkey = hex_hash(pair.first);

        if( p_slot )
        {
            QString skey = hex_hash(p_slot->get_hash());

            QString path = QString(p_slot->get_path());

            qDebug() << " " << hkey << skey << path;
        }
        else
        {
            qDebug() << " " << hkey << "NULLPTR IN GARBAGE";
        }
    }

    qDebug() << "   ----";
}


void gx::root::global_close()
{
     //std::pair<const unsigned int, std::weak_ptr<gx::slot> > pair;
     //for(std::pair<const unsigned int, std::weak_ptr<gx::slot> > pair : gx::root::global_slots() )

    for( std::pair<const unsigned int, std::weak_ptr<gx::slot> >& pair : gx::root::globals() )
    {
        qDebug() << pair.first << pair.second.lock().get();
    };
}


uint gx::root::hash(const char* name) noexcept
{
    QString temp(name);

    return qHash(temp, 42);
}


void gx::slot::on_free() noexcept
{
    qDebug() << "=>SLOT" << (void*) this << "slot::on_free" << path.c_str();

    if( sp_node ) sp_node->sig_free( this );  // signal to self shared node about slot is free

    gx::root::garbage()[hash] = this;

    gx::root::globals().erase(hash);
}


gx::slot::mode* gx::root::slot_mode(const char* ) noexcept
{
    return gx::slot::mode_none();
}


void gx::root::slot_free( gx::slot* p_slot )
{
    qDebug() << "|>SLOT" << (void*) p_slot << "call->on_free" << p_slot->path.c_str();

    p_slot->on_free();
}


void gx::root::error_slot_free( gx::slot* p_slot )
{
    qDebug() << "|>FAIL-SLOT" << (void*) p_slot << "delete" << p_slot->path.c_str();

    delete p_slot;
}


gx::slot::href gx::root::slot_make(const char* path, const uint & hash) noexcept
{
    gx::slot* p_slot = new gx::slot();

    p_slot->path = path;

    p_slot->hash = hash;

    auto sp_new = std::shared_ptr<gx::slot>( p_slot, gx::root::slot_free );

    gx::root::globals()[hash] = sp_new;

    return sp_new;
}


gx::slot::href gx::root::slot_make(const char* path)
{
    uint s_hash = gx::root::hash(path);

    return gx::root::slot_make(path, s_hash);

}


gx::type::href gx::root::type_find(const char *name) noexcept
{
    auto exists = gx::root::types().find(name);

    if( gx::root::types().end() == exists )
    {
        return nullptr;  // gx::root::type_dict(name);
    }

    return exists->second;
}


gx::type::href gx::root::type_dict(const char* name) noexcept
{
    struct user_defined_type : gx::type
    {
        bool is_builtin() { return false; }

        std::string    get_type_name() { return "user_defined"; }  //

        gx::slot::href get_type_dict() noexcept
        {
            if ( nullptr != sp_type_defs )
            {
                return sp_type_defs;
            }

            if ( 0 == second_hash )
            {
                second_hash = gx::root::hash(second_name.c_str());
            }

            auto exists = gx::root::globals().find( second_hash );

            if ( gx::root::globals().end() == exists )
            {
                 sp_type_defs = gx::root::slot_make(second_name.c_str(), second_hash );
                 // TRY TO LOAD TYPE_DEFS JSON FILE
                 //!!!TODO: IF NO EXISTS RETURN SHARED EMPTY DICT;
            }

            return sp_type_defs;
        }

        std::string    second_name;

        uint           second_hash = 0;

        gx::slot::href sp_type_defs = nullptr;

        user_defined_type(const char* name)
            :second_name(name)
        {

        }
    };

    // !!!TODO FIND EXISTS TYPE OBJECT, ELSE - CREATE USER DEFINED
    qDebug() << "TODO: here type object find or create '" << name <<"' " << __FILE__ << __LINE__;
    return nullptr;
}
