#pragma once

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QOpenGLContext>
#ifndef _NDEBUG
#include <QOpenGLDebugLogger>
#endif

#include <exception>
#include <stdexcept>

namespace My
{

/**
 * @brief The OpenGLResolver class
 *
 * Class handles the basic platform dependant code between mobile and desktop devices.
 *
 * @author Ronja Schnur (rschnur@students.uni-mainz.de)
 */
class OpenGLResolver
{
public:
    OpenGLResolver() = delete;

    // Static
public:
    /**
     * @brief load
     * @param path
     * @return Versioned shader source code for available OpenGL API
     */
    static QByteArray load(const QString & path)
    {
        QByteArray versioned_src;

        if (QOpenGLContext::currentContext()->isOpenGLES())
            versioned_src.append(QByteArrayLiteral("#version 320 es\n"));
        else
            versioned_src.append(QByteArrayLiteral("#version 330\n"));

        QFile file(path);
        if (!file.exists()) throw std::runtime_error("ShaderLoader: File not found.");

        file.open(QFile::ReadOnly);
        versioned_src.append(file.readAll());

        return versioned_src;
    }

    /**
     * @brief context_format
     * @return QSurfaceFormat specified for available API
     */
    static QSurfaceFormat format()
    {
        QSurfaceFormat fmt;
        fmt.setDepthBufferSize(24);

        if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
        {
            fmt.setVersion(3, 3); // OpenGL 3.3 core
            fmt.setProfile(QSurfaceFormat::CoreProfile);
        }
        else
        {
            fmt.setVersion(3, 2); // OpenGL 3.2 es
        }
#ifndef _NDEBUG
        fmt.setOption(QSurfaceFormat::DebugContext);
#endif
        return fmt;
    }
};

} // namespace My
