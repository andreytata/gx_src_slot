#include <gx_src_es20.h>

gx::prog::prog_state* gx::prog::get_fail_state()
{
    static struct a :gx::prog::prog_state
    {
        virtual void set_current(prog*){}
        virtual const char* get_failure(prog*) {return "empty FAIL state"; }
    }
    ss; return &ss;
}

gx::prog::prog_state* gx::prog::get_draw_state()
{
    static struct b :gx::prog::prog_state
    {
        virtual void set_current(prog*) {}
        virtual const char* get_failure(prog*) { return "empty DRAW state"; }
    }
    ss; return &ss;
}

gx::prog::prog_state* gx::prog::get_free_state()
{
    static struct cd :gx::prog::prog_state
    {
        virtual void set_current(prog*) {}
        virtual const char* get_failure(prog*) { return "empty FREE state"; }
    }
    ss; return &ss;
}

gx::prog::prog_state* gx::prog::get_init_state()
{
    static struct dd : gx::prog::prog_state
    {
        void set_current(prog* p)
        {
            initializeOpenGLFunctions();
            qDebug() << "!!!!!!!!!!!!";

            if (!p->program.addShaderFromSourceFile(
                        QOpenGLShader::Vertex, ":/vshader.glsl"))
            {
                p->current_state = p->get_fail_state();
                p->switcher_info = "ERROR: Add vertex shader from source file.";
                return;
            }

            qDebug() << "VSHADER_ADDED_FROM_CODE";

            // Compile fragment shader
            if (!p->program.addShaderFromSourceFile(
                        QOpenGLShader::Fragment, ":/fshader.glsl"))
            {
                p->current_state = p->get_fail_state();
                p->switcher_info = "ERROR: Add fragment shader from source file.";
                return;
            }
            qDebug() << "FSHADER_ADDED_FROM_CODE";


            // Link shader pipeline
            if (!p->program.link())
            {
                p->current_state = p->get_fail_state();
                p->switcher_info = "ERROR: Link shader pipeline.";
                return;
            }
            qDebug() << "GLSL SHADER PROGRAM LINKED";

            // Bind shader pipeline for use
            if (!p->program.bind())
            {
                p->current_state = p->get_fail_state();
                p->switcher_info = "ERROR: Bind shader pipeline for use.";
                return;
            }

            qDebug() << "program max geom output vertices => " << p->program.maxGeometryOutputVertices();

            GLuint program_id = p->program.programId();

            qDebug() << "program.programId() => " << program_id;

            GLint i;                    // var's id
            GLint count;                // active vars count
            GLint size;                 // size of the variable

            GLenum type;                // type of the variable (float, vec3 or mat4, etc)

            const GLsizei bufSize = 1024; // maximum name length
            GLchar name[bufSize];         // variable GLSL name
            GLsizei length;               // variable name length

            glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, &count);

            qDebug() << "Active Attributes:" << count;

            for (i = 0; i < count; i++)
            {
                glGetActiveAttrib(program_id, (GLuint)i, bufSize, &length, &size, &type, name);
                // WHAT THE HELL WE DOING HERE? Collect ACTIVE vertex attributes
                // WHY WE MUST DOING SOMETHING LIKE THIS? Some attributes declared at GLSL
                // shader code, maybe not ACTIVE! This depend both used GPU, and OpenGL
                // driver version. So:
                vtxa* attr = gx::vtxa::make_new(type);
                attr->set_prog_indx(i);
                attr->set_prog_size(size);
                p->program_active_attributes[name] = attr;
                printf ( "ATTRIBUTE #%d Type: '%s' Name: '%s' Size: %d\n", attr->get_prog_indx()
                         , vtxa::get_glsl_type_name(attr->get_glsl_type_id())
                         , name
                         , attr->get_prog_size()
                       );
            }

            glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &count);

            qDebug() << "Active Uniforms:" << count;

            for (i = 0; i < count; i++)
            {
                // WHAT THE HELL WE DOING HERE? Collect ACTIVE UNIFORMS
                glGetActiveUniform(program_id, (GLuint)i, bufSize, &length, &size, &type, name);
                unfa* attr = gx::unfa::make_new(type);
                attr->set_prog_indx(i);
                attr->set_prog_size(size);
                p->program_active_uniform[name] = attr;
                printf ( "UNIFORM #%d Type: '%s' Name: '%s' Size: %d\n", attr->get_prog_indx()
                         , unfa::get_glsl_type_name(attr->get_glsl_type_id())
                         , name
                         , attr->get_prog_size()
                       );
            }
        }
        virtual const char* get_failure(prog*) { return "empty INIT state"; }
    } ss; return &ss;
}

void gx::prog::qdebug_active_variables()
{
    qDebug() << QString::asprintf("\ngx::prog at <%p> gl<%d> {", this, this->program.programId())
                .toStdString().c_str();

    // make simplest static "VERTEX ATTRIBUTE"s visitor
    static struct gx_vtxa_prog: gx::vtxa::proc {
        void on(gx::av1f* a) {qDebug() << name << "float        GL_FLOAT        " << a->get_prog_indx();}
        void on(gx::av2f* a) {qDebug() << name << "vec2         GL_FLOAT_VEC2   " << a->get_prog_indx();}
        void on(gx::av3f* a) {qDebug() << name << "vec3         GL_FLOAT_VEC3   " << a->get_prog_indx();}
        void on(gx::av4f* a) {qDebug() << name << "vec4         GL_FLOAT_VEC4   " << a->get_prog_indx();}
        void on(gx::av1i* a) {qDebug() << name << "int          GL_INT          " << a->get_prog_indx();}
        void on(gx::av2i* a) {qDebug() << name << "ivec2        GL_INT_VEC2     " << a->get_prog_indx();}
        void on(gx::av3i* a) {qDebug() << name << "ivec3        GL_INT_VEC3     " << a->get_prog_indx();}
        void on(gx::av4i* a) {qDebug() << name << "ivec4        GL_INT_VEC4     " << a->get_prog_indx();}
        void on(gx::av1u* a) {qDebug() << name << "unsigned int GL_UNSIGNED_INT " << a->get_prog_indx();}
        void on(gx::av1b* a) {qDebug() << name << "bool         GL_BOOL         " << a->get_prog_indx();}
        void on(gx::av2b* a) {qDebug() << name << "bvec2        GL_BOOL_VEC2    " << a->get_prog_indx();}
        void on(gx::av3b* a) {qDebug() << name << "bvec3        GL_BOOL_VEC3    " << a->get_prog_indx();}
        void on(gx::av4b* a) {qDebug() << name << "bvec4        GL_BOOL_VEC4    " << a->get_prog_indx();}
        void on(gx::am2f* a) {qDebug() << name << "mat2         GL_FLOAT_MAT2   " << a->get_prog_indx();}
        void on(gx::am3f* a) {qDebug() << name << "mat3         GL_FLOAT_MAT3   " << a->get_prog_indx();}
        void on(gx::am4f* a) {qDebug() << name << "mat4         GL_FLOAT_MAT4   " << a->get_prog_indx();}
        const char* name;
    }
    vtxa_visitor;
    for(auto pair: this->program_active_attributes)
    {
       //vtxa_visitor.qdebug(pair.first, pair.second);
       //qDebug() << pair.first.c_str();
       QString name = QString("  vtxa: %1 //").arg(pair.first.c_str(), -16, ' ');
       std::string buff = name.toStdString();
       vtxa_visitor.name = buff.c_str();        // pair.first.c_str();
       pair.second->on(&vtxa_visitor);
    }

    // make simplest static UNIFORM variables visitor
    static struct gx_unfa_prog: gx::unfa::prog {
        const char* name;
        void on(gx::unfr* u) {qDebug() << name << "REF          TO UNFA PTR     " << u->get_prog_indx();}
        void on(gx::uv1f* u) {qDebug() << name << "float        GL_FLOAT        " << u->get_prog_indx();}
        void on(gx::uv2f* u) {qDebug() << name << "vec2         GL_FLOAT_VEC2   " << u->get_prog_indx();}
        void on(gx::uv3f* u) {qDebug() << name << "vec3         GL_FLOAT_VEC3   " << u->get_prog_indx();}
        void on(gx::uv4f* u) {qDebug() << name << "vec4         GL_FLOAT_VEC4   " << u->get_prog_indx();}
        void on(gx::uv1i* u) {qDebug() << name << "int          GL_INT          " << u->get_prog_indx();}
        void on(gx::uv2i* u) {qDebug() << name << "ivec2        GL_INT_VEC2     " << u->get_prog_indx();}
        void on(gx::uv3i* u) {qDebug() << name << "ivec3        GL_INT_VEC3     " << u->get_prog_indx();}
        void on(gx::uv4i* u) {qDebug() << name << "ivec4        GL_INT_VEC4     " << u->get_prog_indx();}
        void on(gx::uv1u* u) {qDebug() << name << "unsigned int GL_UNSIGNED_INT " << u->get_prog_indx();}
        void on(gx::uv1b* u) {qDebug() << name << "bool         GL_BOOL         " << u->get_prog_indx();}
        void on(gx::uv2b* u) {qDebug() << name << "bvec2        GL_BOOL_VEC2    " << u->get_prog_indx();}
        void on(gx::uv3b* u) {qDebug() << name << "bvec3        GL_BOOL_VEC3    " << u->get_prog_indx();}
        void on(gx::uv4b* u) {qDebug() << name << "bvec4        GL_BOOL_VEC4    " << u->get_prog_indx();}
        void on(gx::um2f* u) {qDebug() << name << "mat2         GL_FLOAT_MAT2   " << u->get_prog_indx();}
        void on(gx::um3f* u) {qDebug() << name << "mat3         GL_FLOAT_MAT3   " << u->get_prog_indx();}
        void on(gx::um4f* u) {qDebug() << name << "mat4         GL_FLOAT_MAT4   " << u->get_prog_indx();}
        void on(gx::tx2d* u) {qDebug() << name << "sampler2D    GL_SAMPLER_2D   " << u->get_prog_indx();}
        void on(gx::txcb* u) {qDebug() << name << "samplerCube  GL_SAMPLER_CUBE " << u->get_prog_indx();}
    }
    unfa_visitor;
    for(auto pair: this->program_active_uniform)
    {
        QString name = QString("  unfa: %1 //").arg(pair.first.c_str(), -16, ' ');
        std::string buff = name.toStdString();
        unfa_visitor.name = buff.c_str();        // pair.first.c_str();
        pair.second->on(&unfa_visitor);
    }

    qDebug() << "}";
}

