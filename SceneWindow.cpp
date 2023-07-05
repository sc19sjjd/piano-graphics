#include "SceneWindow.h"

// constructor / destructor
SceneWindow::SceneWindow(QWidget *parent)
    : QWidget(parent)
{ // constructor

    // create menu bar
    menuBar = new QMenuBar(this);

    // create file menu
    fileMenu = menuBar->addMenu("&File");

    // create the action
    actionQuit = new QAction("&Quit", this);

    // leave signals & slots to the controller

    // add the item to the menu
    fileMenu->addAction(actionQuit);

    // create the window layout
    windowLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    // create main widget
    sceneWidget = new SceneWidget(this);
    windowLayout->addWidget(sceneWidget);

    // create sliders
    radiusSlider = new QSlider(Qt::Horizontal);
    radiusSlider->setMaximum(500);
    QLabel *radiusLabel = new QLabel("Radius of rotation");
    radiusLabel->setMaximumHeight(15);

    rotationSlider = new QSlider(Qt::Horizontal);
    rotationSlider->setMaximum(500);
    QLabel *rotationLabel = new QLabel("Speed of rotation");
    rotationLabel->setMaximumHeight(15);

    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setMaximum(500);
    QLabel *speedLabel = new QLabel("Speed of piano playing and metronome");
    speedLabel->setMaximumHeight(15);

    //connect sliders and set initial values
    connect(radiusSlider, SIGNAL(valueChanged(int)), sceneWidget, SLOT(updateRadius(int)));
    connect(speedSlider, SIGNAL(valueChanged(int)), sceneWidget, SLOT(updateSpeed(int)));
    connect(rotationSlider, SIGNAL(valueChanged(int)), sceneWidget, SLOT(updateRotationRate(int)));
    radiusSlider->setValue(250);
    speedSlider->setValue(250);
    rotationSlider->setValue(250);

    //start and connect timer
    pTimer = new QTimer;
        pTimer->start(10);
        connect(pTimer, SIGNAL(timeout()),  sceneWidget, SLOT(updateTime()));

    // create and connect combo boxes
    materialCombo = new QComboBox();
    materialCombo->addItem("Light Wood");
    materialCombo->addItem("Dark Wood");
    materialCombo->addItem("Red Wood");
    materialCombo->addItem("Black Shiny");
    connect(materialCombo, SIGNAL(currentIndexChanged(int)), sceneWidget, SLOT(updateMaterial(int)));
    QLabel *materialLabel = new QLabel("Select piano material");
    materialLabel->setMaximumHeight(15);

    textureCombo = new QComboBox();
    textureCombo->addItem("Sheet Music");
    textureCombo->addItem("Mercator Projection");
    textureCombo->addItem("Marc Dekamps");
    textureCombo->addItem("Markus");
    connect(textureCombo, SIGNAL(currentIndexChanged(int)), sceneWidget, SLOT(updateTexture(int)));
    QLabel *textureLabel = new QLabel("Select paper texture");
    textureLabel->setMaximumHeight(15);


    windowLayout->addWidget(radiusLabel);
    windowLayout->addWidget(radiusSlider);
    windowLayout->addWidget(rotationLabel);
    windowLayout->addWidget(rotationSlider);
    windowLayout->addWidget(speedLabel);
    windowLayout->addWidget(speedSlider);
    windowLayout->addWidget(materialLabel);
    windowLayout->addWidget(materialCombo);
    windowLayout->addWidget(textureLabel);
    windowLayout->addWidget(textureCombo);
} // constructor

SceneWindow::~SceneWindow()
{ // destructor
    delete materialCombo;
    delete textureCombo;
    delete rotationSlider;
    delete speedSlider;
    delete radiusSlider;
    delete sceneWidget;
    delete windowLayout;
    delete actionQuit;
    delete fileMenu;
    delete menuBar;
} // destructor

// resets all the interface elements
void SceneWindow::ResetInterface()
{ // ResetInterface()
    rotationSlider->setMinimum(0);
    rotationSlider->setMaximum(500);
    rotationSlider->setValue(250);
    radiusSlider->setMinimum(0);
    radiusSlider->setMaximum(500);
    radiusSlider->setValue(250);
    speedSlider->setMinimum(0);
    speedSlider->setMaximum(500);
    speedSlider->setValue(250);

    // now force refresh
    sceneWidget->update();
    update();
} // ResetInterface()
