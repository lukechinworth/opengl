#version 330

smooth in vec4 vertexColor;
out vec4 color;

void main()
{
    color = vertexColor;
}