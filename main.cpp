// System Headers
#include <cstdint>
#include <fstream>
#include <iostream>

// Project Headers
#include "GameWorldManager.h"
#include "Globals.h"
#include "Player.h"
#include "World.h"
#include "events/KeyEvent.h"
#include "raylib.h"

int main( void )
{
   // Initialization
   //--------------------------------------------------------------------------------------
   InitWindow( screenWidth, screenHeight, "Wizard" );

   // I like to leak memory
   // Creating Player
   auto player = new Player_t();

   // Creating World
   auto world = new World_t();
   // Prepare world (tilemap,layout and so on)
   world->preapreWorld();

   // Adding all objects to senior manager
   // The sequence of adding objects to the world is important because if world
   // is added after player the player will be drawn behind the map.
   GameWorldManager_t gameWorldManager;
   gameWorldManager.addObject( world );
   gameWorldManager.addObject( player );

   SetTargetFPS( 60 );   // Set our game to run at 60 frames-per-second
   //--------------------------------------------------------------------------------------

   // Main game loop
   while ( !WindowShouldClose() )   // Detect window close button or ESC key
   {
      // Update
      //----------------------------------------------------------------------------------
      if ( IsKeyDown( KEY_Q ) )
      {
         break;
      }
      if ( IsKeyDown( KEY_D ) )
      {
         KeyPressedEvent_t event( KEY_D );
         gameWorldManager.onEvent( event );
      }
      if ( IsKeyDown( KEY_A ) )
      {
         KeyPressedEvent_t event( KEY_A );
         gameWorldManager.onEvent( event );
      }
      if ( IsKeyDown( KEY_W ) )
      {
         KeyPressedEvent_t event( KEY_W );
         gameWorldManager.onEvent( event );
      }
      if ( IsKeyDown( KEY_S ) )
      {
         KeyPressedEvent_t event( KEY_S );
         gameWorldManager.onEvent( event );
      }

      //----------------------------------------------------------------------------------

      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

      ClearBackground( RAYWHITE );

      gameWorldManager.draw();

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
