//
//  Deck.cpp
//  ThirteenCommandLine
//
//  Created by Shavaughn on 11/30/24.
//

#include "Deck.h"
#include <stdlib.h>
#include <ctime>

Deck::Deck(){
    srand(static_cast<unsigned int>(time(nullptr)));
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            cards[(13 * i) + j] = Card(j + 1, static_cast<Suit>(i));
        }
    }
}

void Deck::shuffleDeck(){
    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);
        
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

void Deck::dealDeck(int numplayers, Player gamePlayers[numplayers]){
    shuffleDeck();
    for(int i = 0; i < numplayers; i++){
        Card newHand[13];
        for(int j = 0; j < 13; j++){
            newHand[j] = cards[(13*i) + j];
        }
        gamePlayers[i].setHand(newHand);
    }
}
