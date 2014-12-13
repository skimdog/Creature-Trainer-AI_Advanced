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

void SwapOrAttack::swap(int swapSlot, string& response)
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

void SwapOrAttack::swapToHighestHealth(int partyHealths[], int activeSlot, string& response)
{
    int swapSlot = getHighestHealth(partyHealths);
    
    //if swapping with other than itself
    if(swapSlot != activeSlot)
    {
        swap(swapSlot, response);
    }
    //otherwise
    else
    {
        response = "a";
    }
}

void SwapOrAttack::swapToStrElement(string enemyAtkElement, string partyStrElements[], int partyHealths[], string& response)
{
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        if(!isFainted(i, partyHealths) && attackIsNotEffective(enemyAtkElement, partyStrElements[i]))
        {
            
        }
    }
}

bool SwapOrAttack::attackIsNotEffective(string atkElement, string enemyStrElement)
{
    if(atkElement == enemyStrElement)
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::attackIsSuperEffective(string atkElement, string enemyWeakElement)
{
    if(atkElement == enemyWeakElement)
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::isGonnaDie(int health, int enemyATK)
{
    if (health <= enemyATK)
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::areOthersGonnaDieAfterNextTurn(int partyHealths[], int enemyAttack)
{
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //hypothesizing future health one turn later
        int remainHealth = partyHealths[i] - enemyAttack;
        if(!isGonnaDie(remainHealth, enemyAttack))
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

int SwapOrAttack::getUsefulness(int slot, int partyHealths[], int partyAttacks[])
{
    return partyHealths[slot] + partyAttacks[slot];
}

void SwapOrAttack::revive(int usefulSlot, string& response)
{
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

void SwapOrAttack::reviveMostUsefulCreature(int partyHealths[], int partyAttacks[], string& response)
{
    int usefulSlot = 0;
    int mostUsefulness = 0;
    if(isFainted(1, partyHealths))
    {
        mostUsefulness = getUsefulness(1, partyHealths, partyAttacks); //default
        usefulSlot = 1;
    }
    for(int i = 2; i < PARTY_SIZE; i++)
    {
        if(isFainted(i, partyHealths) && mostUsefulness < getUsefulness(i, partyHealths, partyAttacks))
        {
            mostUsefulness = getUsefulness(i, partyHealths, partyAttacks);
            usefulSlot = i;
        }
    }
    revive(usefulSlot, response);
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

