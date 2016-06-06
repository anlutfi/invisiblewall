#include "LiveMask.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctype.h>

using namespace std;
using namespace cv;

char commands[] = {'q', 'd', 'c', 'r'};

char validateCommand(char command)
{
    command = tolower(command);
    for(int i = 0; i < sizeof(commands) / sizeof(char); i++)
    {
        if(commands[i] == command)
            if(i == 0)
                return 255;
            else
                return command;
    }
    return 0;
}

std::vector<std::string> &split(const std::string &s,
                                char delim,
                                std::vector<std::string> &elems
                               )
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

int main (int argc, char** argv)
{
    std::string inifname(argv[0]);
    inifname.append(".ini");
    
    ifstream ini;
    ini.open(inifname.c_str(), ios::out);
    
    if( !ini.is_open() )
    {
        cout << "ini file corrupted or missing\n";
        return 1;
    }
    
    string line;
    std::getline(ini, line);
    std::vector<std::string> tokens = split(line, '=');
    
    int threshold;
    
    try
    {
        threshold = std::stoi(tokens.back());
    }
    catch(...)
    {
        cout << "couldn't convert threshold to integer\n";
        return 1;
    }
    
    ini.close();   
    
    for( char command = ' '; command != 255; command = line.at(0) )
    {
        cout << "Insert command ( [D]isplay Cameras; make [C]ontrol Image; [R]un or [Q]uit ):\n\n>>";
        
        switch(command)
        {
            case 'd':
            break;
            case 'c':
            break;
            case 'r':
            break;
        }
        
    }
    validateCommand(' ');
    
    
    return 0;
}
