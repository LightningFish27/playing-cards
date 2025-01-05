#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

struct Card{
    int value; // The value of the card. Ace is 1.
    int suit; // Hearts, Diamonds, Spades, Clubs
    Card(const int v, const int s) : value(v), suit(s) {}

    friend std::ostream& operator<<(std::ostream& os, const Card& card){
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
            for (int v = 1; v < 14; v++){ // For each value
                cards.emplace_back(Card(v, s));
            }
        }
    }
    // Shuffle a deck randomly. Keeps a static random device to ensure each shuffle
    // is unique
    void shuffle(){
        static std::random_device rd;
        std::shuffle(cards.begin(), cards.end(), rd);
    }
    // Shuffle a deck semi-randomly. For example, modulus shuffles or chunked shuffles.
    // MethodComplement will be used for any shuffle types that need an additional variable,
    // such as how many cards should be in a chunk.
    void shuffle_method(int shuffleMethod, int methodComplement){
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
    Card draw_top(){
        Card r = cards[cards.size()];
        cards.pop_back();
        return r;
    }
    // Draw a card from the bottom of the deck.
    Card draw_bottom(){
        Card r = cards.front();
        cards.erase(cards.begin());
        return r;
    }
    // Draw a card from somewhere in the middle of the deck.
    // includeEnds determines whether the top and bottom card can be drawn.
    Card draw_random(bool includeEnds = false){
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        Card r = cards[index];
        cards.erase(cards.begin() + index);
        return r;
    }

    // Show the top card without removing it from deck
    Card peek_top() const {
        return cards.back();
    }
    // Show the bottom card without removing it from deck
    Card peek_bottom() const {
        return cards.front();
    }
    // Peek at a random card without removing it from deck
    Card peek_random(bool includeEnds = false) const {
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        return cards[index];
    }

    // Add a card to the top of the deck
    void place_top(Card& card) {
        cards.push_back(card);
    }
    // Add a card to the bottom of the deck
    void place_bottom(Card& card) {
        cards.insert(cards.begin(), card);
    }
    // Add a card to a random spot within the deck
    void place_random(Card& card, bool includeEnds = false) {
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        cards.insert(cards.begin() + index, card);
    }
};


int main(){
    srand(time(0));
    Deck deck;
    deck.shuffle();
    std::cout << deck.draw_random();
}