#include <iostream>
#include <bitset>

using namespace std;

#define BITSET_SIZE 193


/**
 * @brief Calculate the Hamming distance by calculating different bits. To get only the different bits we can perfectly use XOR.
 * @param a
 * @param b
 * @return the number of different bits
 */
size_t hamming_distance(const bitset<BITSET_SIZE> &a,
                        const bitset<BITSET_SIZE> &b) {
    bitset<BITSET_SIZE> xor_bitset = a^b;
    return xor_bitset.count();
}
/**
 * @brief Function for estimating the correlation by math formula 1-h(u,z)/N
 * @param u - estimated lfsr sequence
 * @param z - given key stream
 * @return double between 0 and 1, where 1 is the highest correlation
 */
double estimate_correlation(const bitset<BITSET_SIZE> &u,
                            const bitset<BITSET_SIZE> &z) {
    return 1 - (double(hamming_distance(u, z)) / BITSET_SIZE);
}

/**
 * @brief Function for generating a lfsr sequence according to some given state and generating polynomial
 * @param p_len - length of the given polynomial
 * @param polynomial - bitset representing known polynomial
 * @param possible_init_state - number of possible initial state, number is from interval 0 to (2^p_len - 1).
 * @return a bitset which can be the possible lfsr sequence
 */
bitset<BITSET_SIZE> generate_u(size_t p_len,
                               const bitset<BITSET_SIZE> &polynomial,
                               size_t possible_init_state){
    bitset<BITSET_SIZE> mask(size_t(pow(2,p_len)-1));
    bitset<BITSET_SIZE> possible_sequence(possible_init_state);
    for (size_t j = 0; j < (BITSET_SIZE - p_len); j++) {
        bitset<BITSET_SIZE> one_clock = (possible_sequence & mask) & polynomial;
        size_t new_bit = one_clock.count() % 2;
        possible_sequence <<= 1;
        bitset<BITSET_SIZE> increment(new_bit);
        possible_sequence |= increment;
    }
    return possible_sequence;
}

/**
 * @brief Function for generating the key. It tries all possible initial state by binary representation of number from 0 to 2^p_len - 1.
 * Than it calculates p* for each possible given sequence and find the one with highest p*
 * @param p_len length of the given polynomial
 * @param polynomial bitset representing the polynomial
 * @param key_stream given key stream'
 * @return sequence with highest p* value
 */
bitset<BITSET_SIZE> get_key(size_t p_len,
                            const bitset<BITSET_SIZE> &polynomial,
                            const bitset<BITSET_SIZE> &key_stream) {
    double p_star = 0.5;
    bitset<BITSET_SIZE> best_seq_so_far(0);
    for (size_t i = 0; i < size_t(pow(2, p_len)); i++) {
        bitset<BITSET_SIZE> possible_sequence = generate_u(p_len, polynomial, i);
        double tmp_p = estimate_correlation(possible_sequence, key_stream);
        if (tmp_p > p_star) {
            p_star = tmp_p;
            best_seq_so_far.reset();
            best_seq_so_far |= possible_sequence;
        }
    }
    cout << "p*: " << p_star << ", k: ";
    return best_seq_so_far;
}


int main(){
    //polynomial n. 1
    bitset<BITSET_SIZE> C_1(string("1011001101011"));
    //polynomial n. 2
    bitset<BITSET_SIZE> C_2(string("101011001101010"));
    //polynomial n. 2
    bitset<BITSET_SIZE> C_3(string("11001001010011010"));
    //given key stream
    bitset<BITSET_SIZE> key_stream(string("01101000001111000100110011001100100001000001011110"
                                          "11011110011001101011000000111111000001011010100010"
                                          "01011000111100010111110111011100010101001111111101"
                                          "0100110001100110111001110000101011101010011"));
    cout << "K = (" << get_key(13, C_1, key_stream).to_string().substr(BITSET_SIZE-13,13);
    cout << ", " << get_key(15, C_2, key_stream).to_string().substr(BITSET_SIZE-15,15);
    cout << ", " << get_key(17, C_3, key_stream).to_string().substr(BITSET_SIZE-17,17) << ")" << endl;
    return 0;
}
