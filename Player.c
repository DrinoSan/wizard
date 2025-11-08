#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

#include "Player.h"

//-----------------------------------------------------------------------------
enum DIRECTION
{
   DIRECTION_RIGHT,
   DIRECTION_LEFT,
   DIRECTION_UP,
   DIRECTION_DOWN
};

//-----------------------------------------------------------------------------
static void player_update( void* base_ )
{
   Entity_t* base = ( Entity_t* ) base_;
   base->pos.x += base->velocity.x;
   base->pos.y += base->velocity.y;

   // Need to update the hitbox on each update
   base->hitbox = ( Rectangle ) { base->pos.x + 10, base->pos.y, 20, 40 };
   entity_draw( base );

   // Important to reset otherwise we become buz lightyear
   base->velocity.x = 0;
   base->velocity.y = 0;
}

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
   Entity_t* entity =
       entity_create( type, pos.x, pos.y, sprite_path, player_update );
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
void player_free( Entity_t* entity )
{
   UnloadTexture( entity->texture );

   free( entity );
}

//-----------------------------------------------------------------------------
static void handle_frames( Entity_t* base, enum DIRECTION dir )
{
   base->frames_counter++;
   if ( base->frames_counter >= ( 60 / base->frames_speed ) )
   {
      base->frames_counter = 0;
      base->current_frame++;

      if ( base->current_frame > 8 )
         base->current_frame = 0;

      base->frame_rec.x =
          ( float ) base->current_frame * ( float ) base->texture.width / 13;

      float dirY;
      switch ( dir )
      {
      case DIRECTION_RIGHT:
         dirY = base->animation_walk_right_y;
         break;
      case DIRECTION_LEFT:
         dirY = base->animation_walk_left_y;
         break;
      case DIRECTION_UP:
         dirY = base->animation_walk_up_y;
         break;
      case DIRECTION_DOWN:
         dirY = base->animation_walk_down_y;
         break;
      default:
         assert(false && "Asking you nicely where the fuck you go\n" );
      }

      base->frame_rec.y = dirY;
   }
}

//-----------------------------------------------------------------------------
void player_go_right( Player_t* player )
{
   Entity_t* base = &player->base;
   base->velocity.x += 1.5f;

   handle_frames( base, DIRECTION_RIGHT );
}

//-----------------------------------------------------------------------------
void player_go_left( Player_t* player )
{
   Entity_t* base = &player->base;
   base->velocity.x -= 1.5f;

   handle_frames( base, DIRECTION_LEFT );
}

//-----------------------------------------------------------------------------
void player_go_up( Player_t* player )
{
   Entity_t* base = &player->base;
   base->velocity.y -= 1.5f;

   handle_frames( base, DIRECTION_UP );
}

//-----------------------------------------------------------------------------
void player_go_down( Player_t* player )
{
   Entity_t* base = &player->base;
   base->velocity.y += 1.5f;

   handle_frames( base, DIRECTION_DOWN );
}
