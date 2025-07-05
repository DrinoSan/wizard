#pragma once

// System Headers
#include <vector>
#include <cstdint>
#include <fstream>

class GameWorldManager_t
{
   public:
   GameWorldManager_t();
   ~GameWorldManager_t();

   /// Function to draw the whole world including tiles,player,enemies and so on
   inline void draw()
   {
      for( const auto& obj : worldObjects )
      {
         obj.draw();
      }
   }

   /// Function to add objects to World
   /// @TODO: Use move semantic
   inline void addObject( const Graphic_t& object )
   {
      worldObjects.push_back( object );
   }

   private:
   std::vector<Graphic_t> worldObjects;
};
