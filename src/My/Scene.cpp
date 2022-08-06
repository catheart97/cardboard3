#include "Scene.h"

void My::Scene::render(const My::ShaderBundle &bundle)
{
    for (auto & o : _content) o->render(bundle);
}
