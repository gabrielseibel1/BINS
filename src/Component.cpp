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
    }
}

Component::Component(Group group, char *label, int id, data_t *value, const int *nodes) :
        group(group), label(label), id(id), value(value) {
    for (int i = 0; i < MAX_NODES; ++i) {
        this->nodes[i] = nodes[i];
    }
}

Capacitor::Capacitor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void Capacitor::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

Diode::Diode(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void Diode::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

VCVS::VCVS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void VCVS::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

CCCS::CCCS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void CCCS::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

VCCS::VCCS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void VCCS::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

CCVS::CCVS(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void CCVS::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

ISource::ISource(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void ISource::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

Inductor::Inductor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void Inductor::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

MOSFET::MOSFET(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void MOSFET::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

BJT::BJT(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void BJT::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

Resistor::Resistor(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void Resistor::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}

VSource::VSource(Group group, char *label, int id, data_t *value, int *nodes) : Component(
        group, label, id, value, nodes) {}

void VSource::stamp(std::vector<std::vector<double>> matrix) {
    std::cout << "Stamp " << type << "\n";
}
