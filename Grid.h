#pragma once

// System Header
#include <unordered_map>
#include <vector>

// Project Headers
#include "Entity.h"
#include "Globals.h"

struct GridCell_t
{
   std::vector<Entity_t*>                       entities;
   std::vector<std::pair<Entity_t*, Attack_t*>> attacks;
};

class Grid_t
{
 public:
   // Helper
   inline int32_t getCellIndex( Vector2 pos ) const
   {
      // @TODO: check if clamp is needed because if its outside map it should
      // not really be counted
      int32_t col =
          Clamp( ( int32_t ) ( pos.x / GRID_CELL_WIDTH ), 0, GRID_COLS - 1 );
      int32_t row =
          Clamp( ( int32_t ) ( pos.y / GRID_CELL_HEIGHT ), 0, GRID_ROWS - 1 );
      return row * GRID_COLS + col;
   }

   std::unordered_map<int32_t, GridCell_t> collisionGrid;   // Key: cell index
 private:
   static constexpr int32_t GRID_CELL_WIDTH  = 240;   // 960 / 4
   static constexpr int32_t GRID_CELL_HEIGHT = 160;   // 640 / 4
   static constexpr int32_t GRID_COLS = WORLD_WIDTH / GRID_CELL_WIDTH;     // 4
   static constexpr int32_t GRID_ROWS = WORLD_HEIGHT / GRID_CELL_HEIGHT;   // 4
};
