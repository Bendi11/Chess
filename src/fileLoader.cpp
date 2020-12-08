/*
File for loading config files, PGN files (eventually), saving NN to file,
loading NN to file, etc.
*/
#include "main.hpp"

//Function to read a descriptor line and the actual value from cfg file
//and return the actual value in a string
std::string readCfgLine(std::ifstream& file)
{
    std::string line; //String holding contents of readline
    getline(file, line); //Read the descriptor line
    getline(file, line); //Read the actual value
    return line; //Return the string of the actual value
}

//Function to load config file and set up values from them
void chessGame::loadCfg()
{
    logger.open("log.txt"); //Open the log file
    std::ifstream cfg("config.conf"); //Open the config file
    if(!cfg.is_open()) //Make sure the file exists
    {
        logger<<"Config file could not be opened at path ./config.conf!"<<std::endl; //Print an error
        exit(EXIT_FAILURE); //Close the program
    }

    logger<<"Config file opened!"<<std::endl; //Log that we opened the cfg file

    //Start reading config file

    SCREEN_WIDTH = stoi(readCfgLine(cfg)); //Read line of the cfg file and store in screen width
    SCREEN_HEIGHT = stoi(readCfgLine(cfg)); //Read line of cfg and store in screen height



}