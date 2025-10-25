#pragma once

#include "World.h"
#include "raylib.h"

//-----------------------------------------------------------------------------
// Forward declaration
class NpcEnemy_t;

//-----------------------------------------------------------------------------
class PathFindingStrategy_t
{
 public:
   PathFindingStrategy_t()          = default;
   virtual ~PathFindingStrategy_t() = default;

   // PathFinding virtual to be used as strategy pattern
   virtual void findPath( const NpcEnemy_t& npc,  Vector2 playerPosition, const Grid_t& grid, Vector2 playerGridPosition, Vector2 npcGridPosition  ) const = 0;
};

//-----------------------------------------------------------------------------
class A_StarStrategy_t : public PathFindingStrategy_t
{
 public:
   virtual ~A_StarStrategy_t() {}

   void findPath( const NpcEnemy_t& npc, Vector2 playerPosition, const Grid_t& grid, Vector2 playerGridPosition, Vector2 npcGridPosition  ) const override;
};
