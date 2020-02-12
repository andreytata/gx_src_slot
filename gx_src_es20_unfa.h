#ifndef GX_SRC_ES20_UNFA_H
#define GX_SRC_ES20_UNFA_H

#include <QOpenGLFunctions>

#include <gx_src_slot.h>  // shared slots

// GLES2.0 GLSL1.1 uniform attributes

namespace gx
{

struct uv1f; struct uv2f; struct uv3f; struct uv4f;  // float, vec*
struct uv1i; struct uv2i; struct uv3i; struct uv4i;  // int, ivec*
struct uv1b; struct uv2b; struct uv3b; struct uv4b;  // bool, bvec*
struct uv1u; struct um2f; struct um3f; struct um4f;  // unsigned int, mat*
struct tx2d; // sampler2D     GL_SAMPLER_2D
struct txcb; // samplerCube   GL_SAMPLER_CUBE
struct unfr; // reference to any unfa concrete class, even unfr

struct unfa
{
    struct prog
    {
        virtual ~prog(){}
        virtual void on(gx::unfr*) = 0;  // REF TO ANY OTHER unfa*
        virtual void on(gx::uv1f*) = 0;  // float         GL_FLOAT
        virtual void on(gx::uv2f*) = 0;  // vec2          GL_FLOAT_VEC2
        virtual void on(gx::uv3f*) = 0;  // vec3          GL_FLOAT_VEC3
        virtual void on(gx::uv4f*) = 0;  // vec4          GL_FLOAT_VEC4
        virtual void on(gx::uv1i*) = 0;  // int           GL_INT
        virtual void on(gx::uv2i*) = 0;  // ivec2         GL_INT_VEC2
        virtual void on(gx::uv3i*) = 0;  // ivec3         GL_INT_VEC3
        virtual void on(gx::uv4i*) = 0;  // ivec4         GL_INT_VEC4
        virtual void on(gx::uv1u*) = 0;  // unsigned int  GL_UNSIGNED_INT
        virtual void on(gx::uv1b*) = 0;  // bool          GL_BOOL
        virtual void on(gx::uv2b*) = 0;  // bvec2         GL_BOOL_VEC2
        virtual void on(gx::uv3b*) = 0;  // bvec3         GL_BOOL_VEC3
        virtual void on(gx::uv4b*) = 0;  // bvec4         GL_BOOL_VEC4
        virtual void on(gx::um2f*) = 0;  // mat2          GL_FLOAT_MAT2
        virtual void on(gx::um3f*) = 0;  // mat3          GL_FLOAT_MAT3
        virtual void on(gx::um4f*) = 0;  // mat4          GL_FLOAT_MAT4
        virtual void on(gx::tx2d*) = 0;  // sampler2D     GL_SAMPLER_2D
        virtual void on(gx::txcb*) = 0;  // samplerCube   GL_SAMPLER_CUBE
    };

    virtual void on(unfa::prog*) = 0;

    virtual ~unfa() {}

    virtual int get_glsl_type_id() const = 0;

    virtual const char* get_glsl_type_name() const = 0;

    static const char* get_glsl_type_name(const int& n);

    static unfa* make_new(const int& gl_type_id);

    void   set_prog_indx(const GLuint& i) { m_prog_indx = i; }

    GLuint get_prog_indx() const          { return m_prog_indx; }

    void   set_prog_size(const GLint& s)  { m_prog_size = s; }

    GLint  get_prog_size() const          { return m_prog_size; }

protected:

    GLuint m_prog_indx = -1;

    GLint  m_prog_size = 0;
};                                                            // unfa

struct uv1f : unfa
{
    virtual ~uv1f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT; }

    const char* get_glsl_type_name() const { return "float"; }
};

struct uv2f : unfa
{
    virtual ~uv2f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_VEC2; }

    const char* get_glsl_type_name() const { return "vec2"; }
};

struct uv3f : unfa
{
    virtual ~uv3f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_VEC3; }

    const char* get_glsl_type_name() const { return "vec3"; }
};

struct uv4f : unfa
{
    virtual ~uv4f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_VEC4; }

    const char* get_glsl_type_name() const { return "vec4"; }
};

struct uv1i : unfa
{
    virtual ~uv1i() {}
    virtual int* get_buff() { return nullptr; }
    virtual int  get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT; }

    const char* get_glsl_type_name() const { return "int"; }
};

struct uv2i : unfa
{
    virtual ~uv2i() {}
    virtual int* get_buff() { return nullptr; }
    virtual int  get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT_VEC2; }

    const char* get_glsl_type_name() const { return "ivec2"; }
};

struct uv3i : unfa
{
    virtual ~uv3i() {}
    virtual int* get_buff() { return nullptr; }
    virtual int  get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT_VEC3; }

    const char* get_glsl_type_name() const { return "ivec3"; }
};

struct uv4i : unfa
{
    virtual ~uv4i() {}
    virtual int* get_buff() { return nullptr; }
    virtual int  get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT_VEC4; }

    const char* get_glsl_type_name() const { return "ivec4"; }
};

struct uv1u : unfa
{
    virtual ~uv1u() {}
    virtual unsigned int* get_buff() { return nullptr; }
    virtual int           get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_UNSIGNED_INT; }

    const char* get_glsl_type_name() const { return "unsigned int"; }
};

struct uv1b : unfa
{
    virtual ~uv1b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL; }

    const char* get_glsl_type_name() const { return "bool"; }
};

struct uv2b : unfa
{
    virtual ~uv2b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL_VEC2; }

    const char* get_glsl_type_name() const { return "bvec2"; }
};

struct uv3b : unfa
{
    virtual ~uv3b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL_VEC3; }

    const char* get_glsl_type_name() const { return "bvec3"; }
};

struct uv4b : unfa
{
    virtual ~uv4b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL_VEC4; }

    const char* get_glsl_type_name() const { return "bvec4"; }
};

struct um2f : unfa
{
    virtual ~um2f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_MAT2; }

    const char* get_glsl_type_name() const { return "mat2"; }
};

struct um3f : unfa
{
    virtual ~um3f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_MAT3; }

    const char* get_glsl_type_name() const { return "mat3"; }
};

struct um4f : unfa
{
    virtual ~um4f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_MAT4; }

    const char* get_glsl_type_name() const { return "mat4"; }
};

struct tx2d : unfa
{
    virtual ~tx2d() {}

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_SAMPLER_2D; }

    const char* get_glsl_type_name() const { return "sampler2D"; }
};

struct txcb : unfa
{
    virtual ~txcb() {}

    void on(unfa::prog* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_SAMPLER_CUBE; }

    const char* get_glsl_type_name() const { return "samplerCube"; }
};

struct unfr : unfa
{
    virtual ~unfr() {}

    void on(unfa::prog* o) { o->on(this); }

    virtual int  get_glsl_type_id() const {
        return mp_unfa ? mp_unfa->get_glsl_type_id() : 0;
    }

    virtual const char* get_glsl_type_name() const {
        return mp_unfa ? mp_unfa->get_glsl_type_name():"nullptr";
    }

    virtual unfa* get_unfa() const { return mp_unfa; }

    virtual unfa* set_unfa(unfa* pnew) { unfa* prev = mp_unfa; mp_unfa = pnew; return prev; }

protected:

    unfa* mp_unfa = nullptr;
};

}

#endif // GX_SRC_ES20_UNFA_H
