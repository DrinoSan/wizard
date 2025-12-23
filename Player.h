#pragma once
#include <cstdint>

#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "events/KeyEvent.h"
#include "raylib.h"

class Player_t : public Graphic_t, public Entity_t
{
 public:
   Player_t();
   Player_t( Vector2 pos );

   virtual ~Player_t() = default;

   void draw() override;
   void onEvent( Event_t& e ) override;
   bool handleKeyEvent( KeyPressedEvent_t& e );

   // Entity specifics
   void update( float dt ) override;
   void updateAnimation( float dt );

 private:
   float   animTimer     = 0.0f;
   Vector2 lastDirection = { 0.0f, 1.0f };
};
