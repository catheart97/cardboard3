layout(location = 0) in vec3 vertex;

out vec2 tex;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 cam;
uniform mat4 world;


void main(void)
{
    tex.x = 0.5 * vertex.x + 0.5;
    tex.y = 0.5 * vertex.y + 0.5;
    gl_Position = proj * cam * world * model * vec4(vertex, 1.0);
}
