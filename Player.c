#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

#include "Player.h"
#include "events/Event.h"

//-----------------------------------------------------------------------------
enum DIRECTION
{
   DIRECTION_RIGHT,
   DIRECTION_LEFT,
   DIRECTION_UP,
   DIRECTION_DOWN
};

//-----------------------------------------------------------------------------
static bool player_handle_movement( Entity_t* entity, Event_t* event )
{
   KeyPressedEvent_t* key_pressed_event = (KeyPressedEvent_t*)event;
   int key_code = key_pressed_event_get_key_code( key_pressed_event );
   Player_t* player = (Player_t*)entity;

   if ( key_code == KEY_D )
   {
      player_go_right( player );
   }
   else if ( key_code == KEY_A )
   {
      player_go_left( player );
   }
   else if ( key_code == KEY_W )
   {
      player_go_up( player );
   }
   else if ( key_code == KEY_S )
   {
      player_go_down( player );
   }
   else
   {
      printf("[Player.c] [player_handle_movement] unknown key_code %d\n", key_code );
      return false;
   }

   return true;
}

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
static void player_on_event( void* entity_, Event_t* event )
{
   Entity_t* entity = (Entity_t*) entity_;
   EventDispatcher_t dispatcher = event_dispatcher_stack_create( event );
   event_dispatcher_dispatch( &dispatcher, entity, player_handle_movement,
                              EventType_t_KeyPressed );
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
   Entity_t* entity = entity_create( type, pos.x, pos.y, sprite_path,
                                     player_update, player_on_event );
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
         assert( false && "Asking you nicely where the fuck you go\n" );
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
