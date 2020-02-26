#include <gx_src_gxvm.h>


gx::co_process::mode* gx::co_process::st_created() { static gx::co_process::created_state ss; return &ss; }
gx::co_process::mode* gx::co_process::st_success() { static gx::co_process::success_state ss; return &ss; }
gx::co_process::mode* gx::co_process::st_failure() { static gx::co_process::failure_state ss; return &ss; }
gx::co_process::mode* gx::co_process::st_started() { static gx::co_process::started_state ss; return &ss; }
gx::co_process::mode* gx::co_process::st_invalid() { static gx::co_process::invalid_state ss; return &ss; }



gx::co_process::co_process( QObject* parent )
    :QObject(parent)
    ,mp_state(st_created())
{

}

gx::co_process::~co_process()
{

}

