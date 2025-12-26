#pragma once
#include <array>
#include <cstdint>

#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "World.h"
#include "events/KeyEvent.h"
#include "raylib.h"

// ----------------------------------------------------------------------------
class Player_t : public Graphic_t, public Entity_t
{
 public:
   Player_t() = delete;
   Player_t( World_t& world );

   virtual ~Player_t();

   bool       handleKeyEvent( KeyPressedEvent_t& e );
   void       updateAnimation( float dt );
   void       updateAttacks( float dt );
   Texture2D& getAttackTexture( AttackType type );
   void       castAttack();

   // IMGUI
   void drawAttackDebugInfo();

   // Entity specifics
   void draw() override;
   void update( float dt ) override;
   void onEvent( Event_t& e ) override;

 private:
   float   animTimer     = 0.0f;
   Vector2 lastDirection = { 0.0f, 1.0f };

};
