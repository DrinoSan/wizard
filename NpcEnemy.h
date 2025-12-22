#pragma once
// System Headers
#include <cstdint>
#include <functional>
#include <vector>

// Raylib Headers
#include "raylib.h"
#include "raymath.h"

// Project Headers
#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "PathFindingStrategy.h"
#include "Player.h"
#include "events/KeyEvent.h"

class NpcEnemy_t : public Graphic_t, public Entity_t
{
 public:
   explicit NpcEnemy_t( std::unique_ptr<PathFindingStrategy_t> pathFinding );
   explicit NpcEnemy_t( Vector2 pos );

   virtual ~NpcEnemy_t() = default;

   // Movements
   // Moves Player and texture rectangle
   void goRight( float movement = PLAYER_MOVEMENT_SPEED );
   void goLeft( float movement = PLAYER_MOVEMENT_SPEED );
   void goUp( float movement = PLAYER_MOVEMENT_SPEED );
   void goDown( float movement = PLAYER_MOVEMENT_SPEED );

   void draw() override;
   bool handleMovement( KeyPressedEvent_t& e );
   bool handleNpcMovement( World_t& world, Player_t* player );
   void onEvent( Event_t& e ) override;
   void registerOnEventCallback( std::function<void( Event_t& )> callback );
   std::function<void( Event_t& )> onEventCallback;

   // PathFinding for npc
   void pathFindingStrategy( const class World_t& world,
                             const Player_t&      player ) const
   {
      path->findPath( const_cast<NpcEnemy_t&>( *this ), world, player );
   }

   // Cached path produced by the strategy as tile indices (world.worldMap
   // indexes)
   std::vector<int> pathIndices;
   int              pathCursor{ 0 };

   // Entity specifics
   void update( float dt ) override;

   std::unique_ptr<PathFindingStrategy_t> path;
};
