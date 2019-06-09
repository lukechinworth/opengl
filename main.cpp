#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static unsigned int verticesBuffer;

static float vertices[] = {
    0.0f, 0.5f, 0.0f, 1.0f,
    0.5f, -0.366f, 0.0f, 1.0f,
    -0.5f, -0.366f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f};

std::string getFileToString(std::string fileName)
{
    std::ifstream input(fileName);
    std::stringstream fileBuffer;
    fileBuffer << input.rdbuf();

    return fileBuffer.str();
}

static unsigned int CreateShader(unsigned int type, const std::string &source)
{
    unsigned int shaderId = glCreateShader(type);
    const char *shaderSrc = source.c_str();

    glShaderSource(shaderId, 1, &shaderSrc, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;

        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

        char *message = (char *)alloca(length * sizeof(char));

        glGetShaderInfoLog(shaderId, length, &length, message);

        std::cout << message << std::endl;

        glDeleteShader(shaderId);

        return -1;
    }

    return shaderId;
}

static unsigned int CreateShaderProgram(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSrc);
    unsigned int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        int infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        char *strInfoLog = new GLchar(infoLogLength + 1);
        glGetProgramInfoLog(program, infoLogLength, nullptr, strInfoLog);

        std::cout << "Linker failure: %s\n"
                  << strInfoLog << std::endl;

        delete strInfoLog;

        return -1;
    }

    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void ComputePositionOffsets(float &fXOffset, float &fYOffset)
{
    const float fLoopDuration = 5.0f;
    const float fScale = 3.14159f * 2.0f / fLoopDuration;

    float fElapsedTime = glfwGetTime();

    float fCurrentTImeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

    fXOffset = cosf(fCurrentTImeThroughLoop * fScale) * 0.5f;
    fYOffset = sinf(fCurrentTImeThroughLoop * fScale) * 0.5f;
}

int main()
{
    if (!glfwInit())
        return -1;

    // The default GLSL version is low on mac, so we need to
    // set it to an updated version.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window;

    window = glfwCreateWindow(640, 480, "Hello GLFW", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        std::cout << "Glew error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glGenBuffers(1, &verticesBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // We have to bind the vertex array here on MAC, even though on windows it is not required.
    unsigned int vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    std::string vertexShaderSrc = getFileToString("vertexShader.glsl");

    std::string fragmentShaderSrc = getFileToString("fragmentShader.glsl");

    unsigned int shaderProgram = CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    int offsetUniformLocation = glGetUniformLocation(shaderProgram, "offset");

    while (!glfwWindowShouldClose(window))
    {
        float fXOffset = 0.0f;
        float fYOffset = 0.0f;
        ComputePositionOffsets(fXOffset, fYOffset);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniform2f(offsetUniformLocation, fXOffset, fYOffset);

        glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)48);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}