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
#include <string>

using namespace std;

struct cardPlay{
    int amtCards;
    bool isStraight;
    vector<int> cardPlayed;
};

int playFirstRound(Player[], int);
int playRound(Player[], int, int);
int findWithLowestCard(int, Player[]);
bool isMyCardHigher(Card&, Card&);
bool allFolded(int, Player[]);
int promptPlay(Player&, Card&); //will delete once new logic completed
cardPlay promptFirstPlay(Player&, bool);
cardPlay promptPlay(Player&, Card&, bool, int);
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
        bool isWinner = false;
        //Check for 13straight + all2s
        for(int i = 0; i < gameSize; i++){
            if(gamePlayers[i].has13Straight()){
                cout<<gamePlayers[i].getName()<<" has won the game with 13 straight!"<<endl;
                isWinner = true;
                gamePlayers[i].addWin();
                break;
            }
            if(gamePlayers[i].hasAllTwos()){
                cout<<gamePlayers[i].getName()<<" has won the game with all 2s!"<<endl;
                isWinner = true;
                gamePlayers[i].addWin();
                break;
            }
        }
        //play rounds
        if(isWinner == false){
            int nextRoundStarter = playFirstRound(gamePlayers, gameSize);
            for(int i = 0; i < gameSize; i++){
                if(gamePlayers[i].hasWon() == true){
                    isWinner = true;
                }
            }
            while(isWinner == false){
                cout<<gamePlayers[nextRoundStarter].getName()<<" will start the next round."<<endl;
                nextRoundStarter = playRound(gamePlayers, nextRoundStarter, gameSize);
                for(int i = 0; i < gameSize; i++){
                    if(gamePlayers[i].hasWon() == true){
                        isWinner = true;
                    }
                }
            }
            //declare winner
            addWin(gamePlayers, gameSize);
        }
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
    Card currentCard = Card(-2, SPADES);
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
            }else{
                cout << currentPlayer.getName() << " will play the "
                     << currentPlayer.getCard(cardPlayed).cardInfo() << endl;
                currentCard = currentPlayer.getCard(cardPlayed);
                currentPlayer.removeCard(cardPlayed);
            }
        }
        do{
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        }while (playerList[currentPlayerIndex].hasFolded() && playerList[currentPlayerIndex].hasWon());
        
        cout<<"Checking for folds and wins"<<endl;
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

int playRound(Player playerList[], int firstPlayer, int numPlayers){
    bool playerHasWon = false;
    bool allPlayersHaveFolded = false;
    int currentPlayerIndex = 0;
    Card currentCard = Card(-2, SPADES);
    currentPlayerIndex = firstPlayer;
    while (!allPlayersHaveFolded && !playerHasWon) {
        Player& currentPlayer = playerList[currentPlayerIndex];
        if (!currentPlayer.hasFolded()) {
            int cardPlayed = promptPlay(currentPlayer, currentCard);
            if (cardPlayed == -1) {
                currentPlayer.fold();
            }else{
                cout << currentPlayer.getName() << " will play the "
                     << currentPlayer.getCard(cardPlayed).cardInfo() << endl;
                currentCard = currentPlayer.getCard(cardPlayed);
                currentPlayer.removeCard(cardPlayed);
            }
        }
        do{
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        }while (playerList[currentPlayerIndex].hasFolded() && playerList[currentPlayerIndex].hasWon());

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

//Will delete when logic remade
int promptPlay(Player& player, Card& card){
    if(player.hasFolded()){
        return -1;
    }
    int cardPlayed = 13;
    bool playableCardSelected = false;
    if(card.getVal() == -2){
        cout<<"Choose the first play."<<endl;
    }else{
        cout<<"The current card is the "<<card.cardInfo()<<endl;
    }
    player.displayHand();
    while(!playableCardSelected){
        cout<<player.getName()<<", which card will you play? Type the number of card. Type -1 to fold."<<endl;
        cin>>cardPlayed;
        if(cardPlayed == -1){
            playableCardSelected = true;
        }else if(cardPlayed < -1 || cardPlayed > 12){
            cout<<"Invalid input. Try again."<<endl;
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

//code this, no folding cus first play
cardPlay promptFirstPlay(Player& player, bool isFirstRound){
    cardPlay myPlay;
    vector<int> cardPlayed;
    bool isStraight = false;
    int amtCards = 0;
    
    bool playableCardSelected = false;
    bool doneSelecting = false;
    int selection = 13;
    cout<<"Choose the first play."<<endl;
    player.displayHand();
    while(!playableCardSelected && !doneSelecting){
        cout<<player.getName()<<", which card will you play? Type the number of card."<<endl;
        cin>>selection;
        if(selection < 0 || selection > 12){
            cout<<"Invalid input. Try again."<<endl;
        }else if(player.getCard(selection).getPlayed()){
            cout<<"That card has already been played. Try again."<<endl;
        }else{
            cout<<"That card is not a playable card. Try again."<<endl;
        }
        if(playableCardSelected == true && doneSelecting == false){
            cardPlayed.push_back(selection);
            amtCards++;
            sort(cardPlayed.begin(), cardPlayed.end());
            //check if valid + if straight
            bool isValid = true;
            bool diffHasBeenZero = false;
            for(int i = 0; i < cardPlayed.size() - 1; i++){
                if(cardPlayed.size() == 1){
                    break;
                }
                int diff = player.getCard(i + 1).getVal() - player.getCard(i).getVal();
                if(diff == 0){
                    diffHasBeenZero = true;
                }
                if(diffHasBeenZero == true && diff > 0){
                    cout<<"Invalid play. Resetting play."<<endl;
                    isValid = false;
                    break;
                }
                if(diff > 1){
                    cout<<"Invalid play. Resetting play."<<endl;
                    isValid = false;
                    break;
                }
            }
            if(diffHasBeenZero == false && cardPlayed.size() < 3){
                cout<<"Invalid play. Resetting play."<<endl;
                isValid = false;
            }
            if(isFirstRound){
                Card lowestCard = player.getCard(0);
                for(int i = 1; i < 13; i++){
                    if(!isMyCardHigher(player.getCard(i), lowestCard)){
                        lowestCard = player.getCard(i);
                    }
                }
                bool lowBeenPlayed = false;
                for(int i = 0; i < cardPlayed.size() - 1; i++){
                    if(player.getCard(cardPlayed[i]).getVal() == lowestCard.getVal()){
                        lowBeenPlayed = true;
                    }
                }
                if(!lowBeenPlayed){
                    cout<<"Invalid play. Smallest card not included in play. Resetting play."<<endl;
                    isValid = false;
                }
            }
            
            if(!isValid){
                cardPlayed.clear();
                bool isStraight = false;
                int amtCards = 0;
                continue;
            }
            
            string input;
            cout<<"Is your selection complete? Type Y to complete selection, N to keep selecting"<<endl;
            cin>>input;
            
            if(input == "Y" || input == "y"){
                if(!diffHasBeenZero){
                    isStraight = true;
                }
                doneSelecting = true;
                myPlay.amtCards = amtCards;
                myPlay.isStraight = isStraight;
                myPlay.cardPlayed = cardPlayed;
                cout<<"Selection complete."<<endl;
                continue;

            }
            if(input != "N" && input != "n"){
                cout<<"Invalid input, completing selection."<<endl;
                if(!diffHasBeenZero){
                    isStraight = true;
                }
                doneSelecting = true;
                myPlay.amtCards = amtCards;
                myPlay.isStraight = isStraight;
                myPlay.cardPlayed = cardPlayed;
                cout<<"Selection complete."<<endl;
                continue;
            }
            cout<<"Continuing selection."<<endl;
        }
    }
    return myPlay;
}

//diffs btw this and firstplay: must check for last card being higher, pattern being same as that of last play, no check for valid just same,
cardPlay promptPlay(Player& player, Card& card, bool playIsStraight, int amtPlayed){
    cardPlay myPlay;
    vector<int> cardPlayed;
    int amtCards = 0;
    
    bool playableCardSelected = false;
    bool doneSelecting = false;
    int selection = 13;
    player.displayHand();
    while(!playableCardSelected && !doneSelecting){
        cout<<player.getName()<<", which card will you play? Type the number of card. Type -1 to fold."<<endl;
        cin>>selection;
        if(selection == -1){
            cardPlayed.push_back(selection);
            playableCardSelected = true;
            amtCards++;
            doneSelecting = true;
        }
        if(selection < 0 || selection > 12){
            cout<<"Invalid input. Try again."<<endl;
        }else if(player.getCard(selection).getPlayed()){
            cout<<"That card has already been played. Try again."<<endl;
        }else{
            cardPlayed.push_back(selection);
            amtCards++;
            playableCardSelected = true;
        }
        if(playableCardSelected == true && amtCards == amtPlayed){
            sort(cardPlayed.begin(), cardPlayed.end());
            
            //check if valid
            bool isValid = true;
            
            if(!playIsStraight){
                if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getVal() < card.getVal()){
                    cout<<"Invalid play. Resetting play."<<endl;
                    isValid = false;
                }else if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getVal() == card.getVal()){
                    if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getSuit() < card.getSuit()){
                        cout<<"Invalid play. Resetting play."<<endl;
                        isValid = false;
                    }
                }
                for(int i = 0; i < cardPlayed.size() - 1; i++){
                    if(cardPlayed.size() == 1){
                        break;
                    }
                    int diff = player.getCard(i + 1).getVal() - player.getCard(i).getVal();
                    if(diff != 0){
                        cout<<"Invalid play. Resetting play."<<endl;
                        isValid = false;
                        break;
                    }
                }
            }else{
                for(int i = 0; i < cardPlayed.size() - 1; i++){
                    if(cardPlayed.size() == 1){
                        break;
                    }
                    int diff = player.getCard(i + 1).getVal() - player.getCard(i).getVal();
                    if(diff != 1){
                        cout<<"Invalid play. Resetting play."<<endl;
                        isValid = false;
                        break;
                    }
                }
                if(cardPlayed.size() < 3){
                    cout<<"Invalid play. Resetting play."<<endl;
                    isValid = false;
                }
            }
            
            if(!isValid){
                cardPlayed.clear();
                bool isStraight = false;
                int amtCards = 0;
                continue;
            }else{
                doneSelecting = true;
                myPlay.amtCards = amtCards;
                myPlay.isStraight = playIsStraight;
                myPlay.cardPlayed = cardPlayed;
                cout<<"Selection complete."<<endl;
            }
        }
    }
    return myPlay;
}
