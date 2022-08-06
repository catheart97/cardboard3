#pragma once

#include <memory>

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

#include "DistortionMaterial.h"
#include "Object.h"

namespace My
{

class QuadObject : public Object
{
    // Data
private:
    std::unique_ptr<QOpenGLVertexArrayObject> _vao;
    std::unique_ptr<QOpenGLBuffer> _vbo;

    std::shared_ptr<Material> _material;

    // Constructors
public:
    QuadObject(std::shared_ptr<Material> material) : Object()
    {
        _material = material;

        auto * f = QOpenGLContext::currentContext()->functions();

        std::vector<QVector3D> quad{QVector3D(-1, 1, 0), QVector3D(-1, -1, 0), QVector3D(1, 1, 0),
                                    QVector3D(1, -1, 0)};

        _vao = std::make_unique<QOpenGLVertexArrayObject>();
        if (_vao->create()) _vao->bind();

        _vbo = std::make_unique<QOpenGLBuffer>();
        _vbo->create();
        _vbo->bind();

        _vbo->allocate(quad.data(), static_cast<int>(quad.size() * sizeof(QVector3D)));
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);

        _vao->release();
        _vbo->release();
    }

    void render(const ShaderBundle & bundle) override;
};

} // namespace My
