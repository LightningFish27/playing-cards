#include <vector>

struct Card{
    int value; // The value of the card. Ace is 1.
    int suit; // Hearts, Diamonds, Spades, Clubs
    Card(const int v, const int s) : value(v), suit(s) {}
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
};