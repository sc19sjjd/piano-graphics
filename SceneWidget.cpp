#include <GL/glu.h>
#include <QGLWidget>
#include <cmath>
#include "SceneWidget.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// material properties
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;

static materialStruct whiteShiny = {
    { 1.0, 1.0, 1.0, 1.0 },
    { 1.0, 1.0, 1.0, 1.0 },
    { 1.0, 1.0, 1.0, 1.0 },
    100.0
};

static materialStruct darkShiny = {
    { 0.1, 0.1, 0.1, 1.0 },
    { 0.1, 0.1, 0.1, 1.0 },
    { 0.7, 0.7, 0.7, 1.0 },
    60.0
};

static materialStruct lightWood = {
    { 0.45, 0.35, 0.05, 1.0 },
    { 0.45, 0.35, 0.05, 1.0 },
    { 0.65, 0.45, 0.25, 1.0 },
    30.0
};

static materialStruct darkWood = {
    { 0.20, 0.15, 0.05, 1.0 },
    { 0.20, 0.15, 0.05, 1.0 },
    { 0.35, 0.30, 0.10, 1.0 },
    30.0
};

static materialStruct redWood = {
    { 0.45, 0.20, 0.10, 1.0 },
    { 0.45, 0.20, 0.10, 1.0 },
    { 0.70, 0.35, 0.25, 1.0 },
    30.0
};

//used as material under textures
static materialStruct textureMaterial = {
    { 1.0, 1.0, 1.0, 0.0 },
    { 1.0, 1.0, 1.0, 0.0 },
    { 1.0, 1.0, 1.0, 1.0 },
    10.0
};

// constructor
SceneWidget::SceneWidget(QWidget *parent)
  : QGLWidget(parent),
    _currentKey(10),
    _previousKey(9),
    _time(0),
    _keyTime(0),
    _radius(4),
    _speed(25),
    _rotation(0),
    _rotationRate(1),
    _pianoMaterial(&lightWood),
    _currentTex(0)
{ // constructor

} // constructor

// called when OpenGL context is set up
void SceneWidget::initializeGL()
{ // initializeGL()
    // set the widget background colour
    glClearColor(0.3, 0.3, 0.3, 0.0);
    glOrtho(-4.,4.,-4.,4.,-4.,4);

    this->createTextures();

} // initializeGL()

// called every time the widget is resized
void SceneWidget::resizeGL(int w, int h)
{ // resizeGL()
    // set the viewport to the entire widget
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat light_pos[] = {2., -2.,-2., 1.};

    glEnable(GL_LIGHTING); // enable lighting in general
        glEnable(GL_LIGHT0);   // each light source must also be enabled


    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

} // resizeGL()


void SceneWidget::updateRadius(int i){
  _radius = double(i) / 100 + 1;
  this->repaint();
}

void SceneWidget::updateTime(){
  _time += 1.0;
  _keyTime += 1.0;
  _rotation += _rotationRate;
  this->repaint();
}

void SceneWidget::updateSpeed(int i) {
    int val = abs(i - 500);
    _speed = double(val) / 35 + 8;
}

void SceneWidget::updateRotationRate(int i) {
    _rotationRate = double(i) / 300;
}

void SceneWidget::updateMaterial(int i) {
    switch (i) {
        case 0:
            _pianoMaterial = &lightWood;
            break;
        case 1:
            _pianoMaterial = &darkWood;
            break;
        case 2:
            _pianoMaterial = &redWood;
            break;
        case 3:
            _pianoMaterial = &darkShiny;
            break;
    }
    this->repaint();
}

void SceneWidget::updateTexture(int i) {
    _currentTex = i;
    this->repaint();
}


void SceneWidget::createTextures() {
    loadTexture("sheet_music.png", 0);
    loadTexture("Mercator-projection.png", 1);
    loadTexture("Marc_Dekamps.png", 2);
    loadTexture("markus.png", 3);
}

void SceneWidget::loadTexture(char* fileName, int texID) {
    glGenTextures(1, &textures[texID]);
    glBindTexture(GL_TEXTURE_2D, textures[texID]);

    char path[150];
    strcpy(path, "../cwk2Scene/textures/");
    strcat(path, fileName);

    int w, h, nChannels;
    unsigned char* idata = stbi_load(path, &w, &h, &nChannels, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, idata);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    stbi_image_free(idata);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void SceneWidget::rectangle(const materialStruct* p_front) {
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);

    GLfloat normals[][3] = { { 1., 0. ,0.},
                             {-1., 0., 0.},
                             {0., 0.,  1.},
                             {0., 0., -1.},
                             {0.,  1., 0.},
                             {0., -1., 0.} };

    glNormal3fv(normals[0]);
    glBegin(GL_POLYGON);
        glVertex3f( 1.0, -1.0,  1.0);
        glVertex3f( 1.0, -1.0, -1.0);
        glVertex3f( 1.0,  1.0, -1.0);
        glVertex3f( 1.0,  1.0,  1.0);
    glEnd();

    glNormal3fv(normals[3]);
    glBegin(GL_POLYGON);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f( 1.0, -1.0, -1.0);
        glVertex3f( 1.0,  1.0, -1.0);
        glVertex3f(-1.0,  1.0, -1.0);
    glEnd();

    glNormal3fv(normals[2]);
    glBegin(GL_POLYGON);
        glVertex3f(-1.0, -1.0, 1.0);
        glVertex3f( 1.0, -1.0, 1.0);
        glVertex3f( 1.0,  1.0, 1.0);
        glVertex3f(-1.0,  1.0, 1.0);
    glEnd();

    glNormal3fv(normals[1]);
    glBegin(GL_POLYGON);
        glVertex3f( -1.0, -1.0,  1.0);
        glVertex3f( -1.0, -1.0, -1.0);
        glVertex3f( -1.0,  1.0, -1.0);
        glVertex3f( -1.0,  1.0,  1.0);
    glEnd();

    glNormal3fv(normals[4]);
    glBegin(GL_POLYGON);
        glVertex3f(-1.0, -1.0,  1.0);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f( 1.0, -1.0, -1.0);
        glVertex3f( 1.0, -1.0,  1.0);
    glEnd();

    glNormal3fv(normals[5]);
    glBegin(GL_POLYGON);
        glVertex3f(-1.0,  1.0,  1.0);
        glVertex3f(-1.0,  1.0, -1.0);
        glVertex3f( 1.0,  1.0, -1.0);
        glVertex3f( 1.0,  1.0,  1.0);
    glEnd();
}

void SceneWidget::keyWhite(double angle) {
    glPushMatrix();
    glTranslatef(0., 0., 1.2);
    glRotatef(angle, 1., 0., 0.);
    glTranslatef(0., 0., -1.2);
    glScalef(0.14, 0.18, 1.2);
    this->rectangle(&whiteShiny);
    glPopMatrix();
}

void SceneWidget::keyBlack() {
    glPushMatrix();
    glScalef(0.1, 0.28, 1);
    this->rectangle(&darkShiny);
    glPopMatrix();
}

void SceneWidget::pianoBase() {
    //back of piano
    glScalef(4., 3., 1.2);
    this->rectangle(_pianoMaterial);
    glPopMatrix();
    glPushMatrix();
    //sides of front
    glTranslatef(3.8, -1., -2.1);
    glScalef(0.2, 2, 1);
    this->rectangle(_pianoMaterial);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-3.8, -1., -2.1);
    glScalef(0.2, 2, 1);
    this->rectangle(_pianoMaterial);
    glPopMatrix();
    glPushMatrix();
    //bottom of the keys
    glTranslatef(0., 0.2, -2.1);
    glScalef(3.8, 0.12, 0.95);
    this->rectangle(_pianoMaterial);
    glPopMatrix();
    glPushMatrix();
    //paper stand
    glTranslatef(0., 1.3, -1.3);
    glScalef(2.5, 0.05, 0.2);
    this->rectangle(_pianoMaterial);
    glPopMatrix();
}

void SceneWidget::pianoKeys() {

    constexpr double pi = 3.14159265358979323846;
    //speed is twice as fast as the metronome
    double speed = _speed / 2;

    //calculate the angle of the key rotation
    double angleDown = -3 * (((cos(_keyTime / speed + pi)) + 1) / 2);
    double angleUp = -3 * (((cos(_keyTime / speed)) + 1) / 2);

    //check to move on to next key when current key is fully down
    if (angleDown <= -2.9) {
        if (_currentKey != 23) {
            _currentKey++;
            _previousKey = _currentKey - 1;
        } else {
            _currentKey = 0;
            _previousKey = 23;
        }
        //keyTime is reset to prevent switching keys again
        _keyTime = 0;
    }

    //about half the number of keys than a full grand piano
    //white keys
    glTranslatef(3.475, 0.45, -1.8);
    for (int i = 0; i < 24; i++) {
        if (i == _previousKey) {
            this->keyWhite(angleUp);
        } else if (i == _currentKey) {
            this->keyWhite(angleDown);
        } else {
            this->keyWhite(0.0);
        }
        glTranslatef(-0.3, 0., 0.);
    }
    glPopMatrix();
    glPushMatrix();
    //black keys
    glTranslatef(3.325, 0.5, -1.5);
    this->keyBlack();
    glTranslatef(-0.3, 0., 0.);
    for (int i = 0; i < 3; i++) {
        glTranslatef(-0.3, 0., 0.);
        for (int j = 0; j < 2; j++) {
            this->keyBlack();
            glTranslatef(-0.3, 0., 0.);
        }
        glTranslatef(-0.3, 0., 0.);
        for (int j = 0; j < 3; j++) {
            this->keyBlack();
            glTranslatef(-0.3, 0., 0.);
        }
    }
    glPopMatrix();
}

void SceneWidget::paper() {
    materialStruct *p_front = &textureMaterial;
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[_currentTex]);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glNormal3f(0., 0., 1.);
    glBegin(GL_POLYGON);
        glTexCoord2f(1., 1.);
        glVertex3f(0., 0., 0.);
        glTexCoord2f(0., 1.);
        glVertex3f(1., 0., 0.);
        glTexCoord2f(0., 0.);
        glVertex3f(1., 1.5, 0.);
        glTexCoord2f(1., 0.);
        glVertex3f(0., 1.5, 0.);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneWidget::metronome(double time) {
    glPushMatrix();
    //base of the metronome
    glScalef(0.4, 0.6, 0.3);
    this->rectangle(&darkShiny);
    glPopMatrix();
    //arm of the metronome, angle is based on sine function
    glRotatef(sin(time / _speed) * 35, 0., 0., 1.);
    glTranslatef(0., 0.3, -0.35);
    glScalef(0.08, 0.75, 0.05);
    this->rectangle(&lightWood);
    glPopMatrix();
}

void SceneWidget::pianoFull(double time) {
    glRotatef(90, 0., 1., 0.);
    glPushMatrix();
    this->pianoBase();
    glPopMatrix();
    glPushMatrix();
    this->pianoKeys();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2, 3.6, 0);
    this->metronome(time);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.1, 1.3, -1.21);
    glScalef(1.2, 1.2, 0);
    this->paper();
    glTranslatef(-1.2, 0., 0.);
    this->paper();
    glPopMatrix();
    glPopMatrix();
}


void SceneWidget::paintGL()
{ // paintGL()
    // clear the widget
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_NORMALIZE);
        glShadeModel(GL_FLAT);

    // You must set the matrix mode to model view directly before enabling the depth test
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);

    glLoadIdentity();
        gluLookAt(2.,1,1., 0.0,0.0,0.0, 0.0,1.0,0.0);

    glRotatef(_rotation,0.,1.,0.);
    glTranslatef(_radius,0.,0.);
    this->pianoFull(_time);

    // flush to screen
    glFlush();

} // paintGL()
