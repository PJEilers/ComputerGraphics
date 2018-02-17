#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"
#include "vertex.h"

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

    QOpenGLShaderProgram shaderProgram;

public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

    GLuint vboCube[12];
    GLuint vaoCube[12];
    GLuint vboPyramid[6];
    GLuint vaoPyramid[6];

    vertex cube [36] = {

        //Back

        {-1,-1,1,1,0,0},
        {1,-1,1,0,1,0},
        {1,0,1,0,0,1},


        {-1,-1,1,1,0,0},
        {1,1,1,0,0,1},
        {-1,1,1,0,1,0},

        //Front

        {-1,-1,-1,1,0,0},
        {1,-1,-1,0,1,0},
        {1,1,-1,0,0,1},


        {-1,-1,-1,1,0,0},
        {1,1,-1,0,0,1},
        {-1,1,-1,0,1,0},

        //Top

        {-1,1,1,1,0,0},
        {1,1,1,0,1,0},
        {1,1,-1,0,0,1},

        {-1,1,1,1,0,0},
        {1,1,-1,0,0,1},
        {-1,1,-1,0,1,0},

        //Right

        {1,-1,1,1,0,0},
        {1,-1,-1,0,1,0},
        {1,1,-1,0,0,1},

        {1,-1,1,1,0,0},
        {1,1,-1,0,0,1},
        {1,1,1,0,1,0},

        //Left

        {-1,-1,1,1,0,0},
        {-1,-1,-1,0,1,0},
        {-1,1,-1,0,0,1},

        {-1,-1,1,1,0,0},
        {-1,1,-1,0,0,1},
        {-1,1,1,0,1,0},

        //Bottom

        {-1,-1,1,1,0,0},
        {1,-1,1,0,1,0},
        {1,-1,-1,0,0,1},

        {-1,-1,1,1,0,0},
        {1,-1,-1,0,0,1},
        {-1,-1,-1,0,1,0},

    };

    vertex pyramid[18] = {

        /**/

        //Bottom
        {-1,-1,1,1,0,0},
        {1,-1,1,0,1,0},
        {1,-1,-1,0,0,1},

        {-1,-1,1,1,0,0},
        {1,-1,-1,0,0,1},
        {-1,-1,-1,0,1,0},

        //Back
        {-1,-1,1,1,0,0},
        {1,-1,1,0,1,0},
        {0,1,0,0,0,1},

        //Front
        {-1,-1,-1,1,0,0},
        {1,-1,-1,0,1,0},
        {0,1,-1,0,0,1}, //z-coordinate should be 0, but I changed it so we can see both the cube and pyramid

        //Left
        {1,-1,1,1,0,0},
        {1,-1,-1,0,1,0},
        {0,1,0,0,0,1},

        //Right
        {-1,-1,1,1,0,0},
        {-1,-1,-1,0,1,0},
        {0,1,0,0,0,1},
    };


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
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

private:
    void createShaderProgram();

};

#endif // MAINVIEW_H
