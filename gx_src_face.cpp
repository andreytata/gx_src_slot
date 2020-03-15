#include <gx_src_face.h>

#include <QJsonObject>

#include <gx_src_slot.h>


void Interface::cset(const QJsonObject& input, QJsonObject& output)
{
    output["fail"] = "Interface::set TODO: NOT IMPLEMENTED METHOD YET";
    output["echo"] = input;
    return;
}


void Interface::cget(const QJsonObject& i, QJsonObject& o)
{
    QJsonValue v_args = i["args"];

    if( v_args.isUndefined() )
    {
        o["fail"] = "Interface::get ERROR: 'args' undefined";
        return;
    }

    if( ! v_args.isObject() )
    {
        o["fail"] = "Interface::get ERROR: 'args' is not object";
        return;
    }

    QJsonObject args = v_args.toObject();

    QJsonValue v_path = args["path"];

    if( v_path.isUndefined() )
    {
        o["fail"] = "Interface::get ERROR: args.path undefined";
        return;
    }

    if( !v_path.isString() )
    {
        o["fail"] = "Interface::get ERROR: args.path is not string";
        return;
    }

    QString path = v_path.toString();

    QJsonObject obj;
    gx::root::dump( path.toStdString().c_str(), obj);  // dump from path to output json
    o["echo_path"] = path;
    o["echo_echo"] = "hello";
    o["echo_dump"] = obj;
}


// allocate new slot editor ('path' editor) variable
void Interface::edit(const QJsonObject& input , QJsonObject& output)
{
    output["fail"] = "Interface::edit TODO: NOT IMPLEMENTED METHOD YET";
    output["echo"] = input;
    return;
}

// allocate new hardref to slot at 'path' variable
void Interface::href(const QJsonObject& input , QJsonObject& output)
{
    output["fail"] = "Interface::href TODO: NOT IMPLEMENTED METHOD YET";
    output["echo"] = input;
    return;
}

// allocate new weakref to slot at 'path' variable
void Interface::wref(const QJsonObject& input , QJsonObject& output)
{
    output["fail"] = "Interface::wref TODO: NOT IMPLEMENTED METHOD YET";
    output["echo"] = input;
    return;
}

// allocate new self-own variable w/o 'path' or with public 'path'
void Interface::self(const QJsonObject& input , QJsonObject& output)
{
    output["fail"] = "Interface::self TODO: NOT IMPLEMENTED METHOD YET";
    output["echo"] = input;
    return;
}
