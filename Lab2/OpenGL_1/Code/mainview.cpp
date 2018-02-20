#include "mainview.h"
#include "math.h"


#include <QDateTime>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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
    glDeleteBuffers(1, vboPyramid);
    glDeleteVertexArrays(1, vaoPyramid);
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

    //Creating cube

    vertex cube [36] = {

        //Front

        {1,1,1,0,0,1},
        {-1,-1,1,0,1,0},
        {1,-1,1,1,0,0},

        {-1,-1,1,0,1,0},
        {1,1,1,0,0,1},
        {-1,1,1,1,0,0},

        //Back

        {-1,-1,-1,1,0,0},
        {1,-1,-1,0,1,0},
        {1,1,-1,0,0,1},

        {-1,-1,-1,1,0,0},
        {1,1,-1,0,0,1},
        {-1,1,-1,0,1,0},

        //Top

        {-1,1,1,1,0,0},
        {1,1,-1,0,0,1},
        {1,1,1,0,1,0},

        {1,1,-1,0,0,1},
        {-1,1,1,1,0,0},
        {-1,1,-1,0,1,0},

        //Right

        {1,1,-1,0,0,1},
        {1,-1,1,1,0,0},
        {1,-1,-1,0,1,0},

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

    //Creating pyramid

    vertex pyramid[18] = {

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
        {0,1,0,0,0,1},

        //Right
        {1,-1,1,1,0,0},
        {1,-1,-1,0,1,0},
        {0,1,0,0,0,1},

        //Left
        {-1,-1,-1,0,1,0},
        {-1,-1,1,1,0,0},
        {0,1,0,0,0,1},
    };

    //Transformations

    cubeModel.translate(QVector3D(-2,0,-6));
    pyramidModel.translate(QVector3D(2,0,-6));
    projection.perspective(60, 1.4, 1, 50);

    //Initializing cube
    initializeCube(cube);

    //Initializing pyramid
    initializePyramid(pyramid);

}

void MainView::createShaderProgram()
{
    // Create shader program
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader.glsl");
    shaderProgram.link();

    programId = shaderProgram.programId();

    glUseProgram(programId);

    modelLoc = glGetUniformLocation(programId, "modelTransform");
    projectionLoc = glGetUniformLocation(programId, "projectTransform");
    scaleLoc = glGetUniformLocation(programId, "scaleTransform");

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


    //Applying transformations

    shaderProgram.bind();

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cubeModel.data());
    glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, scaling.data());

    // Drawing objects

    glBindVertexArray(*vaoCube);
    glDrawArrays(GL_TRIANGLES, 0,36);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, pyramidModel.data());
    glBindVertexArray(*vaoPyramid);

    glDrawArrays(GL_TRIANGLES, 0,18);

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
    float aspect = ((float) newWidth) / ((float) newHeight);
    projection.setToIdentity();
    projection.perspective(60, aspect, 1, 100);

}

/**
 * @brief MainView::setRotation
 *
 * Setting rotation matrix
 *
 * @param rotateX
 * @param rotateY
 * @param rotateZ
 */

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    if(rotateX == 0 && rotateY == 0 && rotateZ == 0) {
        cubeModel.setToIdentity();
        pyramidModel.setToIdentity();
        cubeModel.translate(QVector3D(-2,0,-6));
        pyramidModel.translate(QVector3D(2,0,-6));
    }
    //Rotating around x-axis

    float x = sin((float)rotateX*M_PI/180/2) *180/M_PI;;
    float w = cos((float) rotateX*M_PI/180/2) *180/M_PI;
    cubeModel.rotate(QQuaternion().fromAxisAndAngle(x,0,0,w));
    pyramidModel.rotate(QQuaternion().fromAxisAndAngle(x,0,0,w));

    //Rotating around y-axis

    float y = sin((float)rotateY*M_PI/180/2) *180/M_PI;;
    w = cos((float) rotateY*M_PI/180/2) *180/M_PI;
    cubeModel.rotate(QQuaternion().fromAxisAndAngle(0,y,0,w));
    pyramidModel.rotate(QQuaternion().fromAxisAndAngle(0,y,0,w));

    //Rotating around z-axis

    float z = (float)sin((float)rotateZ*M_PI/180/2) *180/M_PI;;
    w = (float)cos((float) rotateZ*M_PI/180/2) *180/M_PI;

    cubeModel.rotate(QQuaternion().fromAxisAndAngle(0,0,z,w));
    pyramidModel.rotate(QQuaternion().fromAxisAndAngle(0,0,z,w));

    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    update();

}

/**
 * @brief MainView::setScale
 *
 * Setting scaling matrix
 *
 * @param scale
 */

void MainView::setScale(int scale)
{
    if (scale < 140) {
        float s = (float) scale/100;
        qDebug() << "Scale changed to " << s;
        scaling.setToIdentity();
        scaling.scale(s,s,s);
        update();
    }

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

/**
 * @brief MainView::initializeCube
 *
 * initialize the cube vbo and vao
 */

void MainView::initializeCube (vertex * cube) {
    glGenBuffers(12, vboCube);
    glBindBuffer(GL_ARRAY_BUFFER, *vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*36, cube, GL_STATIC_DRAW);
    glGenVertexArrays(12, vaoCube);
    glBindVertexArray(*vaoCube);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));
}

/**
 * @brief MainView::initializePyramid
 *
 * initialize the pyramid vbo and vao
 */

void MainView::initializePyramid (vertex * pyramid) {
    glGenBuffers(6, vboPyramid);
    glBindBuffer(GL_ARRAY_BUFFER, *vboPyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*18, pyramid, GL_STATIC_DRAW);
    glGenVertexArrays(6, vaoPyramid);
    glBindVertexArray(*vaoPyramid);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));
}
