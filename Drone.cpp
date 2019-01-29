#include <iostream>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <algorithm>
#include <vector>
#include "Drone.h"
using namespace std;

static constexpr float maxVelocity = 0.25f;
static constexpr float acceleration = 0.1f;
static constexpr float searchRange = 5.0f; //Range of localised search.
static int caveWidth;
static int caveHeight;
static vector<vector<int>> cave;

float posX; //Current x position in the cave.
float posY; //Current y position in the cave.
float orientation; //Orientation: 0 -> Facing North.

//###int internalMap[caveWidth][caveHeight]; //Known contents of the cave. Start all unknown.
//###List frontierCells; //Free cells that are adjacent to unknowns.
//###List path; //Position and time pairs.

enum MapCell { Free, Occupied, Unknown, Frontier }; //###

struct Cell {
	int x;
	int y;
	Cell(int _x, int _y) : x(_x), y(_y) {}
};

struct SenseCell {
  float x;
  float y;
  float range;
  MapCell type;
  SenseCell(float _x, float _y, float _range) : x(_x), y(_y), range(_range) {
    type = Unknown;
  }
};

bool operator <(const SenseCell& a, const SenseCell& b) {
  return a.range < b.range;
}


//Sets global cave properties.
void Drone::setParams(int _caveWidth, int _caveHeight, vector<vector<int>> _cave) {
  caveWidth = _caveWidth;
  caveHeight = _caveHeight;
  cave = _cave;
}

//Sets the drone's current position in the cave.
void Drone::setPosition(int _x, int _y) {
  posX = _x;
  posY = _y;
}

//Models the sensing of the immediate local environment.
void Drone::sense() {

  vector<SenseCell> candidates; //List of candidate cells.
  vector<SenseCell> freeCells; //List of found free cells.
  vector<SenseCell> occupiedCells; //List of found occupied cells.

  //For each cell in the bounding box of the search range.
  for (int i = floor(posX - searchRange); i <= ceil(posX + searchRange); i++) {
    for (int j = floor(posY - searchRange); j <= ceil(posY + searchRange); j++) {
      //Discard Out-of-bounds cells.
      if (i < 0 || j < 0 || i >= caveWidth || j >= caveHeight) { continue; }
      //Allows only cells in the range.
      float range = pow(pow(posX - (float)i, 2.0) + pow(posY - (float)j, 2.0), 0.5);
      if (range > searchRange) { continue; }
      //Push the candidate cell onto the vector.
      candidates.push_back(SenseCell(i,j,range));
    }
  }

  //Sorts the list of cells by distance to the drone in increasing order.
  sort(candidates.begin(), candidates.end());


  //Check to make sure you can't sense objects hidden behind something else.
  //ray from drone center to cell center.

  //###
  cout << "Sense Vector" << endl;
  for (vector<SenseCell>::iterator it = candidates.begin(); it != candidates.end(); ++it) {
    int lookup = cave[it->x][it->y];
    cout << '(' << it->x << "," << it->y << ") - " << it->range << " - " << lookup << endl;
  }


  for (vector<SenseCell>::iterator dest = candidates.begin(); dest != candidates.end(); ++dest) {

    //If the cell range is 1 or less then immediately add it to the list.
    if (dest->range <= 1) {
      if (cave[dest->x][dest->y] == Free) {
        freeCells.push_back(*dest);
      }
      else {
        occupiedCells.push_back(*dest);
      }
      continue;
    }

    bool collisionDetected = false;

    //Obstacle in line of sight between drone position and destination cell check.
    for (vector<SenseCell>::iterator occupyCheck = occupiedCells.begin(); occupyCheck != occupiedCells.end(); ++occupyCheck) {
      float dtx0 = (occupyCheck->x - 0.5f - x) / (dest->x - x);
      float dtx1 = (occupyCheck->x + 0.5f - x) / (dest->x - x);
      float dty0 = (occupyCheck->y - 0.5f - y) / (dest->y - y);
      float dty1 = (occupyCheck->y + 0.5f - y) / (dest->y - y);

      if ((dtx0 >= 0 && dtx0 <= 1) && (dtx1 >= 0 && dtx1 <= 1) && (dty0 >= 0 && dty0 <= 1) && (dty1 >= 0 && dty1 <= 1)) {
        cout << "COLLISION at (" << dest->x << "," << dest->y << ")";
        cout << "[" << dtx0 << "," << dtx1 << "," << dty0 << "," << dty1 << "]" << endl;
        collisionDetected = true;
        break;
      }
    }

    if (!collisionDetected) {
      if (cave[dest->x][dest->y] == Free) {
        freeCells.push_back(*dest);
      }
      else {
        occupiedCells.push_back(*dest);
      }
    }

  }

  cout << "FREE CELLS" << endl;
  for (vector<SenseCell>::iterator it = freeCells.begin(); it != freeCells.end(); ++it) {
    cout << "(" << it->x << "," << it->y << ") ";
  }
  cout << endl;

  cout << "OCCUPIED CELLS" << endl;
  for (vector<SenseCell>::iterator it = occupiedCells.begin(); it != occupiedCells.end(); ++it) {
    cout << "(" << it->x << "," << it->y << ") ";
  }
  cout << endl;

  //if range <= 1 no collisions to detect.
  //if free, remove from v add to freecell list.
  //if occupied, remove from v check collisions, if no collisions add to occupiedlist.


  /*//###
  float dx = float(i) - x;
  float dy = float(j) - y;
  //edge cases for dx = 0 or dy = 0;
  x = x + t * dx;
  y = y + t * dy;
  for each cell to check;
  cx; cy;
  tx0 = (cx - 0.5f - x) / dx;
  tx1 = (cx + 0.5f - x) / dx;
  ty0 = (cy - 0.5f - y) / dy;
  ty1 = (cy + 0.5f - y) / dy;*/
  //if any t values are between 0 and 1 inclusive then cell.*/

}












//If drone-to-drone collision avoidance becomes too tough.
//Allow them to pass through each other, saying they take different altitudes.


//(1)
//Search local area.

//(2)
//Place sensed data into internal map.

//(3)
//Compute frontier cells.

//(4)
//Find optimal frontier cell.

//(5)
//Plan path to frontier cell.
