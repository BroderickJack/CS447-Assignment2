# Jack Broderick
# December 2, 2018
# CS447 Assignment 2

N_STATES = 2
N_OUTPUTS = 2

# This is a script for solving the hidden Markhov Model problem
# Create a class that represents a state
class State:
    # This is a class that reperesents a state
    def __init__(self, state, previousState, output, outputIndex, prob):
        # Intitalize the object
        self.state = state
        self.previousState = previousState
        self.output = output
        self.outputIndex = outputIndex
        self.prob = prob

class Table:
    # This is a class for creating the probability table
    def __init__(self, emit, trans, obs):
        # Intiialize the table
        self.emit = emit
        self.trans = trans
        self.obs = obs
        print("len(obs): ", len(obs))
        self.table = [[None] * (len(obs)) for _ in range(N_STATES)]

    def buildTable(self):
        for s in range(N_STATES):
            # Loop through all of the states and fill in the probability of the previous
            self.findPrevious(s, len(self.obs)-1)

    def findPrevious(self, state, outputIndex):
        print("State: ", state, " Output Index: ", outputIndex)
        # print("type(state): ", type(state), " type(outputIndex) ", type(outputIndex))
        # If it is the first output then the probability is the probability of seeing that output from that states
        o = self.obs[outputIndex] # The actual output
        # print("o: ", o)
        outProb = self.emit[state][o]
        if(outputIndex == 0):
            self.table[state][outputIndex] = State(state, -1, o, outputIndex, outProb)
        else:
            # This is not the first output. If it is state 0 we calculate the previous for the others
            if (state == 0):
                # First state so the outputs have not been computed yet
                for s in range(N_STATES):
                    self.findPrevious(s, outputIndex-1)

            # Now we know that the probability has been computed for all of the previous output in the table
            # we loop through the states to see which one has the highest proability of being the one we came from
            maxProb = 0
            prevS = -1
            for s in range(N_STATES):
                prevState = self.table[s][outputIndex-1]
                prevProb = prevState.prob # Probablity of reaching that state for that output
                tranProb = self.trans[prevState.state][state]
                subProb = prevProb * tranProb * outProb # The probabilty of ending in State: state for Output: outputIndex

                if (subProb > maxProb):
                    maxProb = subProb
                    prevS = s

            # Set the previous state to be the one with the maximum probability
            # print("Rows: ", len(self.table), " Columns: ", len(self.table[0]))
            self.table[state][outputIndex] = State(state, prevS, o, outputIndex, maxProb)

def readEmit(emitFile):
    # This function reads in the "emit" txt file which is the proability of seeing an output
    # from a certain state
    # Create a 2-d array that the output is going to go into
    emit = [[0]*N_OUTPUTS for _ in range(N_STATES)]

    f = open(emitFile, "r") # Open the file in read mode

    for x in range(N_STATES):
        p = float(f.readline())
        emit[x][0] = p
        emit[x][1] = 1-p

    f.close()
    return emit

def printEmit(emit):
    print("Output Probabilities")
    for i in range(len(emit)):
        for j in range(len(emit[0])):
            print(emit[i][j], "\t", end=" ")
        print('\n', end="")

def readTransitions(fileName):
    # Create a transition matrix that we will read the values into
    trans = [[0]*N_OUTPUTS for _ in range(N_STATES)]
    f = open(fileName, 'r') # Open the file as read permission

    for i in range(N_STATES):
        val = f.readline()
        trans[i] = [float(x) for x in val.split(" ")]

    f.close()
    return trans

def printTrans(trans):
    # This function prints the transition Matrix
    print("\nTransition Matrix")
    for row in trans:
        for elem in row:
            print(elem, "\t", end="")
        print("\n",end="")

def readObs(fileName):
    # This function reads the observed outputs of the model
    f = open(fileName, 'r')
    output = f.readline()
    obs = [int(o) for o in output.split(" ")]

    return obs

def printObs(obs):
    print("\nPrinting Obs:")
    for elem in obs:
        print(elem, " ", end="")
    print()

if __name__ == "__main__":
    # The files that need to be read are:
    transitionFile = "./Tests/chain-1.txt"
    emitFile = "./Tests/emit-1.txt"
    obsFile = "./Tests/obs-1.txt"

    # Read in the emit file
    emit = readEmit(emitFile)
    trans = readTransitions(transitionFile)
    obs = readObs(obsFile)

    # Print the emit matrix
    printEmit(emit)
    printTrans(trans)
    printObs(obs)

    # Create the table
    table = Table(emit, trans, obs)

    print("Building the table")
    table.buildTable()
