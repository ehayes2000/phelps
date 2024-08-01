uniform vec2 center;
uniform float radius;
uniform float pressure;

void main() {
    vec2 pixelPos = gl_FragCoord.xy;
    float distance = length(pixelPos - center);
    
    if (distance <= radius) {
        float normalizedDistance = distance / radius;
        float intensity = (1.0 - normalizedDistance) * abs(pressure);
        
        vec3 color;
        if (pressure > 0.0) {
            color = vec3(1.0, 0.0, 0.0);  // Red for positive pressure
        } else {
            color = vec3(0.0, 0.0, 1.0);  // Blue for negative pressure
        }
        
        gl_FragColor = vec4(color, intensity);
    } else {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);  // Transparent white outside the radius
    }
}