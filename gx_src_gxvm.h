#ifndef GX_SRC_GXVM_H
#define GX_SRC_GXVM_H

#include<QObject>

namespace gx
{

class job:public QObject
{
    Q_OBJECT
public:
    job(QObject*parent=NULL):QObject(parent){}
    virtual ~job(){}

public slots:
    void get_state (QJsonObject&){}
    void run_step  (QJsonObject&){}
};


class par:public job
{
    Q_OBJECT
public:
    par(QObject*parent=NULL):job(parent){}
    virtual ~par(){}
public slots:
    void get_state     (QJsonObject&)       {}
    void step          (QJsonObject&)       {}
    void set_exceptions(const QJsonObject&) {}
    void set_conditions(const QJsonObject&) {}
    void add_items     (const QJsonObject&) {}
};


class seq:public job
{
    Q_OBJECT
public:
    seq(QObject*parent=NULL):job(parent){}
    virtual ~seq(){}
public slots:
    void get_state     (QJsonObject&)       {}
    void step          (QJsonObject&)       {}
    void set_exceptions(const QJsonObject&) {}
    void set_conditions(const QJsonObject&) {}
    void add_items     (const QJsonObject&) {}
};


class vip:public job
{
    Q_OBJECT
public:
    vip(QObject*parent=NULL):job(parent){}
    virtual ~vip(){}
public slots:
    void new_par(const QJsonObject&) {}
    void new_seq(const QJsonObject&) {}
    void new_vip(const QJsonObject&) {}
};

}
#endif // GX_SRC_GXVM_H
