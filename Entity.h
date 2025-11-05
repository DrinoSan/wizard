#pragma once

#include "stdint.h"

#include "raylib.h"

typedef enum
{
   ENTITY_TYPE_PLAYER,
   ENTITY_TYPE_STATIC,
   ENTITY_TYPE_ENEMY,
   ENTITY_TYPE_NPC,
   ENTITY_TYPE_ITEM,
} ENTITY_TYPE;

//-----------------------------------------------------------------------------
typedef struct
{
   ENTITY_TYPE type;
   Vector2          pos;
   Vector2          velocity;
   Texture2D        texture;
   Rectangle        frame_rec;
   Rectangle        hitbox;
   int32_t          attack_range;
   int32_t          attack_speed;
   int32_t          attack_power;

   // Animation stuff
   float animation_walk_up_y;
   float animation_walk_left_y;
   float animation_walk_down_y;
   float animation_walk_right_y;
   int   current_frame;
   int   frames_counter;
   int   frames_speed;

   // EntityUpdateFn      update;
   // EntityDrawFn        draw;
   // EntityHandleEventFn on_event;
   // EntityStrFn str;
} Entity_t;

//-----------------------------------------------------------------------------
// Entity_t* entity_create( EntityType type, float x, float y,
//                         const char* sprite_path, EntityUpdateFn update,
//                         EntityDrawFn draw, EntityHandleEventFn on_event,
//                         // EntityHandleMovementFn handle_movement,
//                         EntityStrFn str );
Entity_t* entity_create( ENTITY_TYPE type, float x, float y,
                         const char* sprite_path );

//-----------------------------------------------------------------------------
void entity_free( Entity_t* entity );

//-----------------------------------------------------------------------------
void entity_draw( void* entity_ );

//-----------------------------------------------------------------------------
/// void to be able to call it with player/npc and all others having entity as
/// base
char* entity_default_str( void* entity );

//-----------------------------------------------------------------------------
const char* entity_type_to_string( ENTITY_TYPE type );
