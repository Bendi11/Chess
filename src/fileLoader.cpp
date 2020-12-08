/*
File for loading config files, PGN files (eventually), saving NN to file,
loading NN to file, etc.
*/
#include "main.hpp"

//Function to load config file and set up values from them
void chessGame::loadCfg()
{
    std::ifstream cfg("config.conf"); //Open the config file
    if(!cfg.is_open()) //Make sure the file exists
    {

    }
}