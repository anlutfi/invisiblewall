#include "LiveMask.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctype.h>

using namespace std;
using namespace cv;

unsigned char commands[] = {'q', 'd', 'c', 'r'};

unsigned char validateCommand(unsigned char command)
{
    command = tolower(command);
    for(int i = 0; i < sizeof(commands) / sizeof(unsigned char); i++)
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
    
    int threshold;
    int blurkernelsize;
    
    try
    {
        std::getline(ini, line);
        std::vector<std::string> tokens = split(line, '=');
        threshold = std::stoi(tokens.back());
    }
    catch(...)
    {
        cout << "couldn't convert threshold to integer\n";
        return 1;
    }
    
    try
    {
        std::getline(ini, line);
        std::vector<std::string> tokens = split(line, '=');
        blurkernelsize = std::stoi(tokens.back());
    }
    catch(...)
    {
        cout << "couldn't convert threshold to integer\n";
        return 1;
    }
    
    ini.close();   
    
    for( unsigned char command = ' '; command != 255;  )
    {
        cout << "Insert command ( [D]isplay Cameras; make [C]ontrol Image; [R]un or [Q]uit ):\n\n>>";
        
        cin >> line;
        command = command = line.at(0);
        
        command = validateCommand(command);
        
        switch(command)
        {
            case 'd':
                cout << "d";
            break;
            case 'c':
                cout << "c";
            break;
            case 'r':
                cout << "r";
            break;
        }
    }
    
    
    return 0;
}
