#pragma once

namespace My
{

class Light
{
private:
    float _strength{0.0f};

public:
    float strength() { return _strength; }

    void strength(float strength) { _strength = strength; }
};

class DirectionalLight : public Light
{};

class PointLight : public Light
{};

} // namespace My
