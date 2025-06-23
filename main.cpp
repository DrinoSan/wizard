// System Headers
#include <cstdint>
#include <iostream>
#include <fstream>

// Project Headers
#include "Player.h"
#include "raylib.h"

int main( void )
{
   // Initialization
   //--------------------------------------------------------------------------------------
   constexpr int32_t screenWidth  = 800;
   constexpr int32_t screenHeight = 450;

   InitWindow( screenWidth, screenHeight, "Wizard" );

   //Texture2D tileMapLayout = LoadTexture( "resources/dungonmapAdjusted.tmx" );
   std::ifstream file("resources/dungonmapAdjusted.tmx");
   std::string str;
   std::vector<int32_t> tileMapLayout{ };
   while (std::getline(file, str))
   {
      char* head = str.data();
      char* tail = head;
      while( *head != '\0' )
      {
         if( *head != ',' )
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


   Texture2D tileMapPNG    = LoadTexture( "resources/kenney_tinyDungeon/Tilemap/tilemap.png" );
   Rectangle tileSrc = { 0.0f, 0.0f, static_cast<float>( tileMapPNG.width / 30 ), static_cast<float>( tileMapPNG.height / 20 ) };
   Rectangle tileDest= { 0.0f, 0.0f, 300, 300 };

   int framesCounter = 0;
   Player_t player;

   SetTargetFPS( 60 );   // Set our game to run at 60 frames-per-second
   //--------------------------------------------------------------------------------------

   // Main game loop
   while ( !WindowShouldClose() )   // Detect window close button or ESC key
   {
      ++framesCounter;
      // Update
      //----------------------------------------------------------------------------------
      if ( IsKeyDown( KEY_D ) )
      {
         player.goRight( framesCounter );
      }
      if ( IsKeyDown( KEY_A ) )
      {
         player.goLeft( framesCounter );
      }
      if ( IsKeyDown( KEY_W ) )
      {
         player.goUp( framesCounter );
      }
      if ( IsKeyDown( KEY_S ) )
      {
         player.goDown( framesCounter );
      }

      if ( IsKeyDown( KEY_Q ) )
      {
         break;
      }

      //----------------------------------------------------------------------------------

      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

      ClearBackground( RAYWHITE );

      for( int32_t i = 0; i < tileMapLayout.size(); ++i )
      {
         // Todo get width and height from 1d vector of the layout to the src
         //Texture2D tileMapPNG -> contains the whole png texture
         //Rectangle tileSrc = is the recatngle inside the pngTexture
         //Rectangle tileDest= is the rectangle inside our wizard map

         //tileMapPNG 12x12
         //IndexInSrc = 26
         auto indexInSrc = tileMapLayout[ i ];
         auto x = indexInSrc % 12;
         auto y = indexInSrc % 12;
         tileSrc.x = x;
         tileSrc.y = y;

         tileDest.x = i % 30;
         tileDest.y = i / 20;

         std::cout << "------------------------------\n";
         std::cout << "indexInSrc: " << indexInSrc << " i: " << i << "\n";
         std::cout << "tileSrc.x: " << tileSrc.x << " tileSrc.y: " << tileSrc.y << "\n";
         std::cout << "tileDest.x: " << tileDest.x << " tileDest.y: " << tileDest.y << "\n";
         std::cout << "++++++++++++++++++++++++++++++\n";

//------------------------------
//indexInSrc: 1 i: 0
//tileSrc.x: 1 tileSrc.y: 1
//tileDest.x: 0 tileDest.y: 0
//++++++++++++++++++++++++++++++
//------------------------------
//indexInSrc: 1 i: 1
//tileSrc.x: 1 tileSrc.y: 1
//tileDest.x: 1 tileDest.y: 1
//++++++++++++++++++++++++++++++
//------------------------------
//indexInSrc: 1 i: 2
//tileSrc.x: 1 tileSrc.y: 1
//tileDest.x: 2 tileDest.y: 2
//++++++++++++++++++++++++++++++

         DrawTexturePro( tileMapPNG, tileSrc, tileDest, {0,0}, 0, WHITE );

      }

      player.draw();

      DrawText( "move the player with WASD keys", 10, 10, 20, DARKGRAY );

      EndDrawing();
      //----------------------------------------------------------------------------------
   }

   // De-Initialization
   //--------------------------------------------------------------------------------------
   CloseWindow();   // Close window and OpenGL context
   //--------------------------------------------------------------------------------------

   return 0;
}
