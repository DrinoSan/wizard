#pragma once

// System Headers
#include <array>
#include <cstdint>
#include <vector>

// Project Headers
#include "Entity.h"
#include "Graphic.h"
#include "NpcEnemy.h"
#include "Player.h"
#include "World.h"
#include "events/KeyEvent.h"

class GameWorldManager_t
{
 public:
   explicit GameWorldManager_t( std::unique_ptr<World_t> world_ );
   ~GameWorldManager_t();

   /// Function to draw the whole world including tiles,player,enemies and so on
   void draw() const;
   void update( float dt );
   void applyMovement( float dt );
   void updateEntities( float dt );

   /// Function to add objects to World
   /// @TODO: Use move semantic
   inline void addEntity( std::unique_ptr<Entity_t> object )
   {
      enities.push_back( std::move( object ) );
   }

   void onEvent( Event_t& e );

   void handleInputs();
   void handleCollisions( float dt );
   void resolveCollisionEntityStatic( Entity_t* e, float dt );
   void prepareManager();
   void executeNpcMovements();
   void spawnEnemies( int32_t numEnemies_ );

 private:
   std::vector<std::unique_ptr<Entity_t>> enities;
   std::unique_ptr<World_t>               world;
   int32_t                                numEnemies;
   std::vector<NpcEnemy_t>                npcEnemies;
   std::array<int, 15>                    keysDown{ 0 };
};
