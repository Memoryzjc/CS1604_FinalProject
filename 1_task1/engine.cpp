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

// Load map and units
Field* loadMap(istream& is)
{
    int row, col, tNum, mNum, gNum;
    string line;
    getline(is, line);
    istringstream iss1(line);
    iss1 >> row >> col >> tNum >> mNum >> gNum;

    Field *battlefield;
    battlefield = new Field(row, col);
    int count = 1;

    while (true) {
        count++;
        getline(is, line);
        istringstream iss2(line);
        if (count == 47)
            count = 47;
        if (count <= tNum + 1) {
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
        } else if (count > tNum + 1 && count <= tNum + mNum + 1) {
            int mRow, mCol;
            iss2 >> mRow >> mCol;
            Unit u(true, mRow, mCol);
            battlefield->setUnit(mRow, mCol, &u);
        } else {
            int gRow, gCol;
            string t;
            iss2 >> gRow >> gCol >> t;

            if (t != "PG") return nullptr;

            Unit u(false, gRow, gCol);
            battlefield->setUnit(gRow, gCol, &u);
        }

        if (count == tNum + mNum + gNum + 1) break;
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
        numTurns++;
    }
}


