#include "stdio.h"
#include "stdlib.h"

#include "Player.h"

//-----------------------------------------------------------------------------
Player_t* player_create( ENTITY_TYPE type, float x, float y,
                         const char* sprite_path )
{
   Player_t* player = malloc( sizeof( Player_t ) );
   if ( player == NULL )
   {
      printf( "Could not allocate space for Player\n" );
      return NULL;
   }

   Vector2 pos = { x, y };
   // Entity_t* entity = entity_create( ENTITY_TYPE_PLAYER, pos.x, pos.y,
   // sprite_path );
   Entity_t* entity = entity_create( type, pos.x, pos.y, sprite_path );
   if ( entity == NULL )
   {
      free( player );
      return NULL;
   }

   player->base = *entity;
   free( entity );

   return player;
}

//-----------------------------------------------------------------------------
void player_free( Entity_t* entity );

//-----------------------------------------------------------------------------
void player_go_Right( Player_t* player ) {}
void player_go_Left( Player_t* player ) {}
void player_go_Up( Player_t* player ) {}
void player_go_Down( Player_t* player ) {}
