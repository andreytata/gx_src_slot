#include <gx_src_es20.h>

gx::shgr::shgr_state* gx::shgr::get_init_state() // prepare prog and vbo (not inited)
{
        static struct: gx::shgr::shgr_state
        {
            void draw(shgr* p_shgr, const int& ibo_id) const
            {
                 // p_shgr->mp_prog->set_current();  // need bind program before draw
                 bind(p_shgr);           // bind change p_shgr's state to "fail" or "bind" state
                 p_shgr->draw(ibo_id);   // delegate "draw" to state choosen in "bind" method
                                         // of this "initial state"
            }

            void bind(shgr* p_shgr) const
            {
                p_shgr->mp_prog->init();  // gl-functions init
                p_shgr->mp_prog->set_current();  // need bind program before detect variables locations

                // static program's vertex attributes sources
                static struct: gx::vtxa::proc
                {
                    void on(gx::av1f* a){qDebug() << name << "float        GL_FLOAT        " << a->get_prog_indx();}
                    void on(gx::av2f* a){qDebug() << name << "vec2         GL_FLOAT_VEC2   " << a->get_prog_indx();}
                    void on(gx::av3f* a){qDebug() << name << "vec3         GL_FLOAT_VEC3   " << a->get_prog_indx();}
                    void on(gx::av4f* a){
                        // Enter here on each [active-attribute-with-type-vec4]
                        // - UNKNOWN IS, exists or not apropriate vec4 buffer on data-source side
                        // - Active GLSL attribute has valid name, all other fields are unknown
                        // -
                        // Find attribute's offset in surface data source "gx::surf* this->data"
                        // vtxb.bind() =>
                        //
                        qDebug() << name << "vec4         GL_FLOAT_VEC4   " << a->get_prog_indx();
                    }
                    void on(gx::av1i* a){qDebug() << name << "int          GL_INT          " << a->get_prog_indx();}
                    void on(gx::av2i* a){qDebug() << name << "ivec2        GL_INT_VEC2     " << a->get_prog_indx();}
                    void on(gx::av3i* a){qDebug() << name << "ivec3        GL_INT_VEC3     " << a->get_prog_indx();}
                    void on(gx::av4i* a){qDebug() << name << "ivec4        GL_INT_VEC4     " << a->get_prog_indx();}
                    void on(gx::av1u* a){qDebug() << name << "unsigned int GL_UNSIGNED_INT " << a->get_prog_indx();}
                    void on(gx::av1b* a){qDebug() << name << "bool         GL_BOOL         " << a->get_prog_indx();}
                    void on(gx::av2b* a){qDebug() << name << "bvec2        GL_BOOL_VEC2    " << a->get_prog_indx();}
                    void on(gx::av3b* a){qDebug() << name << "bvec3        GL_BOOL_VEC3    " << a->get_prog_indx();}
                    void on(gx::av4b* a){qDebug() << name << "bvec4        GL_BOOL_VEC4    " << a->get_prog_indx();}
                    void on(gx::am2f* a){qDebug() << name << "mat2         GL_FLOAT_MAT2   " << a->get_prog_indx();}
                    void on(gx::am3f* a){qDebug() << name << "mat3         GL_FLOAT_MAT3   " << a->get_prog_indx();}
                    void on(gx::am4f* a){qDebug() << name << "mat4         GL_FLOAT_MAT4   " << a->get_prog_indx();}
                    gx::surf*   data = nullptr;  // data source root, where "link" find data sources;
                    shgr*       view = nullptr;  // target connections info storage instance
                    const char* name = nullptr;  // current vertex attribute name
                }
                for_each_vtxa_in_prog;
                for_each_vtxa_in_prog.data = p_shgr->mp_data;
                for_each_vtxa_in_prog.view = p_shgr;
                for(auto pair: p_shgr->mp_prog->program_active_attributes)
                {
                   for_each_vtxa_in_prog.name = pair.first.c_str();  // append pointer to attr name
                   pair.second->on(&for_each_vtxa_in_prog);          // use visitor to try link
                }
                // vbo stored at mp_data side, use on_paint() bind buffer
                // TODO: copy all uniform attributes in to this (here), to create dynamic namespace for
                // variables data sources switching. So, each created copy store self original data-ptr
                // and self own editable(inserted) data-ptr.
            }
        }
        init_bind_state; return &init_bind_state;
}

void gx::shgr::link_together()
{
        // static program's vertex attributes sources
        static struct: gx::vtxa::proc
        {
            void on(gx::av1f* a){qDebug() << name << "float        GL_FLOAT        " << a->get_prog_indx();}
            void on(gx::av2f* a){qDebug() << name << "vec2         GL_FLOAT_VEC2   " << a->get_prog_indx();}
            void on(gx::av3f* a){qDebug() << name << "vec3         GL_FLOAT_VEC3   " << a->get_prog_indx();}
            void on(gx::av4f* a){qDebug() << name << "vec4         GL_FLOAT_VEC4   " << a->get_prog_indx();}
            void on(gx::av1i* a){qDebug() << name << "int          GL_INT          " << a->get_prog_indx();}
            void on(gx::av2i* a){qDebug() << name << "ivec2        GL_INT_VEC2     " << a->get_prog_indx();}
            void on(gx::av3i* a){qDebug() << name << "ivec3        GL_INT_VEC3     " << a->get_prog_indx();}
            void on(gx::av4i* a){qDebug() << name << "ivec4        GL_INT_VEC4     " << a->get_prog_indx();}
            void on(gx::av1u* a){qDebug() << name << "unsigned int GL_UNSIGNED_INT " << a->get_prog_indx();}
            void on(gx::av1b* a){qDebug() << name << "bool         GL_BOOL         " << a->get_prog_indx();}
            void on(gx::av2b* a){qDebug() << name << "bvec2        GL_BOOL_VEC2    " << a->get_prog_indx();}
            void on(gx::av3b* a){qDebug() << name << "bvec3        GL_BOOL_VEC3    " << a->get_prog_indx();}
            void on(gx::av4b* a){qDebug() << name << "bvec4        GL_BOOL_VEC4    " << a->get_prog_indx();}
            void on(gx::am2f* a){qDebug() << name << "mat2         GL_FLOAT_MAT2   " << a->get_prog_indx();}
            void on(gx::am3f* a){qDebug() << name << "mat3         GL_FLOAT_MAT3   " << a->get_prog_indx();}
            void on(gx::am4f* a){qDebug() << name << "mat4         GL_FLOAT_MAT4   " << a->get_prog_indx();}
            gx::surf*   data = nullptr;  // data source root, where "link" find data sources;
            shgr*       view = nullptr;  // target connections info storage instance
            const char* name = nullptr;  // current vertex attribute name
        }
        for_each_vtxa_in_prog;
        for_each_vtxa_in_prog.data = mp_data;
        for_each_vtxa_in_prog.view = this;
        for(auto pair: mp_prog->program_active_attributes)
        {
           for_each_vtxa_in_prog.name = pair.first.c_str();  // append pointer to attr name
           pair.second->on(&for_each_vtxa_in_prog);          // use visitor to try link
        }
        // vbo stored at mp_data side, use on_paint() bind buffer
        // TODOcopy all uniform attributes in to this (here), to create dynamic namespace for
        // variables data sources switching. So, each created copy store self original data-ptr
        // and self own editable(inserted) data-ptr.
}
