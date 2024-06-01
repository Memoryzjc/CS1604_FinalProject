#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

// Constructor
Unit::Unit(bool sd, int r, int c, UnitType t, PatrolDirection pDir, int aPoint)
: side(sd), row(r), col(c), type(t), pDir(pDir)
{
    actionPoint = (sd) ? 3 : aPoint;
}

Unit::Unit(bool sd, int r, int c, UnitType t, PatrolDirection pd)
: side(sd), row (r), col(c), type(t), pDir(pd)
{
    actionPoint = (sd) ? 3 : 0;
}

Unit::Unit(bool sd, int r, int c, UnitType t)
: side(sd), row(r), col(c), type(t), pDir(Still)
{
    actionPoint = (sd) ? 3 : 0;
}

Unit::Unit(bool sd, int r, int c)
: side(sd), row(r), col(c), pDir(Still)
{
    type = (sd) ? Mage : Patrol;
    actionPoint = (sd) ? 3 : 0;
}

Unit::Unit()
: side(true), row(0), col(0), type(Mage), pDir(Still), actionPoint(3)
{

}

// Get the side
bool Unit::getSide() const
{
    return side;
}

// Get and set the row[col] of a unit
int Unit::getRow() const
{
    return row;
}

int Unit::getCol() const
{
    return col;
}

void Unit::setRow(int r)
{
    row = r;
}

void Unit::setCol(int c)
{
    col = c;
}

void Unit::setSide(bool s)
{
    side = s;
}

UnitType Unit::getType() const
{
    return type;
}

void Unit::setType(UnitType t)
{
    type = t;
}

PatrolDirection Unit::getPatrolDir() const
{
    return pDir;
}

void Unit::setPatrolDir(PatrolDirection pd)
{
    pDir = pd;
}

int Unit::getActionPoint() const
{
    return actionPoint;
}

void Unit::setActionPoint(int ap)
{
    actionPoint = ap;
}

// Get the symbol of the unit
string getUnitSymbol(const Unit& u)
{
    bool side = u.getSide();
    if (side){
        return "@@";
    }
    else{
        UnitType t = u.getType();
        switch (t) {
            case (Tracking):
                return "TG";
            default:
                return "PG";
        }
    }
    assert (false);
}


