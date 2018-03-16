#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <QMatrix4x4>
#include <memory>


class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    QOpenGLDebugLogger *debugLogger;
    QTimer timer; // timer used for animation

    QOpenGLShaderProgram shaderProgramNormal;
    QOpenGLShaderProgram shaderProgramGouraud;
    QOpenGLShaderProgram shaderProgramPhong;

    //Phong uniforms
    GLint uniformModelViewTransformPhong;
    GLint uniformProjectionTransformPhong;
    GLint uniformNormalMatrixPhong;
    GLint uniformLightPositionPhong;
    GLint uniformMaterialStatsPhong;
    GLint uniformMaterialColorPhong;
    GLint uniformSampler2DPhong;

    //Gouraud uniforms
    GLint uniformModelViewTransformGouraud;
    GLint uniformProjectionTransformGouraud;
    GLint uniformNormalMatrixGouraud;
    GLint uniformLightPositionGouraud;
    GLint uniformMaterialStatsGouraud;
    GLint uniformMaterialColorGouraud;
    GLint uniformSampler2DGouraud;

    //Normal uniforms
    GLint uniformModelViewTransformNormal;
    GLint uniformProjectionTransformNormal;
    GLint uniformNormalMatrixNormal;



    // Mesh values
    GLuint meshVAO;
    GLuint meshVBO;
    GLuint meshSize;
    GLuint tex;
    QMatrix4x4 meshTransform;
    QMatrix4x4 lightTransform;

    // Transforms
    float scale = 1.f;
    float t = 0;
    QVector3D rotation;
    QMatrix4x4 projectionTransform;



public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

    ShadingMode currentShading = PHONG; //Current shading mode used.

    MainView(QWidget *parent = 0);
    ~MainView();

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(ShadingMode shading);

    QVector<quint8> imageToBytes(QImage image);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

private:
    void createShaderPrograms();
    void loadMesh();
    void loadTexture(QString file);

    void destroyModelBuffers();

    void updateProjectionTransform();
    void updateModelTransforms();
};

#endif // MAINVIEW_H
