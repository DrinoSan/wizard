#pragma once

// System Headers
#include <cstdint>
#include <vector>

// Project Headers
#include "Graphic.h"
#include "events/KeyEvent.h"

class GameWorldManager_t
{
 public:
   GameWorldManager_t();
   ~GameWorldManager_t();

   /// Function to draw the whole world including tiles,player,enemies and so on
   inline void draw()
   {
      for ( auto& obj : worldObjects )
      {
         obj->draw();
      }
   }

   /// Function to add objects to World
   /// @TODO: Use move semantic
   inline void addObject( std::unique_ptr<Graphic_t> object )
   {
      worldObjects.push_back( std::move( object ) );
   }

   void onEvent( Event_t& e );

 private:
   std::vector<std::unique_ptr<Graphic_t>> worldObjects;
};
