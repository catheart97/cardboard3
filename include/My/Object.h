#pragma once

#include <QOpenGLContext>
#include <QQuaternion>

#include "Material.h"

namespace My
{

/**
 * @brief The Object class
 *
 * Class should hold required shaders and geometry data (especially for lates intersection
 * operations). The render function should draw the object.
 *
 * @author Ronja Schnur (rschnur@students.uni-mainz.de)
 */
class Object
{
    // Data
private:
    QQuaternion _rot;
    QVector3D _pos;
    GLfloat _scale{1.0f};

    // Properties
public:
    void rotation(const QQuaternion & rot) { _rot = rot; }

    QQuaternion rotation() { return _rot; }

    void position(const QVector3D & pos) { _pos = pos; }

    QVector3D position() { return _pos; }

    GLfloat scale() const { return _scale; }

    void scale(const GLfloat & scale) { _scale = scale; }

    // Constructors
public:
    virtual ~Object() = default;

    // Methods
public:
    virtual void render(const ShaderBundle & bundle) = 0;

    QMatrix4x4 model()
    {
        QMatrix4x4 model;
        model.rotate(_rot);
        model.translate(_pos);
        model.scale(_scale);
        return model;
    }
};

} // namespace My
