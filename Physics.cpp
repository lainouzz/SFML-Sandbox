#include "Physics.h"
#include "Constants.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

void fluidCellUpdate(int x, int y, std::vector<std::vector<Cell>>& grid)
{
	Cell& current = grid[y][x];

	if (current.mass <= 0.f || (current.type != MaterialType::Water && current.type != MaterialType::Lava))
		return;

	auto canFlow = [&](int nx, int ny) {
		return inBounds(nx, ny) &&
			(grid[ny][nx].type == MaterialType::Empty || grid[ny][nx].type == MaterialType::Water || grid[ny][nx].type == MaterialType::Lava);
	};

	auto transferFlow = [&](int nx, int ny, float flow) {
		if (grid[ny][nx].type == MaterialType::Empty) {
			grid[ny][nx].type = current.type;
			grid[ny][nx].filled = true;
				grid[ny][nx].color = (current.type == MaterialType::Water) ? varyColor(MaterialColors::Water, 5) : varyColor(MaterialColors::Lava, 5);
		}
		grid[ny][nx].mass += flow;
		current.mass -= flow;
	};

	float remainingMass = current.mass;

	// Try to flow downwards first
	if (canFlow(x, y + 1)) {
		float flow = std::min(remainingMass, (MAX_MASS + MAX_COMPRESS) - grid[y + 1][x].mass);
		if (flow > MIN_FLOW) {
			transferFlow(x, y + 1, flow);
			remainingMass -= flow;
		}
	}

	// If still has mass, try to flow horizontally
	if (remainingMass <= MIN_FLOW) {
		current.mass = 0.f;
		current.filled = false;
		current.type = MaterialType::Empty;
		current.color = sf::Color::Transparent;
		return;
	}

	int dir = (rand() % 2 == 0) ? -1 : 1;
	int first = dir;
	int second = -dir;

	// Try to flow horizontally in the preferred direction
	if(canFlow(x + first, y)){
		float flow = std::clamp((remainingMass - grid[y][x + first].mass) / 2.f, 0.f, remainingMass);
		if (flow > MIN_FLOW) {
			transferFlow(x + first, y, flow);
			remainingMass -= flow;
		}
	}

	// If still has mass, try the opposite direction
	if(canFlow(x + second, y)){
		float flow = std::clamp((remainingMass - grid[y][x + second].mass) / 2.f, 0.f, remainingMass);
		if (flow > MIN_FLOW) {
			transferFlow(x + second, y, flow);
			remainingMass -= flow;
		}
	}

	// Finally, try to flow upwards if still has mass (for lava to rise and for water to find paths)
	if (canFlow(x, y - 1)) {
		Cell& above = grid[y - 1][x];
		if (above.type == current.type || above.type == MaterialType::Empty) {
			float flow = std::clamp((remainingMass - MAX_MASS) / 2.f, 0.f, remainingMass);
			if (flow > MIN_FLOW) {
				transferFlow(x, y - 1, flow);
				remainingMass -= flow;
			}
		}
	}

	current.mass = remainingMass;
	if (current.mass <= MIN_FLOW) {
		current.mass = 0.f;
		current.filled = false;
		current.type = MaterialType::Empty;
		current.color = sf::Color::Transparent;
	}

	if (current.type == MaterialType::Lava) {
		handleLavaInteractions(x, y, grid);
	}
}

void handleLavaInteractions(int x, int y, std::vector<std::vector<Cell>>& grid)
{
	const float IGNITION_TEMP = 150.f;
	const int dx[] = { -1, 1, 0, 0 };
	const int dy[] = { 0, 0, -1, 1 };

	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		if (!inBounds(nx, ny))
			continue;

		Cell& neighbor = grid[ny][nx];

		if (neighbor.type == MaterialType::Wood) {
			neighbor.temperature += 10.f;

			if (neighbor.temperature >= IGNITION_TEMP) {
				neighbor.type = MaterialType::Fire;
				neighbor.mass = 8.f;
				neighbor.burning = true;
				neighbor.color = MaterialColors::Fire;
				neighbor.filled = true;
			}
		}

		if (neighbor.type == MaterialType::Water) {
			neighbor.type = MaterialType::Stone;
			neighbor.mass = 0.f;
			neighbor.burning = false;
			neighbor.temperature = 20.f;
			neighbor.color = MaterialColors::Stone;
			neighbor.filled = true;
		}
	}
}

void updateSandPhysics(std::vector<std::vector<Cell>>& grid) {
	for (int y = GRID_HEIGHT - 2; y >= 0; y--) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (!grid[y][x].filled) continue;
			if (grid[y][x].type != MaterialType::Sand &&
				grid[y][x].type != MaterialType::WetSand) continue;

			if (!grid[y + 1][x].filled) {
				grid[y + 1][x] = grid[y][x];
				grid[y][x] = Cell{};
			}
			else {
				int dir = (rand() % 2 == 0) ? -1 : 1;
				if (inBounds(x + dir, y + 1) &&
					!grid[y + 1][x + dir].filled) {
					grid[y + 1][x + dir] = grid[y][x];
					grid[y][x] = Cell{};
				}
			}
		}
	}
}

void updateFluidPhysics(std::vector<std::vector<Cell>>& grid) {
	static bool scanLeftToRight = true;

	for (int y = GRID_HEIGHT - 2; y >= 0; y--) {
		if (scanLeftToRight) {
			for (int x = 0; x < GRID_WIDTH; x++)
				if (grid[y][x].type == MaterialType::Water || grid[y][x].type == MaterialType::Lava)
					fluidCellUpdate(x, y, grid);
		}
		else {
			for (int x = GRID_WIDTH - 1; x >= 0; x--)
				if (grid[y][x].type == MaterialType::Water || grid[y][x].type == MaterialType::Lava)
					fluidCellUpdate(x, y, grid);
		}
	}

	scanLeftToRight = !scanLeftToRight;
}

void fireCellUpdate(int x, int y, std::vector<std::vector<Cell>>& grid)
{
	Cell& current = grid[y][x];

	if (current.type != MaterialType::Fire && !(current.type == MaterialType::Wood && current.burning))
		return;

	current.mass -= BURN_RATE;

	int fireFlicker = rand() % 40;
	current.color = sf::Color(255, std::max(0, 120 - fireFlicker), 0);

	const int dx[] = { -1, 1, 0 ,0, -1, 1, -1, 1 };
	const int dy[] = { 0, 0, -1, 1, -1, -1, 1, 1 };
	for (int i = 0; i < 8; i++) {
		int nx = x + dx[i], ny = y + dy[i];
		if(!inBounds(nx, ny)) continue;

		Cell& neighbor = grid[ny][nx];
		neighbor.temperature += HEAT_SPREAD;

		if (neighbor.type == MaterialType::Wood && !neighbor.burning
			&& neighbor.temperature >= IGNITION_TEMP) {
			neighbor.burning = true;
			neighbor.type = MaterialType::Fire;
			neighbor.mass = 8.f;
			neighbor.color = MaterialColors::Fire;
		}
	}

	if (current.mass <= FIRE_LIFETIME_MASS) {
		if (inBounds(x, y - 1) && grid[y - 1][x].type == MaterialType::Empty) {

			Cell& smoke = grid[y - 1][x];
			smoke = Cell{};
			smoke.filled = true;
			smoke.type = MaterialType::Smoke;
			smoke.mass = 0.5f;
			smoke.velocityY = -SMOKE_RISE_RATE;
			smoke.color = MaterialColors::Smoke;
			grainCount++;
		}
		current = Cell{};
		grainCount--;
	}
}

void updateFirePhysics(std::vector<std::vector<Cell>>& grid)
{
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[y][x].type == MaterialType::Fire)
				fireCellUpdate(x, y, grid);
		}
	}
}

void updateSmokePhysics(std::vector<std::vector<Cell>>& grid)
{
	// Move smoke upwards and dissipate current code is from "sand" physics, but with upward movement and random horizontal drift
	for (int y = 1; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			Cell& cell = grid[y][x];

			if (!cell.filled || cell.type != MaterialType::Smoke)
				continue;

			if (rand() % 3 == 0) {
				cell.color = varyColor(MaterialColors::Smoke, 30);
			}

			cell.mass -= SMOKE_DISSIPATION_RATE;
			if (cell.mass <= 0.f) {
				grid[y][x] = Cell{};
				continue;
			}

			float roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			if (roll > SMOKE_RISE_RATE)
				continue;

			int dir = (rand() % 2 == 0) ? -1 : 1;

			if (inBounds(x, y - 1) && !grid[y - 1][x].filled) {
				std::swap(grid[y][x], grid[y - 1][x]);
			}
			else if (inBounds(x + dir, y - 1) && !grid[y - 1][x + dir].filled) {
				std::swap(grid[y][x], grid[y - 1][x + dir]);
			}
			else if (inBounds(x + dir, y) && !grid[y][x + dir].filled) {
				std::swap(grid[y][x], grid[y][x + dir]);
			}
		}
	}
}