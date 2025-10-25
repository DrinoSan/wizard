// System headers
#include <iostream>

// Project Headers
#include "PathFindingStrategy.h"

//-----------------------------------------------------------------------------
void A_StarStrategy_t::findPath( const NpcEnemy_t& npc, Vector2 playerPosition,
                                 const Grid_t& grid, Vector2 playerGridPosition, Vector2 npcGridPosition ) const
{
   std::cout << "A* algorithm\n";
   // Find on which gridElement the NPC and player are

   std::vector<GridElement_t> open;
   std::vector<GridElement_t> visited;

   open.push_back(grid.gridMap[npcGridPosition.y][npcGridPosition.x]);




}
