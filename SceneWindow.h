#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QTimer>
#include <QBoxLayout>
#include "SceneWidget.h"

class SceneWindow: public QWidget
{
    public:


    // constructor / destructor
    SceneWindow(QWidget *parent);
    ~SceneWindow();

    // visual hierarchy
    // menu bar
    QMenuBar *menuBar;
        // file menu
        QMenu *fileMenu;
            // quit action
            QAction *actionQuit;

    // window layout
    QBoxLayout *windowLayout;

    // beneath that, the main widget
    SceneWidget *sceneWidget;
    // slider for the radius of circular path
    QSlider *radiusSlider;
    // slider for speed of metronome and piano playing
    QSlider *speedSlider;
    // slider for speed of rotation
    QSlider *rotationSlider;
    //combo box for piano material type
    QComboBox *materialCombo;
    //combo box for paper texture
    QComboBox *textureCombo;

    QTimer *pTimer;

    // resets all the interface elements
    void ResetInterface();


    private:


};

#endif // SCENEWINDOW_H
