#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"
#include "vertex.h"
#include "transformations.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <memory>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    QOpenGLDebugLogger *debugLogger;
    QTimer timer; // timer used for animation

    QOpenGLShaderProgram shaderProgramNormal;
    QOpenGLShaderProgram shaderProgramPhong;
    QOpenGLShaderProgram shaderProgramGouraud;

public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

    ShadingMode currentShading = PHONG;

    GLuint vboCube[12];
    GLuint vaoCube[12];
    GLuint vboPyramid[6];
    GLuint vaoPyramid[6];
    GLuint * vboMesh;
    GLuint * vaoMesh;
    GLuint programId;
    GLint modelLoc;
    GLint projectionLoc;
    GLint normalLoc;

    int meshSize;

    transformations cubeTransformations;
    transformations pyramidTransformations;
    transformations meshTransformations;

    QMatrix4x4 projection;


    MainView(QWidget *parent = 0);
    ~MainView();

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(ShadingMode shading);

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
    void mouseReleaseEvent(QMouseEvent *evs);
    void wheelEvent(QWheelEvent *ev);
    void initializeCube();
    void initializePyramid();
    void initializeMesh(QString name, int scale);
    vertex *  loadModel(QString name, vertex *mesh);
private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

private:
    void createShaderProgram();


};

#endif // MAINVIEW_H
