//  Class3.cpp
//  Helper Class3
//
//  These class heaaders are for use in your Trainer class. If you wish to 
//  write helper classes, write them in these 5 classes. 
//  Use of these classes is optional. You may use none or any or all of them. 
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#include "Class3.h"

// WE ARE GONNA CATCH  STUFF!!!!!!!!! YEAH!

Capture::Capture()
{
    
}

int Capture::getLeastUseful(int partyHealths[], int partyAttacks[])
{
    int uselessSlot = 1;
    int usefulness = partyHealths[1] + partyAttacks[1];
    
    for(int i = 2; i < PARTY_SIZE; i++)
    {
        if(usefulness > partyHealths[i] + partyAttacks[i])
        {
            usefulness = partyHealths[i] + partyAttacks[i];
            uselessSlot = i;
        }
    }
    return uselessSlot;
}

string Capture::captureCreature(int enemyBaseHealth, int enemyBaseAttack, int partyHealths[], int partyAttacks[])
{
    int enemyUsefulness = enemyBaseHealth + enemyBaseAttack;
    int uselessSlot = getLeastUseful(partyHealths, partyAttacks);
    int yourCreatureUsefulness = partyHealths[uselessSlot] + partyAttacks[uselessSlot];
    
    string response = ""; //default
    
    if (enemyUsefulness > yourCreatureUsefulness)
    {
        switch(uselessSlot)
        {
            case 1:
                response = "co1";
                break;
            case 2:
                response = "co2";
                break;
            case 3:
                response = "co3";
                break;
            case 4:
                response = "co4";
                break;
        }
    }
    return response;
}