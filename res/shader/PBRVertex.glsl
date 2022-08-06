layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 vpos;
out vec3 vnormal;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 cam;
uniform mat4 world;

void main()
{
    vpos = vec3(model * vec4(vertex, 1.0));
    vnormal = normalize(transpose(inverse(mat3(model))) * normal);
    gl_Position = proj * cam * world * model * vec4(vertex, 1.0);
}
