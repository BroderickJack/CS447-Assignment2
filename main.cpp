/*

Input file descriptions

emit

The probability of getting heads in each of the states


*/


#include <iostream>
#include <vector>
#include <string>
#include <iostream> // For reading files
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::endl;
using std::string;

#define N_STATES 2 // The number of possible states in the problem
#define N_OUTPUTS 2 // The number of different outputs

// Setup debugging
#define DEBUG 1

#ifdef DEBUG
#define D(X) X
#else
#define D(X)
#endif

struct state_t {
    state_t * previous_state;
    int state; // (n)
    int output; // (m) The character that is output at this state
    float prob; // The max probablility of getting the output Xm from state n assuming the previous state was state_t * previous
};

// Encode the state transitions as a n x n vector whwhere there are n states
float transitions[N_STATES][N_STATES];

// Encode the bias of each coin in each state
float bias[N_STATES][N_OUTPUTS];

void readTransitions(string fileName);
void printTransitions();
void readBias(string fileName);
void printBias();

int main() {
    string transitionFile = "./Tests/chain-1.txt";
    readTransitions(transitionFile);

    D( printTransitions(); );

}

void readTransitions(string fileName) {
    // This is a function to read in the transitions
    ifstream f(fileName, std::ifstream::in);

    for(int i = 0; i < N_STATES; i++) {
        for(int j = 0; j < N_STATES; j++) {
            f >> transitions[i][j];
        }
    }
}

void printTransitions() {
    // Print the transition matrix
    cout << "Transition Matrix:\n";
    for(int i = 0; i < N_STATES; i++) {
        for(int j = 0; j < N_STATES; j++) {
            cout <<  transitions[i][j] << "\t";
        }
        cout << endl;
    }
}

void readBias(string fileName) {
    // This function reads in the bias of the coins in the different states

    float h; // Probablity of seeing a head
    float t; // Probablity of seeing a tail
    // Create the filestream
    ifstream f(fileName, std::ifstream::in);

    // Loop through the number of states
    for(int i = 0; i < N_STATES; i++) {
        f >> h; // Read in the probablity of seeing a head in state i

    }
}
