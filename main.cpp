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
using std::vector;

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
float bias[N_STATES][N_OUTPUTS];
vector<int> obs; // A vector of the observed outputs
string outputs[] = {"H", "T"};
vector< vector< state_t* > > outputTable;

// Function declaration
state_t* getPrev(int d, int m); // State: d, Output: m
void createTable();
void deleteTable();
void readTransitions(string fileName);
void readBias(string fileName);
void readObs(string fileName);
void printTransitions();
void printBias();
void printObs();

int main() {
    string transitionFile = "./Tests/chain-1.txt";
    string biasFile = "./Tests/emit-1.txt";
    string obsFile = "./Tests/obs-1.txt";

    // Read the transitions, bias, and the observed outputs
    readTransitions(transitionFile);
    readBias(biasFile);
    readObs(obsFile);

    D( printTransitions(); );
    D( printBias(); );
    D( printObs(); );

    // Create the table
    createTable();

    // Delete the table
    deleteTable();
}

void createTable() {
    // This function populates the DxM 2-D vector of pointers to state_t structs
    // This table will be used to compute the path of highest probability
    // for a specific output
    float prob; // Probability must be computed for each element in the table
    state_t* prev; // The previous state

    // Must loop trough each of the states to find the maximum probablity for each of the final observed outputs
    for(int i = 0; i < N_STATES; i++) {
        state_t* s = new state_t();
        s->state = i;
        s->output = obs.back();
        prev = getPrev(i, obs.back()); // Compute the maximum proability of getting output
    }

}

state_t * getPrev(int d, int m) {
    // This function returns a pointer to the state that gives you the highets probability of
    // getting the output 
}

void deleteTable() {
    for(int i = 0; i < outputTable.size(); i++) {
        vector< state_t* > current = outputTable[i];
        for (std::vector< state_t* >::iterator it = current.begin() ; it != current.end(); ++it)
        {
          delete (*it);
        }
        current.clear();
    }
    outputTable.clear();
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

void readBias(string fileName) {
    // This function reads in the bias of the coins in the different states

    float h; // Probablity of seeing a head
    float t; // Probablity of seeing a tail
    // Create the filestream
    ifstream f(fileName, std::ifstream::in);

    // Loop through the number of states
    for(int i = 0; i < N_STATES; i++) {
        f >> h; // Read in the probablity of seeing a head in state i
        t = 1-h;
        bias[i][0] = h;
        bias[i][1] = t;
    }
}

void readObs(string fileName) {
    // This function reads in the observed outputs into a vector
    ifstream f(fileName, std::ifstream::in);

    // Read in the observed states until they can't be read anymore
    int s;

    while(f >> s) {
        obs.push_back(s);
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

void printBias() {
    // Print out the bias for each of the states
    cout << "\nPrinting the bias: " << endl;
    cout << "Rows: states Columns: outputs" << endl;
    for(int i = 0; i < N_STATES; i++) {
        for(int j = 0; j < N_OUTPUTS; j++) {
            cout << bias[i][j] << "\t";
        }
        cout << endl;
    }
}

void printObs() {
    // Print the observed outputs
    cout << "\nObserved outputs:" << endl;
    for(int i = 0; i < obs.size(); i++) {
        cout << outputs[obs[i]] << " ";
    }
    cout << endl;
}
