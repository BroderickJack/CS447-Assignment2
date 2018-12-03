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
#define START_STATE 0 // The start state

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
    int output; // (m) the character that is output
    float prob; // The max probablility of getting the output Xm from state n assuming the previous state was state_t * previous
};

// Encode the state transitions as a n x n vector whwhere there are n states
float transitions[N_STATES][N_STATES];
float bias[N_STATES][N_OUTPUTS];
string outputs[] = {"H", "T"};

int recursion = 0;

// Function declaration
void getPrev(int d, int m, vector< vector< state_t *> > & outputTable, vector<int> obs); // State: d, Output: m
vector< vector< state_t* > > createTable(vector<int> obs);
void deleteTable(vector< vector< state_t* > >);
void readTransitions(string fileName);
void readBias(string fileName);
vector<int> readObs(string fileName);
void printTransitions();
void printBias();
void printObs(vector<int> obs);
void printTab(int);
void findPath(vector< vector< state_t * > > outputTable, vector<int> obs);

int main() {
    string transitionFile = "./Tests/chain-1.txt";
    string biasFile = "./Tests/emit-1.txt";
    string obsFile = "./Tests/obs-1.txt";

    // Initialize a vector
    vector<int> obs; // A vector of the observed outputs


    // Read the transitions, bias, and the observed outputs
    readTransitions(transitionFile);
    readBias(biasFile);
    obs = readObs(obsFile);

    D( printTransitions(); );
    D( printBias(); );
    D( printObs(obs); );

    // Create the table
    vector< vector< state_t* > > outputTable = createTable(obs);

    // Calculate the most likely path of states for each output
    findPath(outputTable, obs);

    // Delete the table
    deleteTable(outputTable);
}

void findPath(vector< vector< state_t *> > outputTable, vector<int> obs) {
    // This function finds the path of highest likelihood by working backwards
    cout << endl << endl << "---------- FINDING THE PATH ------------- " << endl;
    cout << "Number Observed: " << obs.size() << endl;
    int numObs = obs.size();

    float maxProb = 0;
    float curProb = 0;
    float maxState;
    for(int i = 0; i < N_STATES; i++) {
        cout << "outputTable[" << i << "][" << numObs << "] == NULL: " << (outputTable[i][numObs] == NULL) << endl;
        curProb = (outputTable[i][numObs-1])->prob;
        cout << "Currnet Output " << endl;
        if(curProb > maxProb) {
            maxProb = curProb;
            maxState = i;
        }
    }

    cout << "Found end state " << endl;

    // Now we know the end state with the maximum probability and we can work backwards from there
    state_t * maxEnd = outputTable[maxState][numObs-1];

    cout << "MaxEnd == NULL: " << (maxEnd == NULL) << endl;

    cout << "End State: " << maxEnd->state << " Output Number: " << maxEnd->output << endl;

    state_t *prev = maxEnd;
    state_t *next;

    // while((prev = prev->previous_state) != NULL) {
    //     cout << "Output: " << prev->output << " State: " << prev->state << " Probability: " << prev->prob << endl;
    // }

}

vector< vector< state_t* > > createTable(vector<int> obs) {
    vector< vector< state_t* > > outputTable( N_STATES, vector< state_t*>(N_OUTPUTS));
    // This function populates the DxM 2-D vector of pointers to state_t structs
    // This table will be used to compute the path of highest probability
    // for a specific output
    float prob; // Probability must be computed for each element in the table
    state_t* prev; // The previous state

    // Fill the table with the required values
    for( int d = 0; d < N_STATES; d++ ) {
        for (int m = 0; m < obs.size(); m++ ) {
            state_t * cur = new state_t();
            cur->state = d;
            cur->output = obs[m];
            cur->previous_state = NULL;
            outputTable[d][m] = cur;
        }
    }

    // Must loop trough each of the states to find the maximum probablity for each of the final observed outputs
    for(int d = 0; d < N_STATES; d++) {
        int m = obs.size() - 1; // We want to start with the last output
        getPrev(d, m, outputTable, obs); // Compute the maximum proability of getting output
    }

    return outputTable;

}

void getPrev(int d, int m, vector< vector< state_t *> > & outputTable, vector<int> obs) {
    // This function fills into the table the element for state d at output m
    // STATE: d
    // OUTPUT: m
    int output = obs[m]; // Get the actual value of the output
    D( printTab(obs.size()-m); cout << " Output Index: " << m << " State: " << d << " Output Value: " << output <<  endl; );
    int currentRecursion = recursion;

    state_t *cur = outputTable[d][m];
    if (cur == NULL) { cout << "Cur is null" << endl; }

    // First check the base case, if it is the first output (output 0) then
    // Then it must be in the start state and the proability is the probability of that output
    if( m == 0) {
        // D( cout << "First output" << endl; );
        // This is the first output so the probability is 0 if we are not in the start state
        // m = state
        if( d != START_STATE ) {
            // D( cout << "\tNot in start state" << endl; );
            // We are not in the start state
            cur->prob = 0;

            // D( cout << "State: " << d << endl; );
            // D( cout << "Output: " << m << endl; );
            // D( cout << "Probaility: " << cur->prob << endl; );
        }
        else {
            // D( cout << "\tIn start state" << endl; );
            // This is the first output and we are in the start state so the probability
            // of getting the output is just the probability of that output from that state
            cur->prob = bias[d][output];

            // D( cout << "State: " << d << endl; );
            // D( cout << "Output: " << m << endl; );
            // D( cout << "Probability: " << cur->prob << endl; );

        }
    }
    else {
        // This is not the first output so we must compute the probability for
        // all of the previous states' probabilities only if this is the first state
        float transProb; // The probablity of the state transition to
        float outputProb = bias[d][output]; // The probability of seeing the output: output from state: d
        float prevProb;
        float maxProb;
        float curOutputProb;
        int maxState; // The state that gives the maximum output
        if ( d == 0) {
            for( int i = 0; i < N_STATES; i++) {
                // Fill the table
                getPrev(i, m-1, outputTable, obs);
            }
        }

        // Only want to get the previous if it is the first state (only do it once per output)
        // Create an array for each of the states in the previous output and their respective probabilities
        vector<float> probs;
        maxProb = 0;
        for( int i = 0; i < N_STATES; i++) {
            transProb = transitions[i][d];
            prevProb = (outputTable[i][m-1])->prob; // Probability of the previous output ending in state i

            // Compute probability of substring ending in state i then going to state m with output: output
            curOutputProb = prevProb * transProb * outputProb ;
            probs.push_back(curOutputProb);
            // printTab(obs.size()-m);

            printTab(obs.size()-m);
            cout << " Prev Prob: " << prevProb << " Trans Prob: " << transProb << " Output Prob: " << outputProb << " State: " << d << endl;
            // cout << "\t Previous State: " << i << " Previous Output Number: " << m-1  << " Previous Output Probability " << prevProb << endl;

            if (curOutputProb > maxProb) {
                // We have a new state that is better
                maxProb = curOutputProb;
                maxState = i;
            }
        }
        // printTab(obs.size()-m);
        // cout << "\t Max Probability: " << maxProb << " State: " << maxState << endl;
        // Set the probability to be the max Probability
        cur->prob = maxProb;
        cur->previous_state = outputTable[maxState][m-1];

    }
    cout << endl;
    printTab(obs.size()-m);
    cout << "Output: " << m << " State: " << d << " Probability: " << (outputTable[d][m])->prob << endl;
}

void deleteTable(vector< vector< state_t* > > outputTable) {
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

vector<int> readObs(string fileName) {
    // This function reads in the observed outputs into a vector
    vector<int> obs;
    D( cout << "Reading in obs: " << endl );
    ifstream f(fileName, std::ifstream::in);

    // Read in the observed states until they can't be read anymore
    int s;

    int count = 0;

    while(f >> s) {
        obs.push_back(s);
        count++;
    }
    // cout << "DONE" << endl;
    return obs;
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

void printObs(vector<int> obs) {
    // Print the observed outputs
    // cout << "\nObserved outputs:" << endl;
    for(int i = 0; i < obs.size(); i++) {
        cout << outputs[obs[i]] << " ";
    }
    cout << endl;

    for(int i = 0; i < obs.size(); i++) {
        cout << obs[i] << " ";
    }
    cout << endl;
}

void printTab(int n) {
    for (int i = 0; i < n; i++) {
        cout << "\t";
    }
}
