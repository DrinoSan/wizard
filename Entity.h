#pragma once

// Project Header
#include "raylib.h"
#include "events/Event.h"
#include "events/KeyEvent.h"

//-----------------------------------------------------------------------------
enum class ENTITY_TYPE
{
   PLAYER,
   STATIC,
   NPC,
};

//-----------------------------------------------------------------------------
class Entity_t
{
 public:
   Entity_t()          = default;
   virtual ~Entity_t() = default;

   virtual void update()                               = 0;
   virtual void onEvent( Event_t& e )                  = 0;
   virtual bool handleMovement( KeyPressedEvent_t& e ) = 0;
   ENTITY_TYPE  type;

   Rectangle frameRec;
   Rectangle hitbox;
   Texture2D playerTexture;
   Vector2   playerPosition;
   Vector2   velocity;
};
