#pragma once

#include <QImage>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include "My/CubeObject.h"
#include "My/Object.h"
#include "My/OpenGLResolver.h"

namespace My
{

struct Skybox
{
    QImage left, right, front, back, top, bottom;
};

class SkyBoxObject : public Object
{
    // Data
private:
    std::unique_ptr<QOpenGLBuffer> _vbo;
    std::unique_ptr<QOpenGLVertexArrayObject> _vao;
    std::shared_ptr<QOpenGLTexture> _texture;
    std::unique_ptr<QOpenGLShaderProgram> _shader;

    // Constructor
public:
    SkyBoxObject(const Skybox & skybox) : Object()
    {
        _shader = std::make_unique<QOpenGLShaderProgram>();
        _shader->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                         OpenGLResolver::load(":/shader/SkyboxVertex.glsl"));
        _shader->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                         OpenGLResolver::load(":/shader/SkyboxFragment.glsl"));
        _shader->link();

        _texture = std::make_shared<QOpenGLTexture>(QOpenGLTexture::TargetCubeMap);

        if (!_texture->create()) throw std::runtime_error("Skymap texture creation failed.");

        QImage v{skybox.right.convertToFormat(QImage::Format_RGBA8888)};
        _texture->setSize(v.width(), v.height(), v.depth());

        _texture->setFormat(QOpenGLTexture::RGBA8_SNorm);
        _texture->allocateStorage();

        _texture->setData(
            0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
            skybox.right.mirrored().convertToFormat(QImage::Format_RGBA8888).constBits(),
            Q_NULLPTR);

        _texture->setData(
            0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
            skybox.bottom.mirrored().convertToFormat(QImage::Format_RGBA8888).constBits(),
            Q_NULLPTR);

        _texture->setData(
            0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
            skybox.front.mirrored().convertToFormat(QImage::Format_RGBA8888).constBits(),
            Q_NULLPTR);

        _texture->setData(
            0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
            skybox.left.mirrored().convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

        _texture->setData(
            0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
            skybox.top.mirrored().convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

        _texture->setData(
            0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
            skybox.back.mirrored().convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

        _texture->generateMipMaps();
        _texture->setWrapMode(QOpenGLTexture::ClampToEdge);
        _texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        _texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

        auto * f = QOpenGLContext::currentContext()->functions();

#ifndef Q_OS_ANDROID
        f->glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif

        _vao = std::make_unique<QOpenGLVertexArrayObject>();
        if (_vao->create()) _vao->bind();

        _vbo = std::make_unique<QOpenGLBuffer>();
        _vbo->create();
        _vbo->bind();
        _vbo->allocate(CubeObject::CUBE.data(),
                       static_cast<int>(CubeObject::CUBE.size() * sizeof(GLfloat)));
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
        _vbo->release();
        _vao->release();
    }

    void render(const ShaderBundle & bundle) override;

    // Methods
public:
    void bind(GLuint index)
    {
        if (_texture) _texture->bind(index);
    }

    QOpenGLTexture * texture() { return _texture.get(); }
};

} // namespace My
