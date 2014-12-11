//
//  Trainer.cpp
//  CreatureTrainer
//
//  You may NOT #include any .h files we provide you (Battle, Creature, CreatureType,
//  EECSRandom, Party, PrintHelper)
//  other than the ones name "ClassX.h" and "Trainer.h"
//  You may #include other <> libraries if you wish to
//
//  Created by Jeremy Gibson on 11/4/14.
//  Copyright (c) 2014 Jeremy Richard Gibson. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "Trainer.h"
#include "Class1.h"
#include "Class2.h"
#include "Class3.h"
#include "Class4.h"
#include "Class5.h"
#include "CreatureType.h"
#include "Item.h"

//#include "CreatureType.h"

using namespace std;

string Trainer::makeMove(stringstream& situation) {
    
    isStartofBattle = false; //default
    isEndofBattle = false; //default
    
    //ParseHelp parseHelp; //make parsing easier!
    
    // pull the entire string out of stringStream& situation
    string situationString = situation.str();
    // situationString now includes ALL of the lines that will be sent to cout
    
    // Here's how to use the stringSplit function
    vector<string> lines = splitString(situationString, "\n");
    // This splits situationString into a vector of individual lines.
    // (it splits on the new line character "\n")
    // To access the 3rd line (as in 0, 1, 2, 3), you would use bracket access
    //   as in: lines[3]
    
    // This loop will iterate through all the lines
    int pipeLine = -1;
    // lines.size() tells us how many elements are in the vector<string> lines
    // In this case, it tells us how many lines we pulled in
    for (int i = 0; i < lines.size(); i++) {
        // Store each line in the string line
        string line = lines[i];
        // Store the first char of line in the char c
        char c = line[0];
        // If c is a pipe ('|'), then this is the line that tells us about our
        //    party and the health of each Creature we own.
        if (c == '|') {
            // Store which line number this is
            pipeLine = i;
        }
    }
    // Now that we know which line has the health, do something with it
    //cout << lines[pipeLine] << "\n"; // Output for testing only
    
    
    // * DETERMINES WHETHER NEW BATTLE HAS STARTED *
    string newBattleLine;
    
    //looking for the line "It's an enemy..."
    for (int i = 0; i < lines.size(); i++)
    {
        newBattleLine = lines[i];
        
        // Store the first three chars (It') of the line
        char c1 = newBattleLine[0];
        char c2 = newBattleLine[1];
        char c3 = newBattleLine[2];
        
        if (c1 == 'I' && c2 == 't' && c3 == '\'')
        {
            isStartofBattle = true;
            break;
        }
    }
    
    // Split the party health line by pipes "|"
    vector<string> creatureHealthBits = splitString(lines[pipeLine], "|");
    // This gives us something like the vector description below
    /* { "",
     " *Jackal     10/10",
     " Yaminon    17/17",
     " Megapode   13/13",
     " Jackal     10/10",
     "" } */
    // You can see that the first and last pipe caused empty strings
    
    // Now, go through this information and pull out info for each entry
    // We do 1 to <creatureHealthBits.size()-1 because we only need the middle
    //    four elements (and are skipping the empty ones.
    
    
    // *DETERMINE IF BATTLE HAS ENDED*
    
    string itemLine;
    
    for (int i = 0; i < lines.size(); i++)
    {
        itemLine = lines[i];
        
        //Store characters
        char c0 = itemLine[0];
        char c1 = itemLine[1];
        char c2 = itemLine[2];
        char c3 = itemLine[3];
        char c4 = itemLine[4];
        
        
        
        if (c0 == 'Y' && c1 == 'o' && c2 == 'u' && c3 == ' ' && (c4 == 'r' || c4 == 'd'))
        {
            isEndofBattle = true;
        }
    }
    
    
    // * INITIALIZE creatureParty *
    //first element (0) is empty; the rest (1,2,3,4) are four slots for four party creatures
    //to make it easier to comprehend
    
    for(int j = 1; j < PARTY_SIZE; j++)
    {
        creatureParty[j] = creatureHealthBits[j];
    }
    
    //* STORING NAMES, LEVELS, ATTACK ELEMENTS, CURRENT ATTACKS, & CURRENT HEALTHS in arrays *
    
    //STORE INFO ABOUT ACTIVE CREATURE
    bool isActive; //true when name starts with '*'
    string activeName = "";
    int activeLevel  = 0;
    int activeHealth = 0;
    string activeAtkElement  = "";
    string activeWeakElement = "";
    string activeStrElement  = "";
    //int activeMaxHealth = 0;
    int activeSlot = 0; // [1, 2, 3, 4]
    
    for (int i = 1; i < PARTY_SIZE; i++)
    {
        isActive = false;
        // cout just for testing
        //cout << creatureHealthBits[i] << "\n";
        
        // We are putting the string into a stringStream so we can use >>
        stringstream ss;
        ss << creatureParty[i];
        
    //STORE NAME-LEVEL
        string name_level;
        ss >> name_level; // name = "*Jackal"
        
        //if this creature is active
        if(name_level[0] == '*')
        {
            isActive = true;
            stringstream nameSS; //just to remove asterisk from name
            nameSS << name_level;
            
            char asterisk;
            nameSS >> asterisk;
            
            nameSS >> name_level;
        }
        
    //STORE LEVEL & RENAME NAME
        string name = "";
        int level = 0;
        
        stringstream nameSS;
        nameSS << name_level;
        
        char firstC; //takes first character
        nameSS >> firstC;
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
            nameSS >> dash;
            while(dash != '-') //skip characteres until reaches '-'
            {
                name += dash;
                nameSS >> dash;
            }
            nameSS >> level; //parse out level#
        }
        //parseHelp.parseNameAndLevel(name, level, name_level);
        
        partyNames[i] = name;
        partyLevels[i] = level;
        
    //STORE HEALTH
        int health;
        ss >> health; // health = 10
        partyHealths[i] = health;
        
        // Get rid of the slash char
        //char slash;
        //ss >> slash; // slash = '/'
        
        // get max health
        //int maxHealth;
        //ss >> maxHealth; // maxHealth = 10
        
    //STORE ATK, WEAK, & STR ELEMENTS
        char c = name[0];
        int typeNum = c - 'A';
    //STORE ATTACK ELEMENT
        string atkElement;
        int atkElement_Num = CreatureType::TYPES[typeNum].getElementalAttackType();
        atkElement = CreatureType::elementName(atkElement_Num, 0);
        partyAtkElements[i] = atkElement;
    
    //STORE WEAK ELEMENT
        string weakElement;
        int weakElement_Num = CreatureType::TYPES[typeNum].getElementalWeakness();
        weakElement = CreatureType::elementName(weakElement_Num, 0);
        partyWeakElements[i] = weakElement;
    
    //STORE STRENGTH ELEMENT
        string strElement;
        int strElement_Num = CreatureType::TYPES[typeNum].getElementalStrength();
        strElement = CreatureType::elementName(strElement_Num, 0);
        partyStrElements[i] = strElement;
        
        
    //if this creature is active
        if(isActive)
        {
            activeName = name;
            activeLevel = level;
            activeHealth = health;
            activeAtkElement = atkElement;
            //activeMaxHealth = maxHealth;
            activeSlot = i;
        }
    }
    
    // * STORE INFO ABOUT ENEMY*
    
    string enemyName = "";
    int enemyLevel      = 0;
    int enemyAttack     = 0;
    int enemyBaseAttack = 0;
    int enemyHealth     = 0;
    string enemyAtkElement  = "";
    string enemyWeakElement = "";
    string enemyStrElement  = "";
    
//[FIRST!] STORE ENEMY DAMAGE TO ACTIVE CREATURE
    string enemyAttackLine;
    stringstream enemySS;
    const int NUM_OF_SKIPS = 8; //number of words to skip before attack damage is read
    
    for (int i = 0; i < lines.size(); i++)
    {
        enemyAttackLine = lines[i];
        
        // Store the first char of line in the char c
        char c1 = enemyAttackLine[0];
        char c2 = enemyAttackLine[1];
        
        //'E' for Enemy
        if (c1 == 'E' && c2 == 'n')
        {
            break;
        }
    }
    enemySS << enemyAttackLine;
    int enemyDamage = 0;
    string skip;
    
    if(isStartofBattle)
    {
        //sets partyDamages to default
        for(int i = 1; i < PARTY_SIZE; i++)
        {
            partyDamages[i] = 0;
        }
    }
    else
    {
        for(int j = 0; j < NUM_OF_SKIPS; j++)
        {
            enemySS >> skip;
        }
        enemySS >> enemyDamage;
        partyDamages[activeSlot] = enemyDamage;
        //cout << "Attack damage: " << enemyATK << "\n";
    }

    
//STORE ENEMY NAME & LEVEL
    string enemyName_Level = "";
    if(isStartofBattle)
    {
        //"It's an enemy **** !"
        vector<string> enemyNameLevelLine = splitString(newBattleLine, " ");
        enemyName_Level = enemyNameLevelLine[3];
    }
    else if(!isEndofBattle) //during battle
    {
        //"Enemy **** attacks..."
        vector<string> enemyNameLevelLine = splitString(enemyAttackLine, " ");
        enemyName_Level = enemyNameLevelLine[1];
    }
    
    stringstream enemyNameSS;
    enemyNameSS << enemyName_Level;
    
    char firstC; //takes first character
    enemyNameSS >> firstC;
    enemyName = firstC;
    
    //ONLY for C-lacanth (exception)
    if(firstC == 'C')
    {
        char dash;
        enemyNameSS >> dash;
        enemyName += dash; //this '-' not be mistaken as the '-' before level#
    }
    else
    {
        char dash;
        enemyNameSS >> dash;
        while(dash != '-') //skip characteres until reaches '-'
        {
            enemyName += dash;
            enemyNameSS >> dash;
        }
        enemyNameSS >> enemyLevel; //parse out level#
    }

//STORE ENEMY ATTACK AT CURRENT LEVEL
    char c = enemyName[0];
    int enemyTypeNum = c - 'A';
    
    enemyBaseAttack = CreatureType::TYPES[enemyTypeNum].getAttackBase();
    enemyAttack = enemyBaseAttack + (CreatureType::TYPES[enemyTypeNum].getAttackPerLevel() * enemyLevel);
    
//STORE ENEMY ATK ELEMENT
    
//STORE ENEMY WEAK ELEMENT
    
//STORE ENEMY STR ELEMENT
    
    // cout for testing only
    
    for (int i = 1; i < PARTY_SIZE; i++)
    {
        cout << "\n";
        cout << "Name: " << partyNames[i] << " | Health: " << partyHealths[i] << " | Level: " << partyLevels[i] << "\n";
        cout << "AtkElement: " << partyAtkElements[i] << " | WeakElement: " << partyWeakElements[i] << " | StrElements: "<< partyStrElements[i] << "\n";
        //cout << "Damaged by: " << partyDamages[i] << "\n";
    }
    
    //cout for testing
    //cout << "Active: #" << activeNum << " " << activeName << " " << activeHealth << "/" << activeMaxHealth << "\n";
    
    // This is something else you can do ONLY for testing.
    // In a previous post, I recommended #including CreatureType.h so that you
    //    can make a Trainer::decide() function that will make the right
    //    decision if you have perfect data about creature types.
    // While developing, you can use the static vector<CreatureType> TYPES to
    //    get that perfect information, though you will eventually need to write
    //    your own function to learn that information from the data that is sent
    //    in via stringstream& situation.
    
    /*
     // This makes ct a copy of the CreatureType for Creature 0 (the Axolotyl).
     // All CreatureType and Element names start with a different letter of the
     //    alphabet (CreatureType is A-Z, Element is A-H).
     CreatureType ct = CreatureType::TYPES[0];
     // You can then get the type of ct, which is 0 because it's the Axolotyl
     int type = ct.getType();
     // You can also directly get info from CreatureTypes::TYPES elements.
     int elementalWeakness3 = CreatureType::TYPES[3].getElementalWeakness();
     */
    
    // Output for human player
    // Instead of doing this, you will need to replace the code of Trainer
    // to parse the input situation and create a proper response,
    // (like "a" for attack or "s3" to swap to the creature in slot 3).
    cout << situationString;

    /*
     * This line is basically what your AI chooses to do
     * When first playing, you may type in your move.
     * For both the core and reach portions, however, you need to
     * comment out "cin >> response" and instead have this function
     * generate a response string.
     */
    
    
    //THE DECISION BEGINS HERE!!!
    SwapOrAttack swapOrAttack;
    
    string response = "a";
    //cin >> response;
    
    //every start of battle, swap
   
    if (isStartofBattle)
    {
        response = swapOrAttack.swapToHighestHealth(partyHealths, activeSlot);
        return response;
    }
    else if (isEndofBattle)
    {
        response = "r";
        return response;
    }
    if (!swapOrAttack.isGonnaDie(activeHealth, enemyDamage))
    {
        response = "a";
    }
    else
    {
        response = swapOrAttack.swapToHighestHealth(partyHealths, activeSlot);
        
        //if next turn any one of other creatures will swap, thus making swapping to loop infinite!
        if(swapOrAttack.areOthersGonnaDie(partyHealths, partyDamages))
        {
            //just risk it!
            response = "a";
            
            //if active already fainted
            if(activeHealth == 0)
            {
                //send other to the front line anyway
                response = swapOrAttack.swapToHighestHealth(partyHealths, activeSlot);
            }
        }
    }
    return response;
}


void Trainer::finalSituation(stringstream& situation) {
    cout << situation.str();
    
}


vector<string> Trainer::splitString(string inString, string splitOn) {
    vector<string> elements;
    string substring;
    
    size_t splitIndex = inString.find(splitOn),
    substrStart = 0;
    
    // keep searching until no spaces remain
    while (splitIndex != string::npos) {
        substring = inString.substr(substrStart, splitIndex - substrStart);
        elements.push_back(substring);
        substrStart = splitIndex + 1;
        // start at next character after finding substring
        splitIndex = inString.find(splitOn, substrStart);
    }
    // add last element to vector
    substring = inString.substr(substrStart, inString.length() - substrStart);
    elements.push_back(substring);
    
    return elements;
}