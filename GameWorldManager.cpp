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

   for ( auto& obj : worldObjects )
   {
      obj->onEvent( e );
		// If a event is handled the other objects do  not need to check it
		// Example: PlayerMovement via KeyPressedEvent which is only for the player
      if ( e.Handled )
      {
         break;
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::handleCollisions()
{
   for ( auto& obj : worldObjects )
   {
      if ( obj->type == OBJECT_TYPE::STATIC )
      {
         continue;
      }

      // Check if Obj is overlapping with the map
      // Get all tiles next to the obj
      // check if the textures overlap
      // kick it back
      // @TODO: make world be a own member not in the worldObjects vector
      if ( obj->type == OBJECT_TYPE::PLAYER )
      {
         world->getClosestTiles( static_cast<Player_t*>( obj.get() )->hitbox );
      }
   }
}
