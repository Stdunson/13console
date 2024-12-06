//
//  Card.h
//  ThirteenCommandLine
//
//  Created by Shavaughn on 11/30/24.
//

#ifndef Card_h
#define Card_h

    #include <string>
    #include <iostream>
    
    using namespace std;

    enum Suit{
        SPADES, CLUBS, DIAMONDS, HEARTS
    };

    class Card{
        public:
            Card(int, Suit);
            Card();
            int getVal();
            Suit getSuit();
            void setVal(int);
            void setSuit(Suit);
            string cardInfo();
            string valToCard(int);
            string suitToCard(Suit);
            void playCard();
            bool getPlayed();
        
            //fixing saving issues wit cards
            Card(const Card& other) {
                value = other.value;
                suit = other.suit;
                isPlayed = other.isPlayed;
            }
            
            Card& operator=(const Card& other) {
                if (this != &other) {
                    value = other.value;
                    suit = other.suit;
                    isPlayed = other.isPlayed;
                }
                return *this;
            }
        private:
            int value;
            Suit suit;
            bool isPlayed;
        
    };

#endif // Card_h
