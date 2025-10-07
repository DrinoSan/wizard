#pragma once
#include <cstdint>

#include "Globals.h"
#include "Entity.h"
#include "Graphic.h"
#include "raylib.h"
#include "events/KeyEvent.h"


class Player_t : public Graphic_t, public Entity_t
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

   void draw() override;
	bool handleMovement( KeyPressedEvent_t& e ) override;
	void onEvent( Event_t& e ) override;

   // Entity specifics
   void update() override;
};
