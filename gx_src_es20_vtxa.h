#ifndef GX_SRC_ES20_VTXA_H
#define GX_SRC_ES20_VTXA_H

#include <QOpenGLFunctions>

// GLES2.0 GLSL1.1 vertex attributes

namespace gx
{

struct av1f;   struct av2f;   struct av3f;   struct av4f;
struct av1i;   struct av2i;   struct av3i;   struct av4i;
struct av1b;   struct av2b;   struct av3b;   struct av4b;
struct av1u;   struct am2f;   struct am3f;   struct am4f;

// GLSL vertex attribute abstract base class

struct vtxa
{
    // program -> get attribute location by name
    // program -> enable attribute array at attribute location
    // program -> set variable attribute location in binded VBO use offset and size"step"buffer
    struct proc
    {
        virtual ~proc() {}
        virtual void on(gx::av1f*) = 0;  // "float"         GL_FLOAT
        virtual void on(gx::av2f*) = 0;  // "vec2"          GL_FLOAT_VEC2
        virtual void on(gx::av3f*) = 0;  // "vec3"          GL_FLOAT_VEC3
        virtual void on(gx::av4f*) = 0;  // "vec4"          GL_FLOAT_VEC4
        virtual void on(gx::av1i*) = 0;  // "int"           GL_INT
        virtual void on(gx::av2i*) = 0;  // "ivec2"         GL_INT_VEC2
        virtual void on(gx::av3i*) = 0;  // "ivec3"         GL_INT_VEC3
        virtual void on(gx::av4i*) = 0;  // "ivec4"         GL_INT_VEC4
        virtual void on(gx::av1u*) = 0;  // "unsigned int"  GL_UNSIGNED_INT
        virtual void on(gx::av1b*) = 0;  // "bool"          GL_BOOL
        virtual void on(gx::av2b*) = 0;  // "bvec2"         GL_BOOL_VEC2
        virtual void on(gx::av3b*) = 0;  // "bvec3"         GL_BOOL_VEC3
        virtual void on(gx::av4b*) = 0;  // "bvec4"         GL_BOOL_VEC4
        virtual void on(gx::am2f*) = 0;  // "mat2"          GL_FLOAT_MAT2
        virtual void on(gx::am3f*) = 0;  // "mat3"          GL_FLOAT_MAT3
        virtual void on(gx::am4f*) = 0;  // "mat4"          GL_FLOAT_MAT4
    };

    virtual void on(vtxa::proc*) = 0;

    virtual int get_glsl_type_id() const = 0;

    virtual const char* get_glsl_type_name() const = 0;

    virtual bool is_prototype() { return true;  }  // is prototype w/o data buffer

    virtual bool is_reference() { return false; }  // is reference to external data buffer

    virtual ~vtxa(){}

    static const char* get_glsl_type_name(const int& n);  // return GLSL name for GL TYPE ID

    static vtxa* make_new(const int& gl_type_id);         // return supported unfa or NULL

    void   set_prog_indx(const GLuint& i) { m_prog_indx = i; }
    GLuint get_prog_indx() const          { return m_prog_indx; }

    void   set_prog_size(const GLint& s)  { m_prog_size = s; }
    GLint  get_prog_size() const          { return m_prog_size; }

protected:

    GLuint m_prog_indx = -1;
    GLint  m_prog_size = 0;

}; // struct vtxa

struct av1f : vtxa
{
    virtual ~av1f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT; }

    const char* get_glsl_type_name() const { return "float"; }
};

struct av2f : vtxa
{
    virtual ~av2f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_VEC2; }

    const char* get_glsl_type_name() const { return "vec2"; }
};

struct av3f : vtxa
{
    virtual ~av3f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_VEC3; }

    const char* get_glsl_type_name() const { return "vec3"; }
};

struct av4f : vtxa
{
    virtual ~av4f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_VEC4; }

    const char* get_glsl_type_name() const { return "vec4"; }
};

struct av1i : vtxa
{
    virtual ~av1i() {}
    virtual int*   get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT; }

    const char* get_glsl_type_name() const { return "int"; }
};

struct av2i : vtxa
{
    virtual ~av2i() {}
    virtual int*   get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT_VEC2; }

    const char* get_glsl_type_name() const { return "ivec2"; }
};

struct av3i : vtxa
{
    virtual ~av3i() {}
    virtual int*   get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT_VEC3; }

    const char* get_glsl_type_name() const { return "ivec3"; }
};

struct av4i : vtxa
{
    virtual ~av4i() {}
    virtual int*   get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_INT_VEC4; }

    const char* get_glsl_type_name() const { return "ivec4"; }
};

struct av1u : vtxa
{
    virtual ~av1u() {}
    virtual unsigned int* get_buff() { return nullptr; }
    virtual int           get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_UNSIGNED_INT; }

    const char* get_glsl_type_name() const { return "unsigned int"; }
};

struct av1b : vtxa
{
    virtual ~av1b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }


    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL; }

    const char* get_glsl_type_name() const { return "bool"; }
};

struct av2b : vtxa
{
    virtual ~av2b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL_VEC2; }

    const char* get_glsl_type_name() const { return "bvec2"; }
};

struct av3b : vtxa
{
    virtual ~av3b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL_VEC3; }

    const char* get_glsl_type_name() const { return "bvec3"; }
};

struct av4b : vtxa
{
    virtual ~av4b() {}
    virtual bool* get_buff() { return nullptr; }
    virtual int   get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_BOOL_VEC4; }

    const char* get_glsl_type_name() const { return "bvec4"; }
};

struct am2f : vtxa
{
    virtual ~am2f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_MAT2; }

    const char* get_glsl_type_name() const { return "mat2"; }
};

struct am3f : vtxa
{
    virtual ~am3f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_MAT3; }

    const char* get_glsl_type_name() const { return "mat3"; }
};

struct am4f : vtxa
{
    virtual ~am4f() {}
    virtual float* get_buff() { return nullptr; }
    virtual int    get_size() { return 0; }

    void on(vtxa::proc* o) { o->on(this); }

    int  get_glsl_type_id() const { return GL_FLOAT_MAT4; }

    const char* get_glsl_type_name() const { return "mat4"; }
};

}  // namespace gx

#endif // GX_SRC_ES20_VTXA_H
