#include "Player.h"
#include "events/KeyEvent.h"

//-----------------------------------------------------------------------------
Player_t* player_create()
{
   Player_t* player = malloc( sizeof( Player_t ) );
   if ( player == NULL )
   {
      printf( "Could not allocate space for Player\n" );
      return NULL;
   }

   Vector2 pos = { ( float ) screenWidth / 2, ( float ) screenHeight / 2 };
   Entity* entity = entity_create( ENTITY_TYPE_PLAYER, pos.x, pos.y, SPRITE_PATH,
                                   player_update, player_draw, player_on_event, player_str );

   if ( !entity )
   {
      free( player );
      return NULL;
   }

   player->base = *entity;

   free( entity );
   return player;
}

//-----------------------------------------------------------------------------
void player_free( Player_t* player )
{
   if ( player )
   {
      entity_free( ( Entity* ) player );
   }
}

//-----------------------------------------------------------------------------
void player_go_Right( Player_t* player, float movement )
{
   // playerPosition.x += movement;
   player->base.velocity.x += movement;

   if ( player->base.frames_counter >= ( 60 / player->base.frames_speed ) )
   {
      player->base.frames_counter = 0;
      player->base.current_frame++;

      if ( player->base.current_frame > 8 )
         player->base.current_frame = 0;

      player->base.frame_rec.x = ( float ) player->base.current_frame *
                                 ( float ) player->base.texture.width / 13;
      frame_rec.y = ANIMATION_WALK_RIGHT_Y;
   }
}

//-----------------------------------------------------------------------------
void player_go_Left( Player_t* player, float movement )
{
   // playerPosition.x -= movement;
   player->base.velocity.x -= movement;

   if ( player->base.frames_counter >= ( 60 / player->base.frames_speed ) )
   {
      player->base.frames_counter = 0;
      player->base.current_frame++;

      if ( player->base.current_frame > 8 )
         player->base.current_frame = 0;

      player->base.frame_rec.x = ( float ) player->base.current_frame *
                                 ( float ) player->base.texture.width / 13;
      player->base.frame_rec.y = ANIMATION_WALK_LEFT_Y;
   }
}

//-----------------------------------------------------------------------------
void player_go_Up( Player_t* player, float movement )
{
   // playerPosition.y -= movement;
   player->base.velocity.y -= movement;

   if ( player->base.frames_counter >= ( 60 / player->base.frames_speed ) )
   {
      player->base.frames_counter = 0;
      player->base.current_frame++;

      if ( player->base.current_frame > 8 )
         player->base.current_frame = 0;

      player->base.frame_rec.x = ( float ) player->base.current_frame *
                                 ( float ) player->base.texture.width / 13;
      player->base.frame_rec.y = ANIMATION_WALK_UP_Y;
   }
}

//-----------------------------------------------------------------------------
void player_go_Down( Player_t* player, float movement )
{
   // playerPosition.y += movement;
   player->base.velocity.y += movement;

   if ( player->base.frames_counter >= ( 60 / player->base.frames_speed ) )
   {
      player->base.frames_counter = 0;
      player->base.current_frame++;

      if ( player->base.current_frame > 8 )
         player->base.current_frame = 0;

      player->base.frame_rec.x = ( float ) player->base.current_frame *
                                 ( float ) player->base.texture.width / 13;
      player->base.frame_rec.y = ANIMATION_WALK_DOWN_Y;
   }
}

//-----------------------------------------------------------------------------
static void player_draw( void* player )
{
   Player_t* p = ( Player_t* ) player;
   // DrawTextureRec( texture, frame_rec, playerPosition,
   // WHITE );   // Draw part of the texture
   DrawTexturePro( p->base.texture, p->base.frame_rec,
                   { p->base.pos.x, p->base.pos.y, 40, 40 }, { 0, 0 }, 0,
                   WHITE );
#ifdef DEBUG
   DrawRectangleLines( p->base.pos.x + 10, p->base.pos.y, 20, 40, RED );
#endif

   // Important to reset otherwise we become buz lightyear
   p->base.velocity.x = 0;
   p->base.velocity.y = 0;
}

//-----------------------------------------------------------------------------
static void player_on_event( Player_t* player, Event_t* e )
{
   EventDispatcher_t dispatcher;
   event_dispatcher_init( &dispatcher, e );

   event_dispatcher_dispatch( &dispatcher, EVENT_KEY_PRESSED,
                              player_handle_movement, player );
}

//-----------------------------------------------------------------------------
static bool player_handle_movement( void* event, void* player )
{
   Player_t*        p = ( Player_t* ) player;
   KeyPressedEvent* e = ( KeyPressedEvent* ) event;
   printf( "Player handling KeyPressedEvent: key_code=%d\n", e->key_code );

   p->base.frames_counter++;
   if ( e->key_code == KEY_D )
   {
      player_go_right( p );
   }
   if ( e->key_code == KEY_A )
   {
      player_go_left( p );
   }
   if ( e->key_code == KEY_W )
   {
      player_go_up( p );
   }
   if ( e->key_code == KEY_S )
   {
      player_go_down( p );
   }

   return true;
}

//-----------------------------------------------------------------------------
static char* player_str( void* player )
{
   Player* p   = ( Player* ) player;
   char*   str = malloc( 256 );
   if ( !str )
      return NULL;
   snprintf(
       str, 256,
       "Player Debug Info:\n"
       "  Type: %s\n"
       "  Frame Rectangle: { x: %.1f, y: %.1f, width: %.1f, height: %.1f }\n"
       "  Hitbox: { x: %.1f, y: %.1f, width: %.1f, height: %.1f }\n"
       "  Texture: { width: %d, height: %d, id: %u }\n"
       "  Position: { x: %.1f, y: %.1f }\n"
       "  Velocity: { x: %.1f, y: %.1f }\n"
       "  Current Frame: %d\n",
       entity_type_to_string( p->base.type ), p->base.frame_rec.x,
       p->base.frame_rec.y, p->base.frame_rec.width, p->base.frame_rec.height,
       p->base.hitbox.x, p->base.hitbox.y, p->base.hitbox.width,
       p->base.hitbox.height, p->base.texture.width, p->base.texture.height,
       p->base.texture.id, p->base.pos.x, p->base.pos.y, p->base.velocity.x,
       p->base.velocity.y, p->base.current_frame );
   return str;
}

//-----------------------------------------------------------------------------
static void player_update( void* p )
{
   Player_t* player = ( Player_t* ) p;
   player->base.pos.x += player->base.velocity.x;
   player->base.pos.y += player->base.velocity.y;

   // Need to update the hitbox on each update
   player->base.hitbox = { player->base.pos.x + 10, player->base.pos.y, 20,
                           40 };
   // player_draw( player );

   // Important to reset otherwise we become buz lightyear
   // This should be done in the draw function
   // player->base.velocity.x = 0;
   // player->base.velocity.y = 0;
}
