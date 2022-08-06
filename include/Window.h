#pragma once
#include <memory>
#include <optional>
#include <tuple>

#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>
#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QQuaternion>
#include <QSequentialAnimationGroup>
#include <QSurfaceFormat>
#include <QTimer>
#include <QVector3D>
#include <QWheelEvent>

#if defined(Q_OS_ANDROID) && USE_SENSORS
#include <QRotationReading>
#include <QRotationSensor>
#endif

#include "My/CubeObject.h"
#include "My/DistortionMaterial.h"
#include "My/Material.h"
#include "My/OpenGLResolver.h"
#include "My/PBRMaterial.h"
#include "My/PhongMaterial.h"
#include "My/QuadObject.h"
#include "My/Scene.h"
#include "My/SkyBoxObject.h"

enum class ViewMode
{
    DEFAULT = 0,
    VR = 1
};

class Window : public QOpenGLWindow
{
    Q_OBJECT

    // CONSTANTS
public:
    static constexpr GLfloat NEAR_{0.01f};
    static constexpr GLfloat FAR_{100.f};

    // Data
private:
#ifdef Q_OS_ANDROID
    ViewMode _mode{ViewMode::VR};
#else
    ViewMode _mode{ViewMode::DEFAULT};
#endif

    // scene containing our stuff
    std::unique_ptr<My::Scene> _scene;
    std::shared_ptr<My::SkyBoxObject> _skybox;

    // Debug stuff
#ifndef _NDEBUG
    std::unique_ptr<QOpenGLDebugLogger> _logger;
#endif

    // for VR rendering
    std::unique_ptr<My::QuadObject> _quad;
    std::shared_ptr<My::DistortionMaterial> _distortion_material{nullptr};
    std::unique_ptr<QOpenGLFramebufferObject> _fbo;
    bool _fbo_diry{true};

#if defined(Q_OS_ANDROID) && USE_SENSORS
    QRotationSensor _sensor;
    QRotationReading * _rot_reading;
#endif

    QMatrix4x4 _world;                          // Matrices
    QVector3D _eye{0, 0, -4}, _target{0, 0, 0}; // Camera properties
    QVector3D _light_pos{3, -2, -3};
    GLfloat _light_strength{100.0};

    GLfloat _phase{0};
    GLfloat _zoom{1};

    std::unique_ptr<QTimer> _loop;

    // CONSTRUCTOR/DESTRUCTOR
public:
    Window();
    ~Window();

    // METHODS
private:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void wheelEvent(QWheelEvent * wevent);
    void keyPressEvent(QKeyEvent * kevent);

    // initialize framebuffer
    void init_fbo();

    // get projection depending on eye position. Points to (0,0,0)
    QMatrix4x4 projection(GLfloat x, GLfloat y, GLfloat z) noexcept;

    // renders containing scene
    void render_scene(const My::ShaderBundle & bundle);

#ifndef _NDEBUG
private slots:
    void log(const QOpenGLDebugMessage & msg);
#endif

public slots:
    void mode(ViewMode mode);
};
