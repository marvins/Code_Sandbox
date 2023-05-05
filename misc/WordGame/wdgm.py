#!/usr/bin/env python3

import random
import string

def load_words( pathname ):

    list_of_words = []
    with open( pathname, 'r' ) as fin:
        for line in fin.readlines():

            # Remove the newline, then convert to lower-case
            list_of_words.append( line.strip().lower() )
    return list_of_words

def create_random_word( num_letters ):
    '''
    Given a number of letters, will create a string of random letters between [a-z]
    Note:  Forces lowercase so we don't mix types
    '''
    return (''.join(random.choice(string.ascii_letters) for x in range(num_letters))).lower()

def compute_histogram( word ):

    hist = {}
    for letter in word:
        if letter in hist.keys():
            hist[letter] += 1
        else:
            hist[letter] = 1
    return hist

def is_inside( dict1, dict2 ):
    '''
    This checks if dict2 is inside dict1
    aka:  Does dict1 contain every letter for dict2
    '''

    for letter in dict2.keys():
        if letter in dict1.keys():
            if dict1[letter] == dict2[letter]:
                pass
            else:
                return False
        else:
            return False

    return True


def compute_word_overlap( word1_hist, word2_hist ):

    #  Increment over each letter in alphabet, then
    # see if they both fall in each word and how often
    result = {}
    full_word1 = is_inside( word2_hist, word1_hist )
    full_word2 = is_inside( word1_hist, word2_hist )
    for letter in string.ascii_letters[0:26]:
        if ( letter in word1_hist.keys() ) and ( letter in word2_hist.keys() ):
            result[letter] = min( word1_hist[letter], word2_hist[letter] )


    return [result, full_word1, full_word2]

def compute_score( overlap, is_full_copy ):

    VOWELS = 'aeiou'
    score = 0
    for letter in overlap.keys():

        #  Check if vowel
        multiplier = 2
        if letter in VOWELS:
            multiplier = 3
        score += multiplier * overlap[letter]

    if is_full_copy:
        score += 25

    return score

def sort_by_score( entry ):
    return entry[0]

if __name__ == '__main__':

    ##  Run test of overlap
    #wordA = 'foo'
    #wordB = 'foobar'
    #wordA_hist = compute_histogram( wordA )
    #wordB_hist = compute_histogram( wordB )
    #overlap = compute_word_overlap( wordA_hist, wordB_hist )
    #print( f'WordA: {wordA_hist}, WordB: {wordB_hist}' )
    #print( f'overlap: {overlap}' )
    #input('pause')

    # Run test of Compute Score (WORK, not all 7 letters)
    overlap = [ { 'w': 1, 'o': 1, 'r': 1, 'k': 1 }, False ]
    score = compute_score( overlap[0], overlap[1] )
    assert( score == 9 )

    #  All 7 letters
    overlap = [ { 'a': 1, 'c': 1, 'e': 1, 'f': 1, 'l': 1, 'r': 1, 'u': 1 }, True ]
    score = compute_score( overlap[0], overlap[1] )
    assert( score == 42 )

    #  List your static variables here so you can test and isolate logic
    NUMBER_LETTERS = 7

    #  Load the list of words from the file
    list_of_words = load_words( 'words.txt' )

    #  create a random word
    #  PROTIP:  Comment out this function and just add a test string to verify this shit works
    #ex:  test_word = 'hello'
    test_word = create_random_word( NUMBER_LETTERS )
    test_word_hist = compute_histogram( test_word )
    print( f'Random Word: {test_word}, Hist: {test_word_hist}' )

    #  iterate over each word
    overlapping_words = []
    for word in list_of_words:

        #  Build histogram of word
        word_hist = compute_histogram( word )

        #  Check overlap
        overlap = compute_word_overlap( word_hist, test_word_hist )
        if overlap[1] == True or overlap[2] == True:
            score = compute_score( overlap[0], overlap[1] )
            overlapping_words.append( [score, word] )

    #  All words have been processed, what's the best?
    #  -> We need to sort by the score
    overlapping_words.sort(key=sort_by_score)
    best_score = overlapping_words[-1]


    #  Now print our results
    print( f'Randomly Generated Word: {test_word}' )
    print( f'Randomly Generated Word Histogram:\n{test_word_hist}' )
    print( '' )
    print( f' Number of words in master list that score points against random word: {len(overlapping_words)}' )
    print( ' -> NOTE:  Ordered by score (Low -> High):')
    for w in overlapping_words:
        print( f'Word: {w[1]}, Score: {w[0]}' )
