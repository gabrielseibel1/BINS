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
    std::string type;

    Component(Group group, char *label, int id, data_t *value, const int *nodes);

    virtual ~Component();

    virtual void stamp(std::vector<std::vector<double>> matrix) = 0;

    void print();
};

class Capacitor: public Component {
public:
    Capacitor(Group group, char *label, int id, data_t *value, int *nodes);

private:
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "CAPACITOR";
};

class Diode: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "DIODE   ";
public:
    Diode(Group group, char *label, int id, data_t *value, int *nodes);
};

class VCVS: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "VCVS    ";
public:
    VCVS(Group group, char *label, int id, data_t *value, int *nodes);
};

class CCCS: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "CCCS    ";
public:
    CCCS(Group group, char *label, int id, data_t *value, int *nodes);
};

class VCCS: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "VCCS    ";
public:
    VCCS(Group group, char *label, int id, data_t *value, int *nodes);
};

class CCVS: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "CCVS    ";
public:
    CCVS(Group group, char *label, int id, data_t *value, int *nodes);
};

class ISource: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "I-SOURCE";
public:
    ISource(Group group, char *label, int id, data_t *value, int *nodes);
};

class Inductor: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "INDUCTOR";
public:
    Inductor(Group group, char *label, int id, data_t *value, int *nodes);
};

class MOSFET: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "MOSFET  ";
public:
    MOSFET(Group group, char *label, int id, data_t *value, int *nodes);
};

class BJT: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "BJT     ";
public:
    BJT(Group group, char *label, int id, data_t *value, int *nodes);
};

class Resistor: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "RESISTOR";
public:
    Resistor(Group group, char *label, int id, data_t *value, int *nodes);
};

class VSource: public Component {
    void stamp(std::vector<std::vector<double>> matrix) override;
    std::string type = "V-SOURCE";
public:
    VSource(Group group, char *label, int id, data_t *value, int *nodes);
};

class ComponentFactory {
public:
    Component *createComponent(RowType type, char *label, int id, int *nodes, data_t *value);
};

#endif //BINS_COMPONENT_H
