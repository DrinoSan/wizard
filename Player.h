#pragma once
#include <cstdint>

#include "Globals.h"
#include "Graphic.h"
#include "raylib.h"
#include "events/KeyEvent.h"

#define BIND_EVENT_FN( x )                                                     \
   std::bind( &Player_t::x, this, std::placeholders::_1 )

class Player_t : public Graphic_t
{
 public:
   Player_t();
   Player_t( Vector2 pos );

   virtual ~Player_t() = default;

   // Movements
   // Moves Player and texture rectangle
   void goRight( float movement = PLAYER_MOVEMENT_SPEED );
   void goLeft( float movement = PLAYER_MOVEMENT_SPEED );
   void goUp( float movement = PLAYER_MOVEMENT_SPEED );
   void goDown( float movement = PLAYER_MOVEMENT_SPEED );

   // Moves player but no animation movement
   void moveRight( float movement = PLAYER_MOVEMENT_SPEED );
   void moveLeft( float movement = PLAYER_MOVEMENT_SPEED );
   void moveUp( float movement = PLAYER_MOVEMENT_SPEED );
   void moveDown( float movement = PLAYER_MOVEMENT_SPEED );

   int32_t   ANIMATION_WALK_UP_Y;
   int32_t   ANIMATION_WALK_LEFT_Y;
   int32_t   ANIMATION_WALK_DOWN_Y;
   int32_t   ANIMATION_WALK_RIGHT_Y;
   Rectangle frameRec;
   Texture2D playerTexture;
   Vector2   playerPosition;
   int       currentFrame  = 0;
   int       framesCounter = 0;
   int       framesSpeed = 8;   // Number of spritesheet frames shown by second

   void draw() override;
	bool handleMovement( KeyPressedEvent_t& e ) override;
	void onEvent( Event_t& e ) override;
};
