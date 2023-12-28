//#include <stdio.h>
//#include <stdlib.h>
//#include "gate.h"
//#include "connection.h"
//#include "type.h"
//#include "function.h"
//#include "simulation.h"
//
//int main() {
//    Gate* gates;
//    int gateCount;
//    Connection* connections;
//    int connectionCount;
//    Type* types;
//    int typeCount;
//    Function* functions;
//    int functionCount;
//    int endTime = 1000; // Default simulation time
//
//    // Optionally get endTime from user
//    printf("Enter the end time for the simulation: ");
//    scanf("%d", &endTime);
//
//    // Parse files and check for errors
//    if (parseGateFile("gate.dat", &gates, &gateCount) != 0 ||
//        parseConnectionFile("connection.dat", &connections, &connectionCount) != 0 ||
//        parseTypeFile("type.dat", &types, &typeCount) != 0 ||
//        parseFunctionFile("function.dat", &functions, &functionCount) != 0) {
//        fprintf(stderr, "Error parsing input files\n");
//        // Free any allocated resources if needed
//        return 1;
//    }
//
//    SimulatedGate* simulatedGates = malloc(sizeof(SimulatedGate) * gateCount);
//    if (simulatedGates == NULL) {
//        perror("Failed to allocate memory for simulatedGates");
//        // Free parsed data
//        free(gates);
//        free(connections);
//        free(types);
//        free(functions);
//        return 1;
//    }
//
//    initializeSimulation(simulatedGates, gates, gateCount, types, typeCount);
//    runSimulation(simulatedGates, gateCount, connections, connectionCount, types, typeCount, functions, functionCount, endTime);
//
//    cleanupSimulation(simulatedGates, gateCount);
//    free(simulatedGates);
//
//    // Free parsed data
//    free(gates);
//    free(connections);
//    free(types);
//    free(functions);
//
//    return 0;
//}

#include <stdio.h>
#include <stdlib.h>
#include "gate.h"
#include "connection.h"
#include "type.h"
#include "function.h"
#include "simulation.h"

int main(int argc, char *argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s <gate_file> <connection_file> <type_file> <function_file> <gateid> <time>\n", argv[0]);
        return 1;
    }

    char *gateFile = argv[1];
    char *connectionFile = argv[2];
    char *typeFile = argv[3];
    char *functionFile = argv[4];
    int gateId = atoi(argv[5]);
    int endTime = atoi(argv[6]);

    // Überprüfen Sie, ob die Konvertierungen gültig sind
    if (gateId < 0 || endTime < 0) {
        fprintf(stderr, "Invalid arguments. gateid and time must be non-negative integers.\n");
        return 1;
    }

    Gate* gates;
    int gateCount;
    Connection* connections;
    int connectionCount;
    Type* types;
    int typeCount;
    Function* functions;
    int functionCount;


    // Parse files and check for errors
    if (parseGateFile(gateFile, &gates, &gateCount) != 0 ||
        parseConnectionFile(connectionFile, &connections, &connectionCount) != 0 ||
        parseTypeFile(typeFile, &types, &typeCount) != 0 ||
        parseFunctionFile(functionFile, &functions, &functionCount) != 0) {
        fprintf(stderr, "Error parsing input files\n");
        // Free any allocated resources if needed
        return 1;
    }

    SimulatedGate* simulatedGates = malloc(sizeof(SimulatedGate) * gateCount);
    if (simulatedGates == NULL) {
        perror("Failed to allocate memory for simulatedGates");
        // Free parsed data
        free(gates);
        free(connections);
        free(types);
        free(functions);
        return 1;
    }

    initializeSimulation(simulatedGates, gates, gateCount, types, typeCount);
    runSimulation(simulatedGates, gateCount, connections, connectionCount, types, typeCount, functions, functionCount, endTime);

    cleanupSimulation(simulatedGates, gateCount);
    free(simulatedGates);

    // Free parsed data
    free(gates);
    free(connections);
    free(types);
    free(functions);

    return 0;
}





