#ifndef GX_SRC_GXVM_H
#define GX_SRC_GXVM_H

#include<QObject>

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
class vpi:public QObject
{
    Q_OBJECT

public:

    struct mode
    {
        virtual ~mode(){}

        virtual mode* sig_state(gx::vpi*) = 0;  // run next step for stareted, return state
        virtual mode* sig_start(gx::vpi*) = 0;  // switch task to started mode, run first step
        virtual mode* sig_break(gx::vpi*) = 0;  // stop started, switch to invalid mode
        virtual mode* sig_error(gx::vpi*) = 0;  // stop started, switch to failure mode

        // predefined fast check
        virtual bool sm_created() const { return false; }  // is state machine's created state
        virtual bool sm_started() const { return false; }  // is state machine's started state
        virtual bool sm_invalid() const { return false; }  // is state machine's invalid state
        virtual bool sm_success() const { return false; }  // is state machine's success state
        virtual bool sm_failure() const { return false; }  // is state machine's failure state
        // virtual bool sm_stopped() const { return true;  }  // any state except 'started' ???
    };

    // signals (not-qt)
    mode* sig_state() { return mp_state->sig_state(this); }
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

    // friend class gx::impl::__created__state__;
    //     struct __created__state__
public:
    virtual gx::vpi::mode* sig_state_on_created() = 0;
    virtual gx::vpi::mode* sig_start_on_created() = 0;
    virtual gx::vpi::mode* sig_break_on_created() = 0;
    virtual gx::vpi::mode* sig_error_on_created() = 0;

private:
    const char* error = nullptr;  // error template pointer
    gx::vpi::mode* mp_state    = nullptr;

public:

    QObject     *pobject = nullptr;
    QMetaMethod *pmeth   = nullptr;

    vpi(QObject* parent=NULL);
    virtual ~vpi();

public slots:
    void get_state (QJsonObject&){}
    void run_step  (QJsonObject&){}
};


class par:public vpi
{
    Q_OBJECT
public:
    par(QObject*parent=NULL):vpi(parent){}
    virtual ~par(){}
public slots:
    void get_state     (QJsonObject&)       {}
    void step          (QJsonObject&)       {}
    void set_exceptions(const QJsonObject&) {}
    void set_conditions(const QJsonObject&) {}
    void add_items     (const QJsonObject&) {}
};


class seq:public vpi
{
    Q_OBJECT
public:
    seq(QObject*parent=NULL):vpi(parent){}
    virtual ~seq(){}
public slots:
    void get_state     (QJsonObject&)       {}
    void step          (QJsonObject&)       {}
    void set_exceptions(const QJsonObject&) {}
    void set_conditions(const QJsonObject&) {}
    void add_items     (const QJsonObject&) {}
};


class vip:public vpi
{
    Q_OBJECT
public:
    vip(QObject*parent=NULL):vpi(parent){}
    virtual ~vip(){}
public slots:
    void new_par(const QJsonObject&) {}
    void new_seq(const QJsonObject&) {}
    void new_vip(const QJsonObject&) {}
};

}
#endif // GX_SRC_GXVM_H
