//
//  Item.cpp
//  CreatureTrainer
//
//  Created by Jeremy Gibson on 11/28/14.
//  Copyright (c) 2014 Jeremy Richard Gibson. All rights reserved.
//

#include "Item.h"
#include "CreatureType.h"
#include "EECSRandom.h"
#include "Party.h"
#include <iostream>
#include <sstream>
using namespace std;

//const string Item::ITEM_NAMES[] = {"NOTHING", "POTION", "SHIELD", "DMG-BOOST",
//    "REVIVE", "LASSO", "SCROLL-A","SCROLL-B","SCROLL-C","SCROLL-D","SCROLL-E",
//    "SCROLL-F","SCROLL-G", "SCROLL-H" };
//
//const string Item::ITEM_CODES[] = {"NOTHING", "POTION", "SHIELD", "DMG-BOOST",
//    "REVIVE", "LASSO", "S-A","S-B","S-C","S-D","S-E",
//    "S-F","S-G", "S-H" };

const string Item::ITEM_NAMES[] = { "Potion", "DefBst", "AtkBst", "Revive",
    "Collar", "Air Scroll", "Bombast Scroll", "Cool Scroll", "Dark Scroll",
    "Earth Scroll", "Funk Scroll", "Gamma Scroll", "Hammer Scroll" };

const string Item::ITEM_CODES[] = { "po", "db", "ab", "re", "co", "sa", "sb",
    "sc", "sd", "se", "sf", "sg", "sh" };

const int Item::ITEM_CHANCE[] = { 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

Item::Item(){
    for (int i= 0; i < NUM_ITEMS; ++i) {
        itemCounts[i] = STARTING_ITEM_COUNT;
    }
}


// Rarities are set by NOTHING_CHANCE and ITEM_CHANCE[]
string Item::randomItem(){
    int totalChances = 0;
    for (int i=0; i<NUM_ITEMS; i++) {
        totalChances += ITEM_CHANCE[i];
    }
    totalChances += NOTHING_CHANCE;
    
    int num = EECSRandom::range(0, totalChances);
    
    for (int i=0; i<NUM_ITEMS; i++) {
        if (num < ITEM_CHANCE[i]) {
            return ITEM_NAMES[i];
        }
        num -= ITEM_CHANCE[i];
    }
    
    return "nothing";
}
    
//returning temp for compilation
string Item::getItemName(int pad){
    return "temp";
}

int Item::getItemCount(int index) const {
    if (index < 0 || index > NUM_ITEMS) {
        return 0;
    }
    return itemCounts[index];
}

int Item::getIndexForCode(string code) {
    for (int i = 0; i < NUM_ITEMS; ++i) {
        if (ITEM_CODES[i] == code) {
            return i;
        }
    }
    return 0;
}

int* Item::getItemArray(){
    return itemCounts;
}

//Not sure how we should do this
//We might need to pass in the attacker and defender creatures
//And since we want the boosts to be temporary, we won't want to modify 
//the actual attack damage
//Also need to write a printHelper for this

//in battle, have int attackBoost = 4, decrement it each time a turn passes
string Item::useItem(string itemCommand, Party& party, Creature& enemy,
               stringstream & ss, int& illegal) {
//string Item::useItem(int itemIndex, int itemCounts[NUM_ITEMS], Creature& active,
//    Creature& friendly, Creature& enemy, stringstream & ss, int& illegalMoves){
    int badMove = 0;
    
    // Parse info from the itemCommand
    int itemNum = -1;
    string iName;
    string iCode;
    int cNum;
    
    // Find the itemNum from the itemCommand
    for (int i=0; i<Item::NUM_ITEMS; i++) {
        if (itemCommand[0] == Item::ITEM_CODES[i][0]) {
            if (itemCommand[1] == Item::ITEM_CODES[i][1]) {
                itemNum = i;
                iName = Item::ITEM_NAMES[i];
                iCode = Item::ITEM_CODES[i];
            }
        }
    }
    
    bool useItem = true;
    
    if (itemNum != -1 && itemCounts[itemNum] == 0) {
        ss << "Illegal Move: You tried to use " << Item::ITEM_NAMES[itemNum]
            << ", but you don't have any.\n";
        badMove = -1;
        useItem = false;
    } else {
        // This is a valid item code, and you actually have one to use!
        switch (itemNum) {
            case -1: { // A non-legal Item Command was entered.
                useItem = false;
                badMove = 1;
                break;
            }
                
            case 0: { // Potion(po)
                // Raise the health of the current Creature by POTION_HEALTH
                int h = party.getActiveCreature().getHealthCurr();
                h += Item::POTION_HEALTH;
                party.getActiveCreature().setHealthCurr(h);
                ss << "You used a Potion to raise your active "
                    << party.getActiveCreature().getTypeName()
                    << "'s health" << " by " << Item::POTION_HEALTH << ".\n";
                break;
            }
                
            case 1: { // DefBst(db)
                party.defenseBoostTurns = Item::BOOST_TURNS+1;
                ss << "You used " << Item::ITEM_NAMES[1] << ", which lasts for "
                << "this turn and " << (Item::BOOST_TURNS-1) << " more.\n";
                break;
            }
                
            case 2: { // AtkBst(ab)
                party.attackBoostTurns = Item::BOOST_TURNS+1;
                ss << "You used " << Item::ITEM_NAMES[2] << ", which lasts for "
                << Item::BOOST_TURNS << " turns.\n";
                break;
            }
                
            case 3: { // Revive(re#)
                if (itemCommand.length() < 3) {
                    badMove = true;
                    break;
                }
                // Revive a Creature
                cNum = itemCommand[2] - '1';
                Creature& cr = party.creatures[cNum];
                if (cr.getHealthCurr() > 0) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Revive " << cr.getTypeName()
                        << " in slot " << (cNum+1) << ", but it is not fainted.\n";
                    break;
                }
                cr.setHealthCurr(Item::REVIVE_HEALTH);
                ss << "You revived " << cr.getTypeName()
                    << " in slot " << (cNum+1) << "!\n";
                break;
            }
                
            case 4: { // Collar(co)
                // This will only work if the enemy Creature has fainted, which
                //   means you can only do it on the turn between enemies.
                if (enemy.getHealthCurr() > 0) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Collar " << enemy.getTypeName()
                        << ", but it is not fainted.\n";
                    break;
                }
                if (itemCommand.length() < 3) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Collar " << enemy.getTypeName()
                        << ", but did not specify a party member to replace.\n";
                    break;
                }
                cNum = itemCommand[2] - '1';
                if (cNum > 3) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Collar " << enemy.getTypeName()
                        << ", but did not specify a valid member to replace.\n";
                    break;
                }
                // This is a valid swap, so make it so #1!
                string oldName = party.creatures[cNum].getTypeName();
                string newName = enemy.getTypeName();
                enemy.setHealthCurr(Item::REVIVE_HEALTH);
                party.creatures[cNum] = enemy;
                
                ss << "You used Collar to swap enemy " << newName
                    << " for your " << oldName << " in slot " << (cNum+1) << ".\n";
                ss << "Your new " << newName << " has " << Item::REVIVE_HEALTH
                    << " health.\n";
                break;
            }
                
            case 5: // Use a scroll!
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12: {
                int element = itemNum - 5;
                if (!Item::scrollDmg(element, Item::SCROLL_DAMAGE, enemy, ss)) {
                    badMove = 1;
                }
                break;
            }
        }
        
        if (useItem && itemNum > -1 && itemNum < Item::NUM_ITEMS) {
            // If there was nothing wrong with the move, use the item.
            itemCounts[itemNum]--;
        }
        
        if (badMove == 1) {
            illegal++;
//            ss << "Illegal move: '" << itemCommand << "' is not a valid!";
            ss << "ERROR: ILLEGAL MOVE\n";
        } else if (badMove == -1) {
            illegal++;
        }
    }
    
    
    /*
    switch (itemCommand[0]) {
        case 'p': // Potion(po)
            if (itemCommand[1] == 'o') {
                if (itemCounts[0] < 1) {
                    ss << "You used a Potion to raise your active creature's health"
                    << " by " << Item::POTION_HEALTH << ".\n";
                    
                }
                // Raise the health of the current Creature by POTION_HEALTH
                int h = party.getActiveCreature().getHealthCurr();
                h += Item::POTION_HEALTH;
                party.getActiveCreature().setHealthCurr(h);
                ss << "You used a Potion to raise your active creature's health"
                    << " by " << Item::POTION_HEALTH << ".\n";
                itemNum = 0;
            } else {
                badMove = true;
            }
            break;
            
        case 'd': // DefBst(db)
            if (itemCommand[1] == 'b') {
                party.defenseBoostTurns = Item::BOOST_TURNS;
                ss << "You used " << Item::ITEM_NAMES[1] << ", which lasts for "
                << Item::BOOST_TURNS << " turns.\n";
                itemNum = 1;
            } else {
                badMove = true;
            }
            break;
            
        case 'a': // AtkBst(ab)
            if (itemCommand[1] == 'b') {
                party.attackBoostTurns = Item::BOOST_TURNS;
                ss << "You used " << Item::ITEM_NAMES[2] << ", which lasts for "
                    << Item::BOOST_TURNS << " turns.\n";
                itemNum = 2;
            } else {
                badMove = true;
            }
            break;
            
        case 'r': // Revive(re#)
            if (itemCommand.length() < 3) {
                badMove = true;
                break;
            }
            if (itemCommand[1] == 'e') {
                // Revive a Creature
                int cNum = itemCommand[2] - '1';
                Creature& cr = party.creatures[cNum];
                if (cr.getHealthCurr() > 0) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Revive " << cr.getTypeName()
                        << " in slot " << (cNum+1) << ", but it is not fainted.";
                    break;
                }
                cr.setHealthCurr(Item::REVIVE_HEALTH);
                ss << "You revived " << cr.getTypeName()
                    << " in slot " << (cNum+1) << "!";
            } else {
                badMove = true;
            }
            break;
            
        case 'c': // Collar(co)
            if (itemCommand[1] == 'o') {
                // This will only work if the enemy Creature has fainted, which
                //   means you can only do it on the turn between enemies.
                if (enemy.getHealthCurr() > 0) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Collar " << enemy.getTypeName()
                        << ", but it is not fainted.";
                    break;
                }
                if (itemCommand.length() < 3) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Collar " << enemy.getTypeName()
                        << ", but did not specify a party member to replace.";
                    break;
                }
                int cNum = itemCommand[2] - '1';
                if (cNum > 3) {
                    badMove = -1;
                    ss << "Illegal Move: You tried to Collar " << enemy.getTypeName()
                        << ", but did not specify a valid member to replace.";
                    break;
                }
                // This is a valid swap, so make it so #1!
                Creature& cOld = party.creatures[cNum];
                enemy.setHealthCurr(Item::REVIVE_HEALTH);
                party.creatures[cNum] = enemy;
                
                ss << "You used Collar to swap enemy " << enemy.getTypeName()
                    << " for your " << cOld.getTypeName() << " in slot " << cNum
                    << ". It now has " << Item::REVIVE_HEALTH << " health\n";
            } else {
                badMove = true;
            }
            break;
            
        case 's': // Use a scroll!
            int scrollElt = itemCommand[1] - 'a';
            int itemNum = scrollElt + 5;
            if (!Item::scrollDmg(scrollElt, Item::SCROLL_DAMAGE, enemy, ss)) {
                badMove = 1;
            }
            break;
    }
    
    if (itemCounts[itemIndex] == 0){
        ss << "ERROR: You have no " << ITEM_NAMES[itemIndex] << "s to use." << endl;
        illegalMoves++;
    }
    else{
        //random number for scroll damage - Will modify
        int dmg = EECSRandom::range(1, 8);
        int addedHealth = .25*friendly.getHealthMax();
        switch (itemIndex){
            //NOTHING
        case 0:
            break;

            //POTION: restores quarter of maxHealth
        case 1:
            ss << "You used a POTION on your " << friendly.getTypeName();
            if (addedHealth > friendly.getHealthMax()){
                friendly.setHealthCurr(friendly.getHealthMax());
                ss << " and it gains " << friendly.getHealthMax() - friendly.getHealthCurr() << " health" << endl;
            }
            else{
                friendly.setHealthCurr(friendly.getHealthCurr() + addedHealth);
                ss << " and it gains " << addedHealth << " health" << endl;
            }
            
           
            
            break;

            //only makes sense to apply boosts to active creature, so check that
            //SHIELD
        case 2:
            if (friendly == active){
                applyBoost(friendly, "SHIELD", ss);
            }
            else{
                ss << "ERROR: Cannot use SHIELD on inactive creature" << endl;
                illegalMoves++;
            }
            break;

            //DMG-BOOST
        case 3:
            if (friendly == active){
                applyBoost(friendly, "DMG-BOOST", ss);
            }
            else{
                ss << "ERROR: Cannot use DMG-BOOST on inactive creature" << endl;
                illegalMoves++;
            }
            break;

            //REVIVE
        case 4:
            if (friendly.getHealthCurr() != 0){
                ss << "ERROR: Cannot use REVIVE on a creature that hasn't yet fainted" << endl;
                illegalMoves++;
            }
            else{
                friendly.setHealthCurr(1);
                ss << "You use REVIVE on your " + friendly.getTypeName() << endl;
                //should he not rest? or should he?
            }
            break;


            //LASSO
        case 5:
            //capture function
            if (enemy.getHealthCurr()){
                ss << "ERROR: Cannot capture a creature that has not yet fainted" << endl;
            }
            else{
                //if lasso is used to capture, it’s 100% chance captured
                int oldIndex = party.getActiveCreatureNum();
                //find index of friendly
                int swapIndex = 0;
                for (int i = 0; i < Party::MAX_PARTY_SIZE; ++i){
                    if (party.creatures[i] == friendly){
                        swapIndex = i;
                    }
                }
                string oldName = friendly.getTypeName();
                //change creature to enemy
                //swap out one of own guys

                //if we swap out active guy, set captured to active
                party.creatures[swapIndex] = enemy;
                //joins team w/ 1 health
                party.creatures[swapIndex].setHealthCurr(1); 
                if (swapIndex == oldIndex){
                    party.setActiveCreatureNum(swapIndex);
                }
                
                ss << "You exchange your " + oldName + " for a ";
                ss << enemy.getTypeName();
                ss << "!\n";       
            }
            break;

            //SCROLL A
        case 6:
            scrollDmg(0, dmg, enemy, ss);
            break;

            //SCROLL B
        case 7:
            scrollDmg(1, dmg, enemy, ss);
            break;

            //SCROLL C 
        case 8:
            scrollDmg(2, dmg, enemy, ss);
            break;

            //SCROLL D
        case 9:
            scrollDmg(3, dmg, enemy, ss);
            break;

            //SCROLL E
        case 10:
            scrollDmg(4, dmg, enemy, ss);
            break;

            //SCROLL F
        case 11:
            scrollDmg(5, dmg, enemy, ss);
            break;

            //SCROLL G
        case 12:
            scrollDmg(6, dmg, enemy, ss);
            break;

            //SCROLL H
        case 13:
            scrollDmg(7, dmg, enemy, ss);
            break;

        default:
            ss << "ERROR: Invalid item selected. You lose your turn" << endl;
            illegalMoves++;
            break;
        }
        --itemCounts[itemIndex];
    }
     */
    
    return "temp";
}


//This is basically Battle::creatureAttack
bool Item::scrollDmg(int scrollElt, int dmg, Creature& defender, stringstream& ss) {
    string dName;

    dName = "Enemy " + defender.getTypeName(0);

    //change to "Your scroll does x damage to Enemy creatureType"???
   
    int damageDone = defender.damage(dmg, scrollElt);
    ss << "You use " << CreatureType::elementName(scrollElt, 0)
        << " Scroll, which does " << damageDone << " damage "
        << "to " << dName << ".\n";

    if (defender.getHealthCurr() == 0) {
        ss << dName << " faints!\n";
    }

    return true;
}

void Item::addItem(string item){
    for (int i = 0; i < NUM_ITEMS; ++i){
        if ((ITEM_NAMES[i] == item) && (itemCounts[i] < 99)){
            ++itemCounts[i];
        }
    }
}