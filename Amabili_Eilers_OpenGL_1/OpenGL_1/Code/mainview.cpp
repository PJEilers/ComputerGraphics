#include "mainview.h"
#include "math.h"
#include <array>


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
    glDeleteBuffers(1, vboMesh);
    glDeleteVertexArrays(1, vaoMesh);
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
    // glEnable(GL_CULL_FACE);

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

    vertex * mesh;
    mesh = loadModel(":/models/sphere.obj", mesh);

    //Transformations

    cubeModel.translate(QVector3D(-2,0,-6));
    pyramidModel.translate(QVector3D(2,0,-6));
    meshModel.translate(QVector3D(0,0,-10));
    meshModel.scale(6);
    projection.perspective(60, 1.4, 1, 50);

    //Initializing cube
    initializeCube(cube);

    //Initializing pyramid
    initializePyramid(pyramid);

    //Initializing mesh
    initializeMesh(mesh);

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

    //Getting locations of uniforms in the vertshader file

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

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, meshModel.data());
    glBindVertexArray(*vaoMesh);
    glDrawArrays(GL_TRIANGLES, 0,meshSize);



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
    cubeModel.setToIdentity();
    pyramidModel.setToIdentity();
    meshModel.setToIdentity();
    cubeModel.translate(QVector3D(-2,0,-6));
    pyramidModel.translate(QVector3D(2,0,-6));
    meshModel.translate(QVector3D(0,0,-10));
    meshModel.scale(6);

    //Rotating the cube around the x,y and z axis

    cubeModel.rotate(rotateX, 1 , 0, 0);
    cubeModel.rotate(rotateY, 0 , 1, 0);
    cubeModel.rotate(rotateZ, 0 , 0, 1);

    //Rotating the pyramid around the x,y and z axis

    pyramidModel.rotate(rotateX, 1 , 0, 0);
    pyramidModel.rotate(rotateY, 0 , 1, 0);
    pyramidModel.rotate(rotateZ, 0 , 0, 1);

    //Rotating the mesh around the x,y and z axis

    meshModel.rotate(rotateX, 1 , 0, 0);
    meshModel.rotate(rotateY, 0 , 1, 0);
    meshModel.rotate(rotateZ, 0 , 0, 1);

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
    float s = (float) scale/100;
    qDebug() << "Scale changed to " << s;
    scaling.setToIdentity();
    scaling.scale(s);
    update();

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

/**
 * @brief MainView::initializeMesh
 *
 * initialize the mesh vbo and vao
 */

void MainView::initializeMesh(vertex * mesh) {
    glGenBuffers(meshSize/3, vboMesh);
    glBindBuffer(GL_ARRAY_BUFFER, *vboMesh);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*meshSize, mesh, GL_STATIC_DRAW);
    glGenVertexArrays(meshSize/3, vaoMesh);
    glBindVertexArray(*vaoMesh);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));
}

/**
 * @brief MainView::loadModel
 *
 * loading in a model and filling an array of vertices.
 *
 * @param name
 * @param mesh
 * @return the array of vertices for the mesh
 */

vertex * MainView::loadModel(QString name, vertex * mesh) {
    Model sph(name);

    QVector<QVector3D> meshVertices = sph.getVertices();

    mesh = new vertex[meshVertices.size()];

    vaoMesh = new GLuint[meshVertices.size()/3];
    vboMesh = new GLuint[meshVertices.size()/3];

    meshSize = meshVertices.size();

    for(int i = 0; i < meshVertices.length(); i++) {
        mesh[i].x = meshVertices[i].x()/180;
        mesh[i].y = meshVertices[i].y()/180;
        mesh[i].z = meshVertices[i].z()/180;
        mesh[i].r = (float)rand() / RAND_MAX;
        mesh[i].g = (float)rand() / RAND_MAX;
        mesh[i].b = (float)rand() / RAND_MAX;
    }
    return mesh;
}
