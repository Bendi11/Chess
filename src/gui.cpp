#include "gui.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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