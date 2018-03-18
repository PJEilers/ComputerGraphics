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
    debugLogger->stopLogging();

    qDebug() << "MainView destructor";

    glDeleteTextures(1, &meshProperties[0].texID);

    destroyModelBuffers();
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
    createInst();

    // Initialize transformations
    updateProjectionTransform();
    updateModelTransforms();

    timer.start(1000.0 / 60.0);

}

void MainView::createInst() {
    loadMeshes();
    loadTextures();
}

void MainView::createShaderProgram()
{
    // Create Normal Shader program
    normalShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_normal.glsl");
    normalShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_normal.glsl");
    normalShaderProgram.link();

    // Create Gouraud Shader program
    gouraudShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_gouraud.glsl");
    gouraudShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_gouraud.glsl");
    gouraudShaderProgram.link();

    // Create Phong Shader program
    phongShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_phong.glsl");
    phongShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_phong.glsl");
    phongShaderProgram.link();

    // Get the uniforms for the normal shader.
    uniformModelTransformNormal  = normalShaderProgram.uniformLocation("modelTransform");
    uniformViewTransformNormal       = normalShaderProgram.uniformLocation("viewTransform");
    uniformProjectionTransformNormal = normalShaderProgram.uniformLocation("projectionTransform");
    uniformNormalTransformNormal     = normalShaderProgram.uniformLocation("normalTransform");

    // Get the uniforms for the gouraud shader.
    uniformModelTransformGouraud  = gouraudShaderProgram.uniformLocation("modelTransform");
    uniformViewTransformGouraud       = gouraudShaderProgram.uniformLocation("viewTransform");
    uniformProjectionTransformGouraud = gouraudShaderProgram.uniformLocation("projectionTransform");
    uniformNormalTransformGouraud     = gouraudShaderProgram.uniformLocation("normalTransform");
    uniformMaterialGouraud            = gouraudShaderProgram.uniformLocation("material");
    uniformLightPositionGouraud       = gouraudShaderProgram.uniformLocation("lightPosition");
    uniformLightColourGouraud         = gouraudShaderProgram.uniformLocation("lightColour");
    uniformTextureSamplerGouraud      = gouraudShaderProgram.uniformLocation("textureSampler");

    // Get the uniforms for the phong shader.
    uniformModelTransformPhong  = phongShaderProgram.uniformLocation("modelTransform");
    uniformViewTransformPhong       = phongShaderProgram.uniformLocation("viewTransform");
    uniformProjectionTransformPhong = phongShaderProgram.uniformLocation("projectionTransform");
    uniformNormalTransformPhong     = phongShaderProgram.uniformLocation("normalTransform");
    uniformMaterialPhong            = phongShaderProgram.uniformLocation("material");
    uniformLightPositionPhong       = phongShaderProgram.uniformLocation("lightPosition");
    uniformLightColourPhong         = phongShaderProgram.uniformLocation("lightColour");
    uniformTextureSamplerPhong      = phongShaderProgram.uniformLocation("textureSampler");
}

void MainView::loadMeshes()
{
    int meshSize1 = loadMesh(":/models/cat.obj", meshProperties[0].vaoID);
    setProperties(QVector3D(2,-1.6,-4), QVector3D(0,0,0), 0.03, meshSize1, meshProperties[0]);
    int meshSize2 = loadMesh(":/models/cube.obj", meshProperties[1].vaoID);
    setProperties(QVector3D(-5,-4.5,-10), QVector3D(0,0,0), 0.06, meshSize2, meshProperties[1]);
    int meshSize3 = loadMesh(":/models/cat.obj", meshProperties[2].vaoID);
    setProperties(QVector3D(0,0,-8), QVector3D(0,270,0), 0.1, meshSize3, meshProperties[2]);
    int meshSize4 = loadMesh(":/models/cube.obj", meshProperties[3].vaoID);
    setProperties(QVector3D(-5,0,-8), QVector3D(0,0,0), 0, meshSize4, meshProperties[3]);
}

int MainView::loadMesh(QString filename, GLuint &meshVAO)
{
    Model model(filename);
    model.unitize();
    QVector<float> meshData = model.getVNTInterleaved();

    int meshSize = model.getVertices().size();

    // Generate VAO
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);

    // Generate VBO
    glGenBuffers(1, &meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    // Write the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(float), meshData.data(), GL_STATIC_DRAW);

    // Set vertex coordinates to location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Set vertex normals to location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set vertex texture coordinates to location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return meshSize;
}

void MainView::drawMesh(GLuint texturePtr, GLuint meshVAO, GLuint meshSize)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturePtr);

    glBindVertexArray(meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, meshSize);

}

void MainView::loadTextures()
{
    glGenTextures(1, &meshProperties[0].texID);
    loadTexture(":/textures/cat_diff.png", meshProperties[0].texID);
    glGenTextures(1, &meshProperties[1].texID);
    loadTexture(":/textures/brick.png", meshProperties[1].texID);
    glGenTextures(1, &meshProperties[2].texID);
    loadTexture(":/textures/cat_diff.png", meshProperties[2].texID);
    glGenTextures(1, &meshProperties[3].texID);
    loadTexture(":/textures/brick.png", meshProperties[3].texID);

}

void MainView::loadTexture(QString file, GLuint texturePtr)
{
    // Set texture parameters.
    glBindTexture(GL_TEXTURE_2D, texturePtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Push image data to texture.
    QImage image(file);
    QVector<quint8> imageData = imageToBytes(image);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
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
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateModelTransforms();

    // Choose the selected shader.
    QOpenGLShaderProgram *shaderProgram;
    switch (currentShader) {
    case NORMAL:
        shaderProgram = &normalShaderProgram;
        shaderProgram->bind();
        updateNormalUniforms();
        for(int i = 0; i < 4 ; i++) {
            glUniformMatrix4fv(uniformModelTransformNormal, 1, GL_FALSE, meshTransform[i].data());
            glUniformMatrix3fv(uniformNormalTransformNormal, 1, GL_FALSE, meshNormalTransform[i].data());
            drawMesh(meshProperties[i].texID, meshProperties[i].vaoID, meshProperties[i].meshSize);
        }
        break;
    case GOURAUD:
        shaderProgram = &gouraudShaderProgram;
        shaderProgram->bind();
        updateGouraudUniforms();
        for(int i = 0; i < 4  ; i++) {
            glUniformMatrix4fv(uniformModelTransformGouraud, 1, GL_FALSE, meshTransform[i].data());
            glUniformMatrix3fv(uniformNormalTransformGouraud, 1, GL_FALSE, meshNormalTransform[i].data());
            drawMesh(meshProperties[i].texID, meshProperties[i].vaoID, meshProperties[i].meshSize);
        }
        break;
    case PHONG:
        shaderProgram = &phongShaderProgram;
        shaderProgram->bind();
        updatePhongUniforms();
        for(int i = 0; i < 4 ; i++) {
            glUniformMatrix4fv(uniformModelTransformPhong, 1, GL_FALSE, meshTransform[i].data());
            glUniformMatrix3fv(uniformNormalTransformPhong, 1, GL_FALSE, meshNormalTransform[i].data());
            drawMesh(meshProperties[i].texID, meshProperties[i].vaoID, meshProperties[i].meshSize);
        }
        break;
    }

    shaderProgram->release();

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

void MainView::updateNormalUniforms()
{
    glUniformMatrix4fv(uniformProjectionTransformNormal, 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformViewTransformNormal, 1, GL_FALSE, viewTransform.data());
}

void MainView::updateGouraudUniforms()
{
    glUniformMatrix4fv(uniformProjectionTransformGouraud, 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformViewTransformGouraud, 1, GL_FALSE, viewTransform.data());

    glUniform4fv(uniformMaterialGouraud, 1, &material[0]);
    glUniform3fv(uniformLightPositionGouraud, 1, &lightPosition[0]);
    glUniform3fv(uniformLightColourGouraud, 1, &lightColour[0]);

    glUniform1i(uniformTextureSamplerGouraud, 0); // Redundant now, but useful when you have multiple textures.
}

void MainView::updatePhongUniforms()
{
    glUniformMatrix4fv(uniformProjectionTransformPhong, 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformViewTransformPhong, 1, GL_FALSE, viewTransform.data());

    glUniform4fv(uniformMaterialPhong, 1, &material[0]);
    glUniform3fv(uniformLightPositionPhong, 1, &lightPosition[0]);
    glUniform3fv(uniformLightColourPhong, 1, &lightColour[0]);

    glUniform1i(uniformTextureSamplerPhong, 0);
}

void MainView::updateProjectionTransform()
{
    float aspect_ratio = static_cast<float>(width()) / static_cast<float>(height());
    projectionTransform.setToIdentity();
    projectionTransform.perspective(60, aspect_ratio, 0.2, 20);
}

void MainView::updateViewTransform() {
    viewTransform.setToIdentity();
    viewTransform.translate(0,0,-10);
    viewTransform.rotate(QQuaternion::fromEulerAngles(rotation));
    viewTransform.translate(0,0,10);
}

void MainView::updateModelTransforms()
{
    updateFirstCat();

    updateFirstCube();

    updateSecondCat();

    updateSecondCube();

    update();
}

// --- OpenGL cleanup helpers

void MainView::destroyModelBuffers()
{
    glDeleteBuffers(1, &meshVBO);
    for(int i = 0; i < 4; i++) {
        glDeleteVertexArrays(1, &meshProperties[i].vaoID);
        glDeleteTextures(1, &meshProperties[i].texID);
    }

}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    rotation = { static_cast<float>(rotateX), static_cast<float>(rotateY), static_cast<float>(rotateZ) };
    updateViewTransform();
}

void MainView::setScale(int newScale)
{
    scale = static_cast<float>(newScale) / 100.f;
    updateProjectionTransform();
}

void MainView::setShadingMode(ShadingMode shading)
{
    qDebug() << "Changed shading to" << shading;
    currentShader = shading;
}

void MainView::updateFirstCat() {
    meshTransform[0].setToIdentity();

    meshProperties[0].rotation+= QVector3D(0,0,2);
    if(meshProperties[0].rotation.z() > 720) {
        meshProperties[0].rotation = QVector3D(0,0,0);
        meshProperties[0].speed = 0.03;
    }
    meshProperties[0].location += QVector3D(0,meshProperties[0].speed,0);
    meshProperties[0].speed -= 0.03/180.0;

    meshTransform[0].translate(meshProperties[0].location);
    meshTransform[0].rotate(QQuaternion::fromEulerAngles(meshProperties[0].rotation));

    meshNormalTransform[0] = meshTransform[0].normalMatrix();
}

void MainView::updateSecondCat() {
    if(waiting > 0) {
        waiting -=1;
    } else {
        meshProperties[2].location += QVector3D(0,0,meshProperties[2].speed);
        meshProperties[2].rotation += QVector3D(meshProperties[2].speed*30,0,meshProperties[2].speed*30);
        if(meshProperties[2].rotation.z() >= 180) {
            waiting = 300;
            meshProperties[2].speed = - meshProperties[2].speed;
        }
        if(meshProperties[2].rotation.z() == 0) {
            waiting = 300;
            meshProperties[2].speed = - meshProperties[2].speed;
        }
    }




    meshTransform[2].setToIdentity();
    meshTransform[2].translate(meshProperties[2].location);
    meshTransform[2].rotate(QQuaternion::fromEulerAngles(meshProperties[2].rotation));
    meshNormalTransform[2] = meshTransform[2].normalMatrix();
}

void MainView::updateFirstCube() {
    meshTransform[1].setToIdentity();

    if(meshProperties[1].rotation.z() == 180) {
        meshProperties[1].speed = 0.06;
        s = -s;
        q = -q;
    }
    if(meshProperties[1].rotation.z() == -1) {
        meshProperties[1].speed = 0.06;
        s = -s;
        q = -q;
    }

    meshProperties[1].rotation+= QVector3D(0,0,q);
    meshProperties[1].location += QVector3D(s,meshProperties[1].speed,0);
    meshProperties[1].speed -= 0.06/90.0;
    meshTransform[1].translate(meshProperties[1].location);
    meshTransform[1].rotate(QQuaternion::fromEulerAngles(meshProperties[1].rotation));
    meshNormalTransform[1] = meshTransform[1].normalMatrix();
}

void MainView::updateSecondCube() {
    meshProperties[3].location += QVector3D(0, meshProperties[3].speed, 0);
    meshProperties[3].speed-= 0.001;
    if(meshProperties[3].location.y() <= -3.3) {
        meshProperties[3].speed = -meshProperties[3].speed;
    }

    meshTransform[3].setToIdentity();
    meshTransform[3].translate(meshProperties[3].location);
    meshTransform[3].rotate(QQuaternion::fromEulerAngles(meshProperties[3].rotation));
    meshNormalTransform[3] = meshTransform[3].normalMatrix();
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

