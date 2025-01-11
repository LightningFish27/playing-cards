#include <vector>
#include <unordered_map>
#include <deque>
#include <queue>
#include <iostream>
#include <algorithm>
#include <random>

namespace DeckSystem{

struct Card
{
    int value; // The value of the card. Ace is 1. Alternate values left to implementation
    int suit;  // Hearts, Diamonds, Spades, Clubs

    // Conversion member variables that are static and shared across cards.
    static std::unordered_map<int, std::string> valueDict;
    static std::unordered_map<int, std::string> suitDict;

    /**
     * @brief Card constructor.
     * 
     * @param v The value of the card.
     * @param s The suit of the card.
     */
    Card(const int v, const int s) : value(v), suit(s) {}

    /**
     * @brief Inserts the card into the stream in form "[value] of [suit]".
     * 
     * @param os The stream to insert into.
     * @param card The card to be inserted.
     * @return The stream is returned to allow chaining.
     */
    friend std::ostream &operator<<(std::ostream &os, const Card &card)
    {
        os << Card::valueDict[card.value] << " of " << Card::suitDict[card.suit];
        return os;
    }
};

// Maps to convert card values into strings
std::unordered_map<int, std::string> Card::valueDict = {
    {1, "ace"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}, {6, "six"}, {7, "seven"}, {8, "eight"}, {9, "nine"}, {10, "ten"}, {11, "jack"}, {12, "queen"}, {13, "king"}};
std::unordered_map<int, std::string> Card::suitDict = {
    {0, "hearts"}, {1, "diamonds"}, {2, "spades"}, {3, "clubs"}};

class Deck
{
private:
    std::vector<Card> cards;
    std::mt19937 randomEngine; // random number engine to allow controlling the seed.
public:
    /**
     * @brief Deck constructor.
     * 
     * @param fillWithStandardCards Setting this true will initialize the deck with the 52 standard cards.
     */
    Deck(bool fillWithStandardCards = true)
    {
        if (fillWithStandardCards)
        {
            cards.reserve(52); // Save on a few vector resizes
            for (int s = 0; s < 4; s++)
            { // For each suit
                for (int v = 1; v < 14; v++)
                { // For each value
                    cards.emplace_back(Card(v, s));
                }
            }
        }
        randomEngine.seed(static_cast<unsigned int>(time(0)));
    }

    /**
     * @brief Shuffle the deck using a uniform random number generator
     * 
     * This shuffle method is the truest shuffle provided, and should be used 
     * in most cases.
     */
    void shuffle()
    {
        std::shuffle(cards.begin(), cards.end(), randomEngine);
    }
    /**
     * @brief shuffle the deck by placing cards into piles, then restacking them.
     * 
     * This shuffle method is technically completely deterministic, but will
     * seem random to most players. Its randomness improves as numPiles increases.
     * @param numPiles The number of piles to shuffle cards into before restacking them.
     */
    void shuffle_modulus(int numPiles)
    {
        std::vector<std::deque<Card>> piles(numPiles); // create a vector of piles
        for (int i = cards.size(); i >= 0; i--)
        {                                            // for every card in deck from top to bottom
            piles[i % numPiles].push_back(cards[i]); // add the card to a pile based on index
        }
        cards.clear(); // empty the original deck to refill it
        for (const auto &pile : piles)
        { // for each pile
            for (const Card card : pile)
            {                          // for each card in each pile
                cards.push_back(card); // add the card back to the deck
            }
        }
    }

    /**
     * @brief Shuffle the deck by moving chunks of cards.
     * 
     * Grab a handful of cards, then insert them into a new index. This is the 
     * worst way to shuffle the deck, and should probably never be used. But you can.
     * 
     * @param chunkSize How many cards to grab in a chunk.
     * @param chunks How many times to grab and move a chunk.
     */
    void shuffle_chunked(int chunkSize, int chunks)
    {
        for (int repetition = 0; repetition < chunks; repetition++)
        { // for each chunk
            std::queue<Card> chunk;

            // Get a position to start grabbing chunk from
            std::uniform_int_distribution<> dist(0, cards.size() - 1);
            int startIndex = dist(randomEngine); // use existing random engine
            startIndex = std::clamp(startIndex, 0, static_cast<int>(cards.size()) - chunkSize - 1);

            // Fill the chunk
            for (int i = 0; i < chunkSize; i++)
            {
                chunk.push(cards[startIndex]);           // add card to chunk
                cards.erase(cards.begin() + startIndex); // remove card from deck
            }

            // get an index to place the chunk into
            int targetIndex = dist(randomEngine);
            targetIndex = std::clamp(targetIndex, 0, static_cast<int>(cards.size()));
            // empty the chunk into that location of the deck
            while (!chunk.empty())
            {
                cards.insert(cards.begin() + targetIndex, chunk.front());
                chunk.pop();
                targetIndex++; // Adjust targetIndex to maintain proper insertion order, rather than flipping the order
            }
        }
    }

    /**
     * @brief Split deck in half then restack by alternating which side cards are pulled from.
     * 
     * The most typical shuffle, performed by bridging cards. Setting cardsPerSize = 1
     * is a perfect 1:1 bridge, but larger numbers can be used to mimick worse bridging.
     * This is a deterministic method, and technically not random. Works better for
     * games with more than 2 players and a changing turn order.
     * @param cardsPerSize The number of cards to take from one half before taking from the other half.
     */
    void shuffle_bridge(int cardsPerSide = 1)
    {
        std::vector<Card> h1;
        h1.reserve(cards.size() / 2);
        std::vector<Card> h2;
        h2.reserve(cards.size() / 2);
        // Split deck into rough halves
        for (int i = 0; i < cards.size(); i++)
        {
            if (i < cards.size() / 2)
            {
                h1.push_back(cards[i]);
            }
            else
            {
                h2.push_back(cards[i]);
            }
        }
    }

    /**
     * @brief Seed the deck's random engine.
     * 
     * @param seed The new seed to use.
     */
    void seed_random_generator(unsigned int seed)
    {
        randomEngine.seed(seed);
    }

    /**
     * @brief Draw the top card of the deck.
     * 
     * @return Returns the top card off the deck.
     */
    Card draw_top()
    {
        Card r = cards[cards.size()];
        cards.pop_back();
        return r;
    }
    /**
     * @brief Draw the bottom card of the deck.
     * 
     * @return Returns the bottom card off the deck.
     */
    Card draw_bottom()
    {
        Card r = cards.front();
        cards.erase(cards.begin());
        return r;
    }
    /**
     * @brief Draw a card from anywhere in the deck.
     * 
     * 
     * @param includeEnds Whether or not to be able to draw the top or bottom card.
     * @return Returns the card drawn.
     */
    Card draw_random(bool includeEnds = false)
    {
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        Card r = cards[index];
        cards.erase(cards.begin() + index);
        return r;
    }

    /**
     * @brief Look at the top card of the deck.
     * 
     * @return Returns the top card of the deck. The card remains in the deck.
     */
    Card peek_top() const
    {
        return cards.back();
    }
    /**
     * @brief Look at the bottom card of the deck.
     * 
     * @return Returns the bottom card of the deck. The card remains in the deck.
     */
    Card peek_bottom() const
    {
        return cards.front();
    }
    /**
     * @brief Look at a card from anywhere in the deck without removing it
     * 
     * 
     * @param includeEnds Whether or not to be able to peek the top or bottom card.
     * @return Returns the card peeked. The card remains in the deck.
     */
    Card peek_random(bool includeEnds = false) const
    {
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        return cards[index];
    }

    /* Add a card to the top of the deck */
    void place_top(Card &card)
    {
        cards.push_back(card);
    }
    /* Add a card to the bottom of the deck.*/
    void place_bottom(Card &card)
    {
        cards.insert(cards.begin(), card);
    }
    /**
     * @brief Place a card somewhere into the deck.
     * 
     * 
     * @param includeEnds Whether or not the card can go on the top or bottom.
     */
    void place_random(Card &card, bool includeEnds = false)
    {
        int index = !includeEnds + (random() % cards.size() - !includeEnds);
        cards.insert(cards.begin() + index, card);
    }

    /**
     * @brief display the deck line by line from the bottom to the top.
     */
    void display_deck()
    {
        for (const auto &card : cards)
        {
            std::cout << card << '\n';
        }
    }
};

} // end namespace DeckSystem