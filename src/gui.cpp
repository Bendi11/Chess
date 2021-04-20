#include "gui.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImVec2 operator +(ImVec2& lhs, ImVec2& rhs)
{
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

void Sprite::display(void)
{
    //Add our image to the Dear ImGui draw list in the background
    ImGui::GetBackgroundDrawList()->AddImage((void*)m_txid, m_pos, m_pos + m_size);
}

Sprite::Sprite(const char* f_name)
{
    m_pos = ImVec2(0., 0.);
    int x, y;
    unsigned char* img_data = stbi_load(f_name, (int*)&x, &y, NULL, 4); //Load the image data from the given path
    m_size = ImVec2(static_cast<float>(x), static_cast<float>(y));

    glGenTextures(1, &m_txid);
    glBindTexture(GL_TEXTURE_2D, m_txid); //Generate and bind the texture ID to do work on

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(m_size.x), static_cast<int>(m_size.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data); //Tell OpenGL about our texture
    stbi_image_free(img_data); //Free the texture data now that we gave it to OpenGL
}

void ChessGui::init(void)
{
    if(!glfwInit()) 
        ;
    
    
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    win = glfwCreateWindow(1280, 720, "Chess", NULL, NULL); //Initialize the GLFW window
    if(win == NULL) return;
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1); //Enable vsync

    gladLoadGL();
    

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

}

void ChessGui::loop(void)
{
    Sprite s("assets/wPawn.png");
    Sprite s2 = s;
    s.setpos(ImVec2(400., 400.));
    s2.setpos(ImVec2(100., 100.));

    int w, h, ch;
    auto data = stbi_load("assets/wPawn.png", &w, &h, &ch, 0);
    unsigned int txID;
    glGenTextures(1, &txID);
    glBindTexture(GL_TEXTURE_2D, txID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    while(!glfwWindowShouldClose(win))
    {

        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        s.display();
        s2.display();


        ImGui::Begin("Chess Menu");

        ImGui::End();



        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(win, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(win);
    }
}