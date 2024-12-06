//
//  Player.cpp
//  ThirteenCommandLine
//
//  Created by Shavaughn on 11/30/24.
//

#include "Player.h"
#include <iostream>
using namespace std;

Player::Player(string pname){
    wincount = 0;
    losscount = 0;
    folded = false;
    name = pname;
}

Player::Player(){
    wincount = 0;
    losscount = 0;
    folded = false;
    name = "Unnamed Player";
}

void Player::setHand(Card newHand[13]){
    for(int i = 0; i < 13; i++){
        hand[i] = newHand[i];
    }
}

Card& Player::getCard(int index){
    if(index >= 0 && index < 13){
        return hand[index];
    }
    cout<<"There was an error"<<endl;
    return hand[0];
}

void Player::removeCard(int index){
    hand[index].playCard();
    cardPlayed[index] = true;
}

bool Player::hasWon(){
    for(int i = 0; i < 13; i++){
        if(!cardPlayed[i]){
            return false;
        }
    }
    return true;
}

 
void Player::fold(){
    folded = true;
}

void Player::unfold(){
    folded = false;
}

bool Player::hasFolded(){
    return folded;
}

void Player::displayHand(){
    cout<<"Your cards: "<<endl;
    for(int i = 0; i < 13; i++){
        if(cardPlayed[i]){
            cout<<"["<<i<<"] "<<"Played"<<", ";
        }else{
            cout<<"["<<i<<"] "<<hand[i].cardInfo()<<", ";
        }
    }
}

int Player::getWins(){
    return wincount;
}

int Player::getLosses(){
    return losscount;
}

bool Player::equals(Player otherPlayer){
    if(wincount != otherPlayer.getWins()){
        return false;
    }
    if(losscount != otherPlayer.getLosses()){
        return false;
    }
    if(name != otherPlayer.getName()){
        return false;
    }
    return true;
}

string Player::getName(){
    return name;
}

void Player::addWin(){
    wincount++;
}

void Player::addLoss(){
    losscount++;
}
