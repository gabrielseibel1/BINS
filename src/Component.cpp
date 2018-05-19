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

Component* ComponentFactory::createComponent(RowType type, char *label, int id, int *nodes, data_t *value) {
    switch (type) { //TODO decide group based on parsed info
        case C: return new Capacitor(GROUP1, label, id, value, nodes);
        case D: return new Diode(GROUP1, label, id, value, nodes);
        case E: return new VCVS(GROUP2, label, id, value, nodes);
        case F: return new CCCS(GROUP1, label, id, value, nodes);
        case G: return new VCCS(GROUP1, label, id, value, nodes);
        case H: return new CCVS(GROUP2, label, id, value, nodes);
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

//CAPACITOR

Capacitor::Capacitor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("CAPACITOR")));
    sprintf(type, "CAPACITOR");
}

void Capacitor::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//DIODE

Diode::Diode(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("DIODE    ")));
    sprintf(type, "DIODE    ");
}

void Diode::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//VCVS

VCVS::VCVS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("VCVS     ")));
    sprintf(type, "VCVS     ");
}

void VCVS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//CCCS

CCCS::CCCS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("CCCS     ")));
    sprintf(type, "CCCS     ");
}

void CCCS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//VCCS

VCCS::VCCS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("VCCS     ")));
    sprintf(type, "VCCS     ");
}

void VCCS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//CCVS

CCVS::CCVS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("CCVS     ")));
    sprintf(type, "CCVS     ");
}

void CCVS::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//CURRENT SOURCE

ISource::ISource(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("I-SOURCE ")));
    sprintf(type, "I-SOURCE ");
}

void ISource::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//INDUCTOR

Inductor::Inductor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("INDUCTOR ")));
    sprintf(type, "INDUCTOR ");
}

void Inductor::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//MOSFET

MOSFET::MOSFET(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("MOSFET   ")));
    sprintf(type, "MOSFET   ");
}

void MOSFET::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//BJT

BJT::BJT(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("BJT      ")));
    sprintf(type, "BJT      ");
}

void BJT::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}

//RESISTOR

Resistor::Resistor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("RESISTOR ")));
    sprintf(type, "RESISTOR ");
}

void Resistor::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";

    /*int nodeVPlus = nodes[0], nodeVMinus = nodes[1];
    if (group == GROUP1) {
        (*matrix)[nodeVPlus][nodeVPlus] = 1/value->value._double;
        (*matrix)[nodeVPlus][nodeVMinus] = -1/value->value._double;
        (*matrix)[nodeVMinus][nodeVPlus] = -1/value->value._double;
        (*matrix)[nodeVPlus][nodeVPlus] = 1/value->value._double;
        (*matrix)[nodeVMinus][nodeVMinus] = 1/value->value._double;
    } else {

    }*/
}

//VOLTAGE SOURCE

VSource::VSource(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {
    type = static_cast<char *>(malloc(sizeof("V-SOURCE ")));
    sprintf(type, "V-SOURCE ");
}

void VSource::stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) {
    std::cout << "Stamp " << type << "\n";
}
