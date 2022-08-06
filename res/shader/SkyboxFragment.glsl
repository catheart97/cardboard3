#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

in vec3 vert;

out vec4 frag_color;

uniform samplerCube tex;

void main()
{
    frag_color = texture(tex, vert.xyz);
}
