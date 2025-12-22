// System Headers
#include <algorithm>
#include <cassert>

#include "GameWorldManager.h"
#include "log.h"

//-----------------------------------------------------------------------------
GameWorldManager_t::GameWorldManager_t( std::unique_ptr<World_t> world_ )
    : world{ std::move( world_ ) }
{
}

//-----------------------------------------------------------------------------
GameWorldManager_t::~GameWorldManager_t() = default;

//-----------------------------------------------------------------------------
void GameWorldManager_t::draw() const
{
   // @TODO: add layers which should be drawn to have a way to know whats in
   // the background and so on
   world->draw();
   for ( const auto& e : enities )
   {
      e->draw();
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::updateEntities( float dt )
{
   for ( auto& entity : enities )
   {
      entity->update( dt );
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::applyMovement( float dt )
{
   for ( auto& entity : enities )
   {
      // Skip purely static objects I mean why would they even move
      if ( entity->type == ENTITY_TYPE::STATIC )
      {
         continue;
      }

      // Only move if the entity actually wants to move this frame
      // (collision system has already zeroed velocity axes if blocked)
      if ( entity->velocity.x != 0.0f || entity->velocity.y != 0.0f )
      {
         Vector2 motion = Vector2Scale( entity->velocity, dt );

         entity->playerPosition = Vector2Add( entity->playerPosition, motion );

         // Sync hitbox
         entity->hitbox.x =
             entity->playerPosition.x + entity->hitboxOffset.x;
         entity->hitbox.y =
             entity->playerPosition.y + entity->hitboxOffset.y;
      }

      // IMPORTANT Without this, entities keep sliding forever
      entity->velocity = { 0.0f, 0.0f };
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::update( float dt )
{
   handleInputs();
   executeNpcMovements();
   updateEntities( dt );
   handleCollisions( dt );
   applyMovement( dt );
   // lateUpdate( dt );
}

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
void GameWorldManager_t::handleCollisions( float dt )
{
   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::STATIC )
      {
         continue;
      }

      // First i want to do the static checks -> object against world map
      resolveCollisionEntityStatic( obj.get(), dt );

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
void GameWorldManager_t::resolveCollisionEntityStatic( Entity_t* e, float dt )
{
   // How far the entity wants to move this frame
   Vector2 desiredMotion = Vector2Scale( e->velocity, dt );

   // Skip if not moving
   if ( desiredMotion.x == 0 && desiredMotion.y == 0 )
   {
      return;
   }

   Rectangle futureHitbox = e->hitbox;

   // Checking around x axis
   futureHitbox.x += desiredMotion.x;

   bool collisionX = false;
   for ( const auto& tile : world->worldMapTilesWithCollision )
   {
      if ( CheckCollisionRecs( futureHitbox, tile.tileDest ) )
      {
         collisionX = true;
         break;
      }
   }

   if ( collisionX )
   {
      e->velocity.x   = 0;
      desiredMotion.x = 0;
      futureHitbox.x  = e->hitbox.x;
   }

   // Checking around y axis
   futureHitbox.y += desiredMotion.y;

   bool collisionY = false;
   for ( const auto& tile : world->worldMapTilesWithCollision )
   {
      if ( CheckCollisionRecs( futureHitbox, tile.tileDest ) )
      {
         collisionY = true;
         break;
      }
   }

   if ( collisionY )
   {
      e->velocity.y   = 0;
      desiredMotion.y = 0;
      futureHitbox.y =
          e->hitbox
              .y;   // Not needed to set but i like if its the same as for X
   }

   // Optional: push-out to prevent getting "sucked" into walls
   // Only needed if floating point couses issues
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::prepareManager()
{
   for ( auto& obj : enities )
   {
      std::cout << "PrepareManager Entity type: "
                << entityTypeToString( obj->type ) << "\n";
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

   assert( player != nullptr && "Player needs to be initialized" );

   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::ENEMY )
      {
         auto enemy = static_cast<NpcEnemy_t*>( obj.get() );
         enemy->handleNpcMovement( *world, player );
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
      if ( key == 0 )
         continue;
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
                  key      = keyCode;
                  keyAdded = true;
                  break;
               }
            }

            if ( keyAdded )
               continue;
         }
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::spawnEnemies( int32_t numEnemies_ )
{
   numEnemies = numEnemies_;
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
