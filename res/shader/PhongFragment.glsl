#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

in vec3 vert;
in vec3 vert_normal;

out vec4 frag_color;

uniform vec3 light_pos;
uniform vec3 color;
uniform float shininess;
uniform float ambient;

void main()
{
    vec3 light_dir = normalize(light_pos - vert);
    vec3 view_dir = normalize(-vert);
    vec3 half_way = normalize(light_dir + view_dir);

    vec3 ambient_ = ambient * color;
    vec3 diffuse = vec3(max(dot(light_dir, vert_normal), 0.0));
    vec3 spec = vec3(.3 * pow(max(dot(vert_normal, half_way), 0.0), shininess));

    frag_color = vec4(ambient_ + diffuse + spec, 1.0);
}
