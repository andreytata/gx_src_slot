#include <gx_src_slot.h>

#include <gx_src_type.h>

#include <QHash>


QString hex_hash(const uint& number);

gx::slot::slot()             { qDebug() << "++<< SLOT >>" << hex_hash( 0 )          << (void*) this; }

gx::slot::~slot()            { qDebug() << "--<< SLOT >>" << hex_hash( this->hash ) << (void*) this; }

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
    static __globals__& get();

    gx::root::smap& allocated_slots() noexcept { static gx::root::smap ss; return ss; }

    gx::root::gmap& garbaged_slots()  noexcept { static gx::root::gmap ss; return ss; }

    gx::root::tmap& type_objects()    noexcept { static gx::root::tmap ss; return ss; }

private:

    __globals__()  // on first usage
    {
        qDebug() << "++GLOBALS(on first call)";

        ///gx::type* p_type = new gx::uv3f_type_object;

        ///std::shared_ptr<gx::type> sp_type = std::shared_ptr<gx::type>(p_type);

        __globals__::type_objects()["uv2f"] = std::shared_ptr<gx::type>(new gx::uv2f_type_object);

        __globals__::type_objects()["uv3f"] = std::shared_ptr<gx::type>(new gx::uv3f_type_object);

        __globals__::type_objects()["um4f"] = std::shared_ptr<gx::type>(new gx::um4f_type_object);
    }

    ~__globals__() // on exit application
    {
        qDebug() << "--GLOBALS";

        gx::root::root_info();

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

__globals__& __globals__::get() { static __globals__ globalsss; return globalsss; }

gx::root::smap& gx::root::globals() noexcept { return __globals__::get().allocated_slots(); }

gx::root::gmap& gx::root::garbage() noexcept { return __globals__::get().garbaged_slots();  }

gx::root::tmap& gx::root::types()   noexcept { return __globals__::get().type_objects();    }


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


QString hex_hash(const uint& number)
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
        if( p_slot )
        {
            qDebug() << " " << hex_hash(pair.first) << hex_hash(p_slot->get_hash()) << QString(p_slot->get_path());
        }
        else
        {
            qDebug() << " " << hex_hash(pair.first) << "NULLPTR IN GARBAGE";
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
    qDebug() << "=>SLOT at " << (void*) this << path.c_str();
    gx::root::garbage()[hash] = this;
    gx::root::globals().erase(hash);
}


gx::slot::mode* gx::root::slot_mode(const char* ) noexcept
{
    return gx::slot::mode_none();
}


void gx::root::slot_free( gx::slot* p_slot )
{
    qDebug() << "|>SLOT_FREE" << (void*) p_slot << "SEND on_free()" << p_slot->path.c_str();

    p_slot->on_free();
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
        return gx::root::type_dict(name);
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

    //!!!TODO FIND EXISTS TYPE OBJECT, ELSE - CREATE USER DEFINED
    qDebug() << "TODO: here type object find or create '" << name <<"' " << __FILE__ << __LINE__;
    return nullptr;
}
