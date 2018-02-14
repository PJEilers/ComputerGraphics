/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <mainview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *settingsBox;
    QFormLayout *formLayout;
    QGroupBox *scalingBox;
    QVBoxLayout *verticalLayout_4;
    QSlider *ScaleSlider;
    QPushButton *ResetScaleButton;
    QGroupBox *shadingBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *PhongButton;
    QRadioButton *NormalButton;
    QRadioButton *GouraudButton;
    QGroupBox *rotationBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QDial *RotationDialX;
    QDial *RotationDialY;
    QDial *RotationDialZ;
    QPushButton *ResetRotationButton;
    MainView *mainView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1048, 569);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        settingsBox = new QGroupBox(centralWidget);
        settingsBox->setObjectName(QStringLiteral("settingsBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(settingsBox->sizePolicy().hasHeightForWidth());
        settingsBox->setSizePolicy(sizePolicy);
        settingsBox->setMinimumSize(QSize(220, 0));
        settingsBox->setMaximumSize(QSize(220, 16777215));
        formLayout = new QFormLayout(settingsBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        scalingBox = new QGroupBox(settingsBox);
        scalingBox->setObjectName(QStringLiteral("scalingBox"));
        sizePolicy.setHeightForWidth(scalingBox->sizePolicy().hasHeightForWidth());
        scalingBox->setSizePolicy(sizePolicy);
        scalingBox->setMinimumSize(QSize(205, 0));
        scalingBox->setMaximumSize(QSize(205, 16777215));
        verticalLayout_4 = new QVBoxLayout(scalingBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        ScaleSlider = new QSlider(scalingBox);
        ScaleSlider->setObjectName(QStringLiteral("ScaleSlider"));
        ScaleSlider->setMinimum(1);
        ScaleSlider->setMaximum(200);
        ScaleSlider->setValue(100);
        ScaleSlider->setOrientation(Qt::Horizontal);
        ScaleSlider->setInvertedAppearance(false);
        ScaleSlider->setInvertedControls(false);
        ScaleSlider->setTickPosition(QSlider::NoTicks);

        verticalLayout_4->addWidget(ScaleSlider);

        ResetScaleButton = new QPushButton(scalingBox);
        ResetScaleButton->setObjectName(QStringLiteral("ResetScaleButton"));

        verticalLayout_4->addWidget(ResetScaleButton);


        formLayout->setWidget(1, QFormLayout::LabelRole, scalingBox);

        shadingBox = new QGroupBox(settingsBox);
        shadingBox->setObjectName(QStringLiteral("shadingBox"));
        sizePolicy.setHeightForWidth(shadingBox->sizePolicy().hasHeightForWidth());
        shadingBox->setSizePolicy(sizePolicy);
        shadingBox->setMinimumSize(QSize(205, 0));
        shadingBox->setMaximumSize(QSize(205, 16777215));
        verticalLayout_2 = new QVBoxLayout(shadingBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        PhongButton = new QRadioButton(shadingBox);
        PhongButton->setObjectName(QStringLiteral("PhongButton"));
        PhongButton->setChecked(true);

        verticalLayout_2->addWidget(PhongButton);

        NormalButton = new QRadioButton(shadingBox);
        NormalButton->setObjectName(QStringLiteral("NormalButton"));

        verticalLayout_2->addWidget(NormalButton);

        GouraudButton = new QRadioButton(shadingBox);
        GouraudButton->setObjectName(QStringLiteral("GouraudButton"));

        verticalLayout_2->addWidget(GouraudButton);


        formLayout->setWidget(2, QFormLayout::LabelRole, shadingBox);

        rotationBox = new QGroupBox(settingsBox);
        rotationBox->setObjectName(QStringLiteral("rotationBox"));
        sizePolicy.setHeightForWidth(rotationBox->sizePolicy().hasHeightForWidth());
        rotationBox->setSizePolicy(sizePolicy);
        rotationBox->setMinimumSize(QSize(205, 0));
        rotationBox->setMaximumSize(QSize(205, 16777215));
        verticalLayout = new QVBoxLayout(rotationBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        RotationDialX = new QDial(rotationBox);
        RotationDialX->setObjectName(QStringLiteral("RotationDialX"));
        RotationDialX->setMaximum(359);
        RotationDialX->setWrapping(true);
        RotationDialX->setNotchTarget(12);
        RotationDialX->setNotchesVisible(true);

        horizontalLayout_2->addWidget(RotationDialX);

        RotationDialY = new QDial(rotationBox);
        RotationDialY->setObjectName(QStringLiteral("RotationDialY"));
        RotationDialY->setMaximum(359);
        RotationDialY->setWrapping(true);
        RotationDialY->setNotchTarget(12);
        RotationDialY->setNotchesVisible(true);

        horizontalLayout_2->addWidget(RotationDialY);

        RotationDialZ = new QDial(rotationBox);
        RotationDialZ->setObjectName(QStringLiteral("RotationDialZ"));
        RotationDialZ->setMaximum(359);
        RotationDialZ->setWrapping(true);
        RotationDialZ->setNotchTarget(12);
        RotationDialZ->setNotchesVisible(true);

        horizontalLayout_2->addWidget(RotationDialZ);


        verticalLayout->addLayout(horizontalLayout_2);

        ResetRotationButton = new QPushButton(rotationBox);
        ResetRotationButton->setObjectName(QStringLiteral("ResetRotationButton"));

        verticalLayout->addWidget(ResetRotationButton);


        formLayout->setWidget(0, QFormLayout::LabelRole, rotationBox);


        horizontalLayout->addWidget(settingsBox);

        mainView = new MainView(centralWidget);
        mainView->setObjectName(QStringLiteral("mainView"));

        horizontalLayout->addWidget(mainView);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        settingsBox->setTitle(QApplication::translate("MainWindow", "Settings", 0));
        scalingBox->setTitle(QApplication::translate("MainWindow", "Scaling", 0));
        ResetScaleButton->setText(QApplication::translate("MainWindow", "Reset Scaling", 0));
        shadingBox->setTitle(QApplication::translate("MainWindow", "Shading", 0));
        PhongButton->setText(QApplication::translate("MainWindow", "Pho&ng", 0));
        NormalButton->setText(QApplication::translate("MainWindow", "Nor&mal", 0));
        GouraudButton->setText(QApplication::translate("MainWindow", "&Gouraud", 0));
        rotationBox->setTitle(QApplication::translate("MainWindow", "Rotation", 0));
#ifndef QT_NO_TOOLTIP
        RotationDialX->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Rotation along the X-axis</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        RotationDialY->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Rotation along the Y-axis</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        RotationDialZ->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Rotation along the Z-axis</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        ResetRotationButton->setText(QApplication::translate("MainWindow", "Reset Rotation", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
