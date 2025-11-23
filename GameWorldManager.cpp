// System Headers
#include <algorithm>
#include <cassert>

#include "GameWorldManager.h"
#include "log.h"

//-----------------------------------------------------------------------------
GameWorldManager_t::GameWorldManager_t( std::unique_ptr<World_t> world_,
                                        int32_t                  numEnemies_ )
    : world{ std::move( world_ ) }, numEnemies{ numEnemies_ }
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
   for ( const auto& tile : world->worldMapTilesWithCollision )
   {
      auto hitboxNextFrame = entityPtr->hitbox;
      hitboxNextFrame.x += entityPtr->velocity.x;
      hitboxNextFrame.y += entityPtr->velocity.y;
      if ( CheckCollisionRecs( hitboxNextFrame, tile.tileDest ) )
      {
         float overlapLeft = ( entityPtr->hitbox.x + entityPtr->hitbox.width ) -
                             tile.tileDest.x;
         float overlapRight =
             ( tile.tileDest.x + tile.tileDest.width ) - entityPtr->hitbox.x;
         float overlapTop = ( entityPtr->hitbox.y + entityPtr->hitbox.height ) -
                            tile.tileDest.y;
         float overlapBottom =
             ( tile.tileDest.y + tile.tileDest.height ) - entityPtr->hitbox.y;

#ifdef DEBUG
         std::cout << entityPtr->str() << "\n";
         std::cout << "OverlapLeft: " << overlapLeft << "\n";
         std::cout << "OverlapRight: " << overlapRight << "\n";
         std::cout << "OverlapTop: " << overlapTop << "\n";
         std::cout << "OverlapBottom: " << overlapBottom << "\n";
#endif
         auto minOverlap = std::min(
             { overlapLeft, overlapRight, overlapTop, overlapBottom } );

         if ( minOverlap == overlapLeft )
         {
            entityPtr->velocity.x = 0;
         }

         if ( minOverlap == overlapRight )
         {
            entityPtr->velocity.x = 0;
         }

         if ( minOverlap == overlapTop )
         {
            entityPtr->velocity.y = 0;
         }

         if ( minOverlap == overlapBottom )
         {
            entityPtr->velocity.y = 0;
         }
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::prepareManager()
{
   for ( int32_t i = 0; i < numEnemies; ++i )
   {
      // INIT ENEMIES
      auto& enemy = enities.emplace_back( std::make_unique<NpcEnemy_t>(
          std::make_unique<A_StarStrategy_t>() ) );
      static_cast<NpcEnemy_t*>( enemy.get() )
          ->registerOnEventCallback( [ this ]( Event_t& e )
                                     { this->onEvent( e ); } );
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::executeNpcMovements()
{
   // @TODO seprate vectors for player and enemies
   Player_t* player = nullptr;
   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::PLAYER )
      {
         player = static_cast<Player_t*>( obj.get() );
      }
   }

   assert( player != nullptr );

   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::ENEMY )
      {
         static_cast<NpcEnemy_t*>( obj.get() )->handleNpcMovement( player );
      }
   }
}

//-----------------------------------------------------------------------------
// I think this is way more complicated than needed
void GameWorldManager_t::handleInputs()
{
   int  keyCode;
   bool keyHandeld{ false };

   for ( auto& key : keysDown )
   {
      if( key == 0 ) continue;
      if ( IsKeyDown( key ) )
      {
         KeyPressedEvent_t event( key );
         onEvent( event );
      }
      else
      {
         key = 0;
      }
   }

   // Working the key off
   while ( ( keyCode = GetKeyPressed() ) != 0 )
   {
      KeyPressedEvent_t event( keyCode );
      onEvent( event );
      if ( IsKeyDown( keyCode ) )
      {
         for ( auto& key : keysDown )
         {
            if ( key == keyCode )
            {
               // Key is already in pressedKeys we can skip
               keyHandeld = true;
               break;
            }
         }

         bool keyAdded{ false };
         if ( !keyHandeld )
         {
            for ( auto& key : keysDown )
            {
               if ( key == 0 )
               {
                  key = keyCode;
                  keyAdded = true;
                  break;
               }
            }

            if( keyAdded ) continue;
         }
      }
   }
}
