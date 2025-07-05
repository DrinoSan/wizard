#include "World.h"

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

//-----------------------------------------------------------------------------
void World_t::draw()
{
   for ( int32_t i = 0; i < tileMapLayout.size(); ++i )
   {
      // Todo get width and height from 1d vector of the layout to the src
      // Texture2D tileMapPNG -> contains the whole png texture
      // Rectangle tileSrc = is the recatngle inside the pngTexture
      // Rectangle tileDest= is the rectangle inside our wizard map

      // tileMapPNG 12x12
      // IndexInSrc = 26
      auto indexInSrc = tileMapLayout[ i ];
      auto x          = ( indexInSrc % 12 ) - 1;
      auto y          = ( indexInSrc / 12 ) * 16;
      tileSrc.x       = x * 16;
      tileSrc.y       = y;

      tileDest.x = ( i % 30 ) * static_cast<float>( 800 / 30 );
      tileDest.y = ( i / 30 ) * static_cast<float>( 450 / 20 );

      //std::cout << "------------------------------\n";
      //std::cout << "indexInSrc: " << indexInSrc << " i: " << i << " x: " << x
      //          << " y: " << y << "\n";
      //std::cout << "tileSrc.x: " << tileSrc.x << " tileSrc.y: " << tileSrc.y
      //          << "\n";
      //std::cout << "tileDest.x: " << tileDest.x << " tileDest.y: " << tileDest.y
      //          << "\n";
      //std::cout << "++++++++++++++++++++++++++++++\n";

      DrawTexturePro( tileMapPNG, tileSrc, tileDest, { 0, 0 }, 0, WHITE );
   }
}
