#include <gx_src_es20.h>

// EXAMPLE: HOW TO USE VISITORS:
//
//#include <QApplication>
//#include <QLabel>
//#include <QSurfaceFormat>
//#ifndef QT_NO_OPENGL
//#include <gx_src_es20.h>
//#endif
//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);
//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    QSurfaceFormat::setDefaultFormat(format);
//    app.setApplicationName("TEST VISITORS");
//    app.setApplicationVersion("0.1");
//#ifndef QT_NO_OPENGL
//    gx::root gl_widget;  // can be replaced with gx::gap_generated widget
//    gl_widget.show();
//#else
//    QLabel note("OpenGL Support required");
//    note.show();
//#endif
//    gl_widget.get_prog()->qdebug_active_variables();  // TEST INVOCATOR
//    return app.exec();
// 1. Get program pointer:  get_prog("program name")
//    gx::prog* p_prog = gl_widget.get_prog();
// 2. Print program variables
//    p_prog->qdebug_active_variables();
// 3. Get surface:
//    gx::shgr test;
//    struct test_surf_t : gx::surf
//    {
//    } test_surf;
//    test.mp_prog = p_prog;
//    test.mp_data = &test_surf;  // default variables data
//    add shading group to draw-loop;
//    gl_widget.add_shgr("test", &test);
//    qDebug() << "Activated shading-groups:";
//    for(auto active_shgr: gl_widget.m_shgr_dict) {
//        qDebug() << active_shgr.first.c_str() << (void*)active_shgr.second;
//    }
//    return app.exec();
//}

void gx::es20::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    qDebug() << "Activated shading-groups:" << m_shgr_dict.size();

    for(auto view_pair: m_shgr_dict) {
        qDebug() << view_pair.first.c_str() << (void*)view_pair.second;
        gx::shgr* curr = view_pair.second;
        curr->bind();
    }
    qDebug() << "--Activated shading-groups\n";
}
