#include "World.h"
#include "Globals.h"

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

      worldMap.push_back( { tileType, indexInSrc, tileSrc, tileDest } );

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
void World_t::preapreWorld()
{
   // Map related stuff
   loadTileLayoutAndTileMap();
   updateTiles();
}
