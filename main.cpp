#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static unsigned int CompileShader(unsigned int type, const std::string &source)
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

        return 0;
    }

    return shaderId;
}

static unsigned int CreateShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    // Will remove shader src code, so we won't be able to breakpoint in the shader src,
    // but can remove these two lines if need to debug shader.
    // glDetachShader(program, vertexShader);
    // glDetachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
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

    float positions[] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f};
    unsigned int buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    // We have to bind the vertex array here on MAC, even though on windows it is not required.
    unsigned int vertexArray;

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    std::string vertexShaderSrc =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShaderSrc =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShaderSrc, fragmentShaderSrc);

    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}