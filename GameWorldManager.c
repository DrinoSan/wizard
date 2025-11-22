
#include "stdlib.h"

#include "GameWorldManager.h"
#include "Player.h"

//-----------------------------------------------------------------------------
GameWorldManager_t* game_world_manager_create( int32_t num_enemies )
{
   GameWorldManager_t* manager = calloc( 1, sizeof( GameWorldManager_t ) );
   manager->entities           = sand_list_create( sizeof( Entity_t* ) );
   // manager->npcEnemies = sand_list_create( sizeof( NpcEnemy_t* ) );

   manager->num_enemies = num_enemies;

   return manager;
}

//-----------------------------------------------------------------------------
void game_world_manager_free( GameWorldManager_t* manager )
{
   printf( "[GameWorldManager] - Destructing GameWorldManager\n" );

   // For loop over all list elements and call desctructor
   for ( int32_t i = 0; i < manager->entities->size; i++ )
   {
      Entity_t* entity_p = *( Entity_t** ) sand_list_at( manager->entities, i );
      player_free( entity_p );
   }
   sand_list_free( manager->entities );

   // @TODO: When i add npc enemies i need to delete them correctyl
   // for( int32_t i = 0; i < manager->npc_enemies->size; i++ )
   // {
   //    Entity_t* entity_p = *(Entity_t**)sand_list_at( manager->npc_enemies, i
   //    );
   //    //npc_enemy_free( (NpcEnemy_t*)entity_p );
   // }
   // sand_list_free( manager->npc_enemies );

   free( manager );
}

//-----------------------------------------------------------------------------
void game_world_manager_update_and_draw( GameWorldManager_t* manager )
{
   // manager->world.draw();
   for ( int32_t i = 0; i < manager->entities->size; i++ )
   {
      Entity_t* entity_p = *( Entity_t** ) sand_list_at( manager->entities, i );
      if ( entity_p == NULL )
      {
         printf( "[GameWorldManager] - Cant draw entity is a NULL pointer\n" );
         continue;
      }

      entity_p->update( entity_p );
   }
}

//-----------------------------------------------------------------------------
void game_world_manager_add_entity( GameWorldManager_t* manager,
                                    Entity_t*           entity )
{
   sand_list_push_back( manager->entities, &entity );
}

//-----------------------------------------------------------------------------
void game_world_manager_handle_collisions( GameWorldManager_t* manager );

//-----------------------------------------------------------------------------
void game_world_manager_resolve_collision_entity_static(
    GameWorldManager_t* manager, Entity_t* entity );

//-----------------------------------------------------------------------------
void game_world_manager_prepare_manager( GameWorldManager_t* manager );

//-----------------------------------------------------------------------------
void game_world_manager_execute_npc_movements( GameWorldManager_t* manager );

//-----------------------------------------------------------------------------
void game_world_manager_on_event( GameWorldManager_t* manager, Event_t* e )
{
   void* entities_end_ptr =
       ( char* ) manager->entities->data +
       ( size_t ) manager->entities->size * sizeof(Entity_t*);

   void* ith_ptr = manager->entities->data;
   while ( ith_ptr != entities_end_ptr )
   {
      // (Entity_t**)ith_ptr    -> ith_ptr is a pointer to a Entity_t pointer
      // (*(Entity_t**)ith_ptr) -> is the derefercing of the pointer
      // Thats also why we increment in the loop by sizeof( Entity_t* ) we just want to increment by 8 byte
      ((*( Entity_t** )ith_ptr) )->on_event( (*( Entity_t** )ith_ptr), e );

      ith_ptr = ( char* ) ith_ptr + ( sizeof( Entity_t* )  );

      if ( e->handeld == true )
      {
         break;
      }
   }
}
