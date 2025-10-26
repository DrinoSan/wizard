#pragma once
#include <cstdint>

#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "events/KeyEvent.h"
#include "raylib.h"

typedef struct
{
   Entity_t entity;
} Player_t;

//-----------------------------------------------------------------------------
Player_t* Player_t_create( EntityType type, float x, float y,
                           const char* sprite_path, EntityUpdateFn update,
                           EntityHandleEventFn    on_event,
                           EntityHandleMovementFn handle_movement,
                           EntityStrFn            str );

//-----------------------------------------------------------------------------
void player_free( Entity_t* entity );

//-----------------------------------------------------------------------------
void player_go_Right( Player_t* player, float movement = PLAYER_MOVEMENT_SPEED );
void player_go_Left( Player_t* player, float movement = PLAYER_MOVEMENT_SPEED );
void player_go_Up( Player_t* player, float movement = PLAYER_MOVEMENT_SPEED );
void player_go_Down( Player_t* player, float movement = PLAYER_MOVEMENT_SPEED );

