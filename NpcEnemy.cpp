#include <iostream>

#include "NpcEnemy.h"
#include "events/KeyEvent.h"

//-----------------------------------------------------------------------------
NpcEnemy_t* npc_enemy_create()
{
   NpcEnemy_t* npc = malloc( sizeof( NpcEnemy_t ) );
   if ( npc == NULL )
   {
      printf( "Could not allocate space for NPC\n" );
   }

   Vector2 pos = { ( float ) screenWidth / 2, ( float ) screenHeight / 2 };
   Entity* entity =
       entity_create( ENTITY_TYPE_ENEMY, pos.x + 20, pos.y + 20, SPRITE_PATH,
                      npc_enemy_update, npc_enemy_draw, npc_enemy_on_event, npc_enemy_str );

   if ( !entity )
   {
      free( npc );
      return NULL;
   }

   npc->base = *entity;

   free( entity );
   return npc;
}

//-----------------------------------------------------------------------------
void npc_enemy_free( NpcEnemy_t* npc )
{
   if ( npc )
   {
      entity_free( ( Entity* ) npc );
   }
}

//-----------------------------------------------------------------------------
bool npc_enemy_handle_movement( NpcEnemy_t* npc, Player_t* player )
{
   if ( npc == NULL || player == NULL )
   {
      printf( "Npc or player or both are NULL\n" );
      return false;
   }

   npc->frames_counter++;

   // Get Richtungsvektor
   // Get normilized vektor
   // scale it for velocity
   Vector2 directionVec  = Vector2Subtract( player->pos, npc->pos );
   Vector2 normalizedVec = Vector2Normalize( directionVec );

   npc->velocity.x += normalizedVec.x * 1.5;
   npc->velocity.y += normalizedVec.y * 1.5;

   if ( npc->velocity.x > 0 )
   {
      npc->npc_enemy_go_right();
   }
   if ( npc->velocity.x < 0 )
   {
      npc->npc_enemy_go_left();
   }
   if ( npc->velocity.y < 0 )
   {
      npc->npc_enemy_go_up();
   }
   if ( npc->velocity.y > 0 )
   {
      npc->npc_enemy_go_down();
   }

   // pathFindingStrategy();

   return true;
}

//-----------------------------------------------------------------------------
static void npc_enemy_on_event( NpcEnemy_t* npc, Event_t* e )
{
   printf( "Called npc_enemy_on_event\n" );
   // EventDispatcher_t dispatcher;
   // event_dispatcher_init( &dispatcher, e );

   // event_dispatcher_dispatch( &dispatcher, EVENT_KEY_PRESSED,
   //                            player_handle_movement, npc );
}

//-----------------------------------------------------------------------------
static void npc_enemy_draw( NpcEnemy_t* npc_enemy )
{
   NpcEnemy_t* npc = (NpcEnemy_t*)npc_enemy;
   // DrawTextureRec( playerTexture, frameRec, playerPosition,
   // WHITE );   // Draw part of the texture
   DrawTexturePro( npc->base.texture, npc->base.frame_rec,
                   { npc->base.pos.x, npc->base.pos.y, 40, 40 }, { 0, 0 }, 0,
                   WHITE );
#ifdef DEBUG
   DrawRectangleLines( npc->base.pos.x + 10, npc->base.pos.y, 20, 40, RED );
#endif

   npc->base.velocity.x = 0;
   npc->base.velocity.y = 0;
}

//-----------------------------------------------------------------------------
static void npc_enemy_update( void* npc_enemy )
{
   NpcEnemy_t* npc = (NpcEnemy_t*)npc_enemy;
   npc->base.pos.x += npc->base.velocity.x;
   npc->base.pos.y += npc->base.velocity.y;

   // Need to update the hitbox on each update
   npc->base.hitbox = { npc->base.pos.x + 10, npc->base.pos.y, 20, 40 };
   //npc->npc_enemy_draw( npc );

   // Important to reset otherwise we become buz lightyear
   // velocity.x = 0;
   // velocity.y = 0;
}

//-----------------------------------------------------------------------------
void npc_enemy_register_on_event_callback( NpcEnemy_t* npc, onEventCallbackFn )
{
   npc->onEventCallbackFn = onEventCallbackFn;
}

