#pragma once

// System Headers
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

// Project Headers
#include "Graphic.h"
#include "Tile.h"
#include "raylib.h"

using worldMapContainer_t = std::vector<Tile_t>;

// ----------------------------------------------------------------------------
class GridElement_t
{
 public:
   GridElement_t( const Vector2& position_, bool isWakable_, int32_t idx )
       : position{ position_ }, isWakable{ isWakable_ }, idxInFlatMap{ idx } {};

   Vector2 position;
   bool    isWakable;
   int32_t idxInFlatMap;

   std::array<GridElement_t*, 8> neighbours;
   GridElement_t* predecessor;
};

using gridMap2D_t         = std::vector<std::vector<GridElement_t>>;

// ----------------------------------------------------------------------------
class Grid_t
{
 public:
   Grid_t() = default;
   Grid_t( const std::vector<Tile_t>& tiles );

   gridMap2D_t         gridMap;
   std::vector<Tile_t> flatGrid;
};

// ----------------------------------------------------------------------------
class World_t : public Graphic_t
{
 public:
   // World_t()          = default;
   virtual ~World_t() = default;

   void draw() override;

   // Contains the layout which will define how our level looks like
   std::vector<std::pair<int32_t, TileType_t>> tileMapLayout{};

   /// Function to preapre the initital world
   /// Loading files for tiles and creating tile objects
   void preapreWorld();

   // Public to get acces in GameWorldManager
   // Includes all possible tiles
   std::vector<Tile_t> worldMap{};
   // Includes only tiles of type COLLISION
   std::vector<Tile_t> worldMapTilesWithCollision{};

   Grid_t grid;

 private:
   /// Function to load png files for files and tile map layout
   void loadTileLayoutAndTileMap();

   /// Function to update the tiles which will be drawn by draw function
   /// Currently this function is called only once to initialize everything but
   /// maybe later it can be used for dynamic map updates
   void updateTiles();

   void initGrid();

   // tileMapPNG contains each single tile packed into one png
   // One tile has 16x16 dimensions
   // The PNG contains 12x12 tiles
   Texture2D tileMapPNG;

   // This defines the tile in the source where we lookup the tile and which
   // will be then later drawn into our game world
   Rectangle tileSrc = { 0.0f, 0.0f, 16.0f, 16.0f };

   // This defines the destination basically tileSrc tells us where to find the
   // tile and tileDest tells us where to draw it in our gameworld
   // @TODO: remove hardcoded dimensions of world 800 and 450
   Rectangle tileDest = { 0.0f, 0.0f, static_cast<float>( 800 / 30.0f ),
                          static_cast<float>( 450 / 20.0f ) };
};
