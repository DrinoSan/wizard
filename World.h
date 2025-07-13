#pragma once

// System Headers
#include <cstdint>
#include <fstream>
#include <iostream>

// Project Headers
#include "Graphic.h"
#include "raylib.h"
#include "Tile.h"


class World_t : public Graphic_t
{
   public:
   World_t() : Graphic_t( OBJECT_TYPE::STATIC ) {}
   virtual ~World_t() = default;

   void draw() override;

   // Contains the layout which will define how our level looks like
   std::vector<int32_t> tileMapLayout{};

   /// Function to preapre the initital world
   /// Loading files for tiles and creating tile objects
   void preapreWorld();

   private:

   void handleMovement() override
   {
      std::cout << "[ WORLD_T ] I AM JUST A STATIC WORLD OBJECT AND CANT MOVE\n";
   }

   /// Function to load png files for files and tile map layout
   void loadTileLayoutAndTileMap();

   /// Function to update the tiles which will be drawn by draw function
   /// Currently this function is called only once to initialize everything but maybe later it can be used for dynamic map updates
   void updateTiles();

   // tileMapPNG contains each single tile packed into one png
   // One tile has 16x16 dimensions
   // The PNG contains 12x12 tiles
   Texture2D tileMapPNG;

   // This defines the tile in the source where we lookup the tile and which will be then later drawn into our game world
   Rectangle tileSrc = { 0.0f, 0.0f, 16.0f, 16.0f };

   // This defines the destination basically tileSrc tells us where to find the tile and tileDest tells us where to draw it in our gameworld
   // @TODO: remove hardcoded dimensions of world 800 and 450
   Rectangle tileDest = { 0.0f, 0.0f, static_cast<float>( 800 / 30.0f ),
                          static_cast<float>( 450 / 20.0f ) };

   std::vector<Tile_t> worldMap{};
};
