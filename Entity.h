#pragma once

// Project Header
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "raylib.h"

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

   virtual void        update()                               = 0;
   virtual void        onEvent( Event_t& e )                  = 0;
   virtual bool        handleMovement( KeyPressedEvent_t& e ) = 0;
   virtual std::string str();

   ENTITY_TYPE type;
   Rectangle   frameRec;
   Rectangle   hitbox;
   Texture2D   playerTexture;
   Vector2     playerPosition;
   Vector2     velocity;

   // Animation stuff
   int32_t ANIMATION_WALK_UP_Y;
   int32_t ANIMATION_WALK_LEFT_Y;
   int32_t ANIMATION_WALK_DOWN_Y;
   int32_t ANIMATION_WALK_RIGHT_Y;
   int     currentFrame  = 0;
   int     framesCounter = 0;
   int     framesSpeed   = 16;   // Number of spritesheet frames shown by second


   // Player/NPC related stuff
   int32_t attackRange;
   int32_t attackSpeed;
   int32_t attackPower;
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
