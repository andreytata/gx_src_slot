#ifndef GX_SRC_TYPE_H
#define GX_SRC_TYPE_H

#include <gx_src_slot.h>

namespace gx
{

struct user;                 // command line interpreter

struct node;                 // ABSTRACT - slot-node class

       struct gxvm;          // command line interpreter handler, used for share OObject-derived
                             // controllers, between depended controllers, as vars or attributes.

       struct proc;          // failure object, can't be setted from outside, contain error info

       struct dict;          // DICT contain std::map< std::string, gx::slot::href >;

       struct list;          // LIST contain std::map< std::size_t, gx::slot::href >;

       struct xvtxa;          // ABSTRACT GLES2.0|GLSL1.1 compatible VerTeX Attributes, programm, shaders

              struct xav2f;   // vec2 vertex array object

              struct xav3f;   // vec3 vertex array object

              struct xindx;   // vertex index buffer object

       struct xunfa;          // abstarct GLES2.0|GLSL1.1 compatible UNiForm Attributes, textures;

              struct xuv2f;

              struct xuv3f;

              struct xum4f;

       struct json;  // Abstarct json data leaf type

              struct none;  // NULL object

              struct Bool;  // raw json boolean node

              struct qstr;  // raw json string  node

              struct real;  // raw json number  node

struct json: gx::node
{
    struct proc
    {
        virtual void on(gx::none*) = 0;

        virtual void on(gx::real*) = 0;

        virtual void on(gx::qstr*) = 0;

        virtual void on(gx::Bool*) = 0;

        virtual ~proc() {}
    };

    virtual void on( gx::json::proc* ) = 0;

    virtual void sig_free(gx::slot*) noexcept {}

    virtual void sig_exit(gx::slot*) noexcept {}

    virtual const char* set_from_list( const QJsonArray&  ) noexcept = 0;

    virtual const char* set_from_dict( const QJsonObject& ) noexcept = 0;

    virtual const char* set_from_qstr( const QString&     ) noexcept = 0;

    virtual const char* set_from_bool( const bool&        ) noexcept = 0;

    virtual const char* set_from_real( const double&      ) noexcept = 0;

    virtual const char* set_from_null( )                    noexcept = 0;

    gx::xunfa* get_xunfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }  // support glsl-prog interface

    gx::dict* get_dict() noexcept { return nullptr; }  // support gx::dict interface

    gx::list* get_list() noexcept { return nullptr; }  // support gx::list interface

    gx::json* get_json() noexcept { return this; }     // support gx::json* interface

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

    virtual ~json() {}

    json():gx::node(){}
};


struct none : gx::json
{
    none()          { qDebug() << "++NONE" << (void*)this; }

    virtual ~none() { qDebug() << "--NONE" << (void*)this; }

    void on( gx::json::proc* p ) { p->on(this); }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept { return "ERROR: SET 'none' from 'list'"; }

    virtual const char* set_from_dict( const QJsonObject& ) noexcept { return "ERROR: SET 'none' from 'dict'"; }

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET 'none' from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET 'none' from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET 'none' from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET 'none' from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return this; }

    gx::proc* get_proc() noexcept { return nullptr; }

    gx::gxvm* get_gxvm() noexcept { return nullptr; }
};


struct Bool : gx::json
{
    Bool()          { qDebug() << "++BOOL" << (void*)this; }

    virtual ~Bool() { qDebug() << "--BOOL" << (void*)this; }

    void on( gx::json::proc* p ) { p->on(this); }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept { return "ERROR: SET 'bool' from 'list'"; }

    virtual const char* set_from_dict( const QJsonObject& ) noexcept { return "ERROR: SET 'bool' from 'dict'"; }

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET 'bool' from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET 'bool' from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET 'bool' from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET 'bool' from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return this; }

    gx::proc* get_proc() noexcept { return nullptr; }

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

    bool  get_bool() const noexcept { return mo_bool; }

protected:

    bool mo_bool = false;
};


struct qstr : gx::json
{
    qstr()          { qDebug() << "++QSTR" << (void*)this; }

    virtual ~qstr() { qDebug() << "--QSTR" << (void*)this; }

    void on( gx::json::proc* p ) { p->on(this); }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept { return "ERROR: SET 'bool' from 'list'"; }

    virtual const char* set_from_dict( const QJsonObject& ) noexcept { return "ERROR: SET 'bool' from 'dict'"; }

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET 'bool' from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET 'bool' from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET 'bool' from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET 'bool' from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return this; }

    gx::proc* get_proc() noexcept { return nullptr; }

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

    const QString*  get_qstr() const noexcept { return &mo_qstr; }

protected:

    QString mo_qstr;
};


struct real : gx::json
{
    real()          { qDebug() << "++REAL" << (void*)this; }

    virtual ~real() { qDebug() << "--REAL" << (void*)this; }

    void on( gx::json::proc* p ) { p->on(this); }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept { return "ERROR: SET 'real' from 'list'"; }

    virtual const char* set_from_dict( const QJsonObject& ) noexcept { return "ERROR: SET 'real' from 'dict'"; }

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET 'real' from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET 'real' from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET 'real' from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET 'real' from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return this; }

    gx::proc* get_proc() noexcept { return nullptr; }

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

    double get_real() const noexcept { return mo_real; }

protected:

    double mo_real = 0.0;
};


struct dict : gx::node
{
    dict()          { qDebug() << "++DICT" << (void*)this; }

    virtual ~dict() { qDebug() << "--DICT" << (void*)this; }

    virtual void sig_free(gx::slot* s) noexcept { qDebug() << "=>FREE SLOT" << s->get_path() << "DICT" << (void*)this;
//        for ( auto& pair: mo_dict )
//        {
//            pair.second.reset();
//        }
    }

    virtual void sig_exit(gx::slot* s) noexcept {
        qDebug() << "=>EXIT SLOT" << s->get_path() << "DICT" << (void*)this;
        for ( auto& pair: mo_dict )
        {
            pair.second.reset();
        }
    }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept { return "ERROR: SET dict from 'list'"; }

    virtual const char* set_from_dict( const QJsonObject& ) noexcept;

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET dict from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET dict from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET dict from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET dict from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return this; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return nullptr; }

    gx::proc* get_proc() noexcept { return nullptr; }  // dict or, at least one field of this dict contain fail-info-object

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

protected:

    std::map<std::string, gx::slot::href> mo_dict;

};

struct gxvm : gx::node
{
    gxvm()          { qDebug() << "++GXVM" << (void*)this; }

    virtual ~gxvm() { qDebug() << "--GXVM" << (void*)this; }

    virtual void sig_free(gx::slot* s) noexcept {
        qDebug() << "=>FREE SLOT" << s->get_path() << "GXVM" << (void*)this;
    }

    virtual void sig_exit(gx::slot* s) noexcept {
        qDebug() << "=>EXIT SLOT" << s->get_path() << "GXVM" << (void*)this;
    }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept { return "ERROR: SET dict from 'list'"; }

    virtual const char* set_from_dict( const QJsonObject& ) noexcept { return "ERROR: SET GXVM from 'list'"; }

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET dict from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET dict from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET dict from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET dict from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return nullptr; }

    gx::proc* get_proc() noexcept { return nullptr; }  // dict or, at least one field of this dict contain fail-info-object

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

protected:

    gx::user* mo_gxvm_implementation = nullptr;  // can't use real delete for QObject based item
};

// PROCEDURE, SOME SLOT COMMANDS SINGLE COMMAND, SEQ(sequence) OR PAR(parallel)
// EXECUTABLE AT OWNER SLOT-PATH. SINGLE IS 'SET', 'GET' or 'NEW' COMMAND, HAS
// STATE EQUAL TO OPERATION'S STATE: SUCCESS|FAILURE|STARTED
// SINGLE COMMAND CONTAIN DETECTED ABSOLUTE PATH, ZERO OR MULTIPLE SOURCE DATA
// HREFS, AND ONLY ONE ITEM IN SEQ LIST.
// MULTI-COMMAND IS SEQ OR PAR SET, GET, NEW COMMANDS WITH MULTIPLE SINNGLE|MULTI
// COMMANDS IN SELF LIST. MULTI-COMMAND'S STATE DEPENDS FROM SUMMARY STATES OF THE
// INCLUDED COMMANDS TREE. EVEN DICT-SET, LIST-SET AND NEW-TYPE, NEW-INST IS SOME
// MULTI-STEP-COMMAND SEQUENCES. ERROR IN ANY INCLUDED INSTANCE - IS FAILURE STATE
// FOR THIS PROC-OBJECT IS NOT FINITE STATE OF THE PROC-STATE MACHINE, SYSTEM IS
// INDETERMINANT SYSTEM, NEXT TRY OR WAIT CAN SWITCH MACHINE IN SUCCESS STATE. IS
// IMPORTANT DIFFERENCE BETWEEN THIS COMPUTATION MODEL AND PROGRAM_LANGUAGES STACK
// BASED MODEL. FAILURE STATE IS ONLY STAGE OF COMPUTATION -- NOT EXCEPTION OR ERROR
// WITH TERMINATE APPLICATION RESULT.

struct proc : gx::node
{
    typedef std::shared_ptr<gx::proc> href;

    proc()          { qDebug() << "++PROC" << (void*)this; }

    virtual ~proc() { qDebug() << "--PROC" << (void*)this; }

    virtual void sig_free(gx::slot* s) noexcept { qDebug() << "=>FREE PROC" << (void*)s << (void*)this; }

    virtual void sig_exit(gx::slot* s) noexcept { qDebug() << "=>EXIT PROC" << (void*)s << (void*)this; }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept { return "ERROR: SET PROC from 'list'"; }

    virtual const char* set_from_dict( const QJsonObject& ) noexcept { return "ERROR: SET PROC from 'dict'"; }

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET PROC from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET PROC from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET PROC from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET PROC from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return nullptr; }

    gx::proc* get_proc() noexcept { return this; }

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

    std::map<std::string, gx::slot::href> mo_dict;  // some dict of slots with error inside

    std::string mo_error;

    std::string mo_path;

    std::vector<gx::proc::href> mo_cmds;  // sub-procedures

};

struct list : gx::node
{
    list()          { qDebug() << "++LIST" << (void*)this; }

    virtual ~list() { qDebug() << "--LIST" << (void*)this; }

    virtual void sig_free(gx::slot* s) noexcept
    {
        qDebug() << "=>FREE SLOT" << s->get_path() << "LIST" << (void*)this;
    }

    virtual void sig_exit(gx::slot* s) noexcept
    {
        qDebug() << "=>EXIT SLOT" << s->get_path() << "LIST" << (void*)this;

        for ( auto& pair: mo_dict )
        {
            pair.second.reset();
        }
    }

    virtual const char* set_from_list( const QJsonArray&  ) noexcept;

    virtual const char* set_from_dict( const QJsonObject& ) noexcept { return "ERROR: SET list from 'dict'"; }

    virtual const char* set_from_qstr( const QString&     ) noexcept { return "ERROR: SET list from 'qstr'"; }

    virtual const char* set_from_bool( const bool&        ) noexcept { return "ERROR: SET list from 'bool'"; }

    virtual const char* set_from_real( const double&      ) noexcept { return "ERROR: SET list from 'real'"; }

    virtual const char* set_from_null( )                    noexcept { return "ERROR: SET list from 'null'"; }

    gx::xunfa* get_xunfa() noexcept { return nullptr; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return this;    }  // this instance support list interface

    gx::json* get_json() noexcept { return nullptr; }

    gx::proc* get_proc() noexcept { return nullptr; }  // at least one item of this list contain fail-info

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

protected:

    std::map<std::size_t, gx::slot::href> mo_dict;

};


struct xunfa : gx::node
{
    struct proc
    {
        virtual void on(gx::xuv2f*) = 0;

        virtual void on(gx::xuv3f*) = 0;

        virtual void on(gx::xum4f*) = 0;

        virtual ~proc() {}
    };

    virtual void on( gx::xunfa::proc* ) = 0;

    virtual const char* set_from_list( const QJsonArray&  ) noexcept = 0;

    virtual const char* set_from_dict( const QJsonObject& ) noexcept = 0;

    virtual const char* set_from_qstr( const QString&     ) noexcept = 0;

    virtual const char* set_from_bool( const bool&        ) noexcept = 0;

    virtual const char* set_from_real( const double&      ) noexcept = 0;

    virtual const char* set_from_null( )                    noexcept = 0;

    gx::xunfa* get_xunfa() noexcept { return this; }

    gx::xvtxa* get_xvtxa() noexcept { return nullptr; }

    gx::unfa* get_unfa() noexcept { return nullptr; }  // support glsl uniform attribute interface

    gx::vtxa* get_vtxa() noexcept { return nullptr; }  // support vertex array object interface

    gx::prog* get_prog() noexcept { return nullptr; }

    gx::dict* get_dict() noexcept { return nullptr; }

    gx::list* get_list() noexcept { return nullptr; }

    gx::json* get_json() noexcept { return nullptr; }

    gx::proc* get_proc() noexcept { return nullptr; }

    gx::gxvm* get_gxvm() noexcept { return nullptr; }

    virtual ~xunfa() {}

    xunfa():gx::node(){}
};

struct xuv2f : xunfa
{
    xuv2f() { buff.resize(2); }

    void on( gx::xunfa::proc* p ) { p->on(this); }

    virtual void sig_free(gx::slot* s) noexcept { qDebug() << "=>FREE SLOT" << s->get_path() << "uv2f" << (void*)this; }

    virtual void sig_exit(gx::slot* s) noexcept { qDebug() << "=>EXIT SLOT" << s->get_path() << "uv2f" << (void*)this; }

    const char* set_from_list( const QJsonArray&  ) noexcept; // { return "'uv2f' set some 'list' unsupported"; }

    const char* set_from_dict( const QJsonObject& ) noexcept { return "'uv2f' set from 'dict' unsupported"; }

    const char* set_from_qstr( const QString&)      noexcept { return "'uv2f' set from 'qstr' unsupported"; }

    const char* set_from_bool( const bool&      )   noexcept { return "'uv2f' set from 'bool' unsupported"; }

    const char* set_from_real( const double&    )   noexcept { return "'uv2f' set from 'real' unsupported"; }

    const char* set_from_null( )  noexcept
    {
        buff[0] = 0; buff[1] = 0;

        return nullptr;
    }

    std::vector<float> buff;
};

struct xuv3f : xunfa
{
    xuv3f() { buff.resize(3); set_from_null(); }  //using gx::unfa::unfa;

    void on( gx::xunfa::proc* p ) { p->on(this); }

    virtual void sig_free(gx::slot* s) noexcept { qDebug() << "=>FREE SLOT" << s->get_path() << "uv3f" << (void*)this; }

    virtual void sig_exit(gx::slot* s) noexcept { qDebug() << "=>EXIT SLOT" << s->get_path() << "uv3f" << (void*)this; }

    const char* set_from_list( const QJsonArray&  ) noexcept;

    const char* set_from_dict( const QJsonObject& ) noexcept { return "'uv3f' set from 'dict' unsupported"; }

    const char* set_from_qstr( const QString&)      noexcept { return "'uv3f' set from 'qstr' unsupported"; }

    const char* set_from_bool( const bool&      )   noexcept { return "'uv3f' set from 'bool' unsupported"; }

    const char* set_from_real( const double&    )   noexcept { return "'uv3f' set from 'real' unsupported"; }

    const char* set_from_null( )  noexcept
    {
        buff[0] = 0; buff[1] = 0; buff[3] = 0;

        return nullptr;
    }

    std::vector<float> buff;
};

struct xum4f : xunfa
{
    xum4f() { buff.resize(16); set_from_null(); }

    void on( gx::xunfa::proc* p ) { p->on(this); }

    virtual void sig_free(gx::slot* s) noexcept { qDebug() << "=>FREE SLOT" << s->get_path() << "um4f" << (void*)this; }

    virtual void sig_exit(gx::slot* s) noexcept { qDebug() << "=>EXIT SLOT" << s->get_path() << "um4f" << (void*)this; }

    const char* set_from_list( const QJsonArray&  ) noexcept; // { return "'um4f' set some 'list' unsupported"; }

    const char* set_from_dict( const QJsonObject& ) noexcept { return "'um4f' set from 'dict' unsupported"; }

    const char* set_from_qstr( const QString&)      noexcept { return "'um4f' set from 'qstr' unsupported"; }

    const char* set_from_bool( const bool&      )   noexcept { return "'um4f' set from 'bool' unsupported"; }

    const char* set_from_real( const double&    )   noexcept { return "'um4f' set from 'real' unsupported"; }

    const char* set_from_null( )  noexcept  // reset
    {
        buff[0]  = 1; buff[1]  = 0; buff[2]  = 0; buff[3]  = 0;

        buff[4]  = 0; buff[5]  = 1; buff[6]  = 0; buff[7]  = 0;

        buff[8]  = 0; buff[9]  = 0; buff[10] = 1; buff[11] = 0;

        buff[12] = 0; buff[13] = 0; buff[14] = 0; buff[15] = 1;

        return nullptr;
    }

    std::vector<float> buff;
};


struct none_type_object: gx::type
{
    none_type_object() { qDebug() << "++none type object"; }

   ~none_type_object() { qDebug() << "--none type object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "none"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


struct xuv2f_type_object: gx::type
{
    xuv2f_type_object() { qDebug() << "++uv2f type object"; }

   ~xuv2f_type_object() { qDebug() << "--uv2f type object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "uv2f"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


struct real_type_object: gx::type
{
    real_type_object() { qDebug() << "++real_type_object"; }

   ~real_type_object() { qDebug() << "--real_type_object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "real"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


struct bool_type_object: gx::type
{
    bool_type_object() { qDebug() << "++bool_type_object"; }

   ~bool_type_object() { qDebug() << "--bool_type_object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "bool"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


struct qstr_type_object: gx::type
{
    qstr_type_object() { qDebug() << "++qstr_type_object"; }

   ~qstr_type_object() { qDebug() << "--qstr_type_object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "qstr"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};

struct gxvm_type_object: gx::type
{
    gxvm_type_object() { qDebug() << "++gxvm type object"; }

   ~gxvm_type_object() { qDebug() << "--gxvm type object"; }

    const char* __init__(gx::slot* ) noexcept;

    std::string get_type_name() const noexcept { return "gxvm"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};

struct dict_type_object: gx::type
{
    dict_type_object() { qDebug() << "++dict type object"; }

   ~dict_type_object() { qDebug() << "--dict type object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "dict"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


struct list_type_object: gx::type
{
    list_type_object() { qDebug() << "++list type object"; }

   ~list_type_object() { qDebug() << "--list type object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "list"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


struct fail_type_object: gx::type
{
    fail_type_object() { qDebug() << "++FAIL type object"; }

   ~fail_type_object() { qDebug() << "--FAIL type object"; }

    const char* __init__(gx::slot*)                   noexcept;    // create or share node-derived object

    std::string get_type_name() const noexcept { return "fail"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


struct xuv3f_type_object: gx::type
{
    xuv3f_type_object() { qDebug() << "++uv3f type object"; }

   ~xuv3f_type_object() { qDebug() << "--uv3f type object"; }

    const char* __init__(gx::slot* )                  noexcept;    // insert type-depended node-derived in slot

    void __copy__(gx::slot* , gx::slot*)              noexcept {}  // make node's copy and install to slot

    void __isas__(gx::slot* , gx::slot*)              noexcept {}  // share

    void __itis__(gx::slot* , gx::slot*)              noexcept {}  // share

    void __load__(gx::slot* , const QJsonObject&)     noexcept {}  // load node fields from json object

    void __save__(gx::slot* , QJsonObject& )          noexcept {}  // save node fields to json object

    void __done__(gx::slot* )                         noexcept {}  // finalize

    std::string get_type_name() const noexcept { return "uv3f"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};

struct xum4f_type_object: gx::type
{
    xum4f_type_object() { qDebug() << "++um4f type object"; }

   ~xum4f_type_object() { qDebug() << "--um4f type object"; }

    const char* __init__(gx::slot* )                  noexcept;    // create or share node-derived object

//    void __copy__(gx::slot* , gx::slot*)              noexcept {}  // make node's copy and install to slot

//    void __isas__(gx::slot* , gx::slot*)              noexcept {}  // share

//    void __itis__(gx::slot* , gx::slot*)              noexcept {}  // share

//    void __load__(gx::slot* , const QJsonObject&)     noexcept {}  // load node fields from json object

//    void __save__(gx::slot* , QJsonObject& )          noexcept {}  // save node fields to json object

//    void __done__(gx::slot* )                         noexcept {}  // finalize

    std::string get_type_name() const noexcept { return "um4f"; }

    virtual std::shared_ptr<gx::slot> get_type_defs() noexcept { return nullptr; }
};


}

#endif // GX_SRC_TYPE_H
