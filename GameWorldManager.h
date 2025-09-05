#pragma once

// System Headers
#include <cstdint>
#include <vector>

// Project Headers
#include "Graphic.h"
#include "Player.h"
#include "World.h"
#include "events/KeyEvent.h"

class GameWorldManager_t
{
 public:
   GameWorldManager_t( std::unique_ptr<World_t> world_ );
   ~GameWorldManager_t();

   /// Function to draw the whole world including tiles,player,enemies and so on
   inline void draw()
   {
      // @TODO: add layers which should be drawn to have a way to know whats in
      // the background and so on
      world->draw();
      for ( auto& obj : worldObjects )
      {
         obj->draw();
         auto* p = static_cast<Player_t*>(
             obj.get() );   // obj is a World_t and Player_t inherits from
                            // World_t
         std::cout << "Player pos: (" << p->playerPosition.x << ","
                   << p->playerPosition.y << ") frameRec: pos: ("
                   << p->frameRec.x << "," << p->frameRec.y
                   << ") frameRec height: " << p->frameRec.height
                   << " frameRec width: " << p->frameRec.width << "..\n";
      }
   }

   /// Function to add objects to World
   /// @TODO: Use move semantic
   inline void addObject( std::unique_ptr<Graphic_t> object )
   {
      worldObjects.push_back( std::move( object ) );
   }

   void onEvent( Event_t& e );

   void handleCollisions();

 private:
   std::vector<std::unique_ptr<Graphic_t>> worldObjects;
   std::unique_ptr<World_t>                world;
};
