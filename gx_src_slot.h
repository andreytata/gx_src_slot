#ifndef GX_SRC_SLOT_H
#define GX_SRC_SLOT_H

#include <string>
#include <memory>
#include <QDebug>
#include <unordered_map>
#include <set>


#define RAW_JSON(...) #__VA_ARGS__

namespace gx
{

struct root; struct slot; struct node; struct type;

struct unfa; struct vtxa; struct prog; struct QObject;

/// TYPE struct:
/// Is used as slot-type and as node-type: so 'slot::sp_type' & 'node::sp_type'
/// gx::root::type_slot(node
/// Each slot contain at least one gx::type::href null-type object
/// Builtin type cant't be removed from memory ( has one extra gx::type::href )
/// Slot with point to null-type-href, null-type is only if slot in init state
/// gx::type::href sp_type = gx::root::type_find(const char*)
struct type
{
    using href = std::shared_ptr<gx::type>;

    type()                 { qDebug() << "++TYPE at" << (void*)this; }

    virtual ~type()        { qDebug() << "--TYPE at" << (void*)this << (void*)sp_home.get(); }

    std::shared_ptr<gx::slot> sp_home;  // type home slot. Node here contain immutable pairs attr_name : attr_type

    // gx::type newer point to gx::type, only to gx::slot with type_type
    // ability. Base Type has no abilites to handle slot, is only option.
    // type derived from gx::type, can hold some set of slot href or weakref objects.
    // ABSTRACT SLOT'S NODE OPERATIONS
    virtual void __init__(gx::slot* self)                     noexcept = 0;  // create or share node-derived object

    //    virtual void __copy__(gx::slot* self, gx::slot*)          noexcept = 0;  // make node's copy and install to slot
//    //  virtual void __move__(gx::slot* self, gx::slot*)      noexcept = 0;  // move field values from node to node
//    virtual void __isas__(gx::slot* self, gx::slot*)          noexcept = 0;  // share
//    virtual void __itis__(gx::slot* self, gx::slot*)          noexcept = 0;  // share
//    virtual void __load__(gx::slot* self, const QJsonObject&) noexcept = 0;  // load node fields from json object
//    virtual void __save__(gx::slot* self, QJsonObject& )      noexcept = 0;  // save node fields to json object
//    virtual void __done__(gx::slot* self)                     noexcept = 0;  // finalize

    virtual std::string get_type_name() { return "user_dict"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept = 0;
};


struct node
{
    node()                 { qDebug() << "++NODE at" << (void*)this << "????"; }

    node(const char* name) { qDebug() << "++NODE at" << (void*)this << name; }

    virtual ~node()        { qDebug() << "--NODE at" << (void*)this << (void*)sp_type.get(); }

    virtual std::shared_ptr<gx::slot> get_attr(const char*) { return nullptr; }

    virtual std::shared_ptr<gx::type> get_type() { return sp_type; }

    virtual gx::unfa* get_unfa() { return nullptr; }  // LO-LEVEL ACCESS FOR GLSL UNIFIRM ATTRIBUTES

    virtual gx::vtxa* get_vtxa() { return nullptr; }  // LO-LEVEL ACCESS FOR GLSL VERTEX ATTRIBUTES

    virtual gx::prog* get_prog() { return nullptr; }  // LO-LEVEL ACCESS FOR GLSL SHADER PROGRAM

//
//  THIS FAST ACCESS DEPRECATED,
//    virtual gx::defs* get_defs() { return nullptr; }  // LO-LEVEL ACCESS FOR GLSL DRAW
//                                                      // NO NULL in dynamic-types ONLY

    virtual void* get_vars() { return nullptr; }  // only for types with properties dict

    friend class gx::type;

    friend class gx::root;

protected:

    std::shared_ptr<gx::type> sp_type = nullptr;
    ///
    /// SP_TYPE is shared pointer to real type. Some type-derived object.
    /// 'node-type' - inited after node created, sometime is 'slot-type',
    /// but 'node-type' and 'slot-type' are different fields for dynamic,
    /// or user defined, types. Dict pairs { field-name : type-name }.
    ///
    /// In this case slot-type is 'user_dict'- shared_prt, but node-type
    /// point to other 'gx::type' instance with on-fly detected typename.
    /// So, gx::node::sp_type field managed with object stored in parent
    /// slot's field gx::slot::sp_type.
    ///

    // DEPRECATED: Because each node-derived class created in 'slot' use 'slot-type' object with ability to
    //             parse self slot's path and hash fields. Pointer to slot are argument of each type-method.
    //
    //  std::set<uint> ss_uses; // set of slot's-hash-keys used with this node's 'slot-type' obj
    //                          // in process of detection this node's value. Usage depend type.
    //                          // multiple path can use only one node's instance  (DAG)
};

/// SLOT: struct
/// Concrete signal's target object. Is shared object with ability manage life time of self
/// private and shared fields, slots chain created on each query to concrete 'path' object
/// Each slot contain shared ptr to meta, and shared ptr to data objects
/// Slot's Self Object - href to any object derived from gx::dict object, at least empty 'dict' of slot::href objects
/// Slot's Meta Object - href to any object derived from gx::meta object, at least empty 'meta' object
///
struct slot
{
    struct mode;

    using href = std::shared_ptr<gx::slot>;

    const char*           get_path() const noexcept { return path.c_str(); }

    uint                  get_hash() const noexcept { return hash; }

    // DEPRECATED:
    //     static gx::slot::href get_slot( const std::string& path );
    //
    // INSTEAD OF THIS USE:
    //     gx::slot::href sp = gx::root::slot_find("path://to/some/slot");  // get exists, or nullptr
    //     gx::slot::href sp = gx::root::slot_make("path://to/some/slot");  // allocate slot in memory
    //     gx::slot::mode *m = gx::root::slot_mode("path://to/some/slot");  // get slot's mode pointer
    //

    void set_type(gx::type::href& sp_type_object) { sp_type_object->__init__(this); }

    gx::type::href  get_type() noexcept;

    gx::slot::mode* get_mode() { return mp_mode; }

    static mode* mode_init() noexcept;

    static mode* mode_main() noexcept;

    static mode* mode_none() noexcept;

    static mode* mode_free() noexcept;

    /// MODE: slot state abstract base type
    /// gx::root::slot_mode(hash) => [ init | main | none | free ];
    ///     p_slot -> get_mode( ) => [ init | main |  --  | free ];
    ///    sp_slot -> get_mode( ) => [ init | main |  --  |  --  ];
    ///
    struct mode
    {
        struct init; struct main; struct none; struct free;

        /// gx::slot::mode* abstarct visitor
        struct proc
        {
            virtual void on(gx::slot::mode::init*) = 0;  // wait self type or self data object

            virtual void on(gx::slot::mode::main*) = 0;  // ready to use, type and data exists

            virtual void on(gx::slot::mode::free*) = 0;  // (simulated-state) mean in garbage,

            virtual void on(gx::slot::mode::none*) = 0;  // (simulated-state) mean not in memory

            virtual ~proc(){}
        };

        virtual void on(gx::slot::mode::proc*) = 0;

    protected:

        virtual ~mode(){}

        mode(){}

        mode(const mode&) = delete;

        mode(mode&&)      = delete;
    };

    friend class gx::root;  // create slot, edit hash & path

    friend class gx::type;  // init sp_node, edit slot mode

private:

    std::string path;

    uint hash;

    explicit slot();

   ~slot();

    slot(const slot&)  = delete;

    slot(slot&&)       = delete;

    void on_free() noexcept;

    gx::slot::mode* mp_mode = gx::slot::mode_init();

public:

    std::shared_ptr<gx::type> sp_type;  // Must be detected to init sp_node and slot's mode switched to mode_main.
                                        // Need sp_type->__new__(gx::slot&) to create node or share exists node.

    std::shared_ptr<gx::node> sp_node;  // Must be initialized by sp_type before slot's mode switched to mode_main.
                                        // Each class derived from gx::node, can contain 0..N gx::slot::href values.
};

/// ROOT struct
/// Front end API to singletoned heap memory object with active and garbaged slots
/// Each slot contain shared ptr to meta, and shared ptr to data objects
/// Slot's Self Object - href to any object derived from gx::dict object, at least empty 'dict' of slot::href objects
/// Slot's Meta Object - href to any object derived from gx::meta object, at least empty 'meta' object
///
struct root
{
    using smap = std::unordered_map< uint, std::weak_ptr<gx::slot> >;  // active slots hash dict

    using gmap = std::map< uint, gx::slot* >;                          // ready to delete ( garbaged )

    using tmap = std::map< std::string, gx::type::href >;              // shared types dict

    static smap& globals() noexcept;

    static gmap& garbage() noexcept;

    static tmap& types() noexcept;

    static void  root_info();

    static void slot_kill( const uint& ) noexcept;

    static void  global_close();

    static uint hash( const char* ) noexcept;

    static void slot_free(gx::slot* p_slot);

    static std::shared_ptr<gx::slot> slot_make( const char* path );

    static gx::slot::mode*           slot_mode( const char* path ) noexcept;

    static std::shared_ptr<gx::slot> slot_make( const char* path, const uint & hash) noexcept;

    static gx::type::href            type_find( const char* name ) noexcept;

    static gx::type::href            type_dict( const char* name ) noexcept;
};

}

#endif // GX_SRC_SLOT_H
