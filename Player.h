#pragma once
#include <array>
#include <cstdint>

#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "events/KeyEvent.h"
#include "raylib.h"

// ----------------------------------------------------------------------------
// For now i only add 4
enum class AttackType : std::uint8_t
{
   LIGHTNING = 0,
   FIRE      = 1,
   WATER     = 2,
   EARTH     = 3
};

// ----------------------------------------------------------------------------
struct Attack
{
   AttackType type;
   Vector2    position;
   Vector2    velocity;
   Rectangle  sourceRec;   // For animation frames
   float      timer;       // For anim speed
   int        currentFrame;
   int        maxFrames;
   float      rotation;
   bool       active = false;
};

// ----------------------------------------------------------------------------
class Player_t : public Graphic_t, public Entity_t
{
 public:
   Player_t();
   Player_t( Vector2 pos );

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

   std::array<Texture2D, 4> attackTextures{};
   std::vector<Attack>      activeAttacks;   // Max 20-50 for perf
   float                    FIRE_RATE{ 0.5f };
   float                    fireCooldown{ 0.0f };
};
