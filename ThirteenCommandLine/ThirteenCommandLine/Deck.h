//
//  Deck.h
//  ThirteenCommandLine
//
//  Created by Shavaughn on 11/30/24.
//
#include "Card.h"
#include "Player.h"

#ifndef Deck_h
#define Deck_h

class Deck{
    public:
        Deck();
        void shuffleDeck();
        void dealDeck(int, Player[]);
    private:
    Card cards[52];
};

#endif /* Deck_h */
