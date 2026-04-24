#pragma once

#include "Cell.h"
#include <vector>

inline constexpr float MAX_MASS = 1.f;
inline constexpr float MAX_COMPRESS = 0.02f;
inline constexpr float MIN_FLOW = 0.001f;
inline constexpr float GRAVITY = 0.5f;
inline constexpr float DAMPING = 0.98f;
inline constexpr float MAX_VELOCITY = 5.f;

inline constexpr float IGNITION_TEMP = 100.f;
inline constexpr float BURN_RATE = 0.5f;
inline constexpr float HEAT_SPREAD = 5.f;
inline constexpr float FIRE_LIFETIME_MASS = 0.f;

inline constexpr float SMOKE_RISE_RATE = 0.5f;
inline constexpr float SMOKE_DISSIPATION_RATE = 0.010f;

void fluidCellUpdate(int x, int y, std::vector<std::vector<Cell>>& grid);
void updateSandPhysics(std::vector<std::vector<Cell>>& grid);
void updateFluidPhysics(std::vector<std::vector<Cell>>& grid);
void fireCellUpdate(int x, int y, std::vector<std::vector<Cell>>& grid);
void updateFirePhysics(std::vector<std::vector<Cell>>& grid);
void updateSmokePhysics(std::vector<std::vector<Cell>>& grid);
void handleLavaInteractions(int x, int y, std::vector<std::vector<Cell>>& grid);
