//
// Created by gabriel on 5/18/18.
//

#include <iostream>
#include "../include/Component.h"

inline void stampIfNotGND(std::vector<std::vector<double> > *matrix, int i, int j, double value) {
    if (i >= 0 && j >= 0) (*matrix)[i][j] += value;
}

inline void stampIfNotGND(std::vector<double> *vector, int i, double value) {
    if (i >= 0) (*vector)[i] += value;
}

void Component::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("ID: %d\t\t", indexInGroup);
    printf("LABEL: %s\t\t", label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, nodes[i]);
    }
    printf("VALUE: ");
    print_cell_data(value);
    printf("\n");
}

Component * ComponentFactory::createComponent(RowType type, char *label, int *nodes, data_t *value, char *controllerCurrent) {
    switch (type) { //TODO decide group based on parsed info
        case C: return new Capacitor(GROUP1, label, value, nodes);
        case D: return new Diode(GROUP1, label, value, nodes);
        case E: return new VCVS(GROUP2, label, value, nodes);
        case F: return new CCCS(GROUP1, label, value, nodes, controllerCurrent);
        case G: return new VCCS(GROUP1, label, value, nodes);
        case H: return new CCVS(GROUP2, label, value, nodes, controllerCurrent);
        case I: return new ISource(GROUP1, label, value, nodes);
        case L: return new Inductor(GROUP1, label, value, nodes);
        case M: return new MOSFET(GROUP1, label, value, nodes);
        case Q: return new BJT(GROUP1, label, value, nodes);
        case R: return new Resistor(GROUP1, label, value, nodes);
        case V: return new VSource(GROUP2, label, value, nodes);
        case CMD: case CMT: return nullptr;
    }
}

Component::Component(Group group, char *label, data_t *value, const int *nodes) :
        group(group), label(label), indexInGroup(indexInGroup), value(value) {
    for (int i = 0; i < MAX_NODES; ++i) {
        this->nodes[i] = nodes[i];
    }
}

Component::~Component() {
    free(label);
    free(type);
}

Component * Component::getControllerIfExists(std::vector<Component *> components) {
    return nullptr; //do nothing, for default. Should be overridden by CC sources
}

//CAPACITOR

Capacitor::Capacitor(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(CAPACITOR_STR);
}

void Capacitor::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";
}

//DIODE

Diode::Diode(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(DIODE_STR);
}

void Diode::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";
}

//VCVS

VCVS::VCVS(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(VCVS_STR);
}

void VCVS::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    int nodeVxP = nodes[2] - 1, nodeVxM = nodes[3] - 1;
    int group2Index = (int) solver->nodesCount + indexInGroup;
    
    stampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
    stampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
    stampIfNotGND(&solver->H, group2Index, nodeVPlus, 1);
    stampIfNotGND(&solver->H, group2Index, nodeVMinus, -1);
    stampIfNotGND(&solver->H, group2Index, nodeVxP, -value->value._double);
    stampIfNotGND(&solver->H, group2Index, nodeVxM, value->value._double);
}

//CCCS

CCCS::CCCS(Group group, char *label, data_t *value, int *nodes, char* controllerCurrent) : Component(
        group, label, value, nodes) {
    type = strdup(CCCS_STR);
    this->controllerCurrent = controllerCurrent;
}

void CCCS::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    int controlCurrentIndex = (int) solver->nodesCount + controller->indexInGroup;
    if (group == GROUP1) {
        stampIfNotGND(&solver->H, nodeVPlus, controlCurrentIndex, value->value._double);
        stampIfNotGND(&solver->H, nodeVMinus, controlCurrentIndex, -value->value._double);

    } else {
        int group2Index = (int) solver->nodesCount + indexInGroup;

        stampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
        stampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
        stampIfNotGND(&solver->H, group2Index, group2Index, 1);
        stampIfNotGND(&solver->H, group2Index, controlCurrentIndex, -value->value._double);
    }
}

void CCCS::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("ID: %d\t\t", indexInGroup);
    printf("LABEL: %s\t\t", label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, nodes[i]);
    }
    printf("CONTROL: %s\t\t", controllerCurrent);
    printf("VALUE: ");
    print_cell_data(value);
    printf("\n");
}

CCCS::~CCCS() {
    free(controllerCurrent);
}

Component *CCCS::getControllerIfExists(std::vector<Component *> components) {
    for (auto *component : components) {
        if (strcmp(component->label, controllerCurrent) == 0) {
            controller = component;
            return component;
        }
    }
    return nullptr;
}

//VCCS

VCCS::VCCS(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(VCCS_STR);
}

void VCCS::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    int nodeVxP = nodes[2] - 1, nodeVxM = nodes[3] - 1;
    if (group == GROUP1) {
        stampIfNotGND(&solver->H, nodeVPlus, nodeVxP, value->value._double);
        stampIfNotGND(&solver->H, nodeVPlus, nodeVxM, -value->value._double);
        stampIfNotGND(&solver->H, nodeVMinus, nodeVxP, -value->value._double);
        stampIfNotGND(&solver->H, nodeVMinus, nodeVxM, value->value._double);

    } else {
        int group2Index = (int) solver->nodesCount + indexInGroup;

        stampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
        stampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
        stampIfNotGND(&solver->H, group2Index, nodeVxP, -value->value._double);
        stampIfNotGND(&solver->H, group2Index, nodeVxM, value->value._double);
        stampIfNotGND(&solver->H, group2Index, group2Index, 1);
    }
}

//CCVS

CCVS::CCVS(Group group, char *label, data_t *value, int *nodes, char *controllerCurrent) : Component(
        group, label, value, nodes) {
    type = strdup(CCVS_STR);
    this->controllerCurrent = controllerCurrent;
}

void CCVS::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    int group2Index = (int) solver->nodesCount + indexInGroup;
    int controlCurrentIndex = (int) solver->nodesCount + controller->indexInGroup;

    stampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
    stampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
    stampIfNotGND(&solver->H, group2Index, nodeVPlus, 1);
    stampIfNotGND(&solver->H, group2Index, nodeVMinus, -1);
    stampIfNotGND(&solver->H, group2Index, controlCurrentIndex, -value->value._double);
}

void CCVS::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("ID: %d\t\t", indexInGroup);
    printf("LABEL: %s\t\t", label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, nodes[i]);
    }
    printf("CONTROL: %s\t\t", controllerCurrent);
    printf("VALUE: ");
    print_cell_data(value);
    printf("\n");
}

CCVS::~CCVS() {
    free(controllerCurrent);
}

Component * CCVS::getControllerIfExists(std::vector<Component *> components) {
    for (auto *component : components) {
        if (strcmp(component->label, controllerCurrent) == 0) {
            controller = component;
            return component;
        }
    }
    return nullptr;
}

//CURRENT SOURCE

ISource::ISource(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(I_SOURCE_STR);
}

void ISource::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    if (group == GROUP1) {
        stampIfNotGND(&solver->b, nodeVPlus, -value->value._double);
        stampIfNotGND(&solver->b, nodeVMinus, value->value._double);

    } else {
        int group2Index = (int) solver->nodesCount + indexInGroup;

        stampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
        stampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
        stampIfNotGND(&solver->H, group2Index, group2Index, 1);
        stampIfNotGND(&solver->b, group2Index, value->value._double);
    }
}

//INDUCTOR

Inductor::Inductor(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(INDUCTOR_STR);
}

void Inductor::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";
}

//MOSFET

MOSFET::MOSFET(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(MOSFET_STR);
}

void MOSFET::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";
}

//BJT

BJT::BJT(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(BJT_STR);
}

void BJT::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";
}

//RESISTOR

Resistor::Resistor(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(RESISTOR_STR);
}

void Resistor::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1;
    if (group == GROUP1) {
        stampIfNotGND(&solver->H, nodeVPlus, nodeVPlus, 1/value->value._double);
        stampIfNotGND(&solver->H, nodeVPlus, nodeVMinus, -1/value->value._double);
        stampIfNotGND(&solver->H, nodeVMinus, nodeVPlus, -1/value->value._double);
        stampIfNotGND(&solver->H, nodeVMinus, nodeVMinus, 1/value->value._double);

    } else {
        int group2Index = (int) solver->nodesCount + indexInGroup;

        stampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
        stampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
        stampIfNotGND(&solver->H, group2Index, nodeVPlus, 1);
        stampIfNotGND(&solver->H, group2Index, nodeVMinus, -1);
        stampIfNotGND(&solver->H, group2Index, group2Index, -value->value._double);
    }
}

//VOLTAGE SOURCE

VSource::VSource(Group group, char *label, data_t *value, int *nodes) : Component(
        group, label, value, nodes) {
    type = strdup(V_SOURCE_STR);
}

void VSource::stamp(Solver *solver) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0] - 1, nodeVMinus = nodes[1] - 1, group2Index = (int) solver->nodesCount + indexInGroup;

    stampIfNotGND(&solver->H, nodeVPlus, group2Index, 1);
    stampIfNotGND(&solver->H, nodeVMinus, group2Index, -1);
    stampIfNotGND(&solver->H, group2Index, nodeVPlus, 1);
    stampIfNotGND(&solver->H, group2Index, nodeVMinus, -1);
    stampIfNotGND(&solver->b, group2Index, value->value._double);
}
