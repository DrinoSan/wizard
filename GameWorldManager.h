#pragma once

// System Headers
#include <cstdint>
#include <vector>

// Project Headers
#include "Entity.h"
#include "Graphic.h"
#include "Player.h"
#include "NpcEnemy.h"
#include "World.h"
#include "events/KeyEvent.h"

class GameWorldManager_t
{
 public:
   GameWorldManager_t( std::unique_ptr<World_t> world_, int32_t numEnemies_ );
   ~GameWorldManager_t();

   /// Function to draw the whole world including tiles,player,enemies and so on
   inline void draw()
   {
      // @TODO: add layers which should be drawn to have a way to know whats in
      // the background and so on
      world->draw();
      for ( auto& obj : enities )
      {
         obj->update();
      }
   }

   /// Function to add objects to World
   /// @TODO: Use move semantic
   inline void addEntity( std::unique_ptr<Entity_t> object )
   {
      enities.push_back( std::move( object ) );
   }

   void onEvent( Event_t& e );

   void handleInputs();
   void handleCollisions();
   void resolveCollisionEntityStatic( Entity_t* entityPtr );
   void prepareManager();
   void executeNpcMovements();

 private:
   std::vector<std::unique_ptr<Entity_t>> enities;
   std::unique_ptr<World_t>               world;
   int32_t numEnemies;
   std::vector<NpcEnemy_t> npcEnemies;
   std::array<int, 15> keysDown{ 0 };
};
