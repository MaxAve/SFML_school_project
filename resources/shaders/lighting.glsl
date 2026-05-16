struct Light
{
    vec2 position;
    vec3 color;
    float range;
    float intensity;
};

uniform sampler2D texture;
uniform vec2 resolution;
uniform Light lightSources[64];
uniform int amountLightSources;

const vec3 vignetteColorMultiplier = vec3(0.4, 0.3, 0.6);

void main(void)
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Vignette
    float vdist = distance(gl_FragCoord.xy, resolution * 0.5);

    color = vec4(
        color.x * (vignetteColorMultiplier.x + (1 - vignetteColorMultiplier.x) * (resolution.y - vdist)/resolution.y),
        color.y * (vignetteColorMultiplier.y + (1 - vignetteColorMultiplier.y) * (resolution.y - vdist)/resolution.y), 
        color.z * (vignetteColorMultiplier.z + (1 - vignetteColorMultiplier.z) * (resolution.y - vdist)/resolution.y), 
        color.w);

    // Light sources
    for(int i = 0; i < amountLightSources; i++)
    {
        float ldist = distance(gl_FragCoord.xy, lightSources[i].position);

        color = vec4(
            color.x + color.x * lightSources[i].color.x * lightSources[i].intensity * (max(0.0, (lightSources[i].range - ldist) / lightSources[i].range)),
            color.y + color.y * lightSources[i].color.y * lightSources[i].intensity * (max(0.0, (lightSources[i].range - ldist) / lightSources[i].range)), 
            color.z + color.z * lightSources[i].color.z * lightSources[i].intensity * (max(0.0, (lightSources[i].range - ldist) / lightSources[i].range)),
            color.w);
    }

    gl_FragColor = color;
}