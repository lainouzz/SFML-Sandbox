#include "Grid.h"
#include "Constants.h"

void clearGrid(std::vector<std::vector<Cell>>& grid) {
	for (int y = 0; y < GRID_HEIGHT; y++)
		for (int x = 0; x < GRID_WIDTH; x++)
			grid[y][x] = Cell{};
	grainCount = 0;
}

