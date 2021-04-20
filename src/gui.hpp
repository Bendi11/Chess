#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "board.hpp"


class ChessGui
{
public:
    void init(void);

    void loop(void);

    ~ChessGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(win);
        glfwTerminate();
    }
private:

    /// Window that contains all chess menus + display
    GLFWwindow* win = nullptr;

    /// The chess board that we will display
    Board chess;
};