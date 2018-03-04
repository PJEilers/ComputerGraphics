#include "mainview.h"
#include "model.h"
#include "vertex.h"

#include <math.h>
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
    makeCurrent();
    debugLogger->stopLogging();
    delete debugLogger;

    qDebug() << "MainView destructor";
    destroyModelBuffers();
    doneCurrent();
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0, 1.0, 0.0, 1.0);

    createShaderProgram();
    loadMesh();

    // Initialize transformations
    updateProjectionTransform();
    updateModelTransforms();
}

void MainView::createShaderProgram()
{
    // Create shader program
    shaderProgramNormal.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/shaders/vertshader_normal.glsl");
    shaderProgramNormal.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/shaders/fragshader_normal.glsl");

    shaderProgramGouraud.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/shaders/vertshader_gouraud.glsl");
    shaderProgramGouraud.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/shaders/fragshader_gouraud.glsl");

    shaderProgramPhong.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/shaders/vertshader_phong.glsl");
    shaderProgramPhong.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/shaders/fragshader_phong.glsl");
    shaderProgramPhong.link();

    // Get the uniforms
    uniformModelViewTransformPhong = shaderProgramPhong.uniformLocation("modelViewTransform");
    uniformProjectionTransformPhong = shaderProgramPhong.uniformLocation("projectionTransform");
    uniformNormalMatrixPhong = shaderProgramPhong.uniformLocation("normalMatrix");
    uniformLightPositionPhong = shaderProgramPhong.uniformLocation("lightPosition");
    uniformMaterialColorPhong = shaderProgramPhong.uniformLocation("materialColor");
    uniformMaterialStatsPhong = shaderProgramPhong.uniformLocation("materialStats");
    uniformSampler2DPhong = shaderProgramGouraud.uniformLocation("s2d");

    shaderProgramGouraud.link();

    uniformModelViewTransformGouraud = shaderProgramGouraud.uniformLocation("modelViewTransform");
    uniformProjectionTransformGouraud = shaderProgramGouraud.uniformLocation("projectionTransform");
    uniformNormalMatrixGouraud = shaderProgramGouraud.uniformLocation("normalMatrix");
    uniformLightPositionGouraud = shaderProgramGouraud.uniformLocation("lightPosition");
    uniformMaterialColorGouraud = shaderProgramGouraud.uniformLocation("materialColor");
    uniformMaterialStatsGouraud = shaderProgramGouraud.uniformLocation("materialStats");
    uniformSampler2DGouraud = shaderProgramGouraud.uniformLocation("s2d");

    shaderProgramNormal.link();

    uniformModelViewTransformNormal = shaderProgramNormal.uniformLocation("modelViewTransform");
    uniformProjectionTransformNormal = shaderProgramNormal.uniformLocation("projectionTransform");
    uniformNormalMatrixNormal = shaderProgramNormal.uniformLocation("normalMatrix");
}

void MainView::loadMesh()
{
    Model model(":/models/cat.obj");
    QVector<QVector3D> vertexCoords = model.getVertices();

    QVector<float> meshData;
    meshData.reserve(2 * 3 * vertexCoords.size());

    QVector<QVector3D> vertexNormals = model.getNormals();
    QVector<QVector2D> textureCoords = model.getTextureCoords();
    int i = 0;
    for (auto coord : vertexCoords)
    {
        meshData.append(coord.x());
        meshData.append(coord.y());
        meshData.append(coord.z());
        meshData.append(vertexNormals[i].x());
        meshData.append(vertexNormals[i].y());
        meshData.append(vertexNormals[i].z());
        //meshData.append(textureCoords[i].x());
       // meshData.append(textureCoords[i].y());
        i++;
    }

    meshSize = vertexCoords.size();

    // Generate VAO
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);

    // Generate VBO
    glGenBuffers(1, &meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    // Write the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(float), meshData.data(), GL_STATIC_DRAW);

    // Set vertex coordinates to location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Set colour coordinates to location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void MainView::loadTexture(QString file) {
    QVector<quint8> pixelData = imageToBytes(":/textures/cat_diff.png");
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.data());
    // glGenerateMipmap(GL_TEXTURE_2D)
    return texture;
}


QVector<QVector2D> Model::getTextureCoords(){}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    // Clear the screen before rendering
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    QMatrix3x3 normalMatrix = meshTransform.normalMatrix();
    QVector3D lightPosition(10,10,10);
    lightPosition = lightTransform * lightPosition;
    QVector3D materialColor(0.75,0.75,0.75);
    QVector4D materialStats(0.2,0.7,0.5,1.0);


    if(currentShading == PHONG) {
        shaderProgramPhong.bind();
        // Set the projection matrix
        glUniformMatrix4fv(uniformProjectionTransformPhong, 1, GL_FALSE, projectionTransform.data());
        glUniformMatrix4fv(uniformModelViewTransformPhong, 1, GL_FALSE, meshTransform.data());
        glUniformMatrix3fv(uniformNormalMatrixPhong, 1 , GL_FALSE, normalMatrix.data());
        glUniform3f(uniformLightPositionPhong, lightPosition.x(), lightPosition.y(), lightPosition.z());
        glUniform3f(uniformMaterialColorPhong, materialColor.x(), materialColor.y(), materialColor.z());
        glUniform4f(uniformMaterialStatsPhong, materialStats.x(), materialStats.y(), materialStats.z(), materialStats.w());

        glBindVertexArray(meshVAO);
        glDrawArrays(GL_TRIANGLES, 0, meshSize);

        shaderProgramPhong.release();
    } else if (currentShading == GOURAUD) {
        shaderProgramGouraud.bind();
        // Set the projection matrix
        glUniformMatrix4fv(uniformProjectionTransformGouraud, 1, GL_FALSE, projectionTransform.data());
        glUniformMatrix4fv(uniformModelViewTransformGouraud, 1, GL_FALSE, meshTransform.data());
        glUniformMatrix3fv(uniformNormalMatrixGouraud, 1 , GL_FALSE, normalMatrix.data());
        glUniform3f(uniformLightPositionGouraud, lightPosition.x(), lightPosition.y(), lightPosition.z());
        glUniform3f(uniformMaterialColorGouraud, materialColor.x(), materialColor.y(), materialColor.z());
        glUniform4f(uniformMaterialStatsGouraud, materialStats.x(), materialStats.y(), materialStats.z(), materialStats.w());

        glBindVertexArray(meshVAO);
        glDrawArrays(GL_TRIANGLES, 0, meshSize);

        shaderProgramGouraud.release();
    } else {
        shaderProgramNormal.bind();
        glUniformMatrix4fv(uniformProjectionTransformNormal, 1, GL_FALSE, projectionTransform.data());
        glUniformMatrix4fv(uniformModelViewTransformNormal, 1, GL_FALSE, meshTransform.data());
        glUniformMatrix3fv(uniformNormalMatrixNormal, 1 , GL_FALSE, normalMatrix.data());

        glBindVertexArray(meshVAO);
        glDrawArrays(GL_TRIANGLES, 0, meshSize);

        shaderProgramNormal.release();
    }

    loadTexture(":/textures/cat_diff.png");
    texture;


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
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
    updateProjectionTransform();
}

void MainView::updateProjectionTransform()
{
    float aspect_ratio = static_cast<float>(width()) / static_cast<float>(height());
    projectionTransform.setToIdentity();
    projectionTransform.perspective(60, aspect_ratio, 0.2, 20);
}

void MainView::updateModelTransforms()
{
    meshTransform.setToIdentity();
    meshTransform.translate(0, 0, -10);
    meshTransform.scale(scale*5);
    meshTransform.rotate(QQuaternion::fromEulerAngles(rotation));

    lightTransform.setToIdentity();
    lightTransform.translate(0,0,-10);
    lightTransform.scale(scale);
    lightTransform.rotate(QQuaternion::fromEulerAngles(rotation));
    lightTransform.translate(0,0,0);

    update();
}

// --- OpenGL cleanup helpers

void MainView::destroyModelBuffers()
{
    glDeleteBuffers(1, &meshVBO);
    glDeleteVertexArrays(1, &meshVAO);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    rotation = { static_cast<float>(rotateX), static_cast<float>(rotateY), static_cast<float>(rotateZ) };
    updateModelTransforms();
}

void MainView::setScale(int newScale)
{
    scale = static_cast<float>(newScale) / 100.f;
    updateModelTransforms();
}

void MainView::setShadingMode(ShadingMode shading)
{
    qDebug() << "Changed shading to" << shading;
    currentShading = shading;
    update();
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

