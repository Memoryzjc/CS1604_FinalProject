#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

// Constructor
Unit::Unit(bool sd, int r, int c, UnitType t, int sr)
: side(sd), row(r), col(c), type(t), sightRange(sr), pDir(Still)
{

}

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
: type(Mage), pDir(Still)
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

int Unit::getSightRange() const
{
    return sightRange;
}

void Unit::setSightRange(int sr)
{
    sightRange = sr;
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


