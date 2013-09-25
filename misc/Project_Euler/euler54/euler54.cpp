/**
 *  @file    euler54.cpp
 *  @author  Marvin Smith
 *  @date    9/24/2013
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const int CLUBS    = 0;
const int HEARTS   = 1;
const int SPADES   = 2;
const int DIAMONDS = 3;

const int HIGH_CARD      = 0;
const int ONE_PAIR       = 1;
const int TWO_PAIR       = 2;
const int THREE_KIND     = 3;
const int STRAIGHT       = 4;
const int FLUSH          = 5;
const int FULL_HOUSE     = 6;
const int FOUR_KIND      = 7;
const int STRAIGHT_FLUSH = 8;
const int ROYAL_FLUSH    = 9;

/**
 * Convert a char character into its int value
 */
int char2value( char const& c ){
    
    if( c == 'A' || c == 'a' ){ return 14; }
    if( c == 'K' || c == 'k' ){ return 13; }
    if( c == 'Q' || c == 'q' ){ return 12; }
    if( c == 'J' || c == 'j' ){ return 11; }
    if( c == 'T' || c == 't' ){ return 10; }
    return (((int)c)-48);
}

char value2char( int const& i ){
    
    switch(i){

        case 14: return 'A';
        case 13: return 'K';
        case 12: return 'Q';
        case 11: return 'J';
        case 10: return 'T';
        default: return ((char)(i+48));
    }
    throw string("ERROR: Unknown parameter");
    return 'x';
}

/**
 * Convert a char character into its suit int value
 */
int char2suit( char const& c ){
    
    if( c == 'S' || c == 's' ){ return SPADES; }
    if( c == 'H' || c == 'h' ){ return HEARTS; }
    if( c == 'C' || c == 'c' ){ return CLUBS; }
    if( c == 'D' || c == 'd' ){ return DIAMONDS; }

    throw string("ERROR: Unknown suit");
    return -1;
}

char suit2char( const int& i ){
    
    switch(i){
        case SPADES:   return 'S';
        case HEARTS:   return 'H';
        case CLUBS:    return 'C';
        case DIAMONDS: return 'D';
        default:       return 'x';
    }
    throw string("ERROR: Unknown parameter");
    return 'x';    
}

/**
 * @class Card
 *
 * Class representing one playing card
 */
class Card{
    
    public:
        
        /**
         *  Constructor given a string
         */
        Card( const string& input ){
            
            // Make sure we have 2 chars, one for value and the other for suit 
            if( input.size() != 2 ){
                throw string("ERROR: Input is probably not a card");
            }

            // input value
            value = char2value(input[0]);

            // input suit
            suit = char2suit(input[1]);

        }

        bool operator < ( const Card& rhs ) const{
            return (value < rhs.value);
        }

        /// Suit (Spades, Hearts, Clubs, and Diamonds)
        int suit;

        /// Value, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 (J), 12 (Q), 13 (K), 14(A)
        int value;

};

ostream& operator << ( ostream& ostr, const Card& rhs ){
    ostr << "Card(" << value2char(rhs.value) << suit2char(rhs.suit) << ")";
    return ostr;
}

class Hand{

    public:
        
        /**
         * Returns true if the hand is a one pair
         */
        bool isOnePair()const{
            
            // iterate through the cards and determine if any two values are the same
            for( size_t i=0; i<cards.size()-1; i++ ){
                for( size_t j=i+1; j<cards.size(); j++ ){
                    if( cards[i].value == cards[j].value ){ return true; }
                }
            }
            return false;
        }
        
        /**
         * Returns true if the hand has two pairs
         */
        bool isTwoPair( )const{
            
            // iterate through the cards and look for two separate pairs
            for( size_t i=  0; i<cards.size()-3; i++ ){
            for( size_t j=i+1; j<cards.size()-2; j++ ){
            for( size_t k=j+1; k<cards.size()-1; k++ ){
            for( size_t l=k+1; l<cards.size()-0; l++ ){
            
                if( ( cards[i].value == cards[j].value ) && ( cards[k].value == cards[l].value )){ return true; }
                if( ( cards[i].value == cards[k].value ) && ( cards[j].value == cards[l].value )){ return true; }
                if( ( cards[i].value == cards[l].value ) && ( cards[j].value == cards[k].value )){ return true; }

            }}}}
            return false;
        }

                /**
         * Returns true if there is a three of a kind
         */
        bool isThreeKind()const{
            
            // iterate through the cards and look for three of akind
            for( size_t i=  0; i<cards.size()-2; i++ ){
            for( size_t j=i+1; j<cards.size()-1; j++ ){
            for( size_t k=j+1; k<cards.size()-0; k++ ){
                if( cards[i].value == cards[j].value && cards[i].value == cards[k].value ){ return true; }
            }}}

            return false;
        }

        
        /**
         * Returns true if the hand is a straight
         */
        bool isStraight()const{
            
            // sort the cards by value
            vector<Card> temp_cards = cards;
            sort( temp_cards.begin(), temp_cards.end() );

            // check for incremental values
            for( size_t i=0; i<4; i++ ){
                
                if( temp_cards[i].value != temp_cards[i+1].value-1){ return false; }
            }

            return true;
        }

        /**
         * Returns true if the hand is a flush
         */
        bool isFlush()const{

            // check if the suits are the same
            for( size_t i=0; i<4; i++ ){
                if( cards[0].suit != cards[i+1].suit ){ return false; }
            }
            return true;
        }
        
        /**
         * Returns true if the hand is a full house
         */
        bool isFullHouse()const{

            // sort cards by value
            vector<Card> temp = cards;
            sort( temp.begin(), temp.end());

            // check the middle indeces for equal values
            if( ( temp[0].value == temp[1].value ) && ( temp[0].value == temp[2].value ) && ( temp[3].value == temp[4].value )){ return true; }
            if( ( temp[0].value == temp[1].value ) && ( temp[2].value == temp[3].value ) && ( temp[3].value == temp[4].value )){ return true; }
            
            return false;
        }


        /**
         * Returns true if the hand is a four of a kind
         */
        bool isFourKind()const{

            // sort cards by value
            vector<Card> temp = cards;
            sort( temp.begin(), temp.end());

            // check that indeces match
            if( temp[0].value == temp[3].value ){ return true; }
            if( temp[1].value == temp[4].value ){ return true; }
            return false;
        }

        /**
         * Returns true if the hand is a straight flush
         */
        bool isStraightFlush()const{
            
            // check if straight and a flush
            return ( isStraight() && isFlush() );
        }

        /**
         * Returns true if the hand is a royal flush
         */
        bool isRoyalFlush()const{

            // check that we have a straight flush
            if( isStraightFlush() ){
                
                // check high card
                if( high_card() == 14 ){ return true; }
            }
            return false;
        }


        /**
         * Return the rank of the hand
         */
        int rank()const{
            
            // look for Royal Flush
            if( isRoyalFlush() == true ){ return ROYAL_FLUSH; }

            // look for straight flush
            if( isStraightFlush() == true ){ return STRAIGHT_FLUSH; }

            // look for four kind
            if( isFourKind() == true ){ return FOUR_KIND; }

            // look for full house
            if( isFullHouse() == true ){ return FULL_HOUSE; }
            
            // look for flush
            if( isFlush() == true ){ return FLUSH; }

            // look for straight
            if( isStraight() == true ){ return STRAIGHT; }
            
            // look for three kind
            if( isThreeKind() == true ){ return THREE_KIND; }

            // look for two pair
            if( isTwoPair() == true ){ return TWO_PAIR; }

            // look for one pair
            if( isOnePair() == true ){ return ONE_PAIR; }

            // otherwise, return high card
            return HIGH_CARD;

        }
        
        /**
         * Return the hand in string format
         */
        string handString()const{
            
            int r = rank();

            switch( r ){

                case ROYAL_FLUSH:     return "Royal Flush";
                case STRAIGHT_FLUSH:  return "Straight Flush";
                case FOUR_KIND:       return "Four of a Kind";
                case FULL_HOUSE:      return "Full House";
                case FLUSH:           return "Flush";
                case STRAIGHT:        return "Straight";
                case THREE_KIND:      return "Three of a Kind";
                case TWO_PAIR:        return "Two Pair";
                case ONE_PAIR:        return "One Pair";
                case HIGH_CARD:       return "High Card";
                default:              return "Unknown";
            }
            return "Unknown";
        }
            
        /**
         * Return the high card value
         */
        int high_card( const bool& absolute = true )const{
            
            // sort the cards by value
            vector<Card> temp = cards;
            sort( temp.begin(), temp.end());
            
            // if we just want the highest, then return the highest
            if( absolute == true ){ return temp[cards.size()-1].value; }

            // if we have a royal flush, just return the highest
            if( isRoyalFlush() == true ){ return temp[cards.size()-1].value; }
            
            // if we have a straight flush, just return the highest
            if( isStraightFlush() == true ){ return temp[cards.size()-1].value; }
            
            // if we have a four kind, return the four pair
            if( isFourKind() == true ){
                if( temp[0].value == temp[3].value ){ return temp[3].value; }
                else{ return temp[4].value; }
            }
            
            // if we have a full house, then return the set of three highs
            if( isFullHouse() == true ){ 
                if( temp[0].value == temp[2].value ){ return temp[2].value; }
                else{ return temp[4].value; }
            }
            // if we have a flush
            if( isFlush() == true ){
                return temp[cards.size()-1].value;
            }

            // if we have a straight
            if( isStraight() == true ){
                return temp[cards.size()-1].value;
            }

            // if we have a three of a kind
            if( isThreeKind() == true ){
                if( temp[0].value == temp[2].value ){ return temp[2].value; }
                else{ return temp[4].value; }
            }

            // if we have a two pair
            if( isTwoPair() == true ){
                if( temp[4].value == temp[5].value ) return temp[5].value;
                return temp[4].value;
            }

            // if we have one pair
            if( isOnePair() == true ){
                if( temp[3].value == temp[4].value ) return temp[4].value;
                if( temp[2].value == temp[3].value ) return temp[3].value;
                if( temp[1].value == temp[2].value ) return temp[2].value;
                if( temp[0].value == temp[1].value ) return temp[1].value;
                return temp[0].value;
            }

            return temp[cards.size()-1].value;
        }

        
        /// List of cards
        vector<Card> cards;
};

bool operator > ( const Hand& handA, const Hand& handB ){
    
    // evaluate hand A and assign ranking
    int rankA = handA.rank();

    // evaluate hand B and assign ranking
    int rankB = handB.rank();

    // if both hands have the same rank
    if( rankA == rankB ){
        return (handA.high_card(false) > handB.high_card(false));
    }

    // otherwise
    return (rankA > rankB);

}


int main( int argc, char* argv[] ){

    try{

        // temp hand
        Hand hand;

        // list of hands
        vector<Hand> p1_hands;
        vector<Hand> p2_hands;

        // open the file and extract all card hands
        ifstream fin;
        fin.open("poker.txt");
    
        string input;
        fin >> input;
     
        while( !fin.eof() ){
        
            // clear the current hand
            hand.cards.clear();

            // hand out five cards to p1
            for( size_t i=0; i<5; i++ ){
        
                hand.cards.push_back(Card(input));            
                fin >> input;

            }
            // give hand to p1
            p1_hands.push_back(hand);
        
            // clear the current hand
            hand.cards.clear();

            // hand out five cards to p2
            for( size_t i=0; i<5; i++ ){
        
                hand.cards.push_back(Card(input));            
                fin >> input;

            }
            // give hand to p2
            p2_hands.push_back(hand);

        }

        fin.close();
        
        // this is the total number of player 1 wins
        int p1_wins = 0;

        // iterate through each hand, finding if you have any special hands
        for( size_t i=0; i<p1_hands.size(); i++ ){
            
            // if player 1 has a higher hand than player 2, then increment
            if( p1_hands[i] > p2_hands[i] ){
                p1_wins++;
            }

            cout << "Player 1 Hand: ";
            for( size_t j=0; j<5; j++ ){ cout << p1_hands[i].cards[j] << ", "; }
            cout << " Hand: " << p1_hands[i].handString() << ", High: " << p1_hands[i].high_card(false) <<  endl;
            cout << "Player 2 Hand: ";
            for( size_t j=0; j<5; j++ ){ cout << p2_hands[i].cards[j] << ", "; }
            cout << " Hand: " << p2_hands[i].handString() << ", High: " << p2_hands[i].high_card(false) << endl;
            if( p1_hands[i] > p2_hands[i] ) cout << "P1 Wins" << endl;
            else                            cout << "P2 Wins" << endl;
            //cin.get();
        }

        cout << "Player 1 won " << p1_wins << " times" << endl;
        cout << "Player 2 won " << (int)p1_hands.size() - p1_wins << " times" << endl;

    } catch( string e ){
        cout << e << endl;
    }
    return 0;
}

