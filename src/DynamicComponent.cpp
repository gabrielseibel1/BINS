//
// Created by gabriel on 7/8/18.
//

#include <cmath>
#include "../include/Component.h"

inline void hardStampIfNotGND(std::vector<std::vector<double> > *matrix, int i, int j, double value) {
    if (i >= 0 && j >= 0) (*matrix)[i][j] = value;
}

inline void hardStampIfNotGND(std::vector<double> *vector, int i, double value) {
    if (i >= 0) (*vector)[i] = value;
}

DynamicComponent::DynamicComponent(Group group, char *label, data_t *value, const int *nodes)
        : Component(group, label, value, nodes) {}

void DynamicComponent::stamp(OPSolver *solver) {
    //do nothing
}

//CAPACITOR

Capacitor::Capacitor(Group group, char *label, data_t *value, int *nodes, double initialCondition) : DynamicComponent(
        group, label, value, nodes) {
    type = strdup(CAPACITOR_STR);
    capacitance = value->value._double;
    u = initialCondition;
    i = 1E-6;
}

void Capacitor::hardStamp(OPSolver *solver, double step, double time) {
    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    int group2Index = (int) solver->nodesCount + indexInGroup;

    hardStampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
    hardStampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
    hardStampIfNotGND(&solver->H, group2Index, nodeVPlus, 1);
    hardStampIfNotGND(&solver->H, group2Index, nodeVMinus, -1);
    hardStampIfNotGND(&solver->b, group2Index, nextSourceValue(step));
}

double Capacitor::nextSourceValue(double h) {
    return u + (h / capacitance) * i;
}

//INDUCTOR

Inductor::Inductor(Group group, char *label, data_t *value, int *nodes, double initialCondition) : DynamicComponent(
        group, label, value, nodes) {
    type = strdup(INDUCTOR_STR);
    inductance = value->value._double;
    i = initialCondition;
    u = 1E-6;
}

void Inductor::hardStamp(OPSolver *solver, double step, double time) {
    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    if (group == GROUP1) {
        hardStampIfNotGND(&solver->b, nodeVPlus, -nextSourceValue(step));
        hardStampIfNotGND(&solver->b, nodeVMinus, nextSourceValue(step));

    } else {
        int group2Index = (int) solver->nodesCount + indexInGroup;

        hardStampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
        hardStampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
        hardStampIfNotGND(&solver->H, group2Index, group2Index, 1);
        hardStampIfNotGND(&solver->b, group2Index, nextSourceValue(step));
    }
}

double Inductor::nextSourceValue(double h) {
    return i + (h / inductance) * u;
}

//SINE SOURCE


SineSource::SineSource(Group group, char *label, data_t *value, int *nodes, SineParams *sineParams)
        : DynamicComponent(group, label, value, nodes) {

    type = strdup(SINE_SOURCE_STR);
    this->sineParams = sineParams;
}

void SineSource::hardStamp(OPSolver *solver, double step, double time) {
    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    int group2Index = (int) solver->nodesCount + indexInGroup;

    hardStampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
    hardStampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
    hardStampIfNotGND(&solver->H, group2Index, nodeVPlus, 1);
    hardStampIfNotGND(&solver->H, group2Index, nodeVMinus, -1);
    hardStampIfNotGND(&solver->b, group2Index, nextSourceValue(time));
}

double SineSource::nextSourceValue(double time) {
    double dcOffset = sineParams->dcOffset;
    double amplitude = sineParams->amplitude;
    double frequency = sineParams->frequency;
    double phase = sineParams->phase;

    double ret = dcOffset + amplitude * sin(2 * M_PI * frequency * (time - phase));
    return ret;
}

void SineSource::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("INDEX_IN_GROUP: %d\t\t", indexInGroup);
    printf("LABEL: %s\t\t", label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, nodes[i]);
    }
    printf("SIN(V0: %f | ", sineParams->dcOffset);
    printf("VA: %f | ", sineParams->amplitude);
    printf("FREQ: %f | ", sineParams->frequency);
    printf("PH: %f)", sineParams->phase);
    printf("\n");
}

//TODO PWL SOURCE