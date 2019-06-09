#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
uniform float loopDuration;
uniform float elapsedTime;

smooth out vec4 vertexColor;

void main()
{
    float timeScale = 3.14159f * 2.0f / loopDuration;
    float currentTime = mod(elapsedTime, loopDuration);
    vec4 totalOffset = vec4(
        cos(currentTime * timeScale) * 0.5f,
        sin(currentTime * timeScale) * 0.5f,
        0.0f,
        0.0f
    );

    gl_Position = position + totalOffset;
    vertexColor = color;
}