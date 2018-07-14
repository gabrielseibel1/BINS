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

    return dcOffset + amplitude * sin(2 * M_PI * frequency * (time - phase));
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

//PWL SOURCE


PWLSource::PWLSource(Group group, char *label, data_t *value, int *nodes, PWLParams *pwlParams)
        : DynamicComponent(group, label, value, nodes) {

    type = strdup(PWL_SOURCE_STR);
    this->pwlParams = pwlParams;
}

void PWLSource::hardStamp(OPSolver *solver, double step, double time) {
    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    int group2Index = (int) solver->nodesCount + indexInGroup;

    hardStampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
    hardStampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
    hardStampIfNotGND(&solver->H, group2Index, nodeVPlus, 1);
    hardStampIfNotGND(&solver->H, group2Index, nodeVMinus, -1);
    hardStampIfNotGND(&solver->b, group2Index, nextSourceValue(time));
}

double PWLSource::nextSourceValue(double time) {
    double v1, v2, t1, t2;

    //find the time range
    for (int i = 0; i < pwlParams->timeVoltagePairs->size(); ++i) {
        std::pair<double, double> point1 = pwlParams->timeVoltagePairs->at(i);
        std::pair<double, double> point2;

        //if there is next point, use it
        if (i + 1 < pwlParams->timeVoltagePairs->size()){
            point2 = pwlParams->timeVoltagePairs->at(i + 1);
        } else { //if not, the next point should be the same as the last ("a" will be 0 and we will return v1)
            point2 = pwlParams->timeVoltagePairs->at(i);
        }

        t1 = point1.first; t2 = point2.first;
        v1 = point1.second; v2 = point2.second;

        if (t1 <= time && time <= t2) {
            double a = (v2 - v1) / (t2 - t1);
            return v1 + a * (time - t1);
        }
    }
    if (time < pwlParams->timeVoltagePairs->at(0).first) { //time is before first point - use 0
        return 0;
    } else { // time is after last point - use last voltage
        return pwlParams->timeVoltagePairs->at(pwlParams->timeVoltagePairs->size() - 1).second;
    }
}

void PWLSource::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("INDEX_IN_GROUP: %d\t\t", indexInGroup);
    printf("LABEL: %s\t\t", label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, nodes[i]);
    }
    printf("PWL(");
    for (auto pair : *(pwlParams->timeVoltagePairs)) {
        printf("[%f, %f]", pair.first, pair.second);
    }
    printf("\n");
}