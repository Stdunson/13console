//
//  Player.h
//  ThirteenCommandLine
//
//  Created by Shavaughn on 11/30/24.
//
#include "Card.h"
#include <string>
using namespace std;

#ifndef Player_h
#define Player_h

class Player{
    public:
        Player();
        Player(string);
        void setHand(Card[13]);
        Card& getCard(int);
        void removeCard(int);
        bool hasWon();
        bool hasFolded();
        void fold();
        void unfold();
        void displayHand();
        int getWins();
        int getLosses();
        string getName();
        bool equals(Player);
        void addWin();
        void addLoss();
        
    private:
        Card hand[13];
        int wincount;
        int losscount;
        bool folded;
        string name;
        bool cardPlayed[13] = {false};
};

#endif /* Player_h */
