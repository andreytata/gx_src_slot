#ifndef GX_SRC_GXVM_H
#define GX_SRC_GXVM_H

#include <QObject>
#include <QDebug>

namespace gx
{

/// Varible Process Interface - Minimal Executable.

/// Lazy-call, Co-Process or Thread wrappers base class.

// Supported States { created, started, success, failure, invalid }
//     created :=> started
//     started :=> invalid, success, failure
//     failure :=> invalid
//     success :=> invalid
//     invalid :=> started

/// Abstract Inspect, Execute, Invalidate methods.
/// bool gx::vpi::state::is_started() = 0;
/// has states and execute method
/// Virtual Process Interface
namespace impl {
   class __created_state__;
}


class co_process:public QObject
{
    Q_OBJECT

public:

    struct mode
    {
        virtual ~mode(){}

        virtual mode* sig_final(gx::co_process*) = 0;  // run next step for stareted, return state
        virtual mode* sig_start(gx::co_process*) = 0;  // switch task to started mode, run first step
        virtual mode* sig_break(gx::co_process*) = 0;  // stop started, switch to invalid mode
        virtual mode* sig_error(gx::co_process*) = 0;  // stop started, switch to failure mode

        // predefined fast check
        virtual bool sm_created() const { return false; }  // is state machine's created state
        virtual bool sm_started() const { return false; }  // is state machine's started state
        virtual bool sm_invalid() const { return false; }  // is state machine's invalid state
        virtual bool sm_success() const { return false; }  // is state machine's success state
        virtual bool sm_failure() const { return false; }  // is state machine's failure state
    };

    class created_state : public gx::co_process::mode
    {
        mode* sig_final(gx::co_process*o) { return o->sig_final_on_created(); }  // run next step for stareted, return state
        mode* sig_start(gx::co_process*o) { return o->sig_start_on_created(); }  // switch task to started mode, run first step
        mode* sig_break(gx::co_process*o) { return o->sig_break_on_created(); }  // stop started, switch to invalid mode
        mode* sig_error(gx::co_process*o) { return o->sig_error_on_created(); }  // stop started, switch to failure mode

        bool sm_created() const { return true; }  // is state machine's created state
    };


    class invalid_state : public gx::co_process::mode
    {
        mode* sig_final(gx::co_process*o) { return o->sig_final_on_invalid(); }
        mode* sig_start(gx::co_process*o) { return o->sig_start_on_invalid(); }
        mode* sig_break(gx::co_process*o) { return o->sig_break_on_invalid(); }
        mode* sig_error(gx::co_process*o) { return o->sig_error_on_invalid(); }

        bool sm_invalid() const { return true; }  // is state machine's created state
    };

    class started_state : public gx::co_process::mode
    {
        mode* sig_final(gx::co_process*o) { return o->sig_final_on_started(); }
        mode* sig_start(gx::co_process*o) { return o->sig_start_on_started(); }
        mode* sig_break(gx::co_process*o) { return o->sig_break_on_started(); }
        mode* sig_error(gx::co_process*o) { return o->sig_error_on_started(); }

        bool sm_started() const { return true; }  // is state machine's created state
    };

    class success_state : public gx::co_process::mode
    {
        mode* sig_final(gx::co_process*o) { return o->sig_final_on_success(); }
        mode* sig_start(gx::co_process*o) { return o->sig_start_on_success(); }
        mode* sig_break(gx::co_process*o) { return o->sig_break_on_success(); }
        mode* sig_error(gx::co_process*o) { return o->sig_error_on_success(); }

        bool sm_success() const { return true; }  // is state machine's created state
    };

    class failure_state : public gx::co_process::mode
    {
        mode* sig_final(gx::co_process*o) { return o->sig_final_on_failure(); }  // run next step for stareted, return state
        mode* sig_start(gx::co_process*o) { return o->sig_start_on_failure(); }  // switch task to started mode, run first step
        mode* sig_break(gx::co_process*o) { return o->sig_break_on_failure(); }  // stop started, switch to invalid mode
        mode* sig_error(gx::co_process*o) { return o->sig_error_on_failure(); }  // stop started, switch to failure mode

        bool sm_failure() const { return true; }  // is state machine's created state
    };

    gx::co_process::mode* st_created();
    gx::co_process::mode* st_success();
    gx::co_process::mode* st_failure();
    gx::co_process::mode* st_started();
    gx::co_process::mode* st_invalid();


    // struct created; struct started; struct success; struct invalid; struct failure;

    // external signals (not-qt)
    mode* sig_final() { return mp_state->sig_final(this); }
    mode* sig_start() { return mp_state->sig_start(this); }
    mode* sig_break() { return mp_state->sig_break(this); }
    mode* sig_error() { return mp_state->sig_error(this); }

    // fast check state
    bool sm_created() const { return mp_state->sm_created(); }  // state machine in invalid state
    bool sm_started() const { return mp_state->sm_started(); }  // state machine in invalid state
    bool sm_invalid() const { return mp_state->sm_invalid(); }  // state machine in invalid state
    bool sm_success() const { return mp_state->sm_success(); }  // state machine in invalid state
    bool sm_failure() const { return mp_state->sm_failure(); }  // state machine in invalid state

    // virtual
    mode* get_state() const { return mp_state; }
    virtual void set_error(const char* e) = 0;

protected:
    void set_started() { mp_state = st_started(); }
    void set_failure() { mp_state = st_failure(); }
    void set_success() { mp_state = st_success(); }
    void set_invalid() { mp_state = st_invalid(); }

    // friend class gx::impl::__created__state__;
    //     struct __created__state__

public:
    virtual gx::co_process::mode* sig_final_on_created() = 0;
    virtual gx::co_process::mode* sig_start_on_created() = 0;
    virtual gx::co_process::mode* sig_break_on_created() = 0;
    virtual gx::co_process::mode* sig_error_on_created() = 0;

    virtual gx::co_process::mode* sig_final_on_started() = 0;
    virtual gx::co_process::mode* sig_start_on_started() = 0;
    virtual gx::co_process::mode* sig_break_on_started() = 0;
    virtual gx::co_process::mode* sig_error_on_started() = 0;

    virtual gx::co_process::mode* sig_final_on_invalid() = 0;
    virtual gx::co_process::mode* sig_start_on_invalid() = 0;
    virtual gx::co_process::mode* sig_break_on_invalid() = 0;
    virtual gx::co_process::mode* sig_error_on_invalid() = 0;

    virtual gx::co_process::mode* sig_final_on_success() = 0;
    virtual gx::co_process::mode* sig_start_on_success() = 0;
    virtual gx::co_process::mode* sig_break_on_success() = 0;
    virtual gx::co_process::mode* sig_error_on_success() = 0;

    virtual gx::co_process::mode* sig_final_on_failure() = 0;
    virtual gx::co_process::mode* sig_start_on_failure() = 0;
    virtual gx::co_process::mode* sig_break_on_failure() = 0;
    virtual gx::co_process::mode* sig_error_on_failure() = 0;

protected:
    const char* error = nullptr;  // error template pointer
    gx::co_process::mode* mp_state    = nullptr;

public:

    QObject     *pobject = nullptr;
    QMetaMethod *pmeth   = nullptr;

    co_process(QObject* parent=NULL);
    virtual ~co_process();

public slots:
    void get_state (QJsonObject&){}
    void run_step  (QJsonObject&){}
};


class par:public co_process
{
    Q_OBJECT

public:

    par(QObject*parent=NULL):co_process(parent){}

    virtual ~par(){}

    virtual gx::co_process::mode* sig_final_on_created() { qDebug()<<"par sig_final_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_created() { qDebug()<<"par sig_start_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_created() { qDebug()<<"par sig_break_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_created() { qDebug()<<"par sig_error_on_created"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_started() { qDebug()<<"par sig_final_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_started() { qDebug()<<"par sig_start_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_started() { qDebug()<<"par sig_break_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_started() { qDebug()<<"par sig_error_on_started"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_invalid() { qDebug()<<"par sig_final_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_invalid() { qDebug()<<"par sig_start_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_invalid() { qDebug()<<"par sig_break_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_invalid() { qDebug()<<"par sig_error_on_invalid"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_success() { qDebug()<<"par sig_final_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_success() { qDebug()<<"par sig_start_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_success() { qDebug()<<"par sig_break_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_success() { qDebug()<<"par sig_error_on_success"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_failure() { qDebug()<<"par sig_final_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_failure() { qDebug()<<"par sig_start_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_failure() { qDebug()<<"par sig_break_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_failure() { qDebug()<<"par sig_error_on_failure"; return gx::co_process::get_state(); }

    virtual void set_error(const char* e) { gx::co_process::error = e ; }

    void switch_started(){ gx::co_process::set_started(); }
    void switch_failure(){ gx::co_process::set_failure(); }
    void switch_success(){ gx::co_process::set_success(); }
    void switch_invalid(){ gx::co_process::set_invalid(); }

public slots:
    void get_state     (QJsonObject&)       {}
    void step          (QJsonObject&)       {}
    void set_exceptions(const QJsonObject&) {}
    void set_conditions(const QJsonObject&) {}
    void add_items     (const QJsonObject&) {}
};


class seq:public co_process
{
    Q_OBJECT

public:

    seq(QObject*parent=NULL);

    virtual ~seq();

    virtual gx::co_process::mode* sig_final_on_created(); //{ qDebug()<<"seq sig_final_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_created(); //{ qDebug()<<"seq sig_start_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_created(); //{ qDebug()<<"seq sig_break_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_created(); //{ qDebug()<<"seq sig_error_on_created"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_started(); //{ qDebug()<<"seq sig_final_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_started(); //{ qDebug()<<"seq sig_start_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_started(); //{ qDebug()<<"seq sig_break_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_started(); //{ qDebug()<<"seq sig_error_on_started"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_invalid(); //{ qDebug()<<"seq sig_final_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_invalid(); //{ qDebug()<<"seq sig_start_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_invalid(); //{ qDebug()<<"seq sig_break_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_invalid(); //{ qDebug()<<"seq sig_error_on_invalid"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_success(); //{ qDebug()<<"seq sig_final_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_success(); //{ qDebug()<<"seq sig_start_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_success(); //{ qDebug()<<"seq sig_break_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_success(); //{ qDebug()<<"seq sig_error_on_success"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_failure(); //{ qDebug()<<"seq sig_final_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_failure(); //{ qDebug()<<"seq sig_start_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_failure();// { qDebug()<<"seq sig_break_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_failure();

    virtual void set_error(const char* e);   //{ gx::co_process::error = e ; }

    void switch_started(){ gx::co_process::set_started(); }
    void switch_failure(){ gx::co_process::set_failure(); }
    void switch_success(){ gx::co_process::set_success(); }
    void switch_invalid(){ gx::co_process::set_invalid(); }


public slots:
    void get_state     (QJsonObject&)       {}
    void step          (QJsonObject&)       {}
    void set_exceptions(const QJsonObject&) {}
    void set_conditions(const QJsonObject&) {}
    void add_items     (const QJsonObject&) {}
};


class job:public co_process
{
    Q_OBJECT

public:

    virtual void set_input_impl(const QJsonObject&) { }

    job(QObject*parent=NULL):co_process(parent){}

    virtual ~job(){}

    virtual gx::co_process::mode* sig_final_on_created() { qDebug()<<"job sig_final_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_created() { qDebug()<<"job sig_start_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_created() { qDebug()<<"job sig_break_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_created() { qDebug()<<"job sig_error_on_created"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_started() { qDebug()<<"job sig_final_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_started() { qDebug()<<"job sig_start_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_started() { qDebug()<<"job sig_break_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_started() { qDebug()<<"job sig_error_on_started"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_invalid() { qDebug()<<"job sig_final_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_invalid() { qDebug()<<"job sig_start_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_invalid() { qDebug()<<"job sig_break_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_invalid() { qDebug()<<"job sig_error_on_invalid"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_success() { qDebug()<<"job sig_final_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_success() { qDebug()<<"job sig_start_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_success() { qDebug()<<"job sig_break_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_success() { qDebug()<<"job sig_error_on_success"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_failure() { qDebug()<<"job sig_final_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_failure() { qDebug()<<"job sig_start_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_failure() { qDebug()<<"job sig_break_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_failure() { qDebug()<<"job sig_error_on_failure"; return gx::co_process::get_state(); }

    virtual void set_error(const char* e) { gx::co_process::error = e ; }

    void switch_started(){ gx::co_process::set_started(); }
    void switch_failure(){ gx::co_process::set_failure(); }
    void switch_success(){ gx::co_process::set_success(); }
    void switch_invalid(){ gx::co_process::set_invalid(); }

public slots:

    void set_input(const QJsonObject& json_data) { this->set_input_impl(json_data); }
};


class vip:public co_process
{
    Q_OBJECT

public:

    vip(QObject*parent=NULL):co_process(parent){}

    virtual ~vip(){}

    virtual gx::co_process::mode* sig_final_on_created() { qDebug()<<"vip sig_final_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_created() { qDebug()<<"vip sig_start_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_created() { qDebug()<<"vip sig_break_on_created"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_created() { qDebug()<<"vip sig_error_on_created"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_started() { qDebug()<<"vip sig_final_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_started() { qDebug()<<"vip sig_start_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_started() { qDebug()<<"vip sig_break_on_started"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_started() { qDebug()<<"vip sig_error_on_started"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_invalid() { qDebug()<<"vip sig_final_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_invalid() { qDebug()<<"vip sig_start_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_invalid() { qDebug()<<"vip sig_break_on_invalid"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_invalid() { qDebug()<<"vip sig_error_on_invalid"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_success() { qDebug()<<"vip sig_final_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_success() { qDebug()<<"vip sig_start_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_success() { qDebug()<<"vip sig_break_on_success"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_success() { qDebug()<<"vip sig_error_on_success"; return gx::co_process::get_state(); }

    virtual gx::co_process::mode* sig_final_on_failure() { qDebug()<<"vip sig_final_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_start_on_failure() { qDebug()<<"vip sig_start_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_break_on_failure() { qDebug()<<"vip sig_break_on_failure"; return gx::co_process::get_state(); }
    virtual gx::co_process::mode* sig_error_on_failure() { qDebug()<<"vip sig_error_on_failure"; return gx::co_process::get_state(); }

    virtual void set_error(const char* e) { gx::co_process::error = e ; }

    void switch_started(){ gx::co_process::set_started(); }
    void switch_failure(){ gx::co_process::set_failure(); }
    void switch_success(){ gx::co_process::set_success(); }
    void switch_invalid(){ gx::co_process::set_invalid(); }

    void on_new_par(const QJsonObject&) {}
    void on_new_seq(const QJsonObject&) {}
    void on_new_vip(const QJsonObject&) {}
    void on_new_job(const QJsonObject&) {}

public slots:

    void new_par(const QJsonObject&o) { on_new_par(o); }
    void new_seq(const QJsonObject&o) { on_new_seq(o); }
    void new_vip(const QJsonObject&o) { on_new_vip(o); }
    void new_job(const QJsonObject&o) { on_new_job(o); }
};

}
#endif // GX_SRC_GXVM_H
