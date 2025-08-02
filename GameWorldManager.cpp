#include "GameWorldManager.h"
GameWorldManager_t::GameWorldManager_t()  = default;
GameWorldManager_t::~GameWorldManager_t() = default;

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
