//  Class1.cpp
//  Helper Class1
//
//  These class heaaders are for use in your Trainer class. If you wish to
//  write helper classes, write them in these 5 classes.
//  Use of these classes is optional. You may use none or any or all of them.
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#include "Class1.h"
#include <iostream>
using namespace std;

SwapOrAttack::SwapOrAttack()
{
    
}

int SwapOrAttack::getHighestHealth(int partyHealths[])
{
    int highestSlot = 1;
    int highestHealth = partyHealths[1];
    for(int i = 2; i < PARTY_SIZE; i++)
    {
        if(highestHealth < partyHealths[i])
        {
            highestHealth = partyHealths[i];
            highestSlot = i;
        }
    }
    return highestSlot;
}

string SwapOrAttack::swapToHighestHealth(int partyHealths[], int activeSlot)
{
    int swapSlot = getHighestHealth(partyHealths);
    string response;
    
    //if swapping with other than itself
    if(swapSlot != activeSlot)
    {
        switch(swapSlot)
        {
            case 1:
                response = "s1";
                break;
            case 2:
                response = "s2";
                break;
            case 3:
                response = "s3";
                break;
            case 4:
                response = "s4";
                break;
        }
    }
    //otherwise
    else
    {
        response = "a";
    }
    return response;
}

bool SwapOrAttack::isGonnaDie(int health, int enemyATK)
{
    if (health <= enemyATK)
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::areOthersGonnaDie(int partyHealths[], int partyDamages[])
{
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //hypothesizing future health one turn later
        int remainHealth = partyHealths[i] - partyDamages[i];
        if(!isGonnaDie(remainHealth, partyDamages[i]))
        {
            return false;
        }
    }
    return true;
}

bool SwapOrAttack::isLastCreatureStanding(int partyHealths[], int activeSlot)
{
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        if(i != activeSlot && !isFainted(i, partyHealths))
        {
            return false;
        }
    }
    return true;
}

bool SwapOrAttack::isFainted(int slot, int partyHealths[])
{
    if(partyHealths[slot] == 0)
    {
        return true;
    }
    return false;
}

void SwapOrAttack::reviveMostUsefulCreature(int partyHealths[], int partyAttacks[], string& response)
{
    int usefulSlot = 0;
    int mostUsefulness = 0;
    if(isFainted(1, partyHealths))
    {
        mostUsefulness = partyHealths[1] + partyAttacks[1]; //default
        usefulSlot = 1;
    }
    for(int i = 2; i < PARTY_SIZE; i++)
    {
        if(isFainted(i, partyHealths) && mostUsefulness < partyHealths[i] + partyAttacks[i])
        {
            mostUsefulness = partyHealths[i] + partyAttacks[i];
            usefulSlot = i;
        }
    }
    switch(usefulSlot)
    {
        case 1:
            response = "re1";
            break;
        case 2:
            response = "re2";
            break;
        case 3:
            response = "re3";
            break;
        case 4:
            response = "re4";
            break;
    }
}

void SwapOrAttack::useScroll(int scrollPos, string& response)
{
    switch(scrollPos)
    {
        case 0:
            response = "sa";
            break;
        case 1:
            response = "sb";
            break;
        case 2:
            response = "sc";
            break;
        case 3:
            response = "sd";
            break;
        case 4:
            response = "se";
            break;
        case 5:
            response = "sf";
            break;
        case 6:
            response = "sg";
            break;
        case 7:
            response = "sh";
            break;
    }
}

