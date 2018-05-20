//
// Created by gabriel on 5/18/18.
//

#include <iostream>
#include "../include/Component.h"

void Component::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("ID: %d\t\t", id);
    printf("LABEL: %s\t\t", label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, nodes[i]);
    }
    printf("VALUE: ");
    print_cell_data(value);
    printf("\n");
}

Component* ComponentFactory::createComponent(RowType type, char *label, int id, int *nodes, data_t *value, char *controllerCurrent) {
    switch (type) { //TODO decide group based on parsed info
        case C: return new Capacitor(GROUP1, label, id, value, nodes);
        case D: return new Diode(GROUP1, label, id, value, nodes);
        case E: return new VCVS(GROUP2, label, id, value, nodes);
        case F: return new CCCS(GROUP1, label, id, value, nodes, controllerCurrent);
        case G: return new VCCS(GROUP2, label, id, value, nodes);
        case H: return new CCVS(GROUP1, label, id, value, nodes, controllerCurrent);
        case I: return new ISource(GROUP1, label, id, value, nodes);
        case L: return new Inductor(GROUP1, label, id, value, nodes);
        case M: return new MOSFET(GROUP1, label, id, value, nodes);
        case Q: return new BJT(GROUP1, label, id, value, nodes);
        case R: return new Resistor(GROUP1, label, id, value, nodes);
        case V: return new VSource(GROUP2, label, id, value, nodes);
        case CMD: case CMT: return nullptr;
    }
}

Component::Component(Group group, char *label, int id, data_t *value, const int *nodes) :
        group(group), label(label), id(id), value(value) {
    for (int i = 0; i < MAX_NODES; ++i) {
        this->nodes[i] = nodes[i];
    }
}

Component::~Component() {
    free(label);
    free(type);
}

void Component::setControllerToGroup2IfControlled(std::vector<Component *> components) {
    //do nothing, for default. Should be overridden by CC sources
}

//CAPACITOR

Capacitor::Capacitor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(CAPACITOR_STR);
}

void Capacitor::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//DIODE

Diode::Diode(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(DIODE_STR);
}

void Diode::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//VCVS

VCVS::VCVS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(VCVS_STR);
}

void VCVS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//CCCS

CCCS::CCCS(Group group, char *label, int id, data_t *value, int *nodes, char* controllerCurrent) : Component(
        group, label, id, value, nodes) {
    type = strdup(CCCS_STR);
    this->controllerCurrent = controllerCurrent;
}

void CCCS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

void CCCS::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("ID: %d\t\t", id);
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

void CCCS::setControllerToGroup2IfControlled(std::vector<Component *> components) {
    for (auto *component : components) {
        if (strcmp(component->label, controllerCurrent) == 0) {
            component->group = GROUP2;
            return;
        }
    }
}

//VCCS

VCCS::VCCS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(VCCS_STR);
}

void VCCS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//CCVS

CCVS::CCVS(Group group, char *label, int id, data_t *value, int *nodes, char *controllerCurrent) : Component(
        group, label, id, value, nodes) {
    type = strdup(CCVS_STR);
    this->controllerCurrent = controllerCurrent;
}

void CCVS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

void CCVS::print() {
    printf("TYPE: %s\t\t", type);
    printf("GROUP: %d\t\t", group);
    printf("ID: %d\t\t", id);
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

void CCVS::setControllerToGroup2IfControlled(std::vector<Component *> components) {
    for (auto *component : components) {
        if (strcmp(component->label, controllerCurrent) == 0) {
            component->group = GROUP2;
            return;
        }
    }
}

//CURRENT SOURCE

ISource::ISource(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(I_SOURCE_STR);
}

void ISource::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//INDUCTOR

Inductor::Inductor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(INDUCTOR_STR);
}

void Inductor::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//MOSFET

MOSFET::MOSFET(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(MOSFET_STR);
}

void MOSFET::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//BJT

BJT::BJT(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(BJT_STR);
}

void BJT::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//RESISTOR

Resistor::Resistor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(RESISTOR_STR);
}

void Resistor::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";

    int nodeVPlus = nodes[0], nodeVMinus = nodes[1];
    if (group == GROUP1) {
        (*matrix)[nodeVPlus][nodeVPlus] = 1/value->value._double;
        (*matrix)[nodeVPlus][nodeVMinus] = -1/value->value._double;
        (*matrix)[nodeVMinus][nodeVPlus] = -1/value->value._double;
        (*matrix)[nodeVPlus][nodeVPlus] = 1/value->value._double;
        (*matrix)[nodeVMinus][nodeVMinus] = 1/value->value._double;
    } else {

    }
}

//VOLTAGE SOURCE

VSource::VSource(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = strdup(V_SOURCE_STR);
}

void VSource::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}
