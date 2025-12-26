#pragma once

// Project Header
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "raylib.h"
#include "raymath.h"

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

//-----------------------------------------------------------------------------
enum class ENTITY_TYPE
{
   PLAYER,
   STATIC,
   ENEMY,
   NPC,
   ITEM,
};

//-----------------------------------------------------------------------------
enum class ENTITY_STATE
{
   ACTIVE,
   IDLE,
   BURNING,
   FROZEN
};

//-----------------------------------------------------------------------------
inline std::string entityStateToString( ENTITY_STATE state )
{
   switch ( state )
   {
   case ENTITY_STATE::ACTIVE:
      return "ACTIVE";
   case ENTITY_STATE::IDLE:
      return "IDLE";
   case ENTITY_STATE::BURNING:
      return "BURNING";
   case ENTITY_STATE::FROZEN:
      return "FROZEN";
   default:
      return "UNKNOWN";
   }
}

//-----------------------------------------------------------------------------
inline std::string entityTypeToString( ENTITY_TYPE type )
{
   switch ( type )
   {
   case ENTITY_TYPE::PLAYER:
      return "PLAYER";
   case ENTITY_TYPE::ENEMY:
      return "ENEMY";
   case ENTITY_TYPE::ITEM:
      return "item";
   case ENTITY_TYPE::STATIC:
      return "item";
   default:
      return "UNKNOWN";
   }
}

//#define BIND_EVENT_FN( x )                                                     \
//   std::bind( &this::x, this, std::placeholders::_1 )
//-----------------------------------------------------------------------------
#define BIND_EVENT_FN( fn ) std::bind( &fn, this, std::placeholders::_1 )

//-----------------------------------------------------------------------------
class Entity_t
{
 public:
   Entity_t()          = default;
   virtual ~Entity_t() = default;

   virtual void        update( float dt )    = 0;
   virtual void        draw()                = 0;
   virtual void        onEvent( Event_t& e ) = 0;
   virtual std::string str();

 protected:
   void drawLifeBar() const;

 public:
   ENTITY_TYPE  type;
   ENTITY_STATE state;
   Rectangle    frameRec;
   Rectangle    hitbox;
   Texture2D    playerTexture;
   Vector2      playerPosition;
   Vector2      velocity;
   Vector2      lastVelocity;   // Used for logging
   Vector2      hitboxOffset{ 10, 0 };
   float        frameWidth = 0.0f;
   const float  SPEED      = 100.0f;

   // Animation stuff
   int32_t ANIMATION_WALK_UP_Y;
   int32_t ANIMATION_WALK_LEFT_Y;
   int32_t ANIMATION_WALK_DOWN_Y;
   int32_t ANIMATION_WALK_RIGHT_Y;

   int32_t ANIMATION_IDLE_UP_Y;
   int32_t ANIMATION_IDLE_LEFT_Y;
   int32_t ANIMATION_IDLE_DOWN_Y;
   int32_t ANIMATION_IDLE_RIGHT_Y;

   int currentFrame  = 0;
   int framesCounter = 0;
   int framesSpeed   = 10;   // Number of spritesheet frames shown by second

   // Player/NPC related stuff
   std::array<Texture2D, 4> attackTextures{};
   std::vector<Attack>      activeAttacks;   // Max 20-50 for perf
   float                    FIRE_RATE{ 0.5f };
   float                    fireCooldown{ 0.0f };
   AttackType               attackType;

   int32_t lifePoints{ 100 };
   int32_t manaPoints{ 100 };
   int32_t stamina{ 100 };
   int32_t armor{ 0 };

   int32_t attackRange{ 10 };
   float   attackSpeed{ FIRE_RATE };
   int32_t attackPower{ 1 };
};

//-----------------------------------------------------------------------------
inline std::string Entity_t::str()
{
   std::stringstream ss;
   ss << "Entity Debug Info:\n";
   ss << "  Type: " << entityTypeToString( type ) << "\n";
   ss << "  Frame Rectangle: { x: " << frameRec.x << ", y: " << frameRec.y
      << ", width: " << frameRec.width << ", height: " << frameRec.height
      << " }\n";
   ss << "  Hitbox: { x: " << hitbox.x << ", y: " << hitbox.y
      << ", width: " << hitbox.width << ", height: " << hitbox.height << " }\n";
   ss << "  Texture: { width: " << playerTexture.width
      << ", height: " << playerTexture.height << ", id: " << playerTexture.id
      << " }\n";
   ss << "  Position: { x: " << playerPosition.x << ", y: " << playerPosition.y
      << " }\n";
   ss << "  Velocity: { x: " << velocity.x << ", y: " << velocity.y << " }\n";
   return ss.str();
}
