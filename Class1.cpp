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

void SwapOrAttack::swapCommand(int swapSlot, string& response)
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
        swapCommand(swapSlot, response);
    }
}

void SwapOrAttack::swapToStrElement(string enemyAtkElement, string partyStrElements[], string partyWeakElements[], int partyHealths[], int partyAttacks[], int activeSlot, string& response)
{
    int swapSlot = 0;
    int useFulness = 0;
    bool strElementIsFound = false;
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //not fainted creature
        if(!isFainted(i, partyHealths))
        {
            //if found
            int newUsefulness = getUsefulness(i, partyHealths, partyAttacks);
            if(attackIsNotEffective(enemyAtkElement, partyStrElements[i]) && useFulness < newUsefulness)
            {
                swapSlot = i;
                useFulness = newUsefulness;
                strElementIsFound = true;
            }
            //otherwise any other
            else if(attackIsNormal(enemyAtkElement, partyStrElements[i], partyWeakElements[i]) && !strElementIsFound)
            {
                swapSlot = i;
                useFulness = newUsefulness;
            }
        }
    }
    //not itself
    if(swapSlot != activeSlot)
    {
        swapCommand(swapSlot, response);
    }
}

void SwapOrAttack::swapToAtkElement(string enemyWeakElement, string enemyStrElement, string partyAtkElements[], int partyHealths[], int partyAttacks[], int activeSlot, string& response)
{
    int swapSlot = 0;
    int useFulness = 0;
    bool superEffectiveFound = false;
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //not fainted creature
        if(!isFainted(i, partyHealths))
        {
            //if found
            int newUsefulness = getUsefulness(i, partyHealths, partyAttacks);
            if(attackIsSuperEffective(partyAtkElements[i], enemyWeakElement) && useFulness < newUsefulness)
            {
                swapSlot = i;
                useFulness = newUsefulness;
                superEffectiveFound = true;
            }
            //otherwise any other
            else if(attackIsNormal(partyAtkElements[i], enemyStrElement, enemyWeakElement) && !superEffectiveFound)
            {
                swapSlot = i;
                useFulness = newUsefulness;
            }
        }
    }
    //not itself
    if(swapSlot != activeSlot)
    {
        swapCommand(swapSlot, response);
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

bool SwapOrAttack::attackIsNormal(string atkElement, string enemyStrElement, string enemyWeakElement)
{
    if(!attackIsNotEffective(atkElement, enemyStrElement) && !attackIsSuperEffective(atkElement, enemyWeakElement))
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::isGonnaDie(int health, string strElement, string weakElement, int enemyAttack, string enemyAtkElement)
{
    if(attackIsSuperEffective(enemyAtkElement, weakElement))
    {
        if(health <= enemyAttack * DOUBLE_FACTOR)
        {
            return true;
        }
    }
    else if(attackIsNotEffective(enemyAtkElement, strElement))
    {
        if(health <= enemyAttack / DOUBLE_FACTOR)
        {
            return true;
        }
    }
    else
    {
        if(health <= enemyAttack)
        {
            return true;
        }
    }
    return false;
}

bool SwapOrAttack::areOthersGonnaDieAfterNextTurn(int partyHealths[], string partyStrElements[], string partyWeakElements[], int enemyAttack, string enemyAtkElement)
{
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //hypothesizing future health one turn later
        int remainHealth = partyHealths[i] - enemyAttack;
        if(!isGonnaDie(remainHealth, partyStrElements[i], partyWeakElements[i], enemyAttack, enemyAtkElement))
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

void SwapOrAttack::reviveCommand(int usefulSlot, string& response)
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
        int newUsefulness = getUsefulness(i, partyHealths, partyAttacks);
        if(isFainted(i, partyHealths) && mostUsefulness < newUsefulness)
        {
            mostUsefulness = newUsefulness;
            usefulSlot = i;
        }
    }
    reviveCommand(usefulSlot, response);
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

