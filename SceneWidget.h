#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QGLWidget>

struct materialStruct;

class SceneWidget: public QGLWidget
{

    Q_OBJECT

    public:
    SceneWidget(QWidget *parent);

    public slots:
    void updateTime();
    void updateRadius(int);
    void updateSpeed(int);
    void updateRotationRate(int);
    void updateMaterial(int);
    void updateTexture(int);

    protected:
    // called when OpenGL context is set up
    void initializeGL();
    // called every time the widget is resized
    void resizeGL(int w, int h);
    // called every time the widget needs painting
    void paintGL();

    private:
    void createTextures();
    void loadTexture(char*, int);
    void rectangle(const materialStruct*);
    void keyWhite(double);
    void keyBlack();
    void pianoBase();
    void pianoKeys();
    void paper();
    void metronome(double);
    void pianoFull(double);

    //direction of key pressing, true = up, false = down
    unsigned int textures[4];
    int _currentTex;
    int _currentKey;
    int _previousKey;
    double _time;
    double _keyTime;
    double _radius;
    double _speed;
    double _rotation;
    double _rotationRate;
    materialStruct *_pianoMaterial;

}; // class SceneWidget

#endif // SCENEWIDGET_H
