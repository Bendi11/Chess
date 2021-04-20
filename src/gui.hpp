#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "board.hpp"

/**
 * @brief A sprite contains all information needed to render an image
 * using OpenGL. This includes size, position, and texture IDs in OpenGL
 * 
 */
struct Sprite
{
public:
    /// Display the sprite at its stored position and size using Dear ImGui's draw list
    void display(void);

    /// Load a sprite from a file using the file path
    Sprite(const char* f_name);

    Sprite(const Sprite& other) = default;

    void setpos(const ImVec2& pos) {m_pos = pos;}

private:    
    /// The texture ID of this sprite
    unsigned int m_txid;
    /// The size of the sprite rendered in OpenGL
    ImVec2 m_size;
    /// The position of the sprite rendered in OpenGL
    ImVec2 m_pos;
};

/**
 * @brief The `ChessGui` class contains a `Board` object and displays it, using user input to 
 * configure and manipulate the board
 * @see Board
 * 
 */
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