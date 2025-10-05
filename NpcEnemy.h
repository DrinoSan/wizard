#pragma once
#include <cstdint>
#include <functional>

#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "events/KeyEvent.h"
#include "raylib.h"

class NpcEnemy_t : public Graphic_t, public Entity_t
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

   int32_t ANIMATION_WALK_UP_Y;
   int32_t ANIMATION_WALK_LEFT_Y;
   int32_t ANIMATION_WALK_DOWN_Y;
   int32_t ANIMATION_WALK_RIGHT_Y;
   int     currentFrame  = 0;
   int     framesCounter = 0;
   int     framesSpeed   = 16;   // Number of spritesheet frames shown by second

   void draw() override;
   bool handleMovement( KeyPressedEvent_t& e ) override;
   bool handleNpcMovement();
   void onEvent( Event_t& e ) override;
   void registerOnEventCallback( std::function<void( Event_t& )> callback );
   std::function<void( Event_t& )> onEventCallback;

   // Entity specifics
   void update() override;
};
