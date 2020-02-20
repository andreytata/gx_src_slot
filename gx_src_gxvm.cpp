#include <gx_src_gxvm.h>

namespace gx   {
namespace impl {

class __created__state__ : public gx::vpi::mode
{
    mode* sig_state(gx::vpi*o) { return o->sig_state_on_created(); }  // run next step for stareted, return state
    mode* sig_start(gx::vpi*o) { return o->sig_start_on_created(); }  // switch task to started mode, run first step
    mode* sig_break(gx::vpi*o) { return o->sig_break_on_created(); }  // stop started, switch to invalid mode
    mode* sig_error(gx::vpi*o) { return o->sig_error_on_created(); }  // stop started, switch to failure mode

    bool sm_created() const { return true; }  // is state machine's created state
};

}
}

static gx::vpi::mode* static_created_state() { static gx::impl::__created__state__ ss; return &ss; }

gx::vpi::vpi( QObject* parent )
    :QObject(parent)
    ,mp_state(static_created_state())
{

}

gx::vpi::~vpi()
{

}
