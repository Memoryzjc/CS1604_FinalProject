#include <sstream>
#include <string>
#include <iomanip>
#include <cassert>
#include <Queue.h>
#include "engine.h"
#include "units.h"
#include "vector.h"
#include "Stack.h"
#include "algorithms.h"
using namespace std;

bool debugMode = true;

// judge whether a string is all composed of digit and is not empty
bool isDigit(string s) {
    if (s.empty()) return false;

    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Load map and units
Field* loadMap(istream& is)
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
        if (count > tNum + mNum + gNum + 1) break;

        getline(is, line);
        istringstream iss2(line);

        if (count <= tNum + 1) {
            string tRow, tCol, terrain;
            Terrain t;
            iss2 >> tRow >> tCol >> terrain;

            if (!isDigit(tRow) || !isDigit(tCol)) return nullptr;
            if ((terrain[0] != 'W' && terrain[0] != 'M') || terrain.length() != 1) return nullptr;

            switch (terrain[0]) {
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

            if (battlefield->getTerrain(stoi(tRow), stoi(tCol)) != PLAIN) return nullptr;
            else battlefield->setTerrain(stoi(tRow), stoi(tCol), t);
        } else if (count > tNum + 1 && count <= tNum + mNum + 1) {
            string mRow, mCol, check;
            iss2 >> mRow >> mCol >> check;

            // guarantee mRow and mCol are digits and not empty
            if (!isDigit(mRow) || !isDigit(mCol)) return nullptr;
            // guarantee input is the mage's form
            if (!check.empty()) return nullptr;

            Unit *u = new Unit(true, stoi(mRow), stoi(mCol));

            // if (mRow, mCol) is occupied, return nullptr
            if(!battlefield->setUnit(stoi(mRow), stoi(mCol), u)) return nullptr;
        } else {
            string gRow, gCol, t;
            iss2 >> gRow >> gCol >> t;

            if (t != "PG") return nullptr;

            if (!isDigit(gRow) || !isDigit(gCol)) return nullptr;

            Unit *u = new Unit(false, stoi(gRow), stoi(gCol));
            if (!battlefield->setUnit(stoi(gRow), stoi(gCol), u)) return nullptr;
        }
    }
    return battlefield;
}


// Main loop for playing the game
void play(Field& field, istream& is, ostream& os, Vector<Unit*>& units)
{

    int numTurns = 1;
    while (is)
    {
        // os << "Turn " << numTurns << " begins:" << endl;
        // Print the new map
        os << field << endl;

        // unit moves
        // Fill in your code here

        // Preventing loop in empty program
        // string line;
        // if (!getline(is,line)) break;
        break;
    }
}


