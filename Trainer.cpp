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

//to count moves
//static int moveCount = 0;

string Trainer::makeMove(stringstream& situation)
{
    //default
    isStartofBattle = false;
    isEndofBattle   = false;
    atkBst_InPlay   = false; //tells whether atkBst is being used
    defBst_InPlay   = false; //tells whether defBst is being used
    canFinishOff    = false; //will explain later
    canScrollOff    = false; //will explain later
    
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
    
    
// * DETERMINE IF BATTLE HAS ENDED *
    
    string battleEndLine;
    
    for (int i = 0; i < lines.size(); i++)
    {
        battleEndLine = lines[i];
        
        //Store characters
        char c0 = battleEndLine[0];
        char c1 = battleEndLine[1];
        char c2 = battleEndLine[2];
        char c3 = battleEndLine[3];
        char c4 = battleEndLine[4];
        char c5 = battleEndLine[5];
        char c6 = battleEndLine[6];
        
        if (c0 == 'Y' && c1 == 'o' && c2 == 'u' && c3 == ' ' && ((c4 == 'r' && c5 == 'e' && c6 == 'c') || c4 == 'd'))
        {
            isEndofBattle = true;
        }
    }
    
    // * DETERMINES WHETHER AtkBst OR DefBst IS IN PLAY *
    //below will parse and determine whether either atkbst or defbst items are being used
    string boostline;
    
    for(int i = 0; i < lines.size(); i++)
    {
        boostline = lines[i];
        char c1 = boostline[3];
        char c2 = boostline[4];
        char c3 = boostline[5];
        
        if(c1 == 'B' && c2 == 's' && c3 == 't')
        {
            break;
        }
    }
    vector<string> boostArray = splitString(boostline, " ");
    
    if(boostArray[0] == "AtkBst")
    {
        atkBst_InPlay = true;
    }
    if(boostArray[0] == "DefBst")
    {
        defBst_InPlay = true;
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
    bool isActive;
    string activeName   = "";
    int activeLevel     = 0;
    int activeHealth    = 0;
    int activeMaxHealth = 0;
    int activeAttack    = 0;
    string activeAtkElement  = "";
    string activeWeakElement = "";
    string activeStrElement  = "";
    int activeRest = 0;
    int activeSlot = 0;
    
    for (int i = 1; i < PARTY_SIZE; i++)
    {
        isActive = false;
        
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
            nameSS >> dash;
            name += dash; //this '-' not be mistaken as the '-' before level#
        }
        
        char dash;
        nameSS >> dash;
        while(dash != '-') //skip characteres until reaches '-'
        {
            name += dash;
            nameSS >> dash;
        }
        nameSS >> level; //parse out level#
        //parseHelp.parseNameAndLevel(name, level, name_level);
        
        partyNames[i] = name;
        partyLevels[i] = level;
        
        //STORE HEALTH
        int health;
        ss >> health; // health = 10
        partyHealths[i] = health;
        
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
        
        //STORE ATTACK AT CURRENT LEVEL
        int attack;
        int attackBase = CreatureType::TYPES[typeNum].getAttackBase();
        attack = attackBase + (CreatureType::TYPES[typeNum].getAttackPerLevel() * level);
        
        if(atkBst_InPlay)
        {
            attack *= SwapOrAttack::DOUBLE_FACTOR;
        }
        
        partyAttacks[i] = attack;
        
        //STORE REST INCREASE //Store party's rests' health increases
        int rest;
        rest = CreatureType::TYPES[typeNum].getHealthRestIncrease();
        partyRests[i] = rest;
        
        
        //STORE MAX HEALTH //Store party's max healths
        int maxHealthBase = CreatureType::TYPES[typeNum].getHealthMaxBase();
        int maxHealth = maxHealthBase + (CreatureType::TYPES[typeNum].getHealthMaxPerLevel() * level);
        partyMaxHealths[i] = maxHealth;

        
        //if this creature is active
        if(isActive)
        {
            activeName = name;
            activeLevel = level;
            activeHealth = health;
            activeAttack = attack;
            activeAtkElement = atkElement;
            activeWeakElement = weakElement;
            activeStrElement = strElement;
            activeRest = rest;
            activeMaxHealth = maxHealth;
            activeSlot = i;
        }
    }
    
// * STORE INFO ABOUT ENEMY*
    
    string enemyName = "";
    int enemyLevel      = 0;
    int enemyAttack     = 0;
    int enemyBaseAttack = 0;
    int enemyMaxHealth  = 0;
    int enemyMaxHealthBase  = 0;
    string enemyAtkElement  = "";
    string enemyWeakElement = "";
    string enemyStrElement  = "";
    
    //FIND LINE WITH "ENEMY"
    string enemyAttackLine;
    stringstream enemySS;
    
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
    
    //STORE ENEMY NAME & LEVEL
    string enemyName_Level = "";
    if(isStartofBattle)
    {
        //"It's an enemy **** !"
        vector<string> enemyNameLevelLine = splitString(newBattleLine, " ");
        enemyName_Level = enemyNameLevelLine[3];
    }
    else //during battle
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
    
    char dash;
    enemyNameSS >> dash;
    while(dash != '-') //skip characteres until reaches '-'
    {
        enemyName += dash;
        enemyNameSS >> dash;
    }
    enemyNameSS >> enemyLevel; //parse out level#

    //STORE ENEMY ATTACK AT CURRENT LEVEL
    char c = enemyName[0];
    int enemyTypeNum = c - 'A';
    
    enemyBaseAttack = CreatureType::TYPES[enemyTypeNum].getAttackBase();
    enemyAttack = enemyBaseAttack + (CreatureType::TYPES[enemyTypeNum].getAttackPerLevel() * enemyLevel);
    
    if(defBst_InPlay)
    {
        enemyAttack /= SwapOrAttack::DOUBLE_FACTOR;
    }
    
    //STORE ENEMY MAX HEALTH
    enemyMaxHealthBase = CreatureType::TYPES[enemyTypeNum].getHealthMaxBase();
    enemyMaxHealth = enemyMaxHealthBase + (CreatureType::TYPES[enemyTypeNum].getHealthMaxPerLevel() * enemyLevel);

     //first calculate enemyMaxHealth into enemyCurrentHealth to keep track during battle
    if(isStartofBattle)
    {
        enemyCurrentHealth = enemyMaxHealth;
    }
    else if(isEndofBattle)
    {
        enemyCurrentHealth = 0;
    }
    
    //STORE ENEMY ATK ELEMENT
    int atkElement_Num = CreatureType::TYPES[enemyTypeNum].getElementalAttackType();
    enemyAtkElement = CreatureType::elementName(atkElement_Num, 0);

    
    //STORE ENEMY WEAK ELEMENT
    int weakElement_Num = CreatureType::TYPES[enemyTypeNum].getElementalWeakness();
    enemyWeakElement = CreatureType::elementName(weakElement_Num, 0);

    
    //STORE ENEMY STR ELEMENT
    int strElement_Num = CreatureType::TYPES[enemyTypeNum].getElementalStrength();
    enemyStrElement = CreatureType::elementName(strElement_Num, 0);
    
    
     // * STORE PARTY RATIOS OF TURNS TO KILL : TURNS TO DIE *
    //below stores bools of whether each creature can win against enemy
    if(!isEndofBattle)
    {
        SwapOrAttack ratios;
        
        for(int i = 1; i < PARTY_SIZE; i++)
        {
            //regular ratios
            bool canWin = ratios.calculateWinLose(enemyAttack, enemyAtkElement, enemyWeakElement, enemyStrElement, enemyCurrentHealth, partyAttacks, partyAtkElements, partyWeakElements, partyStrElements, partyHealths, i, activeSlot, false);
            
            partyWinOrLose[i] = canWin;
            
            //used for last-resort strategy
            bool hope = ratios.calculateWinLose(enemyAttack, enemyAtkElement, enemyWeakElement, enemyStrElement, enemyCurrentHealth, partyAttacks, partyAtkElements, partyWeakElements, partyStrElements, partyMaxHealths, i, activeSlot, true);
            
            partyLastHopes[i] = hope;
        }
    }
    //default
    else
    {
        for(int i = 1; i < PARTY_SIZE; i++)
        {
            partyWinOrLose[i] = false;
        }
    }
    
//* STORE COUNTS FOR ALL ITEMS *
    
    string itemLine;
    string scrollLine;
    
    for (int i = 0; i < lines.size(); i++)
    {
        itemLine = lines[i];
        
        char exclamPoint = itemLine[0];
        if(exclamPoint == '!')
        {
            scrollLine = lines[i + 1];
            break;
        }
    }
    
    //parsing in first line of items
    vector<string> itemLineList = splitString(itemLine, "!");
    const int LIST_SIZE = 7;
    
    for(int i = 1; i < LIST_SIZE - 1; i++)
    {
        stringstream itemSS;
        itemSS << itemLineList[i];
        
        char colon;
        itemSS >> colon;
        while(colon != ':') //skip characters until reaches ':'
        {
            itemSS >> colon;
        }
        itemSS >> itemList[i - 1]; //# of certain item
        
        //testing
        //cout << "Item #" << i << ": " << itemList[i - 1] << "\n";
    }
    
    //parsing in second line of scrolls
    vector<string> scrollLineList = splitString(scrollLine, "!");
    const int SCROLL_SIZE = 11;
    
    for(int i = 2; i < SCROLL_SIZE - 1; i++)
    {
        stringstream scrollSS;
        scrollSS << scrollLineList[i];
        
        char colon;
        scrollSS >> colon;
        while(colon != ':') //skip characters until reaches ':'
        {
            scrollSS >> colon;
        }
        scrollSS >> scrollList[i - 2]; //# of scrolls
        
        //testing
        //cout << "Scroll #" << i << ": " << scrollList[i - 2] << "\n";
    }
    cout << situationString;

    /**///testing
    /**/cout << "\n";
    /**/cout << "\n";
    /**/cout << "\n";
    /**/cout << "Enemy name: " << enemyName << " | Health: " << enemyCurrentHealth << "/" << enemyMaxHealth << " | Level: " << enemyLevel << " | Attack: " << enemyAttack << "\n";
    /**/cout << "AtkElement: " << enemyAtkElement << " | WeakElement: " << enemyWeakElement << " | StrElements: "<< enemyStrElement << "\n";
    
    /**/cout << "\n";
    /**/for (int i = 1; i < PARTY_SIZE; i++)
    /**/{
    /**/    cout << "\n";
    /**/    cout << "Name: ";
            if(i == activeSlot)
            {
                cout << "*";
            }
            cout << partyNames[i] << " | Health: " << partyHealths[i] << "/" << partyMaxHealths[i] << " | Level: " << partyLevels[i] << " | Attack: " << partyAttacks[i] << " | Rest: " << partyRests[i] << "\n";
    /**/    cout << "AtkElement: " << partyAtkElements[i] << " | WeakElement: " << partyWeakElements[i] << " | StrElements: "<< partyStrElements[i] << "\n";
            if(partyWinOrLose[i])
            {
                cout << "Winner! \n";
            }
            else
            {
                cout << "Loser \n";
            }
        /**/}
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //THE DECISION BEGINS HERE!!!
    SwapOrAttack swapOrAttack;
    Capture capture;
    
    
    //default response = attack
    string response = "a";
    
    int predictedAttack = swapOrAttack.getFactoredAttack(partyAttacks[activeSlot], enemyStrElement, enemyWeakElement, partyAtkElements[activeSlot]);
    int scrollDamage = Item::SCROLL_DAMAGE;
    int scrollPos = CreatureType::TYPES[enemyTypeNum].getElementalWeakness();
    
    //every start of battle, swap
    if (isStartofBattle)
    {   //canFinishOff will be true when the active creature can either finish enemy this turn or any turn by itself without swapping/dying
        //HIGH PRIORITY, WILL OVERRIDE ALL RESPONSES
        //if active creature can take this
        if(enemyCurrentHealth <= predictedAttack || partyWinOrLose[activeSlot])
        {
            canFinishOff = true;
        }
        //like canFinishOff, canScrollOff will be true when scroll(if any)can finish enemy this turn (both these bools are included way below, in other parts of loop)
        //determine whether scroll can finish off enemy
        //HIGH PRIORITY, WILL OVERRIDE ALL RESPONSES
        if(scrollList[scrollPos] > 0 && enemyCurrentHealth <= scrollDamage)
        {
            canScrollOff = true;
        }
            //below are swap functions, the response will change or not as it goes down one by one (bottom are prioritized)
            //swapping decisions
            swapOrAttack.swapToHighestHealth(partyHealths, partyAttacks, activeSlot, response);
            //cout << response << "\n";
            swapOrAttack.swapToNormal(enemyAtkElement, enemyStrElement, partyAtkElements, partyWeakElements, partyHealths, activeSlot, response);
            //cout << response << "\n";
            swapOrAttack.swapToOffensive(enemyAtkElement, enemyWeakElement, partyAtkElements, partyWeakElements, partyHealths, activeSlot, response);
            //cout << response << "\n";
            swapOrAttack.swapToDefensive(enemyAtkElement, enemyStrElement, partyAtkElements, partyStrElements, partyHealths, activeSlot, response);
            //cout << response << "\n";
            //swapOrAttack.swapToWinner(partyWinOrLose, partyHealths, activeSlot, response);
            //cout << response << "\n";
            swapOrAttack.swapToWeakWinner(partyWinOrLose, partyLevels, activeSlot, response);
            //cout << response << "\n";
            /*
            if(swapOrAttack.allIsNotWell(partyWinOrLose))
            {
                swapOrAttack.swapToBestLoser(enemyAttack, enemyCurrentHealth, enemyAtkElement, enemyWeakElement, enemyStrElement, partyAttacks, partyHealths, partyAtkElements, partyWeakElements, partyStrElements, activeSlot, response);
                cout << response << "\n";
            }
             */
        //this is just regular scroll response, whether canScrollOff is true or not
        if(scrollList[scrollPos] > 0 && predictedAttack < scrollDamage)
        {
            swapOrAttack.useScroll(scrollPos, response);
        }
        
        //emergency
    }
    
    //end of battle
    else if (isEndofBattle)
    {
        //default response (rest)
        response = "r";
        
        //if have defBst/atkBst
        if(itemList[1] > 0 || itemList[2] > 0)
        {
            if(itemList[2] > itemList[1])
            {
                response = "ab";
            }
            else
            {
                response = "db";
            }
        }
        
        //if active health's is low
        int healthDiff = partyMaxHealths[activeSlot] - activeHealth;
        if(partyRests[activeSlot] <= healthDiff)
        {
            response = "r";
            if(Item::POTION_HEALTH <= healthDiff && itemList[0] > 0) //potion
            {
                response = "po";
            }
        }
        
        //if have revive
        if(itemList[3] > 0)
        {
            swapOrAttack.reviveMostUsefulCreature(partyHealths, partyAttacks, response);
        }
        
        //if have collar
        if(itemList[4] > 0)
        {
            capture.captureCreature(enemyMaxHealth, enemyAttack, partyMaxHealths, partyAttacks, response);
        }
    }
    
    //during battle
    else if(!isStartofBattle && !isEndofBattle)
    {
        //if active creature can take this
        if(enemyCurrentHealth <= predictedAttack || partyWinOrLose[activeSlot])
        {
            canFinishOff = true;
        }
        
        //determine whether scroll can finish off enemy
        if(scrollList[scrollPos] > 0 && enemyCurrentHealth <= scrollDamage)
        {
            canScrollOff = true;
        }
        
        //if active creature will not faint next turn
        if (!swapOrAttack.isGonnaDie(activeHealth, activeStrElement, activeWeakElement, enemyAttack, enemyAtkElement))
        {
            response = "a";
            
            //scroll
            if(scrollList[scrollPos] > 0 && predictedAttack < scrollDamage)
            {
                swapOrAttack.useScroll(scrollPos, response);
            }
            
            if(!partyWinOrLose[activeSlot])
            {
                if(itemList[2] > 0)
                {
                    response = "ab";
                }
                if(itemList[1] > 0)
                {
                    response = "db";
                }
            }
        }
        //if active creature will faint next turn
        else if(!swapOrAttack.isFainted(activeSlot, partyHealths) && swapOrAttack.isGonnaDie(activeHealth, activeStrElement, activeWeakElement, enemyAttack, enemyAtkElement))
        {
            swapOrAttack.swapToHighestHealth(partyHealths, partyAttacks, activeSlot, response);
            
            swapOrAttack.swapToNormal(enemyAtkElement, enemyStrElement, partyAtkElements, partyWeakElements, partyHealths, activeSlot, response);
            
            swapOrAttack.swapToOffensive(enemyAtkElement, enemyWeakElement, partyAtkElements, partyWeakElements, partyHealths, activeSlot, response);
            
            swapOrAttack.swapToDefensive(enemyAtkElement, enemyStrElement, partyAtkElements, partyStrElements, partyHealths, activeSlot, response);
            
            //swapOrAttack.swapToWinner(partyWinOrLose, partyHealths, activeSlot, response);
            swapOrAttack.swapToWeakWinner(partyWinOrLose, partyLevels, activeSlot, response);
            //cout << response << "\n";
            
            //sole survivor
            if(swapOrAttack.isLastCreatureStanding(partyHealths, activeSlot))
            {
                response = "a";
                
                //revive
                if(itemList[3] > 0)
                {
                    swapOrAttack.reviveMostUsefulCreature(partyHealths, partyAttacks, response);
                }
            }
            //if next turn any one of other creatures will swap, thus making swapping to loop infinite!
            else if(swapOrAttack.areOthersGonnaDieAfterNextTurn(partyHealths, partyStrElements, partyWeakElements, enemyAttack, enemyAtkElement))
            {
                response = "a";
                
                if(scrollList[scrollPos] > 0 && predictedAttack < scrollDamage)
                {
                    swapOrAttack.useScroll(scrollPos, response);
                }
                
                if(!partyWinOrLose[activeSlot])
                {
                    if(itemList[2] > 0)
                    {
                        response = "ab";
                    }
                    if(itemList[1] > 0)
                    {
                        response = "db";
                    }
                }
                
                 //(in-progress) LAST-HOPE STRATEGY TO END INFINITE SWAPPING!!!
                /* ONLY WORKS IF AT LEAST ONE IN PARTY CAN DEFEAT ENEMY WHEN AT [FULL HEALTH - 1 x enemyAttack] (since enemy will always attack the swapped creature)
                    When each is party is not at Max Health and cannot defeat enemy singlehandedly,
                    find in the party at least one that can defeat enemy when at "full health".
                    if there is, and it fainted, revive it immediately
                    once it's alive, swap to any creature(that won't die when swapped) other than that "last Hope" creature or active creature.
                    each turn, will automatically keep swapping until "last hope is at full health".
                    Once it does, it will hopefully update the partyWinOrLose bool array,
                    which stores each creature's true/false of defeating the enemy without dying.
                    And the decision loop will normally swap to that creature.
                    If, however, it still cannot win, or if none can win even at "full health" in the
                    first place, the decision loop will run as it is.
                    Still does not fix the occasional infinite swapping (i'll keep looking into that).
                 */
                /*
                if(swapOrAttack.allIsNotWell(partyWinOrLose))
                {
                    if(swapOrAttack.thereisStillHope(partyLastHopes, lastHope))
                    {
                        //if hope has fainted
                        if(swapOrAttack.isFainted(lastHope, partyHealths))
                        {
                            //rekindle!
                            swapOrAttack.reviveCommand(lastHope, response);
                        }
                        else
                        {
                            //i really hope this works!
                            swapOrAttack.swapToStall(partyHealths, partyStrElements, partyWeakElements, enemyAttack, enemyAtkElement, lastHope, activeSlot, response);
                            //cout << "IT WORKED!!!!!!!!!!!!!!!!!!!!" << "\n";
                        }
                        //cout << "IT WORKED!!!!!!!!!!!!!!!!!!!!" << "\n";
                        
                        //cout << "Last hope: " << partyNames[lastHope] << "\n";
                    }
                }
                 */
            }
        }
        
        //if already fainted
        else if(swapOrAttack.isFainted(activeSlot, partyHealths))
        {
            canFinishOff = false;
            
            swapOrAttack.swapToHighestHealth(partyHealths, partyAttacks, activeSlot, response);
            
            swapOrAttack.swapToNormal(enemyAtkElement, enemyStrElement, partyAtkElements, partyWeakElements, partyHealths, activeSlot, response);
            
            swapOrAttack.swapToOffensive(enemyAtkElement, enemyWeakElement, partyAtkElements, partyWeakElements, partyHealths, activeSlot, response);
            
            swapOrAttack.swapToDefensive(enemyAtkElement, enemyStrElement, partyAtkElements, partyStrElements, partyHealths, activeSlot, response);
            
            //swapOrAttack.swapToWinner(partyWinOrLose, partyHealths, activeSlot, response);
            swapOrAttack.swapToWeakWinner(partyWinOrLose, partyLevels, activeSlot, response);
            //cout << response << "\n";
        }
    }
    /* at end of decision loop,
        if either/both canFinishOff or canScrollOff are true,
        response will automatically be either attack/scroll
     */
    if(canFinishOff)
    {
        response = "a";
        if(canScrollOff && predictedAttack < scrollDamage)
        {
            swapOrAttack.useScroll(scrollPos, response);
        }
    }
    else if(canScrollOff)
    {
        swapOrAttack.useScroll(scrollPos, response);
    }
    
    //if final response is attack
    if(response == "a")
    {
        //deduct active creature's attack from enemy's current health
        enemyCurrentHealth -= swapOrAttack.getFactoredAttack(activeAttack, enemyStrElement, enemyWeakElement, activeAtkElement);
    }
    
    //if final response is scroll
    //determine whether scroll has been used, in order to update enemy's current health
    char c1 = response[0];
    char c2 = response[1];
    if(c1 == 's' && (c2 == 'a' || c2 == 'b' || c2 == 'c' || c2 == 'd' || c2 == 'e' || c2 == 'f' || c2 == 'g' || c2 == 'h'))
    {
        enemyCurrentHealth -= Item::SCROLL_DAMAGE;
    }
    
    //moveCount++;
    //cout << "# of moves: " << moveCount << "\n";
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