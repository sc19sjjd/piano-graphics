#include <QApplication>
#include <QVBoxLayout>
#include "SceneWindow.h"

int main(int argc, char *argv[])
{ // main()
    // create the application
    QApplication app(argc, argv);

    // create a master widget
        SceneWindow *window = new SceneWindow(NULL);

    // resize the window
    window->resize(650, 900);

    // show the window
    window->show();

    // start it running
    app.exec();

    // clean up
    //	delete controller;
    delete window;

    // return to caller
    return 0;

} // main()

