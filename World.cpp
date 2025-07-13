#include "World.h"

// ----------------------------------------------------------------------------
void World_t::loadTileLayoutAndTileMap()
{
   // This part loads the tileMapLayout it contains 30x20 entries where the
   // value defines the index in the tileMapPNG
   std::ifstream file( "resources/dungonmapAdjusted.tmx" );
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

         std::string num{ tail, head };
         tileMapLayout.push_back( std::stoi( num ) );

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
   float tileWidth  = 800.0f / 30;   // Screen width / columns
   float tileHeight = 450.0f / 20;   // Screen height / rows

   for ( int32_t i = 0; i < tileMapLayout.size(); ++i )
   {
      auto indexInSrc =
          tileMapLayout[ i ] - 1;   // Convert 1-based to 0-based indexing
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

      worldMap.push_back({ TileType_t::COLLISION, indexInSrc, tileSrc, tileDest });
   }
}

// ----------------------------------------------------------------------------
void World_t::draw()
{
   for( const auto& tile : worldMap )
   {
      DrawTexturePro( tileMapPNG, tile.tileSrc, tile.tileDest, { 0, 0 }, 0, WHITE );
   }
}

// ----------------------------------------------------------------------------
void World_t::preapreWorld()
{
   loadTileLayoutAndTileMap();
   updateTiles();
}
