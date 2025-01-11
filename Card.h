// Card.h
#ifndef MYHEADER_H
#define MYHEADER_H


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
    int value;
    int suit;

    // Conversion member variables that are static and shared across cards.
    static std::unordered_map<int, std::string> valueDict;
    static std::unordered_map<int, std::string> suitDict;

    /**
     * @brief Card constructor.
     * 
     * @param v The value of the card.
     * @param s The suit of the card.
     */
    Card(const int v, const int s);
    
    /**
     * @brief Inserts the card into the stream in form "[value] of [suit]".
     * 
     * @param os The stream to insert into.
     * @param card The card to be inserted.
     * @return The stream is returned to allow chaining.
     */
    friend std::ostream &operator<<(std::ostream &os, const Card &card);
};

// Maps to convert card values into strings
std::unordered_map<int, std::string> Card::valueDict;
std::unordered_map<int, std::string> Card::suitDict;

class Deck
{
private:
    std::vector<Card> cards;
    std::mt19937 randomEngine;
public:
    /**
     * @brief Deck constructor.
     * 
     * @param fillWithStandardCards Setting this true will initialize the deck with the 52 standard cards.
     */
    Deck(bool fillWithStandardCards = true);

    /**
     * @brief Shuffle the deck using a uniform random number generator
     * 
     * This shuffle method is the truest shuffle provided, and should be used 
     * in most cases.
     */
    void shuffle();
    /**
     * @brief shuffle the deck by placing cards into piles, then restacking them.
     * 
     * This shuffle method is technically completely deterministic, but will
     * seem random to most players. Its randomness improves as numPiles increases.
     * @param numPiles The number of piles to shuffle cards into before restacking them.
     */
    void shuffle_modulus(int numPiles);

    /**
     * @brief Shuffle the deck by moving chunks of cards.
     * 
     * Grab a handful of cards, then insert them into a new index. This is the 
     * worst way to shuffle the deck, and should probably never be used. But you can.
     * 
     * @param chunkSize How many cards to grab in a chunk.
     * @param chunks How many times to grab and move a chunk.
     */
    void shuffle_chunked(int chunkSize, int chunks);

    /**
     * @brief Split deck in half then restack by alternating which side cards are pulled from.
     * 
     * The most typical shuffle, performed by bridging cards. Setting cardsPerSize = 1
     * is a perfect 1:1 bridge, but larger numbers can be used to mimick worse bridging.
     * This is a deterministic method, and technically not random. Works better for
     * games with more than 2 players and a changing turn order.
     * @param cardsPerSize The number of cards to take from one half before taking from the other half.
     */
    void shuffle_bridge(int cardsPerSide = 1);
    /**
     * @brief Seed the deck's random engine.
     * 
     * @param seed The new seed to use.
     */
    void seed_random_generator(unsigned int seed);

    /**
     * @brief Draw the top card of the deck.
     * 
     * @return Returns the top card off the deck.
     */
    Card draw_top();
    /**
     * @brief Draw the bottom card of the deck.
     * 
     * @return Returns the bottom card off the deck.
     */
    Card draw_bottom();
    /**
     * @brief Draw a card from anywhere in the deck.
     * 
     * 
     * @param includeEnds Whether or not to be able to draw the top or bottom card.
     * @return Returns the card drawn.
     */
    Card draw_random(bool includeEnds = false);

    /**
     * @brief Look at the top card of the deck.
     * 
     * @return Returns the top card of the deck. The card remains in the deck.
     */
    Card peek_top() const;
    /**
     * @brief Look at the bottom card of the deck.
     * 
     * @return Returns the bottom card of the deck. The card remains in the deck.
     */
    Card peek_bottom() const;
    /**
     * @brief Look at a card from anywhere in the deck without removing it
     * 
     * 
     * @param includeEnds Whether or not to be able to peek the top or bottom card.
     * @return Returns the card peeked. The card remains in the deck.
     */
    Card peek_random(bool includeEnds = false) const;

    /* Add a card to the top of the deck */
    void place_top(Card &card);
    /* Add a card to the bottom of the deck.*/
    void place_bottom(Card &card);
    /**
     * @brief Place a card somewhere into the deck.
     * 
     * 
     * @param includeEnds Whether or not the card can go on the top or bottom.
     */
    void place_random(Card &card, bool includeEnds = false);

    /**
     * @brief display the deck line by line from the bottom to the top.
     */
    void display_deck();
};

} // end namespace DeckSystem


#endif