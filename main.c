// System Headers

// Project Headers
// #include "GameWorldManager.h"
// #include "Entity.h"
// #include "Globals.h"
#include "Player.h"
// #include "World.h"
// #include "events/KeyEvent.h"
#include "raylib.h"

int32_t screenWidth  = 800;
int32_t screenHeight = 450;

int main()
{
   // Initialization
   //--------------------------------------------------------------------------------------
   InitWindow( screenWidth, screenHeight, "Wizard" );

   // I like to leak memory
   // Creating Player
   Player_t* player = player_create( ENTITY_TYPE_PLAYER, 400, 230,
                                     "spritesheets/wizard01.png" );

   // Creating World
   // auto world = std::make_unique<World_t>();
   // Prepare world (tilemap,layout and so on)
   // world->preapreWorld();

   // Adding all objects to senior manager
   // The sequence of adding objects to the world is important because if world
   // is added after player the player will be drawn behind the map.
   // GameWorldManager_t gameWorldManager{ std::move( world ), NUM_ENEMIES };
   // gameWorldManager.addEntity( std::move( player ) );
   // gameWorldManager.prepareManager();

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
         player_go_right( player );
         //KeyPressedEvent_t event( KEY_D );
         //gameWorldManager.onEvent( event );
      }
      if ( IsKeyDown( KEY_A ) )
      {
         player_go_left( player );
         //KeyPressedEvent_t event( KEY_A );
         //gameWorldManager.onEvent( event );
      }
      if ( IsKeyDown( KEY_W ) )
      {
         player_go_up( player );
         //KeyPressedEvent_t event( KEY_W );
         //gameWorldManager.onEvent( event );
      }
      if ( IsKeyDown( KEY_S ) )
      {
         player_go_down( player );
         //KeyPressedEvent_t event( KEY_S );
         //gameWorldManager.onEvent( event );
      }
      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

      player->base.update(player);
      //entity_draw( player );

      ClearBackground( RAYWHITE );

      // gameWorldManager.draw();

      DrawText( "move the player with WASD keys", 10, 10, 20, DARKGRAY );

      EndDrawing();
   }

   // De-Initialization
   //--------------------------------------------------------------------------------------
   CloseWindow();   // Close window and OpenGL context
   //--------------------------------------------------------------------------------------

   return 0;
}
