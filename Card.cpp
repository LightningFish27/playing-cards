#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

struct Card{
    int value; // The value of the card. Ace is 1.
    int suit; // Hearts, Diamonds, Spades, Clubs
    Card(const int v, const int s) : value(v), suit(s) {}

    friend std::ostream& operator<<(std::ostream& os, Card& card){
        os << card.value << " of " << card.suit;
        return os;
    }
};


class Deck{
private:
    std::vector<Card> cards; 
public:
    // Default Deck constructor creates a deck of the standard 52 cards.
    Deck(){
        cards.reserve(52); // Save on a few vector resizes
        for (int s = 0; s < 4; s++){ // For each suit
            for (int v = 0; v < 14; v++){ // For each value
                cards.push_back(Card(v, s));
            }
        }
    }
    // Shuffle a deck randomly
    void shuffle(){
        static std::random_device rd;
        std::shuffle(cards.begin(), cards.end(), rd);
    }
    // Shuffle a deck semi-randomly. For example, modulus shuffles or chunked shuffles.
    // MethodComplement will be used for any shuffle types that need an additional variable,
    // such as how many cards should be in a chunk.
    void shuffle(int shuffleMethod, int methodComplement){
        if (shuffleMethod < 0){
            std::cout << "Attempted to shuffle deck at " << this << " by undefined method.\n";
            return;
        }
        switch(shuffleMethod){
            case 1:
                // Modular shuffle
                break;
            case 2:
                // Chunked shuffle
                break;
        }
    }

    // Draw a card from the top of the deck.
    Card draw_card(){
        return cards[cards.size()];
        cards.pop_back();
    }
};


int main(){
    Deck deck;
    deck.shuffle();
    Card drawn = deck.draw_card();
    std::cout << drawn << '\n';
}