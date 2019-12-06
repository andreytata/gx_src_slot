#ifndef GX_SRC_TYPE_H
#define GX_SRC_TYPE_H

#include <gx_src_slot.h>

namespace gx
{
struct uv2f; struct uv3f; struct um4f;

struct unfa : gx::node
{
    struct proc
    {
        virtual void on(gx::uv2f*) = 0;

        virtual void on(gx::uv3f*) = 0;

        virtual void on(gx::um4f*) = 0;

        virtual ~proc() {}
    };

    virtual void on(gx::unfa::proc*) = 0;

    virtual ~unfa() {}

    unfa():gx::node(){}
};

struct uv2f : unfa
{
    uv2f(){}
    void on(gx::unfa::proc*p){ p->on(this); }
};

struct uv3f : unfa
{
    uv3f(){}  //using gx::unfa::unfa;
    //using gx::unfa::unfa;
    void on(gx::unfa::proc*p){p->on(this);}
};

struct um4f : unfa
{
    um4f(){}  //using gx::unfa::unfa;
    void on(gx::unfa::proc*p){p->on(this);}
};

struct uv2f_type_object: gx::type
{
    uv2f_type_object() { qDebug() << "++uv2f type object"; }
   ~uv2f_type_object() { qDebug() << "--uv2f type object"; }
    void __init__(gx::slot* )                         noexcept;    // create or share node-derived object
    void __copy__(gx::slot* , gx::slot*)              noexcept {}  // make node's copy and install to slot
    void __isas__(gx::slot* , gx::slot*)              noexcept {}  // share
    void __itis__(gx::slot* , gx::slot*)              noexcept {}  // share
    void __load__(gx::slot* , const QJsonObject&)     noexcept {}  // load node fields from json object
    void __save__(gx::slot* , QJsonObject& )          noexcept {}  // save node fields to json object
    void __done__(gx::slot* )                         noexcept {}  // finalize
    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};

struct uv3f_type_object: gx::type
{
    uv3f_type_object(){ qDebug() << "++uv3f type object"; }
   ~uv3f_type_object(){ qDebug() << "--uv3f type object"; }
    void __init__(gx::slot* )                         noexcept;    // create or share node-derived object
    void __copy__(gx::slot* , gx::slot*)              noexcept {}  // make node's copy and install to slot
    void __isas__(gx::slot* , gx::slot*)              noexcept {}  // share
    void __itis__(gx::slot* , gx::slot*)              noexcept {}  // share
    void __load__(gx::slot* , const QJsonObject&)     noexcept {}  // load node fields from json object
    void __save__(gx::slot* , QJsonObject& )          noexcept {}  // save node fields to json object
    void __done__(gx::slot* )                         noexcept {}  // finalize
    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};

struct um4f_type_object: gx::type
{
    um4f_type_object(){ qDebug() << "++um4f type object"; }
   ~um4f_type_object(){ qDebug() << "--um4f type object"; }
    void __init__(gx::slot* )                         noexcept;  // create or share node-derived object
    void __copy__(gx::slot* , gx::slot*)              noexcept {}  // make node's copy and install to slot
    void __isas__(gx::slot* , gx::slot*)              noexcept {}  // share
    void __itis__(gx::slot* , gx::slot*)              noexcept {}  // share
    void __load__(gx::slot* , const QJsonObject&)     noexcept {}  // load node fields from json object
    void __save__(gx::slot* , QJsonObject& )          noexcept {}  // save node fields to json object
    void __done__(gx::slot* )                         noexcept {}  // finalize
    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


}

#endif // GX_SRC_TYPE_H
