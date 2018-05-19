//
// Created by gabriel on 5/18/18.
//

#ifndef BINS_COMPONENT_H
#define BINS_COMPONENT_H

#include <ostream>
#include "table.h"
#include "MatrixManager.h"

#define UNUSED_NODE (-1)
#define MAX_NODES 4

enum Group {
    GROUP1 = 1, GROUP2 = 2
};

class Component {
public:
    Group group;
    char* label;
    int id;
    data_t* value;
    int nodes[MAX_NODES];
    char *type;

    Component(Group group, char *label, int id, data_t *value, const int *nodes);

    virtual ~Component();

    virtual void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) = 0;

    void print();
};

class Capacitor: public Component {
public:
    //char type[10] = "CAPACITOR";
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    Capacitor(Group group, char *label, int id, data_t *value, int *nodes);
};

class Diode: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    Diode(Group group, char *label, int id, data_t *value, int *nodes);
};

class VCVS: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    VCVS(Group group, char *label, int id, data_t *value, int *nodes);
};

class CCCS: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    CCCS(Group group, char *label, int id, data_t *value, int *nodes);
};

class VCCS: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    VCCS(Group group, char *label, int id, data_t *value, int *nodes);
};

class CCVS: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    CCVS(Group group, char *label, int id, data_t *value, int *nodes);
};

class ISource: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    ISource(Group group, char *label, int id, data_t *value, int *nodes);
};

class Inductor: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    Inductor(Group group, char *label, int id, data_t *value, int *nodes);
};

class MOSFET: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    MOSFET(Group group, char *label, int id, data_t *value, int *nodes);
};

class BJT: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    BJT(Group group, char *label, int id, data_t *value, int *nodes);
};

class Resistor: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    Resistor(Group group, char *label, int id, data_t *value, int *nodes);
};

class VSource: public Component {
public:
    void stamp(std::vector<std::vector<double>> *matrix, std::vector<double> *rhs) override;
    VSource(Group group, char *label, int id, data_t *value, int *nodes);
};

class ComponentFactory {
public:
    Component *createComponent(RowType type, char *label, int id, int *nodes, data_t *value);
};

#endif //BINS_COMPONENT_H
