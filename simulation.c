#include "simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void outputStateChanges(SimulatedGate* simulatedGates, int gateCount, int currentTime, int targetGateId) {
    for (int i = 0; i < gateCount; i++) {
        SimulatedGate gate = simulatedGates[i];

        // Überprüfen, ob es sich um das gewünschte Gate handelt
        if (gate.gate.gateId != targetGateId) {
            continue;
        }

        // Iterate over output pins of the gate
        for (int j = 0; j < gate.numOutputPins; j++) {
            PinState pin = gate.outputStates[j];

            // Check if the state change time of the pin is equal to the current time
            if (pin.changeTime == currentTime) {
                printf("Time %d: Gate %d, Output Pin %d changed to state %d\n", currentTime, gate.gate.gateId, j, pin.state);
            }
        }
    }
}

int findNextChangeTime(SimulatedGate* simulatedGates, int gateCount, int currentTime) {
    int nextTime = INT_MAX;

    for (int i = 0; i < gateCount; i++) {
        SimulatedGate gate = simulatedGates[i];

        // Check input pins
        for (int j = 0; j < gate.numInputPins; j++) {
            if (gate.inputStates[j].changeTime > currentTime && gate.inputStates[j].changeTime < nextTime) {
                nextTime = gate.inputStates[j].changeTime;
            }
        }

        // Check output pins
        for (int j = 0; j < gate.numOutputPins; j++) {
            if (gate.outputStates[j].changeTime > currentTime && gate.outputStates[j].changeTime < nextTime) {
                nextTime = gate.outputStates[j].changeTime;
            }
        }
    }

    return nextTime;
}

SimulatedGate* findSimulatedGateById(SimulatedGate* gates, int gateCount, int gateId) {
    for (int i = 0; i < gateCount; i++) {
        if (gates[i].gate.gateId == gateId) {
            return &gates[i];
        }
    }
    return NULL; // Gate not found
}

void propagateStates(Connection* connections, int connectionCount, SimulatedGate* simulatedGates, int gateCount) {
    for (int i = 0; i < connectionCount; i++) {
        Connection conn = connections[i];

        SimulatedGate* fromGate = findSimulatedGateById(simulatedGates, gateCount, conn.fromGateId);
        if (fromGate == NULL || conn.fromPin >= fromGate->numOutputPins) {
            // Handle error: source gate not found or pin index out of range
            continue;
        }
        int outputState = fromGate->outputStates[conn.fromPin].state;

        SimulatedGate* toGate = findSimulatedGateById(simulatedGates, gateCount, conn.toGateId);
        if (toGate == NULL || conn.toPin >= toGate->numInputPins) {
            // Handle error: destination gate not found or pin index out of range
            continue;
        }
        toGate->inputStates[conn.toPin].state = outputState;
    }
}

void runSimulation(SimulatedGate* simulatedGates, int gateCount, Connection* connections, int connectionCount, Type* types, int typeCount, Function* functions, int functionCount, int endTime, int targetGateId) {
    int currentTime = 0;

    while (currentTime < endTime) {
        // Update gate states
        for (int i = 0; i < gateCount; i++) {
            updateGateState(&simulatedGates[i], types, typeCount, functions, functionCount, currentTime);
        }

        // Propagate states
        propagateStates(connections, connectionCount, simulatedGates, gateCount);

        // Find the next time to advance to
        int nextTime = findNextChangeTime(simulatedGates, gateCount, currentTime);

        // Output state changes for the specified gate
        outputStateChanges(simulatedGates, gateCount, currentTime, targetGateId);

        currentTime = nextTime;
    }
}

void cleanupSimulation(SimulatedGate* simulatedGates, int gateCount) {
    for (int i = 0; i < gateCount; i++) {
        free(simulatedGates[i].inputStates);
        free(simulatedGates[i].outputStates);
    }
}
void updateGateState(SimulatedGate* gate, Type* types, int typeCount, Function* functions, int functionCount, int currentTime) {
    // Find the type for this gate
    Type* gateType = NULL;
    for (int i = 0; i < typeCount; i++) {
        if (types[i].typeId == gate->gate.typeId) {
            gateType = &types[i];
            break;
        }
    }

    if (gateType == NULL) {
        // Type for this gate not found
        return;
    }

    // Find the function for this gate type
    Function* gateFunction = NULL;
    for (int i = 0; i < functionCount; i++) {
        if (functions[i].typeId == gate->gate.typeId) {
            gateFunction = &functions[i];
            break;
        }
    }

    if (gateFunction == NULL) {
        // Function for this gate type not found
        return;
    }

    // Process the input bits to determine the output
    unsigned int inputBits = 0;
    for (int i = 0; i < gateType->numInputPins; i++) {
        inputBits |= (gate->inputStates[i].state << i);
    }

    unsigned int outputBits = 0;

    // Implementing logic for different gate types
    switch (gate->gate.typeId) {
        case 1:  // "one" gate, always outputs 1
            outputBits = 1;
            break;

        case 0:  // "zero" gate, always outputs 0
            outputBits = 0;
            break;

        case 10:  // "not" gate, inverts the input bit
            outputBits = ~inputBits & 1;
            break;

        case 20:  // "and" gate, outputs 1 if all inputs are 1
            outputBits = (inputBits == (1 << gateType->numInputPins) - 1) ? 1 : 0;
            break;

        case 21:  // "or" gate, outputs 1 if any input is 1
            outputBits = (inputBits > 0) ? 1 : 0;
            break;

        case 22:  // "nand" gate, outputs 0 if all inputs are 1
            outputBits = (inputBits == (1 << gateType->numInputPins) - 1) ? 0 : 1;
            break;

        case 23:  // "nor" gate, outputs 0 if any input is 1
            outputBits = (inputBits > 0) ? 0 : 1;
            break;

        case 100:  // "osc" gate, toggles its state
            outputBits = gate->outputStates[0].state == 0 ? 1 : 0;
            break;

        default:
            // Handle unknown gate type
            break;
    }


    // Update the output states
    for (int i = 0; i < gateType->numOutputPins; i++) {
        int newState = (outputBits >> i) & 1;
        if (newState != gate->outputStates[i].state) {
            gate->outputStates[i].state = newState;
            gate->outputStates[i].changeTime = currentTime + (int)(gateType->delay);
        }
    }
}


void initializeSimulation(SimulatedGate* simulatedGates, Gate* gates, int gateCount, Type* types, int typeCount) {
    for (int i = 0; i < gateCount; i++) {
        simulatedGates[i].gate = gates[i];
        // Find the type of the gate to determine the number of pins
        int typeId = gates[i].typeId;
        Type* type = findTypeById(types, typeCount, typeId);
        if (type != NULL) {
            simulatedGates[i].numInputPins = type->numInputPins;
            simulatedGates[i].numOutputPins = type->numOutputPins;

            simulatedGates[i].inputStates = malloc(sizeof(PinState) * type->numInputPins);
            if (simulatedGates[i].inputStates == NULL) {
                // Handle memory allocation failure
                perror("Failed to allocate memory for inputStates");
            }

            simulatedGates[i].outputStates = malloc(sizeof(PinState) * type->numOutputPins);
            if (simulatedGates[i].outputStates == NULL) {
                // Handle memory allocation failure
                perror("Failed to allocate memory for outputStates");
            }

            // Initialize pin states
            for (int j = 0; j < type->numInputPins; j++) {
                simulatedGates[i].inputStates[j].state = 0;
                simulatedGates[i].inputStates[j].changeTime = INT_MAX;
            }
            for (int j = 0; j < type->numOutputPins; j++) {
                simulatedGates[i].outputStates[j].state = 0;
                simulatedGates[i].outputStates[j].changeTime = INT_MAX;
            }
        } else {
            // Handle case where type is not found
            fprintf(stderr, "Type ID %d not found for gate ID %d\n", typeId, gates[i].gateId);
        }
    }
}