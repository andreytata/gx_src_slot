#include <gx_src_gxvm.h>

void gx::vip::exit()
{
    for( int i = 0 ; i<200; ++i )
    {
        qDebug()<<"vip exit signaled for" << this;
    }
}

void gx::vip::new_par(const QJsonObject&o)
{
    on_new_par(o);
}

void gx::vip::new_par(const QString s)
{
    qDebug() << "gx::vip::new_par ? " << s;
}

void gx::vip::new_seq(const QJsonObject&o)
{
    on_new_seq(o);
}

void gx::vip::new_vip(const QJsonObject&o)
{
    on_new_vip(o);
}

void gx::vip::new_job(const QJsonObject&o)
{
    on_new_job(o);
}

void gx::vip::new_job(const QString s)
{
    qDebug() << "gx::vip::new_job ? " << s;
}


// CREATED
gx::co_process::mode* gx::vip::sig_final_on_created()
{
    qDebug()<<"vip sig_final_on_created"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_start_on_created()
{
    qDebug()<<"vip sig_start_on_created"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_break_on_created()
{
    qDebug()<<"vip sig_break_on_created"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_error_on_created()
{
    qDebug()<<"vip sig_error_on_created"; return gx::co_process::get_state();
}

// STARTED
gx::co_process::mode* gx::vip::sig_final_on_started()
{
    qDebug()<<"vip sig_final_on_started"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_start_on_started()
{
    qDebug()<<"vip sig_start_on_started"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_break_on_started()
{
    qDebug()<<"vip sig_break_on_started"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_error_on_started()
{
    qDebug()<<"vip sig_error_on_started"; return gx::co_process::get_state();
}

// INVALID
gx::co_process::mode* gx::vip::sig_final_on_invalid()
{
    qDebug()<<"vip sig_final_on_invalid"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_start_on_invalid()
{
    qDebug()<<"vip sig_start_on_invalid"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_break_on_invalid()
{
    qDebug()<<"vip sig_break_on_invalid"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_error_on_invalid()
{
    qDebug()<<"vip sig_error_on_invalid"; return gx::co_process::get_state();
}

// SUCCESS
gx::co_process::mode* gx::vip::sig_final_on_success()
{
    qDebug()<<"vip sig_final_on_success"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_start_on_success()
{
    qDebug()<<"vip sig_start_on_success"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_break_on_success()
{
    qDebug()<<"vip sig_break_on_success"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_error_on_success()
{
    qDebug()<<"vip sig_error_on_success"; return gx::co_process::get_state();
}

// FAILURE
gx::co_process::mode* gx::vip::sig_final_on_failure()
{
    qDebug()<<"vip sig_final_on_failure"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_start_on_failure()
{
    qDebug()<<"vip sig_start_on_failure"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_break_on_failure()
{
    qDebug()<<"vip sig_break_on_failure"; return gx::co_process::get_state();
}
gx::co_process::mode* gx::vip::sig_error_on_failure()
{
    qDebug()<<"vip sig_error_on_failure"; return gx::co_process::get_state();
}

void gx::vip::set_error(const char* e) { gx::co_process::error = e ; }

