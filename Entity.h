#pragma once

#include "stdint.h"

#include "raylib.h"
#include "EntityDef.h"


//-----------------------------------------------------------------------------
// Entity_t* entity_create( EntityType type, float x, float y,
//                         const char* sprite_path, EntityUpdateFn update,
//                         EntityDrawFn draw, EntityHandleEventFn on_event,
//                         // EntityHandleMovementFn handle_movement,
//                         EntityStrFn str );
Entity_t* entity_create( ENTITY_TYPE type, float x, float y,
                         const char* sprite_path, EntityUpdateFn updateFN, EntityHandleEventFn on_eventFN );

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
