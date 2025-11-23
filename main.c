// System Headers

// SandLib
#include "sandlib.h"
// Project Headers
#include "GameWorldManager.h"
// #include "Entity.h"
// #include "Globals.h"
#include "Player.h"
// #include "World.h"
#include "events/Event.h"
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

   GameWorldManager_t* manager = game_world_manager_create( 10 );
   game_world_manager_add_entity( manager, ( Entity_t* ) player );

   // sand_list* entities = sand_list_create( sizeof( Entity_t* ) );
   // Entity_t* e = (Entity_t*)player;
   // sand_list_push_back( entities,  &e );

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
         KeyPressedEvent_t event = key_pressed_event_stack_create( KEY_D, 0 );
         game_world_manager_on_event( manager, ( Event_t* ) &event );
      }
      if ( IsKeyDown( KEY_A ) )
      {
         KeyPressedEvent_t event = key_pressed_event_stack_create( KEY_A, 0 );
         game_world_manager_on_event( manager, ( Event_t* ) &event );
      }
      if ( IsKeyDown( KEY_W ) )
      {
         KeyPressedEvent_t event = key_pressed_event_stack_create( KEY_W, 0 );
         game_world_manager_on_event( manager, ( Event_t* ) &event );
      }
      if ( IsKeyDown( KEY_S ) )
      {
         KeyPressedEvent_t event = key_pressed_event_stack_create( KEY_S, 0 );
         game_world_manager_on_event( manager, ( Event_t* ) &event );
      }
      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

      game_world_manager_update_and_draw( manager );

      // for ( size_t i = 0; i < entities->size; i++ )
      //{
      //    Entity_t** pp     = sand_list_at( entities, i );
      //    Entity_t*  entity = *pp;
      //    Player_t*  p      = ( Player_t* ) entity;

      //   printf( "Player Pos: %f\n", p->base.pos.x );
      //   printf( "Original: %f\n", player->base.pos.x );
      //}
      // player->base.update( player );
      // entity_draw( player );

      ClearBackground( RAYWHITE );

      // gameWorldManager.draw();

      DrawText( "move the player with WASD keys", 10, 10, 20, DARKGRAY );

      EndDrawing();
   }

   // De-Initialization
   //--------------------------------------------------------------------------------------
   game_world_manager_free( manager );
   CloseWindow();   // Close window and OpenGL context
   //--------------------------------------------------------------------------------------

   return 0;
}
