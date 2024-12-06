//
//  main.cpp
//  ThirteenCommandLine
//
//  Main contains game logic, classes contain background function
//  Created by Shavaughn on 11/30/24.
//

#include <iostream>
#include "Deck.h"
#include "Player.h"

using namespace std;

int playFirstRound(Player[], int);
int playRound(Player[], int, int);
int findWithLowestCard(int, Player[]);
bool isMyCardHigher(Card&, Card&);
bool allFolded(int, Player[]);
int promptPlay(Player&, Card&);
bool sbWon(int, Player[]);
void addWin(Player[], int);

int main() {
    //specific to console based play, but have similar process for actual game
    cout<<"Welcome to Shavaughn's 13 in C++!"<<endl;
    //prompt for amt players + player names.
    int gameSize;
    cout<<"How many players will be playing the game? The max is 4, the min is 2. Enter the number of players."<<endl;
    cin>>gameSize;
    cin.ignore();
    Player gamePlayers[gameSize];
    for(int i = 0; i < gameSize; i++){
        string myName;
        cout<<"What will player "<<to_string(i + 1)<<"'s name be?"<<endl;
        getline(cin, myName);
        gamePlayers[i] = Player(myName);
    }
    //play game
    bool donePlaying = false;
    while(donePlaying == false){
        //play rounds
        bool isWinner = false;
        int nextRoundStarter = playFirstRound(gamePlayers, gameSize);
        for(int i = 0; i < gameSize; i++){
            if(gamePlayers[i].hasWon() == true){
                return isWinner = true;
            }
        }
        while(isWinner == false){
            nextRoundStarter = playRound(gamePlayers, nextRoundStarter, gameSize);
            for(int i = 0; i < gameSize; i++){
                if(gamePlayers[i].hasWon() == true){
                    return isWinner = true;
                }
            }
        }
        //declare winner
        addWin(gamePlayers, gameSize);
        //prompt to play again
        string playagain;
        cout<<"Would you like to play another game? Type Y to play again or N to stop"<<endl;
        cin>>playagain;
        if(playagain != "Y" && playagain != "y"){
            donePlaying = true;
        }
    }
    return 0;
}

int playFirstRound(Player playerList[], int numPlayers){
    bool playerHasWon = false;
    bool allPlayersHaveFolded = false;
    int currentPlayerIndex = 0;
    Deck gameDeck;
    gameDeck.shuffleDeck();
    gameDeck.dealDeck(numPlayers, playerList);
    Card currentCard = Card(0, SPADES);
    currentPlayerIndex = findWithLowestCard(numPlayers, playerList);
    Card lowestCard = playerList[currentPlayerIndex].getCard(0);
    int lowestCardIndex = 0;
    for(int i = 1; i < 13; i++){
        if(!isMyCardHigher(playerList[currentPlayerIndex].getCard(i), lowestCard)){
            lowestCard = playerList[currentPlayerIndex].getCard(i);
            lowestCardIndex = i;
        }
    }
    currentCard = playerList[currentPlayerIndex].getCard(lowestCardIndex);
    playerList[currentPlayerIndex].removeCard(lowestCardIndex);
    cout<<playerList[currentPlayerIndex].getName()<<" will play the "<<lowestCard.cardInfo()<<endl;
    currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
    while (!allPlayersHaveFolded && !playerHasWon) {
        Player& currentPlayer = playerList[currentPlayerIndex];
        if (!currentPlayer.hasFolded()) {
            int cardPlayed = promptPlay(currentPlayer, currentCard);
            if (cardPlayed == -1) {
                currentPlayer.fold();
            } else {
                cout << currentPlayer.getName() << " will play the "
                     << currentPlayer.getCard(cardPlayed).cardInfo() << endl;
                currentCard = currentPlayer.getCard(cardPlayed);
                currentPlayer.removeCard(cardPlayed);
            }
        }
        do {
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        } while (playerList[currentPlayerIndex].hasFolded() || playerList[currentPlayerIndex].hasWon());

        allPlayersHaveFolded = allFolded(numPlayers, playerList);
        playerHasWon = sbWon(numPlayers, playerList);
    }
    int lastFolderIndex = 0;
    if(currentPlayerIndex == 0){
        lastFolderIndex = numPlayers - 1;
    }else{
        lastFolderIndex = currentPlayerIndex - 1;
    }
    for(int i = 0; i < numPlayers; i++){
        playerList[i].unfold();
    }
    return currentPlayerIndex;
}

int playRound(Player playerList[], int firstPlayer, int numPlayers){
    bool playerHasWon = false;
    bool allPlayersHaveFolded = false;
    int currentPlayerIndex = 0;
    Card currentCard = Card(0, SPADES);
    currentPlayerIndex = firstPlayer;
    while (!allPlayersHaveFolded && !playerHasWon) {
        Player& currentPlayer = playerList[currentPlayerIndex];
        if (!currentPlayer.hasFolded()) {
            int cardPlayed = promptPlay(currentPlayer, currentCard);
            if (cardPlayed == -1) {
                currentPlayer.fold();
            } else {
                cout << currentPlayer.getName() << " will play the "
                     << currentPlayer.getCard(cardPlayed).cardInfo() << endl;
                currentCard = currentPlayer.getCard(cardPlayed);
                currentPlayer.removeCard(cardPlayed);
            }
        }
        do {
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        } while (playerList[currentPlayerIndex].hasFolded() || playerList[currentPlayerIndex].hasWon());

        allPlayersHaveFolded = allFolded(numPlayers, playerList);
        playerHasWon = sbWon(numPlayers, playerList);
    }
    
    int lastFolderIndex = 0;
    if(currentPlayerIndex == 0){
        lastFolderIndex = numPlayers - 1;
    }else{
        lastFolderIndex = currentPlayerIndex - 1;
    }
    for(int i = 0; i < numPlayers; i++){
        playerList[i].unfold();
    }
    return lastFolderIndex;
}

int findWithLowestCard(int numPlayers, Player players[]){
    Card lowestCard = players[0].getCard(0);
    Player lowestPlayer = players[0];
    for(int i = 0; i < numPlayers; i++){
        for(int j = 0; j < 13; j++){
            if(!isMyCardHigher(players[i].getCard(j), lowestCard)){
                lowestCard = players[i].getCard(j);
                lowestPlayer = players[i];
            }
        }
    }
    for(int i = 0; i < numPlayers; i++){
        if(lowestPlayer.equals(players[i])){
            return i;
        }
    }
    cout<<"error"<<endl;
    return 0;
}

bool isMyCardHigher(Card& card1, Card& card2){
    if(card1.getVal() > card2.getVal()){
        return true;
    }else if(card1.getVal() < card2.getVal()){
        return false;
    }else{
        if(card1.getSuit() > card2.getSuit()){
            return true;
        }else{
            return false;
        }
    }
}

bool allFolded(int numPlayers, Player players[]){
    for(int i = 0; i < numPlayers; i++){
        if(players[i].hasFolded() == false){
            return false;
        }
    }
    return true;
}

//add things that take int amtCards and bool isStraight to implement patterns
int promptPlay(Player& player, Card& card){
    if(player.hasFolded()){
        return -1;
    }
    int cardPlayed = 13;
    bool playableCardSelected = false;
    cout<<"The current card is the "<<card.cardInfo()<<endl;
    player.displayHand();
    while(!playableCardSelected){
        cout<<player.getName()<<", which card will you play? Type the number of card. Type -1 to fold."<<endl;
        cin>>cardPlayed;
        if(cardPlayed == -1){
            playableCardSelected = true;
        }else if(isMyCardHigher(player.getCard(cardPlayed), card)){
            playableCardSelected = true;
        }else if(player.getCard(cardPlayed).getPlayed()){
            cout<<"That card has already been played. Try again."<<endl;
        }else{
            cout<<"That card is not a playable card. Try again."<<endl;
        }
    }
    return cardPlayed;
}

bool sbWon(int numPlayers, Player players[]){
    for(int i = 0; i < numPlayers; i++){
        if(players[i].hasWon() == true){
            return true;
        }
    }
    return false;
}

void addWin(Player playerList[], int numPlayers){
    for(int i = 0; i < numPlayers; i++){
        if(playerList[i].hasWon()){
            playerList[i].addWin();
            cout<<playerList[i].getName()<<" has won the game!"<<endl;
        }else{
            playerList[i].addLoss();
        }
    }
}
