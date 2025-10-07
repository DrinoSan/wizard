#pragma once
// System Headers
#include <cstdint>
#include <functional>

// Raylib Headers
#include "raylib.h"
#include "raymath.h"

// Project Headers
#include "Entity.h"
#include "Player.h"
#include "Globals.h"
#include "Graphic.h"
#include "events/KeyEvent.h"
#include "PathFinding.h"

class NpcEnemy_t : public Graphic_t, public Entity_t, public PathFinding_t
{
 public:
   NpcEnemy_t();
   NpcEnemy_t( Vector2 pos );

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

   // PathFinding

   void pathFindingStrategy( /*...*/ ) const override
   {}

   // Entity specifics
   void update() override;
};
