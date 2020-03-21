#include <gx_src_face.h>

#include <QJsonObject>

#include <gx_src_slot.h>


gx::edit::edit(QObject* parent):attr(parent){}


gx::edit::~edit()
{
    if( sp_slot )  // if has connected slot locked to edit
    {
        sp_slot->mp_edit = nullptr;  // unlock slot edit field
    }
}


void gx::edit::set_path(const QJsonObject & input, QJsonObject & output)
{
    QJsonValue args_val = input["args"];

    if( args_val.isUndefined() )
    {
        output["fail"] = "gx::attr::edit::set_path field 'args' undefined";

        output["echo"] = input;

        return;

    }

    if( ! args_val.isObject() )
    {
        output["fail"] = "gx::attr::edit::set_path field 'args' not dict object";

        output["echo"] = input;

        return;
    }

    QJsonObject args = args_val.toObject();

    QJsonValue path_val = args["path"];

    if( path_val.isUndefined() )
    {
        output["fail"] = "gx::attr::edit::set_path field 'path' undefined";

        output["echo"] = input;

        return;

    }

    if( ! path_val.isString() )
    {
        output["fail"] = "gx::attr::edit::set_path field 'path' not string";

        output["echo"] = input;

        return;
    }

    QString path = path_val.toString();

    const char* set_path_error = set_path(path);

    if( set_path_error )
    {
        output["fail"] = "gx::attr::edit::set_path" + QString(set_path_error);

        output["echo"] = input;

        output["gx::edit::set_path"] = "FAILURE:" + path;

        return;
    }

    output["gx::edit::set_path"] = "SUCCESS: " + path;

    output["echo"] = input;

    return;
}





const char* gx::edit::set_path(const QString& path)
{
    uint hash(gx::root::hash(path.toStdString().c_str()));

    auto exists = gx::root::globals().find(hash);

    if( gx::root::globals().end() == exists )
    {
        qDebug() << "??gx::edit::set_path UNKNOWN" << hash << path;

        gx::slot::href new_sp_slot = gx::root::slot_make( path.toStdString().c_str() );

        if( sp_slot )                    // has previous slot locked to edit
        {
            sp_slot->mp_edit = nullptr;  // unlock slot to next edit connection
        }

        sp_slot = new_sp_slot;

        sp_slot->mp_edit = this;  // lock edit variable on this slot instance
    }
    else
    {
        qDebug() << "??gx::edit::set_path +EXISTS" << hash << path;

        gx::slot::href old_sp_slot = exists->second.lock();

        qDebug() << "??gx::edit::set_path +EXISTS?self" << (void*)this;
        qDebug() << "??gx::edit::set_path +EXISTS.edit" << old_sp_slot->mp_edit;
        qDebug() << "??gx::edit::set_path +EXISTS.ctrl" << old_sp_slot->mp_ctrl;
        // IMPOSSIBLE SET PATH ??

    }

    return nullptr;  //return
}


void gx::edit::eval( const QString& meth, const QJsonObject & input, QJsonObject & output )
{
    output["fail"] = "gx::edit::eval TODO: NOT IMPLEMENTED YET";

    output["echo"] = input;

    output["gx::edit::eval"] = meth;
}
