#pragma once
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "console.h"
#include "renderview.h"

#define GL_SILENCE_DEPRECATION

class GUI
{
public:
    int image_width, image_height;
    GLuint image_texture;

    GUI();
    ~GUI();

    void init();

    void run();

    void update();

private:
    GLFWwindow *window;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
};