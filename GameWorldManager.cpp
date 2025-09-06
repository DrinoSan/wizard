// System Headers
#include <algorithm>

#include "GameWorldManager.h"

//-----------------------------------------------------------------------------
GameWorldManager_t::GameWorldManager_t( std::unique_ptr<World_t> world_ )
    : world{ std::move( world_ ) }
{
}

//-----------------------------------------------------------------------------
GameWorldManager_t::~GameWorldManager_t() = default;

//-----------------------------------------------------------------------------
void GameWorldManager_t::onEvent( Event_t& e )
{
   // EventDispatcher_t dispatcher( e );
   // dispatcher.Dispatch<KeyPressedEvent_t>( BIND_EVENT_FN( onKeyEvent ) );

   for ( auto& obj : enities )
   {
      obj->onEvent( e );
      // If a event is handled the other objects do  not need to check it
      // Example: PlayerMovement via KeyPressedEvent which is only for the
      // player
      if ( e.Handled )
      {
         break;
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::handleCollisions()
{
   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::STATIC )
      {
         continue;
      }

      // First i want to do the static checks -> object against world map
      resolveCollisionEntityStatic( obj.get() );

      // Second i want to check all dynamic objects

      // Check if Obj is overlapping with the map
      // Get all tiles next to the obj
      // check if the textures overlap
      // kick it back
      if ( obj->type == ENTITY_TYPE::PLAYER )
      {
         // world->getClosestTiles( static_cast<Player_t*>( obj.get() )->hitbox
         // );
      }
      else if ( obj->type == ENTITY_TYPE::NPC )
      {
         // @TODO: Implement  me
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::resolveCollisionEntityStatic( Entity_t* entityPtr )
{
   // Get all tiles close to entity
   // Check if tiles are static
   // Check if entity velocity would overlap with tile
   // resolve / push entity back

   for( const auto& tile : world->worldMap )
   {
      //if( tile.x < entityPtr->hitbox.x &&
   }
}








