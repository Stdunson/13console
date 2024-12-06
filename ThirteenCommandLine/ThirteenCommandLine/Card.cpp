//
//  Card.cpp
//  ThirteenCommandLine
//
//  Created by Shavaughn on 11/30/24.
//

#include "Card.h"
#include <string>
#include <iostream>

using namespace std;

Card::Card(int val, Suit cardSuit){
    value = val;
    suit = cardSuit;
    isPlayed = false;
}

Card::Card(){
    value = 0;
    suit = SPADES;
    isPlayed = false;
}

int Card::getVal(){
    return value;
}

Suit Card::getSuit(){
    return suit;
}

void Card::setVal(int newVal){
    value = newVal;
}

void Card::setSuit(Suit newSuit){
    suit = newSuit;
}

string Card::cardInfo(){
    return valToCard(value) + " of " + suitToCard(suit);
}

string Card::valToCard(int myVal){
    if(myVal <= 8){
        return to_string(myVal + 2);
    }else if(myVal == 9){
        return "Jack";
    }else if(myVal == 10){
        return "Queen";
    }else if(myVal == 11){
        return "King";
    }else if(myVal == 12){
        return "Ace";
    }else if(myVal == 13){
        return "2";
    }else{
        return "error";
    }
}

string Card::suitToCard(Suit mySuit){
    switch (mySuit) {
        case 0:
            return "Spades";
        case 1:
            return "Clubs";
        case 2:
            return "Diamonds";
        case 3:
            return "Hearts";
        default:
            return "error";
    }
}

void Card::playCard(){
    isPlayed = true;
}

bool Card::getPlayed(){
    return isPlayed;
}
