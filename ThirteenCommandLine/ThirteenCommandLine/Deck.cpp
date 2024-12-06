//
//  Deck.cpp
//  ThirteenCommandLine
//
//  Created by Shavaughn on 11/30/24.
//

#include "Deck.h"
#include <stdlib.h>

Deck::Deck(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            cards[(13 * i) + j] = Card(j + 1, static_cast<Suit>(i));
        }
    }
}

void Deck::shuffleDeck(){
    Card tempdeck[52] = {};
    for(int i = 0; i < 52; i++){
        int x = 0;
        do{
            int num = rand() % 52;
            if(tempdeck[num].getVal() == 0){
                tempdeck[num].setVal(cards[i].getVal());
                tempdeck[num].setSuit(cards[i].getSuit());
                break;
            }
        }while(x != 1);
    }
    for(int l = 0; l < 52; l++){
        cards[l] = tempdeck[l];
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
