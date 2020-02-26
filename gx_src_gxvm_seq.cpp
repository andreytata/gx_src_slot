#include <gx_src_gxvm.h>

gx::seq::seq(QObject* parent) : co_process(parent) {}

gx::seq::~seq() {}

gx::co_process::mode* gx::seq::sig_final_on_created()
{
    qDebug()<<"seq sig_final_on_created"; return gx::co_process::get_state();
}

gx::co_process::mode* gx::seq::sig_start_on_created()
{
    qDebug()<<"seq sig_start_on_created";
    return gx::co_process::get_state();
}

gx::co_process::mode* gx::seq::sig_break_on_created()
{
    qDebug()<<"seq sig_break_on_created"; return gx::co_process::get_state();
}

gx::co_process::mode* gx::seq::sig_error_on_created()
{
    qDebug()<<"seq sig_error_on_created"; return gx::co_process::get_state();
}

gx::co_process::mode* gx::seq::sig_final_on_started() { qDebug()<<"seq sig_final_on_started"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_start_on_started() { qDebug()<<"seq sig_start_on_started"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_break_on_started() { qDebug()<<"seq sig_break_on_started"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_error_on_started() { qDebug()<<"seq sig_error_on_started"; return gx::co_process::get_state(); }

gx::co_process::mode* gx::seq::sig_final_on_invalid() { qDebug()<<"seq sig_final_on_invalid"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_start_on_invalid() { qDebug()<<"seq sig_start_on_invalid"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_break_on_invalid() { qDebug()<<"seq sig_break_on_invalid"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_error_on_invalid() { qDebug()<<"seq sig_error_on_invalid"; return gx::co_process::get_state(); }

gx::co_process::mode* gx::seq::sig_final_on_success() { qDebug()<<"seq sig_final_on_success"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_start_on_success() { qDebug()<<"seq sig_start_on_success"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_break_on_success() { qDebug()<<"seq sig_break_on_success"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_error_on_success() { qDebug()<<"seq sig_error_on_success"; return gx::co_process::get_state(); }

gx::co_process::mode* gx::seq::sig_final_on_failure() { qDebug()<<"seq sig_final_on_failure"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_start_on_failure() { qDebug()<<"seq sig_start_on_failure"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_break_on_failure() { qDebug()<<"seq sig_break_on_failure"; return gx::co_process::get_state(); }
gx::co_process::mode* gx::seq::sig_error_on_failure() { qDebug()<<"seq sig_error_on_failure"; return gx::co_process::get_state(); }

void gx::seq::set_error(const char* e) { gx::co_process::error = e ; }
