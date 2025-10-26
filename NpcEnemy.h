#pragma once
// System Headers
#include <cstdint>
#include <functional>

// Raylib Headers
#include "raylib.h"
#include "raymath.h"

// Project Headers
#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "PathFindingStrategy.h"
#include "Player.h"
#include "events/KeyEvent.h"

typedef void ( *onEventCallbackFn )( Event_t* e );

typedef struct
{
   Entity_t          base;
   onEventCallbackFn npc_enemy_event_callback;
} NpcEnemy_t;

NpcEnemy_t* npc_create();
void        npc_free( NpcEnemy_t* npc );

bool npc_enemy_handle_movement( NpcEnemy_t* npc, Player_t* player );

void npc_enemy_register_on_event_callback( NpcEnemy_t* npc, onEventCallbackFn );
