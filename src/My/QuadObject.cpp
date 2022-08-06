#include "QuadObject.h"

void My::QuadObject::render(const My::ShaderBundle &bundle)
{
    auto * f = bundle.f;
    _vao->bind();
    _vbo->bind();
    ShaderBundle b{bundle};
    b.model *= model();

    _material->bind(b);
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    _vbo->release();
    _vao->release();
}
