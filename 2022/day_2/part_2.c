#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#define NO_IMPL (assert(("Trying to use no implemented function",0)))

// //////
// CODEs mean:
// A for Rock, B for Paper, and C for Scissors. 
// X - need to lose, Y - need to end as draw, Z - win
// //////
// So, encoding for A,B and C still be the same:
// A - 0b0000'0001, B - 0b0000'0010, C - 0x0000'0011
// Where when we face either X or Y or Z
// we should choose response according to given strategy
// and then encode it:
// ------- ???? ------------
// //////
// In ASCII:
// A - 0x41, B - 0x42, C - 0x43
// X - 0x58, Y - 0x59, Z - 0x5A
// //////
// RULES:
// Rock defeats Scissors, Scissors defeats Paper, and Paper defeats Rock. 
// So, next combination is means lose for me:
// A & Z -> 0b0011'0001
// C & Y -> 0b0010'0011
// B & X -> 0b0001'0010,
// and draft it's:
// A & X -> 0b0001'0001
// B & Y -> 0b0010'0010
// C & Z -> 0b0011'0011
// win:
// A & Y -> 0b0010'0001
// B & Z -> 0b0011'0010
// C & X -> 0b0001'0011



// What we should return?
// Outcome for one rounde
// max:
// 3 (Scissors) +  6 (win) = 9
// min:
// 1 (rock) + 0 (lose) = 1
// So it's always positive
// uint8_t should be enough
uint8_t compute_score(uint8_t round) {
    uint8_t result = round >> 4; // my choice + outcome

    if (round == 0b00100001 ||
        round == 0b00110010 ||
        round == 0b00010011) 
    { // I won
        result += 6;
    } else 
    if (round == 0b00010001 ||
        round == 0b00100010 ||
        round == 0b00110011)
    { // it's draft
        result += 3;
    } // for loosing I'm adding nothing

    return result;
}


void traverse_file(FILE* f) {
    // I should make conversion uint8_t -> uint32_t
    uint32_t total = 0;

    int c;
    uint8_t round = 0;
    int k = 1;

    const uint8_t A = 0x1;
    const uint8_t B = 0x2;
    const uint8_t C = 0x3;

    while ((c = getc(f)) != EOF) {
        if (c == 0x41) {
            round |= 0b00000001;
        } else if (c == 0x42) {
            round |= 0b00000010;
        } else if (c == 0x43) {
            round |= 0b00000011;
        } else if (c == 0x58) { // lose
            // so, X,Y and Z go always as second number
            // it means we have info of our opponent move 
            // A & C -> 0b0011'0001
            // B & A -> 0b0001'0010,
            // C & B -> 0b0010'0011

            if (round == A) {
                round |= 0x30; // complete to mine C
            } else if (round == B) {
                round |= 0x10; // to mine A
            } else {
                round |= 0x20; // to mine B
            }
        } else if (c == 0x59) { // draw
            // A & X -> 0b0001'0001
            // B & Y -> 0b0010'0010
            // C & Z -> 0b0011'0011
                
            round |= (round << 4);
        } else if (c == 0x5A) { // win
            // A & B -> 0b0010'0001
            // B & C -> 0b0011'0010
            // C & A -> 0b0001'0011
            if (round == A) {
                round |= 0x20;
            } else if (round == B) {
                round |= 0x30;
            } else {
                round |= 0x10;
            }                     
        } else if (c == 0xA) {
            uint32_t for_round = (uint32_t)compute_score(round);
            printf("round %d I have (decimal) %+"PRIu32"\n", k, for_round);
            total += for_round;
            round = 0;
            k += 1;
        }
    }

    // because we face EOF
    uint32_t for_round = (uint32_t)compute_score(round);
    printf("round %d I have (decimal) %+"PRIu32"\n", k, for_round);
    total += for_round;
 
    printf("total score overall rounds: %+"PRIu32"\n", total);
}


int main() {
    FILE* f;

    char *fname = "input.txt";
    f = fopen(fname, "r");

    if (!f) {
        printf("Problem with opening a file %s\n", fname);
        return 1;
    }

    traverse_file(f);

    fclose(f);

    return 0;
}