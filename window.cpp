#include "window.h"

GLFWwindow* Window::currentWindow = NULL;
int Window::currentBufferWidth = NULL;
int Window::currentBufferHeight = NULL;
int Window::lastWindowedBufferWidth = NULL;
int Window::lastWindowedBufferHeight = NULL;
float Window::deltaTime = 0.f;
float Window::lastTime = 0.f;

void Window::Init(int vMajor, int vMinor, int profile, bool forwardCompatible, int fps)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forwardCompatible);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, fps);
}

void Window::InitLib(int vMajor, int vMinor, int profile, bool forwardCompatible, int fps)
{
    Window::Init(vMajor, vMinor, profile, forwardCompatible, fps);
    Listener::Init();
    Texture::Init();
}

GLFWwindow* Window::Create(char* title, int width, int height, bool fullscreen)
{
    currentWindow = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (!currentWindow)
        err("Cannot create window with GLFW", nullptr);
    Window::Bind();
    Listener::Create(currentWindow);
    //glfwSetWindowUserPointer(currentWindow, this);
    return currentWindow;
        
}

void Window::Delete()
{
    if (currentWindow)
    {
        glfwSetWindowShouldClose(currentWindow, true);
        glfwDestroyWindow(currentWindow);
        glfwTerminate();
    }
    else
        ThrowErr::UndefinedWindow("Window::Update");
}

int Window::GetBufferWidth()
{
    if (currentBufferWidth)
        return currentBufferWidth;
    else
        ThrowErr::UndefinedWindow("Window::GetBufferWidth");
}

int Window::GetBufferHeight()
{
    if (currentBufferHeight)
        return currentBufferHeight;
    else
        ThrowErr::UndefinedWindow("Window::GetBufferHeight");
}

void Window::SetBufferWidth(int value)
{
    currentBufferWidth = value;
}

void Window::SetBufferHeight(int value)
{
    currentBufferHeight = value;
}

void Window::Bind(GLFWwindow* window)
{
    currentWindow = window;
    Window::UpdateBufferSize();
    glfwMakeContextCurrent(currentWindow);
}

void Window::SetViewport(GLint xStart, GLint xLength, GLint yStart, GLint yLength)
{
    glViewport(xStart, yStart, xLength, yLength);
}

void Window::SetFullscreen(bool fullscreen)
{
    if (fullscreen)
    {
        lastWindowedBufferWidth = currentBufferWidth;
        lastWindowedBufferHeight = currentBufferHeight;
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        currentBufferWidth = mode->width;
        currentBufferHeight = mode->height;
    }
    else
    {
        currentBufferWidth = lastWindowedBufferWidth;
        currentBufferHeight = lastWindowedBufferHeight;
    }
    glfwSetWindowMonitor(currentWindow, fullscreen ? glfwGetPrimaryMonitor() : NULL, 0, 0, currentBufferWidth, currentBufferHeight, GLFW_DONT_CARE);
    Window::SetViewport();
}

void Window::SetLoop(void (*func)())
{
    while (!glfwWindowShouldClose(currentWindow))
    {
        Window::ResetBuffers();
        Camera::Update(deltaTime);
        func();
        Window::Update();
    }
}

void Window::ExitLoop()
{
    if (currentWindow)
        glfwSetWindowShouldClose(currentWindow, true);
    else
        ThrowErr::UndefinedWindow("Window::SetShouldClose");
}

void Window::SetBgColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    glClearColor(r, g, b, a);
}

void Window::Enable3D()
{
    glEnable(GL_DEPTH_TEST);
}

void Window::EnableCursor()
{
    glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::DisableCursor()
{
    glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

float Window::GetDeltaTime()
{
    return deltaTime;
}

void Window::UpdateBufferSize()
{
    glfwGetFramebufferSize(currentWindow, &currentBufferWidth, &currentBufferHeight);
}

void Window::ResetBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{
    if (currentWindow)
    {
        glfwSwapBuffers(currentWindow);
        glfwPollEvents();
        float now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
    }
    else
        ThrowErr::UndefinedWindow("Window::Update");
}