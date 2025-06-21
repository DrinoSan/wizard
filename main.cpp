// System Headers
#include <cstdint>
#include <iostream>

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

   int framesCounter = 0;
   Player_t player;

   SetTargetFPS( 60 );   // Set our game to run at 60 frames-per-second
   //--------------------------------------------------------------------------------------

   // Main game loop
   while ( !WindowShouldClose() )   // Detect window close button or ESC key
   {
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
         break;
      //----------------------------------------------------------------------------------

      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

      ClearBackground( RAYWHITE );

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
