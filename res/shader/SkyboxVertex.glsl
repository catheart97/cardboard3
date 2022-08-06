layout(location = 0) in vec3 vertex;

out vec3 vert;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 cam;
uniform mat4 world;

uniform float scale;

void main()
{
    vert = vec3(vec4(vertex, 1.0)) / scale;
    gl_Position = proj * cam * world * model * vec4(vertex, 1.0);
}
