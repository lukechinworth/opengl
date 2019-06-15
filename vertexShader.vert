#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform vec2 offset;
uniform float zNear;
uniform float zFar;
uniform float frustumScale;

smooth out vec4 vertexColor;

void main()
{
    vec4 cameraPosition = position + vec4(offset.x, offset.y, 0.0f, 0.0f);
    vec4 clipPosition;

    clipPosition.xy = cameraPosition.xy * frustumScale;
    clipPosition.z = cameraPosition.z * (zNear + zFar) / (zNear - zFar);
    clipPosition.z += 2 * zNear * zFar / (zNear - zFar);
    clipPosition.w = -cameraPosition.z;

    gl_Position = clipPosition;
    vertexColor = color;
}