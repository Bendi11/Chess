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
        msg::error("Failed to initialize GLFW3");
        exit(-1);
    }
    
    
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    win = glfwCreateWindow(800, 800, "Chess", NULL, NULL); //Initialize the GLFW window
    if(win == NULL) //Check for window creation errors
    {
        msg::error("Failed to create GLFW3 window!");
        exit(-1);
    }

    glfwMakeContextCurrent(win);
    glfwSwapInterval(1); //Enable vsync

    //Check for glad extension load fails
    if(gladLoadGL() == 0)
    {
        msg::error("glad extension loader failed to load OpenGL 3.3 extensions"); //Display the error message
        exit(-1); //Exit the program
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(win, true); //Start ImGui for OpenGL 3.3 and Glfw3
    ImGui_ImplOpenGL3_Init(glsl_version);

    Pa_Initialize();

    //Open the capture sound from the wav file
    SF_INFO capsound_info;
    SNDFILE* capsound_file = sf_open("assets/sounds/capture.wav", SFM_READ, &capsound_info); //Open the capture sound information
    capture_sound = new float[capsound_info.frames * capsound_info.channels];
    sf_readf_float(capsound_file, capture_sound, capsound_info.frames);

    
    SF_INFO movsound_info;
    SNDFILE* mov_file = sf_open("assets/sounds/move.wav", SFM_READ, &movsound_info);
    move_sound = new float[movsound_info.frames * movsound_info.channels];
    sf_readf_float(mov_file, move_sound, movsound_info.frames);

    sf_close(capsound_file);
    sf_close(mov_file);

    num_frames = capsound_info.frames;

    PaError err = Pa_OpenDefaultStream(&audio_stream,
        0,
        capsound_info.channels,
        paFloat32,
        capsound_info.samplerate,
        capsound_info.frames,
        NULL,
        NULL
    );
    if(err != paNoError)
    {
        msg::error("Unable to open audio output! Code %i", err);
        exit(-1);
    }
    
    Pa_StartStream(audio_stream);

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
    moveable = Sprite("assets/moveable.png");
}

ChessGui::ChessGui() 
{
    
}

void ChessGui::loop(void)
{
    chess.gen_moves();
    bool clicked = false; //If the user clicked on a position
    ImVec2 old_clickpos;
    bool game_over = false;

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
            ImVec2 piece_pos = to_screencoords(piece.get().m_pos);

            //Draw the piece using the correct sprite for the color and piece type
            Sprite s = piece_sprites[static_cast<std::size_t>(piece.get().m_kind) + ( (piece.get().m_flags[Piece::Flags::COLOR]) ? 0 : 6)];
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
                if(old_click.valid() && chess[old_click].has_value())
                {
                    //Display a dot to represent where a piece can move
                    for(auto& mov : chess[old_click]->m_moves)
                    {
                        Sprite moveable_sprite = moveable;
                        moveable_sprite.display(ImVec2((float)min / 8, (float)min / 8), to_screencoords(mov.m_new));
                    }
                    
                    // Display the sprite at the dragged cursor pos
                    Sprite s = piece_sprites[static_cast<std::size_t>(chess[old_click]->m_kind) + ( ((chess[old_click]->m_flags[Piece::Flags::COLOR]) ? 0 : 6)) ];
                    s.display(ImVec2((float)min / 8, (float)min / 8), ImVec2(ImGui::GetMousePos().x - (float)min / 16, ImGui::GetMousePos().y - (float)min / 16));
                    
                }
            }
        }
        else 
        {
            if(clicked && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                clicked = false;
                if(chess.make_move(Move(to_chesscoords(old_clickpos), to_chesscoords(ImGui::GetMousePos()))) )
                {
                    //Pa_StartStream(audio_stream);
                    Pa_WriteStream(audio_stream, move_sound, num_frames);
                }
                chess.gen_moves();
            }
        }

        auto state = chess.get_state();
        if(state.black_checkmate || state.white_checkmate)
        {
            game_over = true;
        }
        if(game_over)
        {
            ImGui::Begin("Game Over");
            ImGui::Text("%s won the game by checkmate", (state.white_checkmate) ? "Black" : "White");
            if(ImGui::Button("Resset"))
            {
                chess = Board();
                game_over = false;
            }
            ImGui::End();
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