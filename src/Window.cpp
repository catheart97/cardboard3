#include "Window.h"

Window::Window() : _target(0, 0, -1)
{
    makeCurrent();
    _loop = std::make_unique<QTimer>();
    _loop->setInterval(16);
    QObject::connect(_loop.get(), &QTimer::timeout, [this]() { this->update(); });
    _loop->start();
}

Window::~Window() {}

void Window::initializeGL()
{
    QOpenGLExtraFunctions * f =
        QOpenGLContext::currentContext()
            ->extraFunctions();     // Get GLFunctions depending on GLES3 or GL3 context
    f->initializeOpenGLFunctions(); // Not necessarily required

#ifndef _NDEBUG
    _logger = std::make_unique<QOpenGLDebugLogger>(this);
    if (!_logger->initialize()) throw std::runtime_error("Debug Logger intialization failed.");
    connect(_logger.get(), &QOpenGLDebugLogger::messageLogged, this, &Window::log);
    _logger->startLogging();
#endif

    _skybox = std::make_shared<My::SkyBoxObject>(My::Skybox{
        QImage(":/cubemap/left.jpg"), QImage(":/cubemap/right.jpg"), QImage(":/cubemap/front.jpg"),
        QImage(":/cubemap/back.jpg"), QImage(":/cubemap/up.jpg"), QImage(":/cubemap/down.jpg")});

    _skybox->scale(99);

    _scene = std::make_unique<My::Scene>();
    _scene->add(std::make_shared<My::CubeObject>(std::make_shared<My::PBRMaterial>()));
    //    _scene->add(std::make_shared<My::CubeObject>(std::make_shared<My::PhongMaterial>()));
    _scene->add(_skybox);

    _distortion_material = std::make_shared<My::DistortionMaterial>();
    _quad = std::make_unique<My::QuadObject>(_distortion_material);

    f->glEnable(GL_DEPTH_TEST);
    f->glDisable(GL_CULL_FACE);
}

void Window::resizeGL(int, int) // w, h
{
    _fbo_diry = true;
}

void Window::paintGL()
{
    QOpenGLExtraFunctions * f =
        QOpenGLContext::currentContext()
            ->extraFunctions(); // Extra function from OpenGL 3 (which are not in 2)
    // Setup model matrix
    QMatrix4x4 model;

    // Setup view matrix
    QMatrix4x4 camera;
    camera.lookAt(_eye, _target - _eye, QVector3D(0, 1, 0));
    camera.scale(_zoom);

    // Setup world matrix
    QMatrix4x4 world{_world};
#if defined(Q_OS_ANDROID) && USE_SENSORS
    _rot_reading = _sensor.reading();
    world.rotate(90.0, 0.0, 0.0, 1.0);
    world.rotate(
        QQuaternion::fromEulerAngles(-_rot_reading->x(), -_rot_reading->y(), -_rot_reading->z()));
    world.rotate(90.0, 1.0, 0.0, 0.0);
#endif
    world.translate(QVector3D(0, 0, -.5f));

    _phase += .5f;
    _scene->get(0)->rotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 1, 1), _phase));

    auto aspect_ratio = width() / static_cast<GLfloat>(height());

    // Render and set projection matrices
    if (_mode == ViewMode::VR)
    {
        if (_fbo_diry) init_fbo();

        _fbo->bind();
        f->glClearColor(0, 0, 0, 1);
        f->glClear((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        f->glViewport(0, 0, width() / 2, height());

        QVector3D eyel{_eye.x() + .03f, _eye.y(), _eye.z()};
        render_scene(My::ShaderBundle{
            f, model, camera, world * projection(_eye.x() - .03f, _eye.y(), _eye.z()), world, eyel,
            world * _light_pos, _light_strength, aspect_ratio, _skybox->texture()});

        f->glViewport(width() / 2, 0, width() / 2, height());
        QVector3D eyer{_eye.x() + .03f, _eye.y(), _eye.z()};
        render_scene(My::ShaderBundle{
            f, model, camera, world * projection(_eye.x() + .03f, _eye.y(), _eye.z()), world, eyer,
            world * _light_pos, _light_strength, aspect_ratio, _skybox->texture()});

        _fbo->release();

        f->glClearColor(0, 0, 0, 1);
        f->glClear((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        f->glViewport(0, 0, width(), height());

        QMatrix4x4 proj;
        proj.ortho(-1, 1, -1, 1, -1, 1);
        _distortion_material->setTextureID(_fbo->texture());
        My::ShaderBundle bundle;
        bundle.f = f;
        bundle.projection = proj;
        bundle.aspect_ratio = aspect_ratio;
        _quad->render(bundle);
    }
    else
    {
        f->glClearColor(0, 0, 0, 1);
        f->glClear((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        f->glViewport(0, 0, width(), height());
        render_scene(My::ShaderBundle{f, model, camera, projection(_eye.x(), _eye.y(), _eye.z()),
                                      world, world * QVector3D(_eye), world * _light_pos, _light_strength,
                                      aspect_ratio, _skybox->texture()});
    }
}

void Window::wheelEvent(QWheelEvent * wevent)
{
    QPointF angles = wevent->angleDelta();

    if (angles.y() > 0) _zoom *= (static_cast<GLfloat>(angles.y()) / 100.f);
    if (angles.y() < 0) _zoom /= (-static_cast<GLfloat>(angles.y()) / 100.f);
}

void Window::keyPressEvent(QKeyEvent * kevent)
{
    if (kevent->key() == Qt::Key_M)
        _mode = static_cast<ViewMode>((static_cast<int>(_mode) + 1) % 2);

    if (kevent->key() == Qt::Key_Plus) _light_pos *= 1.1f;
    if (kevent->key() == Qt::Key_Minus) _light_pos /= 1.1f;

    if (kevent->key() == Qt::Key_B) _light_strength += .1f;
    if (kevent->key() == Qt::Key_D) _light_strength -= .1f;
}

void Window::init_fbo()
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    _fbo = std::make_unique<QOpenGLFramebufferObject>(width(), height(), format);
}

void Window::mode(ViewMode mode) { _mode = mode; }

QMatrix4x4 Window::projection(GLfloat x, GLfloat y, GLfloat z) noexcept
{
    auto aspect_ratio = width() / static_cast<GLfloat>(height());
    if (_mode == ViewMode::VR) aspect_ratio /= 2.0f;
    GLfloat display_height{1.5f}, display_width{display_height * aspect_ratio};

    GLfloat left{NEAR_ * (-display_width / 2.0f - x) / z};
    GLfloat right{NEAR_ * (display_width / 2.0f - x) / z};
    GLfloat bottom{NEAR_ * (-display_height / 2.0f - y) / z};
    GLfloat top{NEAR_ * (display_height / 2.0f - y) / z};

    QMatrix4x4 frustum;
    frustum.frustum(left, right, bottom, top, NEAR_, FAR_);
    frustum.translate(x, y, z);
    return frustum;
}

void Window::render_scene(const My::ShaderBundle & bundle) { _scene->render(bundle); }

#ifndef _NDEBUG
void Window::log(const QOpenGLDebugMessage & msg) { qDebug() << msg; }
#endif
