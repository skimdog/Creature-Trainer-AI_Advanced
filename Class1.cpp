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

int SwapOrAttack::getHighestHealth(int partyHealths[], int partyAttacks[])
{
    int highestSlot = 1;
    int highestHealth = partyHealths[1];
    int attack = partyAttacks[1];
    for(int i = 2; i < PARTY_SIZE; i++)
    {
        if(highestHealth < partyHealths[i])
        {
            highestHealth = partyHealths[i];
            attack = partyAttacks[i];
            highestSlot = i;
        }
        else if(highestHealth == partyHealths[i])
        {
            if(attack < partyAttacks[i])
            {
                highestHealth = partyHealths[i];
                attack = partyAttacks[i];
                highestSlot = i;
            }
        }
    }
    return highestSlot;
}

void SwapOrAttack::swapToHighestHealth(int partyHealths[], int partyAttacks[], int activeSlot, string& response)
{
    int swapSlot = getHighestHealth(partyHealths, partyAttacks);
    
    //if swapping with other than itself
    if(swapSlot != activeSlot)
    {
        swapCommand(swapSlot, response);
    }
}

void SwapOrAttack::swapToNormal(string enemyAtkElement, string enemyStrElement, string partyAtkElements[], string partyWeakElements[], int partyHealths[], int activeSlot, string& response)
{
    int swapSlot = 0;
    int highHealth = 0;
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //not fainted creature
        if(!isFainted(i, partyHealths))
        {
            //if found
            int higherHealth = partyHealths[i];
            if(!attackIsSuperEffective(enemyAtkElement, partyWeakElements[i]) && !attackIsNotEffective(partyAtkElements[i], enemyStrElement) && highHealth < higherHealth)
            {
                swapSlot = i;
                highHealth = higherHealth;
            }
        }
    }
    if(swapSlot != activeSlot && swapSlot != 0)
    {
        swapCommand(swapSlot, response);
    }

}

void SwapOrAttack::swapToDefensive(string enemyAtkElement, string enemyStrElement, string partyAtkElements[], string partyStrElements[], int partyHealths[], int activeSlot, string& response)
{
    int swapSlot = 0;
    int highHealth = 0;
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //not fainted creature
        if(!isFainted(i, partyHealths))
        {
            //if found
            int higherHealth = partyHealths[i];
            if(attackIsNotEffective(enemyAtkElement, partyStrElements[i]) && highHealth < higherHealth)
            {
                swapSlot = i;
                highHealth = higherHealth;
            }
        }
    }
    if(swapSlot != activeSlot && swapSlot != 0)
    {
        swapCommand(swapSlot, response);
    }
}

void SwapOrAttack::swapToOffensive(string enemyAtkElement, string enemyWeakElement, string partyAtkElements[], string partyWeakElements[], int partyHealths[], int activeSlot, string& response)
{
    int swapSlot = 0;
    int highHealth = 0;
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //not fainted creature
        if(!isFainted(i, partyHealths))
        {
            //if found
            int higherHealth = partyHealths[i];
            if(attackIsSuperEffective(partyAtkElements[i], enemyWeakElement) && !attackIsSuperEffective(enemyAtkElement, partyWeakElements[i]) && highHealth < higherHealth)
            {
                swapSlot = i;
                highHealth = higherHealth;
            }
        }
    }
    if(swapSlot != activeSlot)
    {
        swapCommand(swapSlot, response);
    }
}

void SwapOrAttack::swapToWinner(bool partyWinOrLose[], int partyHealths[], int activeSlot, string& response)
{
    int swapSlot = 0;
    int highHealth = 0;
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        int higherHealth = partyHealths[i];
        if(partyWinOrLose[i] && highHealth < higherHealth)
        {
            swapSlot = i;
            highHealth = higherHealth;
        }
    }
    if(swapSlot != activeSlot)
    {
        swapCommand(swapSlot, response);
    }
}

/*
void SwapOrAttack::swapDecision(string enemyAtkElement, string enemyWeakElement, string enemyStrElement, string partyAtkElements[], string partyWeakElements[], string partyStrElements[], int partyHealths[], int activeSlot, string& response)
{
    swapToHighestHealth(partyHealths, partyAttacks, activeSlot, response);
    swapToNormal(enemyAtkElement, enemyStrElement, partyAtkElements, partyWeakElements, partyHealths, activeSlot, response);
    swapToOffensive(enemyWeakElement, partyAtkElements, partyHealths, activeSlot, response);
    swapToDefensive(enemyAtkElement, partyStrElements, partyHealths, activeSlot, response);
}
 */

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
    if(atkElement != enemyStrElement && atkElement != enemyWeakElement)
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::isGonnaDie(int health, string strElement, string weakElement, int enemyAttack, string enemyAtkElement)
{
    int factoredAtk = getFactoredAttack(enemyAttack, strElement, weakElement, enemyAtkElement);
    
    if(health <= factoredAtk)
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::isGonnaDieAfterNextTurn(int health, string strElement, string weakElement, int enemyAttack, string enemyAtkElement)
{
    int factoredAtk = getFactoredAttack(enemyAttack, strElement, weakElement, enemyAtkElement);
    
    if(health <= factoredAtk * DOUBLE_FACTOR)
    {
        return true;
    }
    return false;
}

bool SwapOrAttack::areOthersGonnaDieAfterNextTurn(int partyHealths[], string partyStrElements[], string partyWeakElements[], int enemyAttack, string enemyAtkElement)
{
    for(int i = 1; i < PARTY_SIZE; i++)
    {
        //hypothesizing future health one turn later
        if(!isGonnaDieAfterNextTurn(partyHealths[i], partyStrElements[i], partyWeakElements[i], enemyAttack, enemyAtkElement))
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

int SwapOrAttack::getFactoredAttack(int activeAttack, string enemyStrElement, string enemyWeakElement, string activeAtkElement)
{
    if(attackIsSuperEffective(activeAtkElement, enemyWeakElement))
    {
        return activeAttack * DOUBLE_FACTOR;
    }
    else if(attackIsNotEffective(activeAtkElement, enemyStrElement))
    {
        return activeAttack / DOUBLE_FACTOR;
    }
    else
    {
        return activeAttack;
    }
}

