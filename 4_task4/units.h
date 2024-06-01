#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <memory>
#include <string>
#include "field.h"


enum UnitType {Mage, Patrol, Tracking};
enum PatrolDirection {Still, Up = 'W', Down = 'S', Left = 'A', Right = 'D'};

/* Class for units */
class Unit {
public:

    // Constructor
    Unit(bool sd, int r, int c, UnitType t, int sr);
    Unit(bool sd, int row, int col, UnitType type, PatrolDirection pDir, int actionPoint);
    Unit(bool sd, int row, int col, UnitType type, PatrolDirection pDir);
    Unit(bool sd, int row, int col, UnitType type);
    Unit(bool sd, int row, int col);
    Unit();

    // Check which side the unit belongs to
    bool getSide() const;
    void setSide(bool s);

    // Get and set row(col) of the unit
    int getRow() const;
    void setRow(int row);

    int getCol() const;
    void setCol(int col);

    // get and set the type of the unit
    UnitType getType() const;
    void setType(UnitType type);

    // get and set the patrol direction of goblin
    PatrolDirection getPatrolDir() const;
    void setPatrolDir(PatrolDirection patrolDirection);

    // get and set the action point of unit
    int getActionPoint() const;
    void setActionPoint(int actionPoint);

    // get and set the sight range of unit
    int getSightRange() const;
    void setSightRange(int sr);

private:
    // Position in the field
    int row{};
    int col{};
    bool side{true};
    UnitType type;
    PatrolDirection pDir;
    int actionPoint{3};
    int sightRange{};
};

std::string getUnitSymbol(const Unit& u);
#endif // UNITS_H_INCLUDED
