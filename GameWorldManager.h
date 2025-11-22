#pragma once

#include "sandlib.h"
#include "Entity.h"
#include "events/Event.h"

typedef struct
{
   sand_list* entities;
   // World_t world;
   int32_t    num_enemies;
   sand_list* npc_enemies;
} GameWorldManager_t;

//-----------------------------------------------------------------------------
GameWorldManager_t* game_world_manager_create( int32_t num_enemies );

//-----------------------------------------------------------------------------
void game_world_manager_free( GameWorldManager_t* manager );

//-----------------------------------------------------------------------------
void game_world_manager_update_and_draw( GameWorldManager_t* manager );

//-----------------------------------------------------------------------------
void game_world_manager_add_entity( GameWorldManager_t* manager, Entity_t* entity );

//-----------------------------------------------------------------------------
void game_world_manager_handle_collisions( GameWorldManager_t* manager );

//-----------------------------------------------------------------------------
void game_world_manager_resolve_collision_entity_static( GameWorldManager_t* manager, Entity_t* entity );

//-----------------------------------------------------------------------------
void game_world_manager_prepare_manager( GameWorldManager_t* manager );

//-----------------------------------------------------------------------------
void game_world_manager_execute_npc_movements( GameWorldManager_t* manager );


//-----------------------------------------------------------------------------
void game_world_manager_on_event( GameWorldManager_t* manager, Event_t* e );

