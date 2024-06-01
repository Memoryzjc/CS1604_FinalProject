#include <iostream>
#include <fstream>
#include "field.h"
#include "units.h"
#include "engine.h"

using namespace std;

int main(int argc, char* argv[])
{

    // string filename = "map1.txt";

    Vector<Unit*> units;
    Field *f = loadMap(cin, units);

    if (f == nullptr) {
        cout << "Failed to load map!" << endl;
        return 0;
    }

    // play(*f, ifs, ofs);
    play(*f, cin, cout,units);

    delete f;
    for (auto & unit : units) delete unit;

    return 0;
}
