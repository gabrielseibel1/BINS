//
// Created by gabriel on 7/8/18.
//

#include <iostream>
#include <fstream>
#include "TranSolver.h"
#include "../include/SpiceInterpreter.h"
#include "../include/NodeMap.h"

TranSolver::TranSolver(SpiceInterpreter *interpreter) {
    spiceInterpreter = interpreter;
    step = interpreter->tranStep;
    maxTime = interpreter->tranMaxTime;

    NodeMap *nodeMap = interpreter->getNodeMap();
    nodeCount = nodeMap->getSize() - 1 /*don't count GND*/;
    group2Size = static_cast<size_t>(interpreter->getGroup2Count());
    size = nodeCount + group2Size;

    //initialize OP solver
    opSolver = new OPSolver(size, nodeCount);

    //separate references to dynamic components from static ones
    for (auto *component : interpreter->getComponents()) {
        if (strcmp(component->type, CAPACITOR_STR) == 0 ||
            strcmp(component->type, INDUCTOR_STR) == 0 ||
            strcmp(component->type, SINE_SOURCE_STR) == 0 ||
            strcmp(component->type, PWL_SOURCE_STR) == 0)
            dynamicComponents.emplace_back(dynamic_cast<DynamicComponent*>(component));
        else
            staticComponents.emplace_back(component);
    }
}

TranSolver::~TranSolver() {
    delete opSolver;
}

void TranSolver::solveTransient() {

    auto dynMatrices = new OPSolver(size, nodeCount);


    for (double time = 0; time < maxTime; time += step) {
        opSolver = new OPSolver(size, nodeCount);
        opSolver->accumulativeStamp(staticComponents);

        //(substitutive) stamp dynamic components, considering their U and I at this time
        dynMatrices->substitutiveStamp(dynamicComponents, step, time);

        //add contributions of dynamic elements
        opSolver->sum(dynMatrices);

        //solve for current time
        opSolver->solveOP();


        //save solution
        auto solution = std::make_pair(time, opSolver->x);
        solutions.emplace_back(solution);

        //recalculate U and I for capacitors, inductors and dynamic sources
        updateDynamicComponents(opSolver);

        //subtract contributions of dynamic elements
        opSolver->sub(dynMatrices);

        //opSolver->interpretedPrint(spiceInterpreter);
    }

}

void TranSolver::updateDynamicComponents(OPSolver *solver) {
    for (auto *component : dynamicComponents) {
        if (strcmp(component->type, CAPACITOR_STR) == 0) {

            component->u = component->nextSourceValue(step);
            component->i = solver->x[solver->p[nodeCount + component->indexInGroup]];

        } else if (strcmp(component->type, INDUCTOR_STR) == 0) {

            component->i = component->nextSourceValue(step);

            int nodeVPlus = component->nodes[0] - 1, nodeVMinus = component->nodes[1] - 1;
            if (nodeVPlus == 0) {
                component->u = solver->x[solver->p[nodeVMinus]];
            } else if (nodeVMinus == 0) {
                component->u = solver->x[solver->p[nodeVPlus]];
            } else {
                component->u = solver->x[solver->p[nodeVPlus]] - solver->x[solver->p[nodeVMinus]];
            }

        }
    }
}

void TranSolver::printSolutions(char *filename) {
    std::ofstream file(filename);

    file << "#Transient analysis:\n";

    //print comment with column headers
    file << "#time\t";

    std::string varType = "V";
    NodeMap *nodeMap = spiceInterpreter->getNodeMap();
    for (auto pair: nodeMap->getNodeMap()) {
        if (pair.second == 0) continue;
        file << varType << "(" << pair.first << ")\t";
    }

    varType = "I";
    for (Component *comp : spiceInterpreter->getComponents()) {
        if (comp->group == GROUP2){
            file << varType << "(" << comp->label << ")\t";
        }
    }
    file << "\n";

    //print data
    for (auto timeSolutionPair : solutions) {
        //print time
        file << timeSolutionPair.first << "\t";

        DoubleVector solution = timeSolutionPair.second;


        for (auto pair: nodeMap->getNodeMap()) {
            if (pair.second == 0) continue;
            double value = solution[opSolver->p[pair.second - 1]];
            file << value << "\t";
        }

        varType = "I";
        for (Component *comp : spiceInterpreter->getComponents()) {
            if (comp->group == GROUP2){
                int index = static_cast<int>(nodeMap->getSize() - 1 + comp->indexInGroup);
                double value = solution[opSolver->p[index]];
                file << value << "\t";
            }
        }
        file << "\n";
    }
}
