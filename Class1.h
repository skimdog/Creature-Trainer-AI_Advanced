//
//  Class1.h
//  Class1
//
//  These class heaaders are for use in your Trainer class. If you wish to
//  write helper classes, write them in these 5 classes.
//  Use of these classes is optional. You may use none or any or all of them.
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//  This class is for Creature CLASSES; Namely, for recording strengths and weakness, move type, and damage amount.

#ifndef __CreatureTrainer__Class1__
#define __CreatureTrainer__Class1__

#include <iostream>
using namespace std;
// vvvvvv Your implementations go between this line

//maybe "SwapOrAttack"?


class SwapOrAttack {
public:
    static const int PARTY_SIZE = 5;
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Default constructor
     */
    SwapOrAttack();
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Swaps to creature with highest current health
     */
    string swapToHighestHealth(int partyHealths[], int activeSlot);
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects: Swaps to creature with strength element equal to enemy's attack element
                If none, returns 'a'
     */
    string swapToStrElement(string enemyAtkElement, string partyStrElements[]);
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Swaps to creature with attack element equal to enemy's weakness
                 If none, returns 'a'
     */
    string swapToAtkElement(string enemyWeakElement, string partyAtkElements[]);
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Returns true if attack element equal to enemy's strength element
     */
    bool attackIsNotEffective(string enemyStrElement, string atkElement);
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Returns true if enemy attack next turn will deplete active creature's health
     Otherwise false
     */
    bool isGonnaDie(int health, int enemyATK);
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Returns true if enemy attack two turns later will deplete health of any
     one of other creatures besides active one
     */
    bool areOthersGonnaDie(int partyHealths[], int partyDamages[]);
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Returns true if other three creatures have fainted
     */
    bool isLastCreatureStanding(int partyHealths[], int activeSlot);
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Returns true if creature at specific slot is fainted
     */
    bool isFainted(int slot, int partyHealths[]);
    
private:
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Returns the slot (1,2,3,4) of creature
     with highest health
     */
    int getHighestHealth(int partyHealths[]);
    
};

// ^^^^^^ And this line. That's it!

#endif /* defined(__CreatureTrainer__Class1__) */
