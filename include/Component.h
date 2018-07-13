//
// Created by gabriel on 5/18/18.
//

#ifndef BINS_COMPONENT_H
#define BINS_COMPONENT_H

class OPSolver;

#include <ostream>
#include <vector>
#include "table.h"
#include "OPSolver.h"

#define UNUSED_NODE (-1)
#define MAX_NODES 4

enum Group {
    GROUP1 = 1, GROUP2 = 2
};

static const char *const I_SOURCE_STR = "I-SOURCE ";

static const char *const CAPACITOR_STR = "CAPACITOR";

static const char *const DIODE_STR = "DIODE    ";

static const char *const VCVS_STR = "VCVS     ";

static const char *const CCCS_STR = "CCCS     ";

static const char *const VCCS_STR = "VCCS     ";

static const char *const CCVS_STR = "CCVS     ";

static const char *const INDUCTOR_STR = "INDUCTOR ";

static const char *const MOSFET_STR = "MOSFET   ";

static const char *const BJT_STR = "BJT      ";

static const char *const RESISTOR_STR = "RESISTOR ";

static const char *const V_SOURCE_STR = "V-SOURCE ";

static const char *const SINE_SOURCE_STR = "SINE     ";

class Component {
public:
    Group group;
    char* label;
    int indexInGroup;
    data_t* value;
    int nodes[MAX_NODES];
    char *type;

    Component(Group group, char *label, data_t *value, const int *nodes);

    virtual ~Component();

    virtual void stamp(OPSolver *solver) = 0;

    virtual void print();

    virtual Component * getControllerIfExists(std::vector<Component *> components);
};

class DynamicComponent : public Component {

public:
    double u;
    double i;
    DynamicComponent(Group group, char *label, data_t *value, const int *nodes);

    virtual double nextSourceValue(double h) = 0;

    void stamp(OPSolver *solver) override;

    virtual void hardStamp(OPSolver *solver, double step, double time) = 0;
};

class Capacitor: public DynamicComponent {
    double capacitance;
public:
    /**
     * Calculate next voltage
     */
    double nextSourceValue(double h) override;
    void hardStamp(OPSolver *solver, double step, double time) override;
    Capacitor(Group group, char *label, data_t *value, int *nodes, double initialCondition);
};

class Inductor: public DynamicComponent {
    double inductance;
public:
    /**
     * Calculate next current
     */
    double nextSourceValue(double h) override;
    void hardStamp(OPSolver *solver, double step, double time) override;
    Inductor(Group group, char *label, data_t *value, int *nodes, double initialCondition);
};

typedef struct SineParamsStruct {
    double dcOffset;
    double amplitude;
    double frequency;
    double phase;
} SineParams;

class SineSource: public DynamicComponent {
private:
    SineParams *sineParams;
public:
    /**
     * Calculate next voltage
     */
    double nextSourceValue(double time) override;
    void hardStamp(OPSolver *solver, double step, double time) override;
    SineSource(Group group, char *label, data_t *value, int *nodes, SineParams *sineParams);

    void print() override;
};

typedef struct PWLParamsStruct {
    std::vector<std::pair<double, double> > timeVoltagePairs;
} PWLParams;

//TODO PWL SOURCE CLASS

class Diode: public Component {
public:
    void stamp(OPSolver *solver) override;
    Diode(Group group, char *label, data_t *value, int *nodes);
};

class VCVS: public Component {
public:
    void stamp(OPSolver *solver) override;
    VCVS(Group group, char *label, data_t *value, int *nodes);
};

class CCCS: public Component {
public:
    char *controllerCurrent;
    Component *controller;
    void stamp(OPSolver *solver) override;
    void print() override;
    Component *getControllerIfExists(std::vector<Component *> components) override;
    ~CCCS() override;
    CCCS(Group group, char *label, data_t *value, int *nodes, char* controllerCurrent);
};

class VCCS: public Component {
public:
    void stamp(OPSolver *solver) override;
    VCCS(Group group, char *label, data_t *value, int *nodes);
};

class CCVS: public Component {
public:
    char *controllerCurrent;
    Component *controller;
    void stamp(OPSolver *solver) override;
    void print() override;
    Component * getControllerIfExists(std::vector<Component *> components) override;
    ~CCVS() override;
    CCVS(Group group, char *label, data_t *value, int *nodes, char* controllerCurrent);
};

class ISource: public Component {
public:
    void stamp(OPSolver *solver) override;
    ISource(Group group, char *label, data_t *value, int *nodes);
};

class MOSFET: public Component {
public:
    void stamp(OPSolver *solver) override;
    MOSFET(Group group, char *label, data_t *value, int *nodes);
};

class BJT: public Component {
public:
    void stamp(OPSolver *solver) override;
    BJT(Group group, char *label, data_t *value, int *nodes);
};

class Resistor: public Component {
public:
    void stamp(OPSolver *solver) override;
    Resistor(Group group, char *label, data_t *value, int *nodes);
};

class VSource: public Component {
public:
    void stamp(OPSolver *solver) override;
    VSource(Group group, char *label, data_t *value, int *nodes);
};

class ComponentFactory {
public:
    Component *
    createComponent(RowType type, char *label, int *nodes, data_t *value, char *controllerCurrent, double initialCondition,
                        SineParams *sineParams, PWLParams *pwlParams);
};

#endif //BINS_COMPONENT_H
