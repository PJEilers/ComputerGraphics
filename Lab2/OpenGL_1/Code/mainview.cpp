#include "mainview.h"
#include "math.h"
#include "vertex.h"

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    debugLogger->stopLogging();
    glDeleteBuffers(1, vboCube);
    glDeleteVertexArrays(1, vaoCube);
    qDebug() << "MainView destructor";
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
             this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderProgram();


    vertex cube [36] = {



                            {1,1,1,1,0,0},
                            {1,-1,-1,0,1,0},
                            {1,1,-1,0,0,1},

                            {-1,-1,-1,1,0,0},
                            {1,-1,-1,0,1,0},
                            {1,1,-1,0,0,1},

                            {-1,-1,-1,1,0,0},
                            {1,-1,-1,0,1,0},
                            {1,1,-1,0,0,1},

                            {-1,-1,-1,1,0,0},
                            {-1,1,-1,0,1,0},
                            {1,1,-1,0,0,1},

                            {-1,-1,-1,1,0,0},
                            {-1,1,1,0,1,0},
                            {-1,1,-1,0,1,0},

                            {-1,-1,1,1,0,0},
                            {-1,-1,-1,0,1,0},
                            {1,-1,-1,0,0,1},
        {1,1,1,1,0,0},
        {-1,1,1,0,1,0},
        {1,-1,1,0,0,1},

        {-1,-1,1,1,0,0},
        {-1,1,1,0,1,0},
        {1,-1,1,0,0,1},


            {-1,-1,-1,1,0,0},
            {-1,-1,1,0,1,0},
            {1,-1,1,0,0,1},


                            {-1,-1,1,1,0,0},
                            {-1,1,1,0,1,0},
                            {-1,-1,-1,0,0,1},



                            {1,1,1,1,0,0},
                            {1,-1,-1,0,1,0},
                            {1,-1,1,0,0,1},

                            {-1,-1,-1,1,0,0},
                            {1,-1,-1,0,1,0},
                            {1,-1,1,0,0,1},

                            };

    glGenBuffers(12, vboCube);
    glGenVertexArrays(12, vaoCube);
    glBindVertexArray(*vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, *vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*36, &cube[0].x, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    #define BUFFER_OFFSET(i) ((char *)NULL + (i))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));
}

void MainView::createShaderProgram()
{
    // Create shader program
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader.glsl");
    shaderProgram.link();
}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    shaderProgram.bind();

    // Draw here
    glDrawArrays(GL_TRIANGLES, 0,36);

    shaderProgram.release();
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) 
{
    // TODO: Update projection to fit the new aspect ratio
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    Q_UNIMPLEMENTED();
}

void MainView::setScale(int scale)
{
    qDebug() << "Scale changed to " << scale;
    Q_UNIMPLEMENTED();
}

void MainView::setShadingMode(ShadingMode shading)
{
    qDebug() << "Changed shading to" << shading;
    Q_UNIMPLEMENTED();
}

// --- Private helpers

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
