#pragma once

// System Headers
#include <vector>
#include <cstdint>

// Project Headers
#include "Graphic.h"

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

   inline void handleMovements()
   {
      for ( auto& obj : worldObjects )
      {
         if ( obj->type == OBJECT_TYPE::PLAYER )
         {
            obj->handleMovement();
         }
      }
   }

   /// Function to add objects to World
   /// @TODO: Use move semantic
   inline void addObject( Graphic_t* object )
   {
      worldObjects.push_back( object );
   }

 private:
   std::vector<Graphic_t*> worldObjects;
};
