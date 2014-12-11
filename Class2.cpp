//  Class2.cpp
//  Helper Class2
//
//  These class heaaders are for use in your Trainer class. If you wish to 
//  write helper classes, write them in these 5 classes. 
//  Use of these classes is optional. You may use none or any or all of them. 
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#include "Class2.h"
#include <sstream>
/*
ParseHelp::ParseHelp()
{
    
}

void ParseHelp::parseNameAndLevel(string& name, int& level, string name_level)
{
    stringstream ss;
    ss << name_level;
    
    char firstC; //takes first character
    ss >> firstC;
    name = firstC;
    
    //ONLY for C-lacanth (exception)
    if(firstC == 'C')
    {
        char dash;
        ss >> dash;
        name += dash; //this '-' not be mistaken as the '-' before level#
    }
    else
    {
        char dash;
        ss >> dash;
        while(dash != '-') //skip characteres until reaches '-'
        {
            name += dash;
            ss >> dash;
        }
        ss >> level; //parse out level#
    }
}
*/