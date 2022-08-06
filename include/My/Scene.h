#pragma once

#include <memory>
#include <vector>

#include "Object.h"

namespace My
{

class Scene : public Object
{
    // Data
private:
    std::vector<std::shared_ptr<My::Object>> _content;

    // Properties
public:
    void add(std::shared_ptr<My::Object> object) { _content.push_back(object); }

    std::shared_ptr<My::Object> get(size_t i) { return _content[i]; }

    // Constructors
public:
    Scene() : Object() {}

    // Methods
public:
    void render(const ShaderBundle & bundle) override;
};

} // namespace My
