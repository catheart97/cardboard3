#pragma once

#include <memory>

#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "Material.h"
#include "OpenGLResolver.h"

namespace My
{

/**
 * @brief The DistortionMaterial class
 *
 * This Material is implementing a radial distortion on a @class CubeObject.
 * The texture id from the framebuffer is required to be set before rendering.
 *
 * @author Ronja Schnur (rschnur@students.uni-mainz.de)
 */
class DistortionMaterial : public Material
{
    // Data
private:
    GLuint _tex{0};
    std::unique_ptr<QOpenGLShaderProgram> _shader;

    // Properties
public:
    void setTextureID(GLuint id) { _tex = id; }

    // Cosntructors
public:
    DistortionMaterial() : Material()
    {
        _shader = std::make_unique<QOpenGLShaderProgram>();
        _shader->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                         OpenGLResolver::load(":/shader/DistortionVertex.glsl"));
        _shader->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                         OpenGLResolver::load(":/shader/DistortionFragment.glsl"));
        _shader->link();
    }

    void bind(const ShaderBundle & bundle) override;
};

} // namespace My
