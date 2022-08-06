#pragma once

#include <memory>

#include "Material.h"

namespace My
{

/**
 * @brief The PhongMaterial class
 *
 * Simple Blinn-Phong Material implementation.
 *
 * @author Ronja Schnur (rschnur@students.uni-mainz.de)
 */
class PhongMaterial : public Material
{
    // Data
private:
    QVector3D _color{.5, .5, .5};
    GLfloat _shininess{32.0f};
    GLfloat _ambient{0.5f};

    std::unique_ptr<QOpenGLShaderProgram> _shader;

    // Properties
public:
    QVector3D color() { return _color; }
    void color(const QVector3D & color) { _color = std::move(color); }
    GLfloat shininess() const { return _shininess; }
    void shininess(const GLfloat & shininess) { _shininess = shininess; }
    GLfloat ambient() const { return _ambient; }
    void ambient(const GLfloat & ambient) { _ambient = ambient; }

    // Construction
public:
    PhongMaterial() : Material()
    {
        _shader = std::make_unique<QOpenGLShaderProgram>();
        _shader->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                         OpenGLResolver::load(":/shader/PhongVertex.glsl"));
        _shader->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                         OpenGLResolver::load(":/shader/PhongFragment.glsl"));
        _shader->link();
    }

    void bind(const ShaderBundle & bundle)
    {
        _shader->bind();
        int proj_mat_loc = _shader->uniformLocation("proj");
        int cam_mat_loc = _shader->uniformLocation("cam");
        int world_mat_loc = _shader->uniformLocation("world");
        int model_mat_loc = _shader->uniformLocation("model");
        int light_pos_loc = _shader->uniformLocation("light_pos");
        int color_loc = _shader->uniformLocation("color");
        int shininess_loc = _shader->uniformLocation("shininess");
        int ambient_loc = _shader->uniformLocation("ambient");

        _shader->setUniformValue(proj_mat_loc, bundle.projection);
        _shader->setUniformValue(cam_mat_loc, bundle.camera);
        _shader->setUniformValue(world_mat_loc, bundle.world);
        _shader->setUniformValue(model_mat_loc, bundle.model);
        _shader->setUniformValue(color_loc, _color);
        _shader->setUniformValue(light_pos_loc, bundle.light_pos);
        _shader->setUniformValue(shininess_loc, _shininess);
        _shader->setUniformValue(ambient_loc, _ambient);
    }
};

} // namespace My
