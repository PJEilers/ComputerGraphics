#include "mainview.h"
#include "math.h"
#include <QDateTime>



MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

MainView::~MainView() {
    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);
}

void MainView::resizeGL(int newWidth, int newHeight) {

    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

void MainView::paintGL(GLuint* VBO, GLuint* VAO) {


}

void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
