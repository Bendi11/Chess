#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <array>
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

    /**
     * @brief Display the sprite at is scale and offset
     * 
     * @param scale The scale size data
     * @param offset The screen coordinates to put the sprite at
     */
    void display(ImVec2 scale, ImVec2 offset);

    /// Load a sprite from a file using the file path
    Sprite(const char* f_name);
    Sprite() = default;

    Sprite(const Sprite& other) = default;

    Sprite& operator =(const Sprite& other) = default;

    /// The size of the sprite rendered in OpenGL
    ImVec2 size;
    /// The position of the sprite rendered in OpenGL
    ImVec2 pos;

private:    
    /// The texture ID of this sprite
    unsigned int m_txid;
    
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
    ChessGui();
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

    std::array<std::array<Sprite, 8>, 8> m_boardsprites; //Sprites that are displayed as the board background
};