#include <QtDebug>
#include <iostream>

#include "My/OpenGLResolver.h"

#include "Window.h"

int main(int argc, char * argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat::setDefaultFormat(My::OpenGLResolver::format());

    int exit_code{0};
    try
    {
        Window window;
#ifdef Q_OS_ANDROID
        window.showFullScreen();
#else
        window.showMaximized();
#endif
        exit_code = app.exec();
    }
    catch (std::exception & e)
    {
        qDebug() << e.what();
    }

    return exit_code;
}
