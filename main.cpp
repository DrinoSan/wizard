// System Headers
#include <memory>

// Project Headers
#include "Entity.h"
#include "GameWorldManager.h"
#include "Globals.h"
#include "Player.h"
#include "World.h"
#include "events/KeyEvent.h"
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

int main()
{
   // Initialization
   //--------------------------------------------------------------------------------------
   InitWindow( screenWidth, screenHeight, "Wizard" );

   rlImGuiSetup( true );

   // Adding all objects to senior manager
   // The sequence of adding objects to the world is important because if world
   // is added after player the player will be drawn behind the map.
   GameWorldManager_t gameWorldManager;
   gameWorldManager.spawnPlayer();
   gameWorldManager.spawnEnemies( NUM_ENEMIES );
   gameWorldManager.prepareManager();

   SetTargetFPS( 60 );   // Set our game to run at 60 frames-per-second
   //--------------------------------------------------------------------------------------

   // Main game loop
   while ( !WindowShouldClose() )   // Detect window close button or ESC key
   {
      // Update
      //----------------------------------------------------------------------------------
      // Input Handling
      if ( IsKeyDown( KEY_Q ) )
      {
         break;
      }
      if ( IsKeyPressed( KEY_F ) )
      {
         if ( IsWindowFullscreen() )
         {
            SetWindowSize( screenWidth, screenHeight );
            ToggleFullscreen();
         }
         else
         {
            int32_t monitor = GetCurrentMonitor();
            SetWindowSize( GetMonitorWidth( monitor ),
                           GetMonitorHeight( monitor ) );
            ToggleFullscreen();
         }

         //gameWorldManager.updateWorld();
      }

      float dt = GetFrameTime();
      gameWorldManager.update( dt );

      //----------------------------------------------------------------------------------

      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

      ClearBackground( RAYWHITE );

      gameWorldManager.draw();

      DrawText( "move the player with WASD keys", 10, 10, 20, RED );

      // IMGUI stuff
      {
         rlImGuiBegin();
         ImGui::Begin( "Debug Stuff" );

         ImGui::Text( "FPS: %d", GetFPS() );
         gameWorldManager.imgui_debug();
         ImGui::TextLinkOpenURL( "https://github.com/DrinoSan/wizard" );

         ImGui::End();
         rlImGuiEnd();
      }

      EndDrawing();
      //----------------------------------------------------------------------------------
   }

   // De-Initialization
   //--------------------------------------------------------------------------------------
   rlImGuiShutdown();
   CloseWindow();   // Close window and OpenGL context
   //--------------------------------------------------------------------------------------

   return 0;
}
