#include "mainwindow.h"
#include "vertex.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Request OpenGL 3.3 Core
    QSurfaceFormat glFormat;
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(3, 3);
    glFormat.setOption(QSurfaceFormat::DebugContext);

    // Some platforms need to explicitly set the depth buffer size (24 bits)
    glFormat.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(glFormat);
    QOpenGLShaderProgram program;

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    program.link();
    program.bind();

    vertex v1, v2, v3;
    v1.x = -1;
    v1.y = -1;
    v1.r = 1;
    v1.b = 0;
    v1.g = 0;
    v2.x = 1;
    v2.y = -1;
    v2.r = 0;
    v2.b = 1;
    v2.g = 0;
    v3.x = 0.5;
    v3.y = 1;
    v3.r = 0;
    v3.b = 0;
    v3.g = 0;
    vertex vArray [2];
    vArray[0] = v1;
    vArray [1] = v2;
    vArray[2] = v3;

    MainWindow w;
    w.show();

    return a.exec();
}
