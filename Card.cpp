#include <vector>
#include <unordered_map>
#include <deque>
#include <queue>
#include <iostream>
#include <algorithm>
#include <random>

struct Card{
    int value; // The value of the card. Ace is 1. Alternate values left to implementation
    int suit; // Hearts, Diamonds, Spades, Clubs

    // Conversion member variables that are static and shared across cards.
    static std::unordered_map<int, std::string> valueDict;
    static std::unordered_map<int, std::string> suitDict;

    // Constructor for cards
    Card(const int v, const int s) : value(v), suit(s) {}

    /*  Overriding stream insertion operator to allow easy printing of cards. 
        Can also be used alongside a stringstream to process card names.*/
    friend std::ostream& operator<<(std::ostream& os, const Card& card) {
        os << Card::valueDict[card.value] << " of " << Card::suitDict[card.suit];
        return os;
    }
};

// Maps to convert card values into strings 
std::unordered_map<int, std::string> Card::valueDict = {
    {1, "ace"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"},
    {6, "six"}, {7, "seven"}, {8, "eight"}, {9, "nine"}, {10, "ten"},
    {11, "jack"}, {12, "queen"}, {13, "king"}
};

std::unordered_map<int, std::string> Card::suitDict = {
    {0, "hearts"}, {1, "diamonds"}, {2, "spades"}, {3, "clubs"}
};


class Deck{
private:
    std::vector<Card> cards; 
    std::mt19937 randomEngine; // random number engine to allow controlling the seed.
public:
    /*  Default deck constructor. Takes a boolean argument that determines wether to
        initialize the deck with the standard 52 playing cards or not. Parameter defaults to
        true. Also seeds the random engine with the current time. */
    Deck(bool fillWithStandardCards = true){
        if (fillWithStandardCards){
            cards.reserve(52); // Save on a few vector resizes
            for (int s = 0; s < 4; s++){ // For each suit
                for (int v = 1; v < 14; v++){ // For each value
                    cards.emplace_back(Card(v, s));
                }
            }
        }
        randomEngine.seed(static_cast<unsigned int>(time(0)));
    }


    // Shuffle a deck randomly. 
    void shuffle(){
        std::shuffle(cards.begin(), cards.end(), randomEngine);
    }

    /*  Shuffle the deck into separate piles, then restack the piles in the new order.
        This seems random in play, but technically follows a rigid pattern. */
    void shuffle_modulus(int numPiles){
        std::vector<std::deque<Card>> piles(numPiles); // create a vector of piles
        for (int i = cards.size(); i >= 0; i--) { // for every card in deck from top to bottom
            piles[i % numPiles].push_back(cards[i]); // add the card to a pile based on index
        }
        cards.clear(); // empty the original deck to refill it
        for (const auto& pile : piles) { // for each pile
            for (const Card card : pile) { // for each card in each pile
                cards.push_back(card); // add the card back to the deck
            }
        }
    }

    /*  Shuffle the deck by grabbing chunks of cards and inserting them into new positions.
        chunkSize determines number of cards within each chunk.
        chunks determines how many times to repeat this process.
        This is a bad way to shuffle a deck of cards. Use when appropriate. */
    void shuffle_chunked(int chunkSize, int chunks){
        for (int repetition = 0; repetition < chunks; repetition++){ // for each chunk
            std::queue<Card> chunk;

            // Get a position to start grabbing chunk from
            std::uniform_int_distribution<> dist(0, cards.size() - 1);
            int startIndex = dist(randomEngine); // use existing random engine
            startIndex = std::clamp(startIndex, 0, static_cast<int>(cards.size()) - chunkSize - 1);

            // Fill the chunk
            for (int i = 0; i < chunkSize; i++){
                chunk.push(cards[startIndex]); // add card to chunk
                cards.erase(cards.begin() + startIndex); // remove card from deck
            }

            // get an index to place the chunk into
            int targetIndex = dist(randomEngine);
            targetIndex = std::clamp(targetIndex, 0, static_cast<int>(cards.size()));
            // empty the chunk into that location of the deck
            while (!chunk.empty()) {
                cards.insert(cards.begin() + targetIndex, chunk.front());
                chunk.pop();
                targetIndex++; // Adjust targetIndex to maintain proper insertion order, rather than flipping the order
            }
        }
    }

    /*  Split the deck into two halves, then put them back together by alternating
        which side cards are taken from. If cardsPerSide is left at the default 1,
        this is a perfect bridge. Changing it changes how many cards are taken from
        one side before switching to the other. */
    void shuffle_bridge(int cardsPerSide = 1){
        std::vector<Card> h1;
        h1.reserve(cards.size() / 2);
        std::vector<Card> h2;
        h2.reserve(cards.size() / 2);
        // Split deck into rough halves
        for (int i = 0; i < cards.size(); i++){
            if (i < cards.size() / 2){
                h1.push_back(cards[i]);
            } else {
                h2.push_back(cards[i]);
            }
        }
    }

    /*  Set the random engine's seed to a new seed. 
        Useful for controlling replicability. */
    void seed_random_generator(unsigned int seed){
        randomEngine.seed(seed);
    }

    /* Draw a card from the top of the deck. */
    Card draw_top(){
        Card r = cards[cards.size()];
        cards.pop_back();
        return r;
    }
    /* Draw a card from the bottom of the deck. */
    Card draw_bottom(){
        Card r = cards.front();
        cards.erase(cards.begin());
        return r;
    }
    /*  Draw a card from somewhere in the middle of the deck.
        includeEnds determines whether the top and bottom card can be drawn. */
    Card draw_random(bool includeEnds = false){
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        Card r = cards[index];
        cards.erase(cards.begin() + index);
        return r;
    }

    /* Show the top card without removing it from deck */
    Card peek_top() const {
        return cards.back();
    }
    /* Show the bottom card without removing it from deck */
    Card peek_bottom() const {
        return cards.front();
    }
    /*  Peek at a random card without removing it from deck.
        includeEnds determines whether the top and bottom card are options. */
    Card peek_random(bool includeEnds = false) const {
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        return cards[index];
    }

    /* Add a card to the top of the deck */
    void place_top(Card& card) {
        cards.push_back(card);
    }
    /* Add a card to the bottom of the deck.*/
    void place_bottom(Card& card) {
        cards.insert(cards.begin(), card);
    }
    /*  Add a card to a random spot within the deck.
        includeEnds determines whether the top and bottom are valid placements. */
    void place_random(Card& card, bool includeEnds = false) {
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        cards.insert(cards.begin() + index, card);
    }

    /* Print the contents of the deck line by line from bottom to top.*/
    void display_deck(){
        for (const auto& card : cards){
            std::cout << card << '\n';
        }
    }

};


int main(){
    Deck deck;

    deck.shuffle_chunked(3, 10);

    deck.display_deck();

}