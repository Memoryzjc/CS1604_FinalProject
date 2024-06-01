#include <sstream>
#include <string>
#include <cassert>
#include "engine.h"
#include "units.h"
#include "vector.h"
#include "algorithms.h"

using namespace std;

bool debugMode = true;

enum Site {UL = 0, U, UR, L, R, DL, D, DR};


// need to find all the situation that will lead to failure
// Load map and units
Field* loadMap(istream& is, Vector<Unit*>& units)
{
    int row, col, tNum = -1, mNum = -1, gNum = -1;
    string line;
    getline(is, line);
    istringstream iss1(line);
    iss1 >> row >> col >> tNum >> mNum >> gNum;

    if (tNum < 0 || mNum < 0 || gNum < 0) return nullptr;

    Field *battlefield;
    battlefield = new Field(row, col);
    int count = 1;

    while (true) {
        count++;
        if (count > tNum + mNum + gNum + 1) break;  // break when no input

        getline(is, line);
        istringstream iss2(line);
        if (count <= tNum + 1) {  // generate terrains
            int tRow, tCol;
            char terrain;
            Terrain t;
            iss2 >> tRow >> tCol >> terrain;

            if (terrain != 'W' && terrain != 'M') return nullptr;

            switch (terrain) {
                case('W'):
                    t = WATER;
                    break;
                case('M'):
                    t = MOUNTAIN;
                    break;
                default:
                    t = PLAIN;
                    break;
            }

            battlefield->setTerrain(tRow, tCol, t);
        }
        else if (count > tNum + 1 && count <= tNum + mNum + 1) {
            int mRow, mCol;  // generate mages
            iss2 >> mRow >> mCol;
            Unit *u = new Unit(true, mRow, mCol);
            units.add(u);  // add mages to the units
            battlefield->setUnit(mRow, mCol, u);
        }
        else {
            int gRow, gCol, ap = 0, sightRange;  // generate goblins
            string t;
            char dir;

            iss2 >> gRow >> gCol >> t;

            if (t == "PG") {
                iss2 >> dir >> ap;
                Unit *u = new Unit(false, gRow, gCol, Patrol,
                                   static_cast<PatrolDirection>(dir), ap);
                units.add(u);  // add mages to the units
                battlefield->setUnit(gRow, gCol, u);
            }
            else if (t == "TG") {
                iss2 >> sightRange;
                Unit *u = new Unit(false, gRow, gCol, Tracking, sightRange);
                units.add(u);  // add mages to the units
                battlefield->setUnit(gRow, gCol, u);
            }
            else return nullptr;
        }

    }


    return battlefield;
}

// cast fireball
void castFireball(Field& field, Unit *mage, char direction, Vector<Unit*>& units) {
    int sRow = mage->getRow(), sCol = mage->getCol();
    Unit *u;
    switch (direction) {
        case 'W': {
            for (int row = sRow - 1; row >= 0; row--) {
                u = field.getUnit(row, sCol);
                if (u != nullptr) {
                    for (auto & unit : units) {
                        if (unit == u) {
                            unit = nullptr;
                            break;
                        }
                    }
                    field.removeUnit(row, sCol);
                    break;
                } else if (field.getTerrain(row, sCol) == MOUNTAIN) break;
            }
            break;
        }
        case 'A': {
            for (int col = sCol - 1; col >= 0; col--) {
                u = field.getUnit(sRow, col);
                if (u != nullptr) {
                    for (auto & unit : units) {
                        if (unit == u)  {
                            unit = nullptr;
                            break;
                        }
                    }
                    field.removeUnit(sRow, col);
                    break;
                } else if (field.getTerrain(sRow, col) == MOUNTAIN) break;
            }
            break;
        }
        case 'S': {
            for (int row = sRow + 1; row < field.getHeight(); row++) {
                u = field.getUnit(row, sCol);
                if (u != nullptr) {
                    for (auto & unit : units) {
                        if (unit == u) {
                            unit = nullptr;
                            break;
                        }
                    }
                    field.removeUnit(row, sCol);
                    break;
                } else if (field.getTerrain(row, sCol) == MOUNTAIN) break;
            }
            break;
        }
        case 'D': {
            for (int col = sCol + 1; col < field.getWidth(); col++) {
                u = field.getUnit(sRow, col);
                if (u != nullptr) {
                    for (auto & unit : units) {
                        if (unit == u)  {
                            unit = nullptr;
                            break;
                        }
                    }
                    field.removeUnit(sRow, col);
                    break;
                } else if (field.getTerrain(sRow, col) == MOUNTAIN) break;
            }
            break;
        }
        default:
            break;
    }
}

// implement the work of repelling
void repel(Field & field, int mRow, int mCol, Site loc, Vector<Unit*> & units) {
    // the number of units that will be repelled
    int num = 0;

    // denote the reason of stopping while loop
    // flag == 0: out of bounds or encounter mountain
    // flag == 1: encounter water
    // flag == 2: encounter plain with no units
    int flag = 0;

    // denote the parameter of target row and target column
    int rArg, cArg;

    // ul: tRow = mRow - (num + 1), tCol = mCol - (num + 1)
    // u: tRow = mRow - (num + 1), tCol = mCol
    // ur: tRow = mRow - (num + 1), tCol = mCol + (num + 1)
    // l: tRow = mRow, tCol = mRow - (num + 1)
    // r: tRow = mRow, tCol = mRow + (num + 1)
    // dl: tRow = mRow + (num + 1), tCol = mRow - (num + 1)
    // d: tRow = mRow + (num + 1), tCol = mCol
    // dr: tRow = mRow + (num + 1), tCol = mCol + (num + 1)
    switch (loc) {
        case UL:
            rArg = -(num + 1);
            cArg = -(num + 1);
            break;
        case U:
            rArg = -(num + 1);
            cArg = 0;
            break;
        case UR:
            rArg = -(num + 1);
            cArg = num + 1;
            break;
        case L:
            rArg = 0;
            cArg = -(num + 1);
            break;
        case R:
            rArg = 0;
            cArg = num + 1;
            break;
        case DL:
            rArg = num + 1;
            cArg = -(num + 1);
            break;
        case D:
            rArg = num + 1;
            cArg = 0;
            break;
        case DR:
            rArg = num + 1;
            cArg = num + 1;
            break;
    }
    // target row and column
    int tRow = mRow + rArg, tCol = mCol + cArg;

    // if the target is nullptr, directly return
    if (!field.inBounds(tRow, tCol) || field.getUnit(tRow, tCol) == nullptr) return;

    // get the number of units needed to be moved
    // and get the reason why the loop is stopped
    while (true) {
        if (!field.inBounds(tRow, tCol) || field.getTerrain(tRow, tCol) == MOUNTAIN) break;
        else if (field.getTerrain(tRow, tCol) == WATER) {
            flag = 1;
            break;
        } else if (field.getUnit(tRow, tCol) == nullptr) {
            flag = 2;
            break;
        } else if (field.getUnit(tRow, tCol) != nullptr) {
            num++;

            switch (loc) {
                case UL:
                    rArg = -(num + 1);
                    cArg = -(num + 1);
                    break;
                case U:
                    rArg = -(num + 1);
                    cArg = 0;
                    break;
                case UR:
                    rArg = -(num + 1);
                    cArg = num + 1;
                    break;
                case L:
                    rArg = 0;
                    cArg = -(num + 1);
                    break;
                case R:
                    rArg = 0;
                    cArg = num + 1;
                    break;
                case DL:
                    rArg = num + 1;
                    cArg = -(num + 1);
                    break;
                case D:
                    rArg = num + 1;
                    cArg = 0;
                    break;
                case DR:
                    rArg = num + 1;
                    cArg = num + 1;
                    break;
            }

            tRow = mRow + rArg;
            tCol = mCol + cArg;
        }
    }

    if (flag == 0) return;
    else {
        int index, rPara = 0, cPara = 0, rPara2 = 0, cPara2 = 0;
        index = (flag == 1) ? 1 : 0;

        switch (loc) {
            case (UL):
                rPara = index;
                cPara = index;
                rPara2 = index + 1;
                cPara2 = index + 1;
                break;
            case (U):
                rPara = index;
                rPara2 = index + 1;
                break;
            case (UR):
                rPara = index;
                cPara = -index;
                rPara2 = index + 1;
                cPara2 = -(index + 1);
                break;
            case (R):
            case (L):
                cPara = (loc == L) ? index : -index;
                cPara2 = (loc == L) ? index + 1 : -(index + 1);
                break;
            case (DL):
                rPara = -index;
                cPara = index;
                rPara2 = -(index + 1);
                cPara2 = index + 1;
                break;
            case (D):
                rPara = -index;
                rPara2 = -(index + 1);
                break;
            case (DR):
                rPara = -index;
                cPara = -index;
                rPara2 = -(index + 1);
                cPara2 = -(index + 1);
                break;
        }

        // if encountering water, remove the unit adjacent to water
        if (flag == 1) {
            Unit *u = field.getUnit(tRow + rPara, tCol + cPara);
            for (auto & unit : units) {
                if (unit == u) {
                    unit = nullptr;
                }
            }
            field.removeUnit(tRow + rPara, tCol + cPara);
        }

        while (index < num) {
            switch (loc) {
                case (UL):
                    rPara = index;
                    cPara = index;
                    rPara2 = index + 1;
                    cPara2 = index + 1;
                    break;
                case (U):
                    rPara = index;
                    rPara2 = index + 1;
                    break;
                case (UR):
                    rPara = index;
                    cPara = -index;
                    rPara2 = index + 1;
                    cPara2 = -(index + 1);
                    break;
                case (R):
                case (L):
                    cPara = (loc == L) ? index : -index;
                    cPara2 = (loc == L) ? index + 1 : -(index + 1);
                    break;
                case (DL):
                    rPara = -index;
                    cPara = index;
                    rPara2 = -(index + 1);
                    cPara2 = index + 1;
                    break;
                case (D):
                    rPara = -index;
                    rPara2 = -(index + 1);
                    break;
                case (DR):
                    rPara = -index;
                    cPara = -index;
                    rPara2 = -(index + 1);
                    cPara2 = -(index + 1);
                    break;
            }

            field.exchangeUnit(tRow + rPara, tCol + cPara, tRow + rPara2, tCol + cPara2);

            index++;
        }
    }
}

// cast tornado
void castTornado(Field& field, Unit *mage, Vector<Unit*>& units) {
    int sRow = mage->getRow(), sCol = mage->getCol();
    for (int loc = UL; loc < 8; loc++) {
        Site location = static_cast<Site>(loc);
        repel(field, sRow, sCol, location, units);
    }
}

// move mages and cast spell
void mageAction(Field& field, Unit *mage, const Vector<char>& commands, Vector<Unit*>& units) {
    int n = commands.size();
    int actionPoints = 3;  // denote the action point
    for (int i = 0; i < n; i++) {
        int sRow = mage->getRow(), sCol = mage->getCol();
        char cmd = commands[i];
        switch (cmd) {
            case 'W': {
                if (actionPoints == 0) break;
                field.moveUnit(sRow, sCol, sRow - 1, sCol);
                actionPoints--;
                break;
            }
            case 'A': {
                if (actionPoints == 0) break;
                field.moveUnit(sRow, sCol, sRow, sCol - 1);
                actionPoints--;
                break;
            }
            case 'S': {
                if (actionPoints == 0) break;
                field.moveUnit(sRow, sCol, sRow + 1, sCol);
                actionPoints--;
                break;
            }
            case 'D': {
                if (actionPoints == 0) break;
                field.moveUnit(sRow, sCol, sRow, sCol + 1);
                actionPoints--;
                break;
            }
            case 'X': {
                if (i + 1 >= commands.size()) break;
                char direction = commands[i + 1];
                castFireball(field, mage, direction, units);
                i++;
                break;
            }
            case 'Y': {
                castTornado(field, mage, units);
                break;
            }
            default: {
                break;
            }
        }
    }
}

// find whether the adjacent point has a mage
bool findMage(Field & field, int gRow, int gCol) {
    Unit *u;
    if (field.inBounds(gRow - 1, gCol)) {
        u = field.getUnit(gRow - 1, gCol);
        if (u != nullptr && u->getSide()) return true;
    }

    if (field.inBounds(gRow + 1, gCol)) {
        u = field.getUnit(gRow + 1, gCol);
        if (u != nullptr && u->getSide()) return true;
    }

    if (field.inBounds(gRow, gCol - 1)) {
        u = field.getUnit(gRow, gCol - 1);
        if (u != nullptr && u->getSide()) return true;
    }

    if (field.inBounds(gRow, gCol + 1)) {
        u = field.getUnit(gRow, gCol + 1);
        if (u != nullptr && u->getSide()) return true;
    }

    return false;
}

// implement the attack of goblins
void attack(Field & field, int gRow, int gCol, Vector<Unit*> & units) {
    Unit *u;

    // up
    if (field.inBounds(gRow - 1, gCol)) {
        u = field.getUnit(gRow - 1, gCol);
        if (u != nullptr && u->getSide()) {
            for (auto & unit : units) {
                if (unit == u) {
                    unit = nullptr;
                    break;
                }
            }
            field.removeUnit(gRow - 1, gCol);
        }
    }

    // down
    if (field.inBounds(gRow + 1, gCol)) {
        u = field.getUnit(gRow + 1, gCol);
        if (u != nullptr && u->getSide()) {
            for (auto & unit : units) {
                if (unit == u) {
                    unit = nullptr;
                    break;
                }
            }
            field.removeUnit(gRow + 1, gCol);
        }
    }

    // left
    if (field.inBounds(gRow, gCol - 1)) {
        u = field.getUnit(gRow, gCol - 1);
        if (u != nullptr && u->getSide()) {
            for (auto & unit : units) {
                if (unit == u) {
                    unit = nullptr;
                    break;
                }
            }
            field.removeUnit(gRow, gCol - 1);
        }
    }

    // right
    if (field.inBounds(gRow, gCol + 1)) {
        u = field.getUnit(gRow, gCol + 1);
        if (u != nullptr && u->getSide()) {
            for (auto & unit : units) {
                if (unit == u) {
                    unit = nullptr;
                    break;
                }
            }
            field.removeUnit(gRow, gCol + 1);
        }
    }

}

// implement the action of patrol goblin
void patrolGoblinAction(Field & field, Unit* goblin, Vector<Unit*> & units) {
    int ap = goblin->getActionPoint();
    bool stop = false;

    // patrol until ap == 0
    while(true) {
        PatrolDirection pDir = goblin->getPatrolDir();
        int gRow = goblin->getRow(), gCol = goblin->getCol();
        switch (pDir) {
            case (Up):

                if (findMage(field, gRow, gCol)) {
                    attack(field, gRow, gCol, units);
                    goblin->setPatrolDir(Down);
                    stop = true;
                }
                else if (ap == 0) {
                    goblin->setPatrolDir((Down));
                    stop = true;
                }
                else if (!field.inBounds(gRow - 1, gCol) ||
                         field.getTerrain(gRow - 1, gCol) != PLAIN ||
                         field.getUnit(gRow - 1, gCol) != nullptr) {
                    goblin->setPatrolDir(Down);
                    stop = true;
                }
                else {
                    field.exchangeUnit(gRow, gCol, gRow - 1, gCol);
                }

                break;
            case (Down):

                if (findMage(field, gRow, gCol)) {
                    attack(field, gRow, gCol, units);
                    goblin->setPatrolDir(Up);
                    stop = true;
                }
                else if (ap == 0) {
                    goblin->setPatrolDir((Up));
                    stop = true;
                }
                else if (!field.inBounds(gRow + 1, gCol) ||
                         field.getTerrain(gRow + 1, gCol) != PLAIN ||
                         field.getUnit(gRow + 1, gCol) != nullptr) {
                    goblin->setPatrolDir(Up);
                    stop = true;
                }
                else {
                    field.exchangeUnit(gRow, gCol, gRow + 1, gCol);
                }

                break;
            case (Left):

                if (findMage(field, gRow, gCol)) {
                    attack(field, gRow, gCol, units);
                    goblin->setPatrolDir(Right);
                    stop = true;
                }
                else if (ap == 0) {
                    goblin->setPatrolDir((Right));
                    stop = true;
                }
                else if (!field.inBounds(gRow, gCol - 1) ||
                         field.getTerrain(gRow, gCol - 1) != PLAIN ||
                         field.getUnit(gRow, gCol - 1) != nullptr) {
                    goblin->setPatrolDir(Right);
                    stop = true;
                }
                else {
                    field.exchangeUnit(gRow, gCol, gRow, gCol - 1);
                }

                break;
            case (Right):

                if (findMage(field, gRow, gCol)) {
                    attack(field, gRow, gCol, units);
                    goblin->setPatrolDir(Left);
                    stop = true;
                }
                else if (ap == 0) {
                    goblin->setPatrolDir((Left));
                    stop = true;
                }
                else if (!field.inBounds(gRow, gCol + 1) ||
                         field.getTerrain(gRow, gCol + 1) != PLAIN ||
                         field.getUnit(gRow, gCol + 1) != nullptr) {
                    goblin->setPatrolDir(Left);
                    stop = true;
                }
                else {
                    field.exchangeUnit(gRow, gCol, gRow, gCol + 1);
                }

                break;
            default: break;
        }

        if (stop) break;
        ap--;
    }
}


// search all mages within the tracking goblin's sight
Vector<Unit*> searchMage(Field & field, Unit* tGoblin) {
    int gRow = tGoblin->getRow(), gCol = tGoblin->getCol();
    int sightR = tGoblin->getSightRange();
    int fieldH = field.getHeight(), fieldL = field.getWidth();
    Vector<Unit*> mages;

    // determine the search range
    int rowS = gRow - min(gRow, sightR), rowE = gRow + min(fieldH - gRow, sightR) - 1;
    int colS = gCol - min(gCol, sightR), colE = gCol + min(fieldL - gCol, sightR) - 1;

    // search within the range
    for (int i = rowS; i <= rowE; i++) {
        for (int j = colS; j <= colE; j++) {
            if (abs(i - gRow) + abs(j - gCol) > sightR
                || field.getUnit(i, j) == nullptr) continue;
            else {
                if (field.getUnit(i, j)->getSide())
                    mages.add(field.getUnit(i, j));
            }
        }
    }

    return mages;
}


// implement the action of tracking goblin
void trackingGoblinAction(Field & field, Unit* tGoblin, Vector<Unit*> & units) {
    // step 1: search all mages within its sight
    Vector<Unit*> mages = searchMage(field, tGoblin);
    if (mages.size() == 0) return;

    // step 2: find the nearest mage and figure out the path
    int gRow = tGoblin->getRow(), gCol = tGoblin->getCol();
    pair<int, Vector<point>> shortestPath =
            searchShortestPath(field, gRow, gCol, mages[0]->getRow(), mages[0]->getCol());
    // paths: store the shortest path to every reachable mage
    // nearestMage: store the nearest mages
    Vector<pair<int, Vector<point>>> paths, nearestMage;


    // find the nearest mage
    // and store the shortest way to
    // every reachable mage within the sight individually
     for (int i = 0; i < mages.size(); i++) {
        pair<int, Vector<point>> path =
                searchShortestPath(field, gRow, gCol, mages[i]->getRow(), mages[i]->getCol());
        if (path.first != -1) {
            paths.add(path);
            if (path.first < shortestPath.first) shortestPath = path;
        }
    }

    // find the number of nearest mages and store the path to nearestMage
    for (auto path: paths) {
        if (path.first == shortestPath.first) nearestMage.add(path);
    }

    // step 3: use the path to find and attack the mage
    if (nearestMage.size() == 0) return;
    else if (nearestMage.size() == 1) {
        Vector<point> way = nearestMage[0].second;
        int tRow = way[max(way.size() - 2, 0)].first, tCol = way[max(way.size() - 2, 0)].second;
        field.exchangeUnit(gRow, gCol, tRow, tCol);
        attack(field, tRow, tCol, units);
        return;
    }
    else {
        Vector<point> way = nearestMage[0].second, target = way;
        int tRow = way[way.size() - 1].first, tCol = way[way.size() - 1].second;

        // find the smallest partial order
        for (int i = 1; i < nearestMage.size(); i++) {
            way = nearestMage[i].second;
            if (coordOrder(way[way.size() - 1].first, way[way.size() - 1].second, tRow, tCol)) {
                tRow = way[way.size() - 1].first;
                tCol = way[way.size() - 1].second;
                target = way;
            }
        }

        field.exchangeUnit(gRow, gCol,
                           target[max(target.size() - 2, 0)].first,
                           target[max(target.size() - 2, 0)].second);
        attack(field, target[max(target.size() - 2, 0)].first,
               target[max(target.size() - 2, 0)].second, units);
    }
}


// implement the action of goblins
void goblinAction(Field & field, Unit* goblin, Vector<Unit*> & units) {
    UnitType t = goblin->getType();
    switch (t) {
        case (Patrol):{
            patrolGoblinAction(field, goblin, units);
            break;
        }
        case (Tracking): {
            trackingGoblinAction(field, goblin, units);
            break;
        }
        default: break;
    }
}

// judge whether a game ends
bool playerWin(Vector<Unit*> & units) {
    for (auto unit : units) {
        if (unit != nullptr && !unit->getSide()) return false;
    }
    return true;
}

bool playerLose(Vector<Unit*> & units) {
    for (auto unit : units) {
       if (unit != nullptr && unit->getSide()) return false;
    }
    return true;
}

// Main loop for playing the game
void play(Field& field, istream& is, ostream& os, Vector<Unit*>& units)
{

    int numTurns = 1;
    while (is)
    {
        os << "Turn " << numTurns << " begins:" << endl;
        // Print the new map
        os << field << endl;

        if (playerWin(units)) {
            cout << "You Win!";
            break;
        }
        else if (playerLose(units)) {
            cout << "You Lose!";
            break;
        }

        // unit moves
        for (int i = 0; i < units.size(); i++) {
            // If a unit is beaten by a fireball, dismiss the loop of the unit.
            if (units[i] == nullptr) continue;

            bool side = units[i]->getSide();

            // move mages
            if (side) {
                int r = units[i]->getRow(), c = units[i]->getCol();
                Vector<char> commands;  // store commands
                os << "Please move the unit at " << '(' << r << ',' << c << ')' << endl;
                string cmd;
                getline (is, cmd);

                // add commands to steps from inputs
                for (int j = 0; j < static_cast<int>(cmd.length()); j++) {
                    if (!isspace(cmd[j])) commands.add(cmd[j]);
                }

                // mages take action
                mageAction(field, units[i], commands, units);
                os << field << endl;
            }
            else {
                goblinAction(field, units[i], units);
            }
        }

        numTurns++;
    }
}


