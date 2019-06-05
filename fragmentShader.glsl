#version 330

out vec4 color;

void main()
{
    float lerpValue = gl_FragCoord.y / 480.0f;

    color = mix(vec4(0.0, 1.0, 0.0, 1.0), vec4(0.0, 0.0, 1.0, 1.0), lerpValue);
}