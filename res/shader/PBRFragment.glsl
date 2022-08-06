#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

in vec3 vpos;
in vec3 vnormal;

out vec4 frag_color;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 camera_pos;

uniform vec3 albedo; // prop. texture
uniform float roughness; // prop. texture
uniform float metalness; // prop texture
uniform float ao; //prop texture
uniform float ior;
uniform float alpha; // prop texture

const float PI = 3.1415926538;

// Trowbridge-Reitz GGX
float distribution_ggx(vec3 normal, vec3 half_dir, float roughness)
{
    float a = roughness * roughness;
    float a_sq = a * a;
    float ndoth = max(dot(normal, half_dir), 0.0);
    float ndoth_sq = ndoth * ndoth;
    float nom = a_sq;
    float denom = ndoth_sq * (a_sq - 1.0) + 1.0;
    denom = PI * denom * denom;
    return nom / max(denom, 0.001);
}

// Schlick Beckmann GGX
float geometry_schlick_ggx(vec3 normal, vec3 v, float roughness)
{
    float r = roughness + 1.;
    float k = (r*r) / 8.0;
    float ndotv = max(dot(normal, v), 0.0);
    return ndotv / (ndotv * (1.0 - k) + k);
}

// Smith GGX
float geometry_smith_ggx(vec3 normal, vec3 view, vec3 light, float roughness)
{
    float ggx1 = geometry_schlick_ggx(normal, view, roughness);
    float ggx2 = geometry_schlick_ggx(normal, light, roughness);
    return ggx1 * ggx2;
}

vec3 fresnel_schlick(float cosine_theta, vec3 f0)
{
    return f0 + (1.0 - f0) * pow(1.0 - cosine_theta, 5.0);
}

float calculate_attenuation(vec3 p, vec3 l)
{
    float distance = abs(length(l - p));
    return 1.0 / (distance * distance); // energy preserving
}

void main()
{
    vec3 normal = normalize(vnormal);
    vec3 view_dir = normalize(camera_pos - vpos);

    vec3 fresnel_0 = vec3(0.04);
    fresnel_0 = mix(fresnel_0, albedo, metalness);

    vec3 radiance_t = vec3(0.0);

    // for each (point) light // area lights need to be integrated differently

    // per light radiance
    vec3 light_dir = normalize(light_pos - vpos);
    vec3 half_dir = normalize(view_dir + light_dir);
    float attenuation = calculate_attenuation(vpos, light_pos);
    vec3 radiance = light_color * attenuation;

    // Cook-Torrance BRDF
    float normal_dist = distribution_ggx(normal, half_dir, roughness);
    float geometry = geometry_smith_ggx(normal, view_dir, light_dir, roughness);
    vec3 fresnel = fresnel_schlick(clamp(dot(half_dir, view_dir), 0.0, 1.0), fresnel_0);

    vec3 nominator = normal_dist * geometry * fresnel;
    float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, light_dir), 0.0);
    vec3 specular = nominator / max(denominator, 0.001);

    vec3 reflect_ = fresnel;
    vec3 refract_ = (vec3(1.0) - reflect_) * (1.0 - metalness);
    float ndotl = max(dot(normal, light_dir), 0.0);
    radiance_t += (refract_ * albedo / PI + specular) * radiance * ndotl;

    // endfor

    vec3 ambient = vec3(.03) * albedo * ao;
    vec3 color = ambient + radiance_t;

    // gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    frag_color = vec4(color, alpha);
}
