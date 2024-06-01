#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <ostream>
#include <Grid.h>

// Forward declaration of the class of units
class Unit;

/* Terrains */
enum Terrain {PLAIN, WATER, MOUNTAIN};

/* Battlefield */
class Field {
public:
    // Constructor
    Field(int h, int w);

    // Get the height and width of the field
    int getHeight() const;
    int getWidth() const;

    // Check if the row and column is in bounds
    bool inBounds(int row, int col) const;

    // Get the unit at row and col
    Unit* getUnit(int row, int col) const;

    // Set a new unit at row and col
    // Return false if the coordinate is already occupied
    bool setUnit(int row, int col, Unit* u);

    // exchange the unit at (sRow and sCol) to the unit at (tRow, tCol).
    void exchangeUnit(int sRow, int sCol, int tRow, int tCol);

    // Remove the unit at row and col
    void removeUnit(int row, int col);
    // Return false if there is no unit at that coordinate
    // bool removeUnit(int row, int col);

    // Move a unit to a different coordinate
    void moveUnit(int sRow, int sCol, int tRow, int tCol);
    // Return false if the target coordinate is already occupied
    // bool moveUnit(int sRow, int sCol, int tRow, int tCol);

    // Get and set the terrain of a square
    Terrain getTerrain(int row, int col) const;
    void setTerrain(int row, int col, Terrain t);

    // Destructor
    ~Field();

private:
    // Store the units
    Grid<Unit*> units;
    // Store the terrain
    Grid<Terrain> terrains;
};

// Display the field on the out stream os
std::ostream& operator<<(std::ostream& os, const Field& field);


#endif // FIELD_H_INCLUDED
