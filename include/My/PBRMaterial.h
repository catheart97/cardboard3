#pragma once

#include <memory>

#include "Material.h"

namespace My
{

class PBRMaterial : public Material
{
    // Data
private:
    QVector3D _albedo{.5, 0., 0.5};
    GLfloat _roughness{.05f};
    GLfloat _metalness{0.f};
    GLfloat _ambient_occlusion{1.f};
    GLfloat _ior{1.5f};
    GLfloat _alpha{1.f};

    std::unique_ptr<QOpenGLShaderProgram> _shader;

    int _proj_loc, _cam_loc, _world_loc, _model_loc, _eye_loc;
    int _light_loc, _light_col_loc;
    int _albedo_loc, _roughness_loc, _metalness_loc, _ao_loc, _ior_loc, _alpha_loc;

    // Properties
public:
    // Construction
public:
    PBRMaterial() : Material()
    {
        _shader = std::make_unique<QOpenGLShaderProgram>();
        _shader->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                         OpenGLResolver::load(":/shader/PBRVertex.glsl"));
        _shader->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                         OpenGLResolver::load(":/shader/PBRFragment.glsl"));
        _shader->link();

        _proj_loc = _shader->uniformLocation("proj");
        _cam_loc = _shader->uniformLocation("cam");
        _world_loc = _shader->uniformLocation("world");
        _model_loc = _shader->uniformLocation("model");

        _eye_loc = _shader->uniformLocation("camera_pos");

        _light_loc = _shader->uniformLocation("light_pos");
        _light_col_loc = _shader->uniformLocation("light_color");

        _albedo_loc = _shader->uniformLocation("albedo");
        _roughness_loc = _shader->uniformLocation("roughness");
        _metalness_loc = _shader->uniformLocation("metalness");
        _ao_loc = _shader->uniformLocation("ao");
        _ior_loc = _shader->uniformLocation("ior");
        _alpha_loc = _shader->uniformLocation("alpha");
    }

    void bind(const ShaderBundle & bundle);
};

} // namespace My
