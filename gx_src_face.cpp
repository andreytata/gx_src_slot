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


void Interface::vars(const QJsonObject& input , QJsonObject& output)
{
    QJsonValue args_val = input["args"];

    if( args_val.isUndefined() )
    {
        output["fail"] = "Interface::edit field 'args' undefined";

        output["echo"] = input;

        return;

    }

    if( ! args_val.isObject() )
    {
        output["fail"] = "Interface::edit field 'args' not dict object";

        output["echo"] = input;

        return;
    }

    QJsonObject args = args_val.toObject();

    QJsonValue proc_val = args["proc"];

    if( proc_val.isUndefined() )
    {
        output["fail"] = "Interface::vars field 'proc' undefined";

        output["echo"] = input;

        return;

    }

    if( ! proc_val.isString() )
    {
        output["fail"] = "Interface::vars field 'proc' not string";

        output["echo"] = input;

        return;
    }

    QJsonValue name_val = args["name"];

    if( name_val.isUndefined() )
    {
        output["fail"] = "Interface::vars field 'name' undefined";

        output["echo"] = input;

        return;

    }

    if( ! name_val.isString() )
    {
        output["fail"] = "Interface::vars field 'name' not string";

        output["echo"] = input;

        return;
    }

    QString name = name_val.toString();

    QString proc = proc_val.toString();

    qDebug() << "Inerface::vars try to call vars[" << name << "].proc =" << proc;

    output["fail"] = "Interface::vars TODO: IMPLEMENT 'Interface::vars' META METHOD";

    gx::attr* pvar = __vars[name.toStdString()];

    if( nullptr == pvar )
    {
        output["fail"] = "Interface::vars has no variable named '" + name + "'";

        output["echo"] = input;

        return;
    }

    QString meta_method_name = proc + "(QJsonObject,QJsonObject&)";

    int meta_method_index = pvar->metaObject()->indexOfMethod(meta_method_name.toLatin1().data());

    if( meta_method_index < 0 )
    {
        // if metha method not exists in variable, execution delegated to gx::attr::eval
        pvar->eval(meta_method_name, input, output);

        return;
    }

    QMetaMethod meta_method = pvar->metaObject()->method(meta_method_index);

    meta_method.invoke( pvar, Qt::DirectConnection,
                        Q_ARG(const QJsonObject&, input  ),
                        Q_ARG(      QJsonObject&, output ));

    QJsonObject back;

    back["attr"] = "0x" + QString::number( uint(pvar), 16 );

    back["name"] = name;

    back["proc"] = proc;

    output["args"] = back;

    output["echo"] = input;

    return;
}

// allocate new slot editor ('path' editor) variable. Create named variable.
// is edit decorator to exists or not exists yet slot or variable
void Interface::edit(const QJsonObject& input , QJsonObject& output)
{
    QJsonValue args_val = input["args"];

    if( args_val.isUndefined() )
    {
        output["fail"] = "Interface::edit field 'args' undefined";

        output["echo"] = input;

        return;

    }

    if( ! args_val.isObject() )
    {
        output["fail"] = "Interface::edit field 'args' not dict object";

        output["echo"] = input;

        return;
    }

    QJsonObject args = args_val.toObject();

    QJsonValue name_val = args["name"];

    if( name_val.isUndefined() )
    {
        output["fail"] = "Interface::edit field 'name' undefined";

        output["echo"] = input;

        return;

    }

    if( ! name_val.isString() )
    {
        output["fail"] = "Interface::edit field 'name' not string";

        output["echo"] = input;

        return;
    }

    QJsonValue path_val = args["path"];

    if( path_val.isUndefined() )
    {
        output["fail"] = "Interface::edit field 'path' undefined";

        output["echo"] = input;

        return;

    }

    if( ! path_val.isString() )
    {
        output["fail"] = "Interface::edit field 'path' not string";

        output["echo"] = input;

        return;
    }

    QString name = name_val.toString();

    QString path = path_val.toString();

    qDebug() << "Inerface::edit try to create variable with name" << name;

    std::map<std::string, gx::attr*>::iterator exists_iter = __vars.find( name.toStdString() );

    if( __vars.end() != exists_iter )
    {
        output["fail"] = "Interface::edit target 'name' already exists in vars";

        output["echo"] = input;

        return;
    }

    // Allocate new EDIT is VARS. After allocation "vars" JSON form can be used for invocate
    // created variable's methods:
    // {"meta":"vars", "args":{"name":"...", "meth":"...", "args":{...} } }
    //
    gx::edit* p_edit = new gx::edit(this);  // delete interface instance is delete all VARS

    __vars[name.toStdString()] = p_edit;

    QJsonObject echo_args;

    echo_args["meth"] = "vars contain new instance of edit";

    echo_args["name"] = name;

    echo_args["path"] = path;

    echo_args["mode"] = "success";

    echo_args["edit"] = "0x" + QString::number( uint( (void*) p_edit ), 16);

    output["echo"] = echo_args;

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

void gx::href::eval( const QString& meth, const QJsonObject & input, QJsonObject & output )
{
    output["fail"] = "gx::href::eval TODO: NOT IMPLEMENTED YET";

    output["echo"] = input;

    output["gx::href::eval"] = meth;
}

void gx::wref::eval( const QString& meth, const QJsonObject & input, QJsonObject & output )
{
    output["fail"] = "gx::wref::eval TODO: NOT IMPLEMENTED YET";

    output["echo"] = input;

    output["gx::wref::eval"] = meth;
}

void gx::self::eval( const QString& meth, const QJsonObject & input, QJsonObject & output )
{
    output["fail"] = "gx::self::eval TODO: NOT IMPLEMENTED YET";

    output["echo"] = input;

    output["gx::self::eval"] = meth;
}
