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
     glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderProgram();

    projection.perspective(60, 1.4, 1, 100);

    //Initializing cube
    initializeCube();

    //Initializing pyramid
    initializePyramid();

    //Initializing mesh
    initializeMesh(":/models/sphere.obj");

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
    QMatrix4x4 cubeT;
    QMatrix4x4 pyramidT;
    QMatrix4x4 meshT;

    cubeT.translate(cubeTransformations.translateX, cubeTransformations.translateY, cubeTransformations.translateZ);
    pyramidT.translate(pyramidTransformations.translateX, pyramidTransformations.translateY, pyramidTransformations.translateZ);
    meshT.translate(meshTransformations.translateX, meshTransformations.translateY, meshTransformations.translateZ);

    cubeT.rotate(cubeTransformations.rotateX, 1,0,0);
    cubeT.rotate(cubeTransformations.rotateY, 0,1,0);
    cubeT.rotate(cubeTransformations.rotateZ, 0,0,1);

    pyramidT.rotate(pyramidTransformations.rotateX, 1,0,0);
    pyramidT.rotate(pyramidTransformations.rotateY, 0,1,0);
    pyramidT.rotate(pyramidTransformations.rotateZ, 0,0,1);

    meshT.rotate(meshTransformations.rotateX, 1,0,0);
    meshT.rotate(meshTransformations.rotateY, 0,1,0);
    meshT.rotate(meshTransformations.rotateZ, 0,0,1);

    cubeT.scale(cubeTransformations.scale);
    pyramidT.scale(pyramidTransformations.scale);
    meshT.scale(meshTransformations.scale);

    shaderProgram.bind();

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cubeT.data());


   // Drawing objects

    glBindVertexArray(*vaoCube);
    glDrawArrays(GL_TRIANGLES, 0,36);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, pyramidT.data());

    glBindVertexArray(*vaoPyramid);
    glDrawArrays(GL_TRIANGLES, 0,18);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, meshT.data());
    glBindVertexArray(*vaoMesh);
    glDrawArrays(GL_TRIANGLES, 0, meshSize);




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

    setRotate(rotateX, rotateY, rotateZ, cubeTransformations);
    setRotate(rotateX, rotateY, rotateZ, pyramidTransformations);
    setRotate(rotateX, rotateY, rotateZ, meshTransformations);


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
    cubeTransformations.scale = s;
    pyramidTransformations.scale = s;
    meshTransformations.scale = 6*s;
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

void MainView::initializeCube () {

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

        {1,-1,-1,0,1,0},
        {-1,-1,-1,1,0,0},  
        {1,1,-1,0,0,1},

        {1,1,-1,0,0,1},
        {-1,-1,-1,1,0,0},
        {-1,1,-1,0,1,0},

        //Top

        {1,1,-1,0,0,1},
        {-1,1,1,1,0,0},
        {1,1,1,0,1,0},

        {1,1,-1,0,0,1},  
        {-1,1,-1,0,1,0},
        {-1,1,1,1,0,0},

        //Right

        {1,1,-1,0,0,1},
        {1,-1,1,1,0,0},
        {1,-1,-1,0,1,0},

        {1,-1,1,1,0,0},
        {1,1,-1,0,0,1},
        {1,1,1,0,1,0},

        //Left

        {-1,-1,1,1,0,0},
        {-1,1,-1,0,0,1},
        {-1,-1,-1,0,1,0},

        {-1,1,-1,0,0,1},
        {-1,-1,1,1,0,0},      
        {-1,1,1,0,1,0},

        //Bottom

        {1,-1,1,0,1,0},
        {-1,-1,1,1,0,0},
        {1,-1,-1,0,0,1},

        {1,-1,-1,0,0,1},
        {-1,-1,1,1,0,0},      
        {-1,-1,-1,0,1,0},

    };

    glGenBuffers(1, vboCube);
    glBindBuffer(GL_ARRAY_BUFFER, *vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*36, &cube[0].x, GL_STATIC_DRAW);
    glGenVertexArrays(1, vaoCube);
    glBindVertexArray(*vaoCube);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));

    setTranslation(2,0,-6, cubeTransformations);
    setRotate(0,0,0,cubeTransformations);
    cubeTransformations.scale = 1;


}

/**
 * @brief MainView::initializePyramid
 *
 * initialize the pyramid vbo and vao
 */

void MainView::initializePyramid () {

    //Creating pyramid

    vertex pyramid[18] = {

        //Bottom
        {1,-1,1,0,1,0},
        {-1,-1,1,1,0,0},  
        {1,-1,-1,0,0,1},

        {1,-1,-1,0,0,1},
        {-1,-1,1,1,0,0},       
        {-1,-1,-1,0,1,0},

        //Front
        {-1,-1,1,1,0,0},
        {1,-1,1,0,1,0},
        {0,1,0,0,0,1},

        //Back
        {1,-1,-1,0,1,0},
        {-1,-1,-1,1,0,0},       
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

    glGenBuffers(1, vboPyramid);
    glBindBuffer(GL_ARRAY_BUFFER, *vboPyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*18, &pyramid[0].x, GL_STATIC_DRAW);
    glGenVertexArrays(1, vaoPyramid);
    glBindVertexArray(*vaoPyramid);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));

    setTranslation(-2,0,-6, pyramidTransformations);
    setRotate(0,0,0,pyramidTransformations);
    pyramidTransformations.scale = 1;
}

/**
 * @brief MainView::initializeMesh
 *
 * initialize the mesh vbo and vao
 */

void MainView::initializeMesh(QString name) {
    Model m(name);

    QVector<QVector3D> meshVertices = m.getVertices();

    vertex mesh[meshVertices.size()];

    meshSize = meshVertices.size();

    for(int i = 0; i < meshVertices.length(); i++) {
        mesh[i].x = meshVertices[i].x()/180;
        mesh[i].y = meshVertices[i].y()/180;
        mesh[i].z = meshVertices[i].z()/180;
        mesh[i].r = (float)rand() / RAND_MAX;
        mesh[i].g = (float)rand() / RAND_MAX;
        mesh[i].b = (float)rand() / RAND_MAX;
    }

    vaoMesh = new GLuint[meshVertices.size()/3];
    vboMesh = new GLuint[meshVertices.size()/3];

    glGenBuffers(1, vboMesh);
    glBindBuffer(GL_ARRAY_BUFFER, *vboMesh);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*meshSize, &mesh[0].x, GL_STATIC_DRAW);
    glGenVertexArrays(1, vaoMesh);
    glBindVertexArray(*vaoMesh);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));

    setTranslation(0,0,-10, meshTransformations);
    setRotate(0,0,0,meshTransformations);
    meshTransformations.scale = 6;

}

