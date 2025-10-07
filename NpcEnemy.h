#pragma once
// System Headers
#include <cstdint>
#include <functional>

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
   bool handleMovement( KeyPressedEvent_t& e ) override;
   bool handleNpcMovement( Player_t* player );
   void onEvent( Event_t& e ) override;
   void registerOnEventCallback( std::function<void( Event_t& )> callback );
   std::function<void( Event_t& )> onEventCallback;

   // PathFinding for npc
   void pathFindingStrategy( /*...*/ ) const
   {
      path->findPath( *this /*...*/ );   // Delegating to strategy class
   }

   // Entity specifics
   void update() override;

   std::unique_ptr<PathFindingStrategy_t> path;
};
