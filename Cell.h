#ifndef CELL_H
#define CELL_H

struct Cell {
	int x;
	int y;
	Cell() : x(0), y(0) {}
	Cell(int _x, int _y) : x(_x), y(_y) {}
};

#endif
