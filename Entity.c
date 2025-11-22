#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

#include "Entity.h"
#include "events/Event.h"

//-----------------------------------------------------------------------------
Entity_t* entity_create( ENTITY_TYPE type, float x, float y,
                         const char* sprite_path, EntityUpdateFn updateFN,
                         EntityHandleEventFn on_eventFN )
{
   assert( updateFN && "Entity update function must be provided" );
   assert( on_eventFN && "Entity on_event function must be provided" );

   Entity_t* entity = malloc( sizeof( Entity_t ) );
   if ( entity == NULL )
   {
      printf( "Failed to allocate memory for entity\n" );
      return NULL;
   }

   entity->type     = type;
   entity->pos      = ( Vector2 ) { x, y };
   entity->velocity = ( Vector2 ) { 0, 0 };
   entity->texture  = LoadTexture( sprite_path );
   if ( entity->texture.id == 0 )
   {
      TraceLog( LOG_ERROR, "Failed to load texture: %s", sprite_path );
      free( entity );
      return NULL;
   }

   entity->frame_rec =
       ( Rectangle ) { 0.0f, 0.0f, ( float ) entity->texture.width / 13,
                       ( float ) ( entity->texture.height / 54 ) };
   entity->hitbox = ( Rectangle ) { entity->pos.x + 10, entity->pos.y, 20, 40 };
   entity->animation_walk_up_y    = ( entity->texture.height / 54 ) * 8;
   entity->animation_walk_left_y  = ( entity->texture.height / 54 ) * 9;
   entity->animation_walk_down_y  = ( entity->texture.height / 54 ) * 10;
   entity->animation_walk_right_y = ( entity->texture.height / 54 ) * 11;
   entity->current_frame          = 0;
   entity->frames_counter         = 0;
   entity->frames_speed           = 8;

   // Callbacks
   entity->update   = updateFN;
   entity->on_event = on_eventFN;
   ////entity->handle_movement = handle_movement;
   // entity->str             = str;

   return entity;
}

//-----------------------------------------------------------------------------
void entity_free( Entity_t* entity )
{
   if ( entity )
   {
      UnloadTexture( entity->texture );
      free( entity );
   }
}

//-----------------------------------------------------------------------------
void entity_draw( void* entity_ )
{
   Entity_t* entity = ( Entity_t* ) entity_;
   // Maybe DrawTextureRec works maybe not, i dont know what pro parameters are
   DrawTexturePro( entity->texture, entity->frame_rec,
                   ( Rectangle ) { entity->pos.x, entity->pos.y, 40, 40 },
                   ( Vector2 ) { 0, 0 }, 0, WHITE );
   // DrawTextureRec( entity->texture, entity->frame_rec,
   //                 ( Vector2 ) { entity->hitbox.x, entity->hitbox.y }, WHITE
   //                 );
}

//-----------------------------------------------------------------------------
char* entity_default_str( void* entity )
{
   Entity_t* e   = ( Entity_t* ) entity;
   char*     str = malloc( 512 );
   if ( !str )
   {
      TraceLog( LOG_ERROR, "Failed to allocate string" );
      return NULL;
   }
   snprintf(
       str, 256,
       "Entity Debug Info:\n"
       "  Type: %s\n"
       "  Frame Rectangle: { x: %.1f, y: %.1f, width: %.1f, height: %.1f }\n"
       "  Hitbox: { x: %.1f, y: %.1f, width: %.1f, height: %.1f }\n"
       "  Texture: { width: %d, height: %d, id: %u }\n"
       "  Position: { x: %.1f, y: %.1f }\n"
       "  Velocity: { x: %.1f, y: %.1f }\n",
       entity_type_to_string( e->type ), e->frame_rec.x, e->frame_rec.y,
       e->frame_rec.width, e->frame_rec.height, e->hitbox.x, e->hitbox.y,
       e->hitbox.width, e->hitbox.height, e->texture.width, e->texture.height,
       e->texture.id, e->pos.x, e->pos.y, e->velocity.x, e->velocity.y );
   return str;
}

//-----------------------------------------------------------------------------
const char* entity_type_to_string( ENTITY_TYPE type )
{
   switch ( type )
   {
   case ENTITY_TYPE_PLAYER:
      return "PLAYER";
   case ENTITY_TYPE_ENEMY:
      return "ENEMY";
   case ENTITY_TYPE_ITEM:
      return "ITEM";
   case ENTITY_TYPE_STATIC:
      return "STATIC";
   default:
      return "UNKNOWN";
   }
}
