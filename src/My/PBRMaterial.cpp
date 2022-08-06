#include "PBRMaterial.h"

void My::PBRMaterial::bind(const My::ShaderBundle & bundle)
{
    _shader->bind();

    _shader->setUniformValue(_proj_loc, bundle.projection);
    _shader->setUniformValue(_cam_loc, bundle.camera);
    _shader->setUniformValue(_world_loc, bundle.world);
    _shader->setUniformValue(_model_loc, bundle.model);

    _shader->setUniformValue(_eye_loc, bundle.eye);

    _shader->setUniformValue(_light_loc, bundle.light_pos);
    _shader->setUniformValue(_light_col_loc, bundle._light_strength * QVector3D(1.f, 1.f, 1.f));

    _shader->setUniformValue(_albedo_loc, _albedo);
    _shader->setUniformValue(_roughness_loc, _roughness);
    _shader->setUniformValue(_metalness_loc, _metalness);
    _shader->setUniformValue(_ao_loc, _ambient_occlusion);
    _shader->setUniformValue(_ior_loc, _ior);
    _shader->setUniformValue(_alpha_loc, _alpha);
}
