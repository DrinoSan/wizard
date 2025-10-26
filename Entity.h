#pragma once

// Project Header
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "raylib.h"

#define SPRITE_PATH "spritesheets/wizard01.png"

//-----------------------------------------------------------------------------
enum class ENTITY_TYPE
{
   PLAYER,
   STATIC,
   ENEMY,
   NPC,
   ITEM,
};

typedef enum
{
   ENTITY_TYPE_PLAYER,
   ENTITY_TYPE_NPC,
   ENTITY_TYPE_STATIC,
   ENTITY_TYPE_ITEM,
   ENTITY_TYPE_ENEMY
} ENTITY_TYPE;

typedef void ( *EntityUpdateFn )( void* entity );
typedef void ( *EntityDrawFn )( void* entity );
typedef int ( *EntityHandleEventFn )( void* entity, Event* event );
// typedef int ( *EntityHandleMovementFn )( void* entity, Event* event );
typedef char* ( *EntityStrFn )( void* entity );

//#define BIND_EVENT_FN( x )                                                     \
//   std::bind( &this::x, this, std::placeholders::_1 )
//-----------------------------------------------------------------------------
#define BIND_EVENT_FN( fn ) std::bind( &fn, this, std::placeholders::_1 )

//-----------------------------------------------------------------------------
typedef struct
{
   EntityType type;
   Vector2    pos;
   Vector2    velocity;
   Texture2D  texture;
   Rectangle  frame_rec;
   Rectangle  hitbox;
   int32_t    attack_range;
   int32_t    attack_speed;
   int32_t    attack_power;

   // Animation stuff
   float animation_walk_up_y;
   float animation_walk_left_y;
   float animation_walk_down_y;
   float animation_walk_right_y;
   int   current_frame;
   int   frames_counter;
   int   frames_speed;

   EntityUpdateFn      update;
   EntityDrawFn        draw;
   EntityHandleEventFn on_event;
   // EntityHandleMovementFn handle_movement;
   EntityStrFn str;
} Entity_t;

//-----------------------------------------------------------------------------
Entity_t* entity_create( EntityType type, float x, float y,
                         const char* sprite_path, EntityUpdateFn update,
                         EntityDrawFn draw, EntityHandleEventFn on_event,
                         // EntityHandleMovementFn handle_movement,
                         EntityStrFn str );

//-----------------------------------------------------------------------------
void entity_free( Entity_t* entity );

//-----------------------------------------------------------------------------
void entity_draw( Entity_t* entity );

//-----------------------------------------------------------------------------
/// void to be able to call it with player/npc and all others having entity as
/// base
char* entity_default_str( void* entity );

//-----------------------------------------------------------------------------
const char* entity_type_to_string( EntityType type );
