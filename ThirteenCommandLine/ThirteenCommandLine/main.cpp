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
cardPlay promptPlay(Player&, vector<Card>&, bool, int);
bool sbWon(int, Player[]);
void addWin(Player[], int);
void announceCards(vector<int>, Player&);
vector<Card> doPlay(vector<int>, Player&);

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

//change card to an array sized based on the size of the first play
//for folding, change condition to if it contains -1 at all, not ==
//when removing + playing cards must use all the indexes in the play vector, prolly need a for loop
int playFirstRound(Player playerList[], int numPlayers){
    bool playerHasWon = false;
    bool allPlayersHaveFolded = false;
    int currentPlayerIndex = 0;
    vector<Card> currentCard;
    Deck gameDeck;
    gameDeck.shuffleDeck();
    gameDeck.dealDeck(numPlayers, playerList);
    currentPlayerIndex = findWithLowestCard(numPlayers, playerList);
    cout<<playerList[currentPlayerIndex].getName()<<" will start the game."<<endl;
    cardPlay currentPlay = promptFirstPlay(playerList[currentPlayerIndex], true);
    announceCards(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
    //turn currentcard into card vector using for loop
    currentCard = doPlay(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
    for(int i = 0; i < currentPlay.cardPlayed.size(); i++){
        playerList[currentPlayerIndex].removeCard(currentPlay.cardPlayed[i]);
    }
    currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
    while (!allPlayersHaveFolded && !playerHasWon){
        Player& currentPlayer = playerList[currentPlayerIndex];
        if (!currentPlayer.hasFolded()) {
            currentPlay = promptPlay(currentPlayer, currentCard, currentPlay.isStraight, currentPlay.amtCards);
            bool gonFold = false;
            for(int i = 0; i < currentPlay.cardPlayed.size(); i++){
                if (currentPlay.cardPlayed[i] == -1) {
                    currentPlayer.fold();
                    gonFold = true;
                    break;
                }
                if(!gonFold){
                    announceCards(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
                    currentCard = doPlay(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
                    for(int i = 0; i < currentPlay.cardPlayed.size(); i++){
                        currentPlayer.removeCard(currentPlay.cardPlayed[i]);
                    }
                }
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

int playRound(Player playerList[], int firstPlayer, int numPlayers){
    bool playerHasWon = false;
    bool allPlayersHaveFolded = false;
    int currentPlayerIndex = 0;
    vector<Card> currentCard;
    currentPlayerIndex = firstPlayer;
    cardPlay currentPlay = promptFirstPlay(playerList[currentPlayerIndex], false);
    announceCards(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
    currentCard = doPlay(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
    for(int i = 0; i < currentPlay.cardPlayed.size(); i++){
        playerList[currentPlayerIndex].removeCard(currentPlay.cardPlayed[i]);
    }
    currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
    while (!allPlayersHaveFolded && !playerHasWon){
        Player& currentPlayer = playerList[currentPlayerIndex];
        if (!currentPlayer.hasFolded()) {
            currentPlay = promptPlay(currentPlayer, currentCard, currentPlay.isStraight, currentPlay.amtCards);
            bool gonFold = false;
            for(int i = 0; i < currentPlay.cardPlayed.size(); i++){
                if (currentPlay.cardPlayed[i] == -1) {
                    currentPlayer.fold();
                    gonFold = true;
                    break;
                }
                if(!gonFold){
                    announceCards(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
                    currentCard = doPlay(currentPlay.cardPlayed, playerList[currentPlayerIndex]);
                    for(int i = 0; i < currentPlay.cardPlayed.size(); i++){
                        currentPlayer.removeCard(currentPlay.cardPlayed[i]);
                    }
                }
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
    
    bool doneSelecting = false;
    int selection = 13;
    cout<<"Choose the first play."<<endl;
    player.displayHand();
    while(!doneSelecting){
        bool playableCardSelected = false;
        cout<<player.getName()<<", which card will you play? Type the number of card."<<endl;
        cin>>selection;
        if(selection < 0 || selection > 12){
            cout<<"Invalid input. Try again."<<endl;
        }else if(player.getCard(selection).getPlayed()){
            cout<<"That card has already been played. Try again."<<endl;
        }else{
            cardPlayed.push_back(selection);
            amtCards++;
            playableCardSelected = true;
        }
        if(playableCardSelected == true && doneSelecting == false){
            for(int i = 0; i < cardPlayed.size() - 1; i++){
                for(int j = 0; j < cardPlayed.size() - i - 1; j++){
                    if(player.getCard(cardPlayed[j]).getVal() > player.getCard(cardPlayed[j]).getVal()){
                        int temp = cardPlayed[j + 1];
                        cardPlayed[j + 1] = cardPlayed[j];
                        cardPlayed[j] = temp;
                    }
                    if(player.getCard(cardPlayed[j]).getVal() == player.getCard(cardPlayed[j]).getVal()){
                        if(player.getCard(cardPlayed[j]).getSuit() > player.getCard(cardPlayed[j]).getSuit()){
                            int temp = cardPlayed[j + 1];
                            cardPlayed[j + 1] = cardPlayed[j];
                            cardPlayed[j] = temp;
                        }
                    }
                }
            }
            //check if valid + if straight
            bool isValid = true;
            bool diffHasBeenZero = false;
            if(cardPlayed.size() == 1){
                diffHasBeenZero = true;
            }else if(cardPlayed.size() == 2){
                if(player.getCard(cardPlayed[1]).getVal() - player.getCard(cardPlayed[0]).getVal() == 1){
                    diffHasBeenZero = false;
                    continue;
                }else if (player.getCard(cardPlayed[1]).getVal() - player.getCard(cardPlayed[0]).getVal() == 0){
                    diffHasBeenZero = true;
                }else{
                    cout<<"Invalid play. Resetting play."<<endl;
                    isValid = false;
                }
            }else{
                diffHasBeenZero = false;
                for(int i = 0; i < cardPlayed.size() - 1; i++){
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
            }
            if(isFirstRound){
                Card lowestCard = player.getCard(0);
                for(int i = 1; i < 13; i++){
                    if(!isMyCardHigher(player.getCard(i), lowestCard)){
                        lowestCard = player.getCard(i);
                    }
                }
                bool lowBeenPlayed = false;
                for(int i = 0; i < cardPlayed.size(); i++){
                    if(player.getCard(cardPlayed[i]).getVal() == lowestCard.getVal()){
                        if(player.getCard(cardPlayed[i]).getSuit() == lowestCard.getSuit()){
                            lowBeenPlayed = true;
                        }
                    }
                }
                if(!lowBeenPlayed){
                    cout<<"Invalid play. Smallest card not included in play. Resetting play."<<endl;
                    isValid = false;
                }
            }
            
            if(!isValid){
                cardPlayed.clear();
                isStraight = false;
                amtCards = 0;
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
cardPlay promptPlay(Player& player, vector<Card>& card, bool playIsStraight, int amtPlayed){
    cardPlay myPlay;
    vector<int> cardPlayed;
    int amtCards = 0;
    
    bool doneSelecting = false;
    int selection = 13;
    player.displayHand();
    while(!doneSelecting){
        bool playableCardSelected = false;
        cout<<player.getName()<<", which card will you play? Type the number of card. Type -1 to fold."<<endl;
        cin>>selection;
        if(selection == -1){
            cardPlayed.push_back(selection);
            playableCardSelected = true;
            amtCards++;
            doneSelecting = true;
            myPlay.amtCards = amtCards;
            myPlay.isStraight = playIsStraight;
            myPlay.cardPlayed = cardPlayed;
            break;
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
            for(int i = 0; i < cardPlayed.size() - 1; i++){
                for(int j = 0; j < cardPlayed.size() - i - 1; j++){
                    if(player.getCard(cardPlayed[j]).getVal() > player.getCard(cardPlayed[j]).getVal()){
                        int temp = cardPlayed[j + 1];
                        cardPlayed[j + 1] = cardPlayed[j];
                        cardPlayed[j] = temp;
                    }
                    if(player.getCard(cardPlayed[j]).getVal() == player.getCard(cardPlayed[j]).getVal()){
                        if(player.getCard(cardPlayed[j]).getSuit() > player.getCard(cardPlayed[j]).getSuit()){
                            int temp = cardPlayed[j + 1];
                            cardPlayed[j + 1] = cardPlayed[j];
                            cardPlayed[j] = temp;
                        }
                    }
                }
            }
            for(int i = 0; i < card.size() - 1; i++){
                for(int j = 0; j < card.size() - i - 1; j++){
                    if(card[j].getVal() > card[j + 1].getVal()){
                        Card temp = card[j + 1];
                        card[j + 1] = card[j];
                        card[j] = temp;
                    }
                    if(card[j].getVal() == card[j + 1].getVal()){
                        if(card[j].getSuit() > card[j + 1].getSuit()){
                            Card temp = card[j + 1];
                            card[j + 1] = card[j];
                            card[j] = temp;
                        }
                    }
                }
            }
            
            //check if valid
            bool isValid = true;
            
            if(!playIsStraight){
                if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getVal() < card[card.size() - 1].getVal()){
                    cout<<"Invalid play. Resetting play."<<endl;
                    isValid = false;
                }else if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getVal() == card[card.size() - 1].getVal()){
                    if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getSuit() < card[card.size() - 1].getSuit()){
                        cout<<"Invalid play. Resetting play."<<endl;
                        isValid = false;
                    }
                }
                for(int i = 0; i < cardPlayed.size() - 1; i++){
                    if(cardPlayed.size() == 1){
                        break;
                    }
                    int diff = player.getCard(cardPlayed[i + 1]).getVal() - player.getCard(cardPlayed[i]).getVal();
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
                    int diff = player.getCard(cardPlayed[i + 1]).getVal() - player.getCard(cardPlayed[i]).getVal();
                    if(diff != 1){
                        cout<<"Invalid play. Resetting play."<<endl;
                        isValid = false;
                        break;
                    }
                }
                //check if highest higher than played highest
                if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getVal() < card[card.size() - 1].getVal()){
                    cout<<"Invalid play. Resetting play."<<endl;
                    isValid = false;
                }else if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getVal() == card[card.size() - 1].getVal()){
                    if(player.getCard(cardPlayed[cardPlayed.size() - 1]).getSuit() < card[card.size() - 1].getSuit()){
                        
                    }
                }
            }
            
            if(!isValid){
                cardPlayed.clear();
                amtCards = 0;
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

void announceCards(vector<int> cards, Player& player){
    cout<<player.getName()<<" will play the ";
    for(int i = 0; i < cards.size(); i++){
        cout<<player.getCard(cards[i]).cardInfo();
        
        if(i != cards.size() - 1){
            cout<<" + ";
        }
    }
    cout<<endl;
}

vector<Card> doPlay(vector<int> cards, Player& player){
    vector<Card> output;
    for(int i = 0; i < cards.size(); i++){
        output.push_back(player.getCard(cards[i]));
    }
    return output;
}
