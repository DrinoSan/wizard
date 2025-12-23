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

   // I like to leak memory
   // Creating Player
   auto player = std::make_unique<Player_t>();

   // Creating World
   auto world = std::make_unique<World_t>();
   // Prepare world (tilemap,layout and so on)
   world->preapreWorld();

   // Adding all objects to senior manager
   // The sequence of adding objects to the world is important because if world
   // is added after player the player will be drawn behind the map.
   GameWorldManager_t gameWorldManager{ std::move( world ) };
   gameWorldManager.spawnEnemies( NUM_ENEMIES );
   gameWorldManager.addEntity( std::move( player ) );
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
         ToggleFullscreen();
         gameWorldManager.updateWorld();
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

      auto* player = gameWorldManager.getPlayer();
      {
         rlImGuiBegin();
         ImGui::Begin( "Debug Stuff" );

         ImGui::Text( "FPS: %d", GetFPS() );
         if ( ImGui::CollapsingHeader( "Player Info" ) )
         {
            ImGui::SeparatorText( "General:" );
            ImGui::BulletText( "State: %s", entityStateToString( player->state ).c_str() );
            ImGui::BulletText( "Position: %.2f, %.2f",
                               player->playerPosition.x,
                               player->playerPosition.y );
         }
         if ( ImGui::CollapsingHeader( "World Info" ) )
         {
            ImGui::SeparatorText( "General:" );
            ImGui::BulletText( "RenderWidth: %d - RenderHeight: %d",
                               GetRenderWidth(), GetRenderHeight() );
            ImGui::BulletText( "ScreenWidth: %d - ScreenHeight: %d",
                               GetScreenWidth(), GetScreenHeight() );
         }

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
