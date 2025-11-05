#pragma once

#include "Entity.h"

typedef struct
{
   Entity_t base;
} Player_t;

//-----------------------------------------------------------------------------
Player_t* player_create( ENTITY_TYPE type, float x, float y,
                           const char* sprite_path );

//-----------------------------------------------------------------------------
void player_free( Entity_t* entity );

//-----------------------------------------------------------------------------
void player_go_Right( Player_t* player );
void player_go_Left( Player_t* player );
void player_go_Up( Player_t* player );
void player_go_Down( Player_t* player );
