#include "gui.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>


ImVec2 operator +(ImVec2& lhs, ImVec2& rhs)
{
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

void Sprite::display(void)
{
    //Add our image to the Dear ImGui draw list in the background
    ImGui::GetBackgroundDrawList()->AddImage((void*)m_txid, pos, pos + size);
}

void Sprite::display(ImVec2 scale, ImVec2 offset)
{
    //Add our image to the Dear ImGui draw list in the background
    ImGui::GetBackgroundDrawList()->AddImage((void*)m_txid, offset, offset + scale);
}

Sprite::Sprite(const char* f_name)
{
    pos = ImVec2(0., 0.);
    int x, y;
    unsigned char* img_data = stbi_load(f_name, (int*)&x, &y, NULL, 4); //Load the image data from the given path
    size = ImVec2(static_cast<float>(x), static_cast<float>(y));

    glGenTextures(1, &m_txid);
    glBindTexture(GL_TEXTURE_2D, m_txid); //Generate and bind the texture ID to do work on

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(size.x), static_cast<int>(size.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data); //Tell OpenGL about our texture
    stbi_image_free(img_data); //Free the texture data now that we gave it to OpenGL
}

void ChessGui::init(void)
{
    if(!glfwInit()) 
    {
        
    }
    
    
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    win = glfwCreateWindow(1280, 720, "Chess", NULL, NULL); //Initialize the GLFW window
    if(win == NULL) //Check for window creation errors
    {

    }

    glfwMakeContextCurrent(win);
    glfwSwapInterval(1); //Enable vsync

    //Check for glad extension load fails
    if(gladLoadGL() == 0)
    {
        
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(win, true); //Start ImGui for OpenGL 3.3 and Glfw3
    ImGui_ImplOpenGL3_Init(glsl_version);

    Sprite black_tile("assets/bSquare.png"); //Load the black tile texture
    Sprite white_tile("assets/wSquare.png"); //Load the white tile texture
    
    //Set the board sprites to the correct colors
    std::size_t x = 0;
    std::transform(m_boardsprites.begin(), m_boardsprites.end(), m_boardsprites.begin(), [&x, &black_tile, &white_tile](std::array<Sprite, 8>& file) -> std::array<Sprite, 8>
    {
        std::size_t y = 0;
        std::transform(file.begin(), file.end(), file.begin(), [&x, &y, &black_tile, &white_tile](Sprite& s) -> Sprite
        {
            Sprite ret;// = ((x % 2 == 0) && (++y % 2 == 0) ? white_tile : black_tile);
            if(x % 2 == 0)
            {   
                ret = (y % 2 == 0) ? white_tile : black_tile;
            }
            else 
            {
                ret = (y % 2 == 0) ? black_tile : white_tile;
            }
            ret.pos = ImVec2(x * ret.size.x, y * ret.size.y);
            ++y;
            return ret;
        });
        x++;
        return file;
    });

    piece_sprites[Piece::Type::PAWN] = Sprite("assets/wPawn.png");
    piece_sprites[Piece::Type::ROOK] = Sprite("assets/wRook.png");
    piece_sprites[Piece::Type::BISHOP] = Sprite("assets/wBishop.png");
    piece_sprites[Piece::Type::KNIGHT] = Sprite("assets/wKnight.png");
    piece_sprites[Piece::Type::KING] = Sprite("assets/wKing.png");
    piece_sprites[Piece::Type::QUEEN] = Sprite("assets/wQueen.png");

    piece_sprites[Piece::Type::PAWN + 6] = Sprite("assets/bPawn.png");
    piece_sprites[Piece::Type::ROOK + 6] = Sprite("assets/bRook.png");
    piece_sprites[Piece::Type::BISHOP + 6] = Sprite("assets/bBishop.png");
    piece_sprites[Piece::Type::KNIGHT + 6] = Sprite("assets/bKnight.png");
    piece_sprites[Piece::Type::KING + 6] = Sprite("assets/bKing.png");
    piece_sprites[Piece::Type::QUEEN + 6] = Sprite("assets/bQueen.png");
}

ChessGui::ChessGui() 
{
    
}

void ChessGui::loop(void)
{
    chess.gen_moves();
    //ImVec2 offset;
    bool clicked = false; //If the user clicked on a position
    ImVec2 old_clickpos;

    while(!glfwWindowShouldClose(win))
    {
        int display_w, display_h;
        glfwGetFramebufferSize(win, &display_w, &display_h);

        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Get the largest display dimension and smallest to properly scale and align the chess board
        max = (display_w > display_h) ? display_w : display_h;
        min = (display_w > display_h) ? display_h : display_w;
        width_max = max == display_w;
        
        std::size_t x = 0, y = 0;
        for(auto& file : m_boardsprites)
        {
            for(auto& rank : file)
            {
                if(width_max)
                    rank.display(ImVec2((float)min / 8, (float)min / 8), ImVec2(x *((float)min / 8) + (float)((max - min) / 2), y *((float)min / 8)) );
                else
                    rank.display(ImVec2((float)min / 8, (float)min / 8), ImVec2(x *((float)min / 8), y *((float)min / 8) + (float)((max - min) / 2)));
                x++;
            }
            y++;
            x = 0;
        }

        for(auto piece : chess.pieces())
        {
            //Transform and scale the piece coordinates based on screen dimensions
            //float piece_x = (width_max) ? piece.m_pos.x *((float)min / 8) + (float)((max - min) / 2) : piece.m_pos.x *((float)min / 8);
            //float piece_y = (width_max) ? -(piece.m_pos.y - 7) *((float)min / 8) : -(piece.m_pos.y - 7) * ((float)min / 8) + (float)((max - min) / 2) ;
            ImVec2 piece_pos = to_screencoords(piece.m_pos);

            //Draw the piece using the correct sprite for the color and piece type
            Sprite s = piece_sprites[static_cast<std::size_t>(piece.m_kind) + ( (piece.m_flags[Piece::Flags::COLOR]) ? 0 : 6)];
            s.display(
                ImVec2((float)min / 8, (float)min / 8), 
                piece_pos
            );
        }

        if(ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left) )
        {
            if(!clicked)
            {
                clicked = true;
                old_clickpos = ImGui::GetMousePos();
            }
            else 
            {
                Position old_click = to_chesscoords(old_clickpos);
                if(chess[old_click].has_value())
                {
                    Sprite s = piece_sprites[static_cast<std::size_t>(chess[old_click]->m_kind) + ( ((chess[old_click]->m_flags[Piece::Flags::COLOR]) ? 0 : 6)) ];
                    s.display(ImVec2((float)min / 8, (float)min / 8), ImVec2(ImGui::GetMousePos().x - (float)min / 16, ImGui::GetMousePos().y - (float)min / 16));
                }
            }
        }
        else 
        {
            if(clicked && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                msg::info("Mouse move (%zu, %zu) to (%zu, %zu)", to_chesscoords(old_clickpos).x, to_chesscoords(old_clickpos).y, to_chesscoords(ImGui::GetMousePos()).x, to_chesscoords(ImGui::GetMousePos()).y);
                clicked = false;
                if(chess.make_move(Move(to_chesscoords(old_clickpos), to_chesscoords(ImGui::GetMousePos()))) )
                {
                    chess.gen_moves();
                }
            }
        }

        // Rendering
        ImGui::Render();
        //nt display_w, display_h;
        //glfwGetFramebufferSize(win, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(win);
        
    }
}