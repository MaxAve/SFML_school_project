uniform sampler2D texture;
uniform vec2 resolution;

void main(void) {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    float dist = distance(gl_FragCoord.xy, resolution * 0.5);
    gl_FragColor = vec4(
        color.x * (0.4 + 0.6 * (resolution.y - dist)/resolution.y),
        color.y * (0.3 + 0.7 * (resolution.y - dist)/resolution.y), 
        color.z * (0.6 + 0.4 * (resolution.y - dist)/resolution.y), 
        color.w);
}