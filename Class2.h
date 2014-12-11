//
//  Class2.h
//  Class2
//
//  These class heaaders are for use in your Trainer class. If you wish to 
//  write helper classes, write them in these 5 classes. 
//  Use of these classes is optional. You may use none or any or all of them. 
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#ifndef __CreatureTrainer__Class2__
#define __CreatureTrainer__Class2__

#include <iostream>
using namespace std;

// vvvvvv Your implementations go between this line…

class ParseHelp
{
public:
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Default constructor
     */
    ParseHelp();
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Parses out name and level from name-level
                 Changes reference parameters name & level
                 Ex: before: name-level = "Dugong-0"
                     after: name = "Dugong" & level = 0
     */
    void parseNameAndLevel(string& name, int& level, string name_level);
};

// ^^^^^^ And this line. That's it!

#endif /* defined(__CreatureTrainer__Class2__) */
