#include "DistortionMaterial.h"

void My::DistortionMaterial::bind(const My::ShaderBundle &bundle)
{
    _shader->bind();

    int proj_mat_loc = _shader->uniformLocation("proj");
    int cam_mat_loc = _shader->uniformLocation("cam");
    int world_mat_loc = _shader->uniformLocation("world");
    int model_mat_loc = _shader->uniformLocation("model");
    int tex_loc = _shader->uniformLocation("frame");
    int asp_loc = _shader->uniformLocation("aspect_ratio");

    bundle.f->glBindTexture(GL_TEXTURE_2D, _tex);
    _shader->setUniformValue(proj_mat_loc, bundle.projection);
    _shader->setUniformValue(cam_mat_loc, bundle.camera);
    _shader->setUniformValue(world_mat_loc, bundle.world);
    _shader->setUniformValue(model_mat_loc, bundle.model);
    _shader->setUniformValue(tex_loc, 0);
    if (!bundle.aspect_ratio.has_value())
        throw std::runtime_error("Aspect Ratio not set in ShaderBundle instance.");
    _shader->setUniformValue(asp_loc, bundle.aspect_ratio.value());
}
