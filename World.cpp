#include <ranges>

#include "Globals.h"
#include "World.h"
#include "log.h"

// ----------------------------------------------------------------------------
void World_t::loadTileLayoutAndTileMap()
{
   // This part loads the tileMapLayout it contains 30x20 entries where the
   // value defines the index in the tileMapPNG
   std::ifstream file( "tilemapWithTypes.sand" );
   std::string   str;
   while ( std::getline( file, str ) )
   {
      char* head = str.data();
      char* tail = head;
      while ( *head != '\0' )
      {
         if ( *head != ',' )
         {
            ++head;
            continue;
         }

         std::string tileElement{ tail, head };
         // Delimiter == '-'
         // Here i have the form 3-C
         auto delimiterIt = tileElement.find( '-' );

         std::pair<int32_t, TileType_t> tile;
         tile.first  = std::stoi( tileElement.substr( 0, delimiterIt ) );
         tile.second = TileType_t::NO_COLLISION;

         if ( tileElement[ delimiterIt + 1 ] == 'C' )
         {
            tile.second = TileType_t::COLLISION;
         }

         tileMapLayout.push_back( tile );

         ++head;
         tail = head;
      }
   }

   // tileMapPNG contains each single tile packed into one png
   // One tile has 16x16 dimensions
   // The PNG contains 12x12 tiles
   tileMapPNG =
       LoadTexture( "resources/kenney_tinyDungeon/Tilemap/tilemap_packed.png" );
}

// ----------------------------------------------------------------------------
void World_t::updateTiles()
{
   float tileWidth =
       screenWidth / 30;   // @TODO: Take size (30) from metdata in xml file
                           // @TODO: Also add dimensions to the world so i can
                           // use that for creating the grid map
   float tileHeight =
       screenHeight / 20;   // @TODO: Take size (20) from metdata in xml file

   for ( int32_t i = 0; i < tileMapLayout.size(); ++i )
   {
      // tileMapLayout first = index
      // tileMapLayout second = tile type (COLLISION or Not)
      auto indexInSrc =
          tileMapLayout[ i ].first - 1;   // Convert 1-based to 0-based indexing
      auto tileType = tileMapLayout[ i ].second;

      // Source rectangle (from tilemap PNG, 12 columns x 11 rows)
      tileSrc.x      = ( indexInSrc % 12 ) * 16;
      tileSrc.y      = ( indexInSrc / 12 ) * 16;
      tileSrc.width  = 16;
      tileSrc.height = 16;

      // Destination rectangle (on screen)
      tileDest.x      = ( i % 30 ) * tileWidth;
      tileDest.y      = ( i / 30 ) * tileHeight;
      tileDest.width  = tileWidth;
      tileDest.height = tileHeight;

      worldMap.push_back( { tileType, indexInSrc, tileSrc, tileDest, i } );

      if ( tileType == TileType_t::COLLISION )
      {
         worldMapTilesWithCollision.push_back(
             { tileType, indexInSrc, tileSrc, tileDest } );
      }
   }
}

// ----------------------------------------------------------------------------
void World_t::draw()
{
   for ( const auto& tile : worldMap )
   {
      DrawTexturePro( tileMapPNG, tile.tileSrc, tile.tileDest, { 0, 0 }, 0,
                      WHITE );

#ifdef DEBUG
      if ( tile.tileType == TileType_t::COLLISION )
      {
         DrawRectangleLines( tile.tileDest.x, tile.tileDest.y,
                             tile.tileDest.width, tile.tileDest.height, RED );
      }

#endif
   }
}

// ----------------------------------------------------------------------------
void World_t::initGrid()
{
   grid = Grid_t( worldMap );
}

// ----------------------------------------------------------------------------
void World_t::preapreWorld()
{
   // Map related stuff
   loadTileLayoutAndTileMap();
   updateTiles();
   initGrid();
}

// ----------------------------------------------------------------------------
Grid_t::Grid_t( const std::vector<Tile_t>& tiles )
{
   printStringln( "Init game grid path finding algorithms" );
   flatGrid = tiles;

   for ( int32_t i = 0; i < 20; ++i )
   {
      gridMap.push_back( std::vector<GridElement_t>() );
      for ( int32_t j = 0; j < 30; ++j )
      {
         // To get from a 1d vec to a 2d vec i = row, 30 == max columns, j =
         // column
         auto tile = tiles[ i * 30 + j ];
         gridMap[ i ].push_back( GridElement_t(
             { static_cast<float>( i ), static_cast<float>( j ) },
             tile.tileType == TileType_t::COLLISION ? true : false, tile.i ) );
      }
   }

   // clang-format off
   std::vector<std::pair<int32_t, int32_t>> goToPossibleNeighbours{
       { -1, -1 },  { -1, 0 },  { -1, 1 },
       { 0, -1  },  { 0, 0  },  { 0, 1  },
       { 1, -1  },  { 1, 0  },  { 1, 1  } };
   // clang-format on

   auto fillNeighbours = [ & ]( int32_t y, int32_t x )
   {
      std::pair<int32_t, int32_t> currentPosition{ y, x };
      int32_t                     idx{ 0 };
      for ( const auto& pair : goToPossibleNeighbours )
      {
         if ( currentPosition.first + pair.first < 0 )
         {
            ++idx;
            continue;
         }

         if ( currentPosition.second + pair.second < 0 )
         {
            ++idx;
            continue;
         }

         if ( currentPosition.first + pair.first >= gridMap.size() )
         {
            ++idx;
            continue;
         }

         if ( currentPosition.second + pair.second >= gridMap[ y ].size() )
         {
            ++idx;
            continue;
         }

         gridMap[ currentPosition.first ][ currentPosition.second ]
             .neighbours[ idx ] =
             &gridMap[ currentPosition.first + pair.first ]
                     [ currentPosition.second + pair.second ];
         ++idx;
      }
   };

   for ( int32_t i = 0; i < gridMap.size(); ++i )
   {
      for ( int32_t j = 0; j < gridMap[ 0 ].size(); ++j )
      {
         fillNeighbours( i, j );
      }
   }
}
