#pragma once

#include <memory>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

#include "Material.h"
#include "Object.h"
#include "PhongMaterial.h"

namespace My
{

/**
 * @brief The CubeObject class
 *
 * Representing a renderable Cube. Works with DiffuseMaterial.
 *
 * @author Ronja Schnur (rschnur@students.uni-mainz.de)
 */
class CubeObject : public Object
{
    // Constants
public:
    static const std::vector<GLfloat> CUBE;

    // Data
private:
    std::unique_ptr<QOpenGLBuffer> _vbo;
    std::unique_ptr<QOpenGLVertexArrayObject> _vao;
    std::shared_ptr<Material> _material;

    // Constructor
public:
    CubeObject(std::shared_ptr<Material> material);

    void render(const ShaderBundle & bundle) override;
};

} // namespace My
