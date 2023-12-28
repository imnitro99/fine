#ifndef SIMULATION_H
#define SIMULATION_H

#include "gate.h"
#include "connection.h"
#include "type.h"
#include "function.h"

typedef struct {
    int state;  // Current state (0 or 1)
    int changeTime;  // Time at which the state changes
} PinState;

typedef struct {
    Gate gate;
    int numInputPins;
    int numOutputPins;
    PinState* inputStates;
    PinState* outputStates;
} SimulatedGate;

void initializeSimulation(SimulatedGate* simulatedGates, Gate* gates, int gateCount, Type* types, int typeCount);
void runSimulation(SimulatedGate* simulatedGates, int gateCount, Connection* connections, int connectionCount, Type* types, int typeCount, Function* functions, int functionCount, int endTime, int targetGateId);;
int findNextChangeTime(SimulatedGate* simulatedGates, int gateCount, int currentTime);
void outputStateChanges(SimulatedGate* simulatedGates, int gateCount, int currentTime, int targetGateId);
void cleanupSimulation(SimulatedGate* simulatedGates, int gateCount);
void updateGateState(SimulatedGate* gate, Type* types, int typeCount, Function* functions, int functionCount, int currentTime);
SimulatedGate* findSimulatedGateById(SimulatedGate* gates, int gateCount, int gateId);
void propagateStates(Connection* connections, int connectionCount, SimulatedGate* simulatedGates, int gateCount);

#endif // SIMULATION_H
