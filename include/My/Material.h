#pragma once

#include <optional>

#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>

#include "OpenGLResolver.h"

namespace My
{

struct ShaderBundle
{
    QOpenGLExtraFunctions * f;
    QMatrix4x4 model;
    QMatrix4x4 camera;
    QMatrix4x4 projection;
    QMatrix4x4 world;
    QVector3D eye;
    QVector3D light_pos;
    GLfloat _light_strength;
    std::optional<GLfloat> aspect_ratio;
    QOpenGLTexture * _cubemap;
};

/**
 * @brief The Material class
 *
 * Material interface. Implementations should compile and hold required shaders.
 * Expect as input only values contained in ShaderBundle. Other should be stored inside of the
 * derived class.
 *
 * @author Ronja Schnur (rschnur@students.uni-mainz.de)
 */
class Material
{
    // Constructors
public:
    virtual ~Material() = default;

    // Methods
public:
    virtual void bind(const ShaderBundle & bundle) = 0;
};

} // namespace My
