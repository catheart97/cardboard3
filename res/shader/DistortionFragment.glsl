#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

in vec2 tex;

uniform sampler2D frame;
uniform float aspect_ratio;

out vec4 frag_color;

void main(void)
{
    float alpha =6.0, beta = 0.0;
    float aspect_squared = aspect_ratio * aspect_ratio;

    vec2 m = vec2((tex.x < 0.5) ? 0.25 : 0.75, 0.5);

    float r2 = (tex.x - m.x)*(tex.x - m.x)+(tex.y - m.y)*(tex.y - m.y)/aspect_squared;
    vec2 t = (tex - m)*(1.0 + alpha * r2 + beta * r2 * r2) + m;

    if (tex.x < 0.5 && t.x >= 0.5) discard;
    if (tex.x > 0.5 && t.x <= 0.5) discard;
    if (t.y < 0.0 || t.y > 1.0) discard;
    if (t.x < 0.0 || t.x > 1.0) discard;

    frag_color = texture(frame,t);
}
