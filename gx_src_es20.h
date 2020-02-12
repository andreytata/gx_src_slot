#ifndef GX_SRC_ES20_H
#define GX_SRC_ES20_H

#include <map>
#include <list>
#include <string>
#include <memory>

#include <QBasicTimer>
#include <QOpenGLTexture>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QObject>


#include <gx_src_es20_vtxa.h>

#include <gx_src_es20_unfa.h>

#define JSON(...) #__VA_ARGS__
/* // HOWTO use this JSON define:
const char* sample_json = JSON ( // comment
{"run_machine":"load_parallel_threads"
//,"0" : "https://godbolt.org?123&123"
,"1" : "https://godbolt.org??123&123"
,"2" : "https://god_bolt.org?123&123"
,"3" : "https://god___bolt.org??123&123"
,"4" : "https://god_____bolt.org?123&123"
,"5" : "https://godbolt.org???123&123"
,"one_success": ["stop_other","print_curr", "sw_machine"]
,"one_failure": ["print_curr","print_fail", "_continue"]
,"all_failure": ["reset_to_initial_state"]
});
*/

namespace gx
{
    struct prog;  // glsl program state machine

    struct indx;  // draw method base class TRIANGLE_FAN, TRIANGLE_STRIP, TRIANGLES, e.t.c.

    struct vtxa;  // glsl program abstract attribute
                  // void set_curr(prog*)

    struct vtxb;  // vertex buffer (possible interlived)
                  // void set_attr(const char* glsl_name, vtxa*);

    struct unfa;  // glsl program abstract uniform attribute

    struct surf;  // immutable surface's description base class
                  // void set_vtxb(const char* glsl_name, std::shared_ptr<vtxa>);

    struct shdg
    {
        std::shared_ptr<prog> prog_ref;  // self shdg

        std::shared_ptr<vtxb> vtxb_ref;  // self vtxb

        struct vars_map;

        virtual void calculate_vars_map(){}

        std::list<std::shared_ptr<indx> > draw_map;  // ibo bind & draw surface part
    };

    struct tran
    {    // base class for proj/node/joint with get_mat4 pointer ability
        virtual ~tran() {}
        virtual float* get_mat4() = 0;
    };

    struct surf
    {    // shared surface ( exported const object )
        std::map<std::string, vtxa*> m_buff;  // exported vertex attributes                (const)
        std::map<std::string, unfa*> m_vars;  // exported dependances (uniform attributes) (const)
                                              // initial textures, weights,
                                              // ...
        std::list<indx*> m_ibo;               // index arrays with draw mode
        std::list<tran*> m_to_export_root;    // const exported scene position
        std::list<tran*> joints;              // skin's fixed exported joints in bind-pos
    };

    struct shgr  // shading group material and surface based(real-time animated view-surface)
    {
        std::map<std::string, unfa*> m_vars;  // changed dependances (uniform attributes)
                                              // changed textures and other uniforms
                                              // initial contain self mp_surf->m_vars;
        std::list<tran*> m_to_export_root;        // editable exported scene position
        std::list<tran*> m_to_mount_root;         // scene root scene initial - empty
        std::list<tran*> m_joints;                // animated joints, binded to other transform

        gx::prog* mp_prog = nullptr;              // set once - create new shgr with other prog
        gx::surf* mp_data = nullptr;              // set once - create new shgr with other surf

        struct shgr_state                         // abstract shading group state
        {
            virtual ~shgr_state(){}
            virtual void bind(shgr*) const = 0;
            virtual void draw(shgr*, const int&) const = 0;
        };

        shgr_state* curr_state=get_init_state();  // actual shading group state

        // prepare program and vertex-data to draw ( delegated to current state )
        void bind() {
            curr_state->bind(this);
        }

        // draw elements use ibo from sub_geom[id] ( delegated to current state )
        void draw(const int& ibo_id = 0) { curr_state->draw(this, ibo_id); }

        static shgr_state* get_fail_state() { return nullptr; }  // shgr in failure state
        static shgr_state* get_bind_state() { return nullptr; }  // this prog and vbo are binded
        static shgr_state* get_free_state() { return nullptr; }  // this prog and vbo are released
        static shgr_state* get_init_state();  // prepare prog and vbo (not inited)

        bool is_curr_state(const gx::shgr::shgr_state* p_state) const
        {
            return curr_state == p_state ? true : false;
        }

        void make_draw_prog_data(gx::prog* p, gx::surf* s)
        {
            this->mp_prog = p;  // program and active attributes info
            this->mp_data = s;  // immutable ? data source or exported data
        }

        void link_together();

        const char* fail() const      // return last error description or NULL on success
        {
            return nullptr;
        }


        // change data source for uniform or flag variable
        void bind( const char*, gx::unfa* )
        {
        }

        void free( const char* )                   // reset changed data source for variable
        {
        }

        int       get_geom_max()              const { return 0; }  // subgeomerties (index buffer)
        int       get_geom_ibo(const char*)   const { return 0; }  //
        gx::indx* get_geom_ptr(const int&)    const { return nullptr; }  //
    };


    // GL index array
    // Surf contain one or more named index arrays. Each faces set can be drawed
    // separately with other draw method, textures, programm, gl-option, e.t.c.
    struct indx
    {
        struct triangles;
        struct triangle_fan;
        struct triangle_strip;
        struct proc
        {   // visitor needed?
            virtual ~proc() {}
            virtual void on(gx::indx::triangles      *) = 0;
            virtual void on(gx::indx::triangle_fan   *) = 0;
            virtual void on(gx::indx::triangle_strip *) = 0;
        };
        virtual ~indx() {}
        virtual unsigned  get_size() const = 0;
        virtual unsigned* get_buff() = 0;  // is can be calculated, deprecate 'const'
        virtual void on(gx::indx*) = 0;
    };

    struct indx::triangles:indx {
        void on(indx::proc*o){o->on(this);}
        virtual ~triangles() {}
    };

    struct indx::triangle_strip:indx {
        void on(indx::proc*o){o->on(this);}
        virtual ~triangle_strip() {}
    };

    struct indx::triangle_fan:indx {
        void on(indx::proc*o){o->on(this);}
        virtual ~triangle_fan() {}
    };

    struct stse
    {
        struct item; struct tran; struct draw;
        struct proc
        {
            virtual ~proc(){}
            virtual void on(item*) = 0;
            virtual void on(tran*) = 0;
            virtual void on(draw*) = 0;
        };
        struct item {
            virtual ~item(){}
            virtual void on(stse::proc*) = 0;
        };
    };

    struct vtxb : stse::item {
        virtual ~vtxb(){}
        virtual float* get_buff() const = 0;
        virtual std::map<std::string, std::pair<vtxa*, int> >* get_vars() = 0;
        void on(stse::proc*p){p->on(this);}
    };

    struct prog  // shared program fsm, some set of the prog* stored at GL-widget side
    {
        // ACTIVE GLES & GLSL VARS INFO
        QOpenGLShaderProgram         program;                    // qt5 glsl program implementation
        std::map<std::string, vtxa*> program_active_attributes;  // Active vertex attributes
        std::map<std::string, unfa*> program_active_uniform;     // Active uniform attributes

        // Abstract Behavior Interface (GOF State Machine)
        struct prog_state: protected QOpenGLFunctions
        {
            void init()
            {
                initializeOpenGLFunctions();
            }
            virtual ~prog_state() {}
            virtual void set_current(prog*) = 0;
            virtual const char* get_failure(prog*) = 0;
        };
        const char* switcher_info = "INIT: Constructor";         // Last switcher's message
        prog_state* current_state = get_init_state();            // Set Initial State
        prog_state* get_draw_state();
        prog_state* get_fail_state();
        prog_state* get_init_state();
        prog_state* get_free_state();

        void init()
        {
            qDebug() << "--> initializeOpenGLFunctions() in states";
            get_draw_state()->init();
            get_fail_state()->init();
            get_init_state()->init();
            get_free_state()->init();
        }

        void set_current()        {        current_state->set_current(this); }

        const char* get_failure() { return current_state->get_failure(this); }

        void qdebug_active_variables();  // qDebug() << GLSL prog info ( visitors test & example )
    };


    class es20 : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT

    public:
        es20(QWidget* parent = 0):QOpenGLWidget(parent) {}

        virtual ~es20(){}

        virtual gx::prog* get_prog(const char* = nullptr) { return &prog0; }

        virtual gx::surf* get_surf(const char* = nullptr) { return &test_surface0; }

    protected:

        virtual void mousePressEvent   (QMouseEvent *e)
        {
            qDebug()<<e;
            update();
        }

        virtual void mouseReleaseEvent (QMouseEvent *e)
        {
            qDebug()<<e;
        }

        virtual void timerEvent        (QTimerEvent *)
        {
            // update();
        }

        virtual void initializeGL ()
        {
            initializeOpenGLFunctions();

            glClearColor(0, 0, 0, 1);

            glEnable(GL_DEPTH_TEST);

            glEnable(GL_CULL_FACE);
            // timer.start(120, this);
        }

        virtual void resizeGL     (int w, int h)  { qDebug()<<"resizeGL"<<w<<h; }

        virtual void paintGL();


        // Return binded or not binded VBO data in some current state
        // Binding invocated on "bind" in states "init" "fail" "main"
        // from SHGR->bind/draw methods of states "init" "fail" "main"
        virtual vtxb* get_vtxb( const char* = 0 )  // 0 -> return surface from path
        {
            static struct : vtxb  // return test surface
            {
                float* get_buff() const { static float a[] = {1.f,2.f,3.f}; return a; }
                int get_size() const { return 3*sizeof(float); }
                std::map<std::string, std::pair<vtxa*, int > >* get_vars()
                {
                    static struct a_position: av4f  {
                        vtxb* vbo;
                        a_position(vtxb* interleaved):vbo(interleaved){}
                        float* get_buff() { return vbo->get_buff(); }
                        int    get_size() { return 1; }
                    } attr0(this);
                    static struct a_normals: av4f  {
                        vtxb* vbo;
                        a_normals(vtxb* interleaved):vbo(interleaved){}
                        float* get_buff() { return vbo->get_buff(); }
                        int    get_size() { return 1; }
                    } attr1(this);
                    static std::map<std::string, std::pair<vtxa*, int> > vars = {
                       {"a_position", { &attr0, 0 } },
                       {"a_normals" , { &attr1, 0 } },
                    };
                    return &vars;
                }
            }
            mapped_vbo;
            return &mapped_vbo;
        }

    public:
        void add_shgr(const char* shgr_name, shgr* shgr_ptr)
        {
            m_shgr_dict[shgr_name] = shgr_ptr;
        }

        void del_shgr(const char* shgr_name)
        {
            m_shgr_dict.erase(shgr_name);
        }
        std::map<std::string, gx::shgr*> m_shgr_dict;

    private:
        gx::prog              prog0;
        gx::surf              test_surface0;

        QBasicTimer           timer;
        QOpenGLShaderProgram  program;
        QOpenGLTexture       *texture;
        QMatrix4x4            projection;
    };
}       // end namespace gx

#endif // GX_SRC_ES20_H
