layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 vert;
out vec3 vert_normal;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 cam;
uniform mat4 world;

uniform vec3 color;

void main()
{
    vert = vec3(world * model * vec4(vertex, 1.0));
    vert_normal = normalize(mat3(transpose(inverse(world * model))) * normal);
    gl_Position = proj * cam * world * model * vec4(vertex, 1.0);
}
