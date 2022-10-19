/*
 * BreakOut game.
 * 
 * Developed by Gustavo Zille.
 */

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/ShaderProgram.h"
#include "engine/core/Texture.h"

#include "engine/Game.h"

// GLFW window callbacks.
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Screen variables.
unsigned int g_ScreenWidth = 800;
unsigned int g_ScreenHeight = 600;

// Global general variables.
float g_DeltaTime = 0.0f;
float g_LastFrame = 0.0f;

// Game variables.
Game g_BreakOut(g_ScreenWidth, g_ScreenHeight);

int main(int argc, char* argv[])
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    /* Define OpenGL version and profile model. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(g_ScreenWidth, g_ScreenHeight, "BreakOut", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!window)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    /* Define window callbacks. */
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyboardCallback);

    /* Configure OpenGL context. */
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Initialize the game. */
    g_BreakOut.setup();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();

        g_DeltaTime = currentFrame - g_LastFrame;
        g_LastFrame = currentFrame;

        glfwPollEvents();

        /* Process inputs. */
        g_BreakOut.processInput(g_DeltaTime);

        /* Update game systems. */
        g_BreakOut.update(g_DeltaTime);

        /* Render game entities. */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        g_BreakOut.render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    g_ScreenWidth = width;
    g_ScreenHeight = height;

    glViewport(0, 0, width, height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // Window should close.
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            g_BreakOut.setKeyState(key, true);
        }
        else if (action == GLFW_RELEASE)
        {
            g_BreakOut.setKeyState(key, false);
            g_BreakOut.setKeyProcessedState(key, false);
        }
    }
}
