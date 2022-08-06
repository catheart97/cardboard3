#include "SkyBoxObject.h"

void My::SkyBoxObject::render(const My::ShaderBundle &bundle)
{
    auto * f{bundle.f};

    f->glDisable(GL_CULL_FACE);
    _vao->bind();
    _vbo->bind();

    _shader->bind();

    int proj_mat_loc = _shader->uniformLocation("proj");
    int cam_mat_loc = _shader->uniformLocation("cam");
    int world_mat_loc = _shader->uniformLocation("world");
    int model_mat_loc = _shader->uniformLocation("model");
    int scale_loc = _shader->uniformLocation("scale");
    int tex_loc = _shader->uniformLocation("tex");

    _shader->setUniformValue(proj_mat_loc, bundle.projection);
    _shader->setUniformValue(cam_mat_loc, bundle.camera);
    _shader->setUniformValue(world_mat_loc, bundle.world);
    _shader->setUniformValue(model_mat_loc, bundle.model * model());
    _shader->setUniformValue(scale_loc, scale());
    bind(0);
    _shader->setUniformValue(tex_loc, 0);

    f->glDrawArrays(GL_TRIANGLES, 0, 36);

    _vbo->release();
    _vao->release();
}
