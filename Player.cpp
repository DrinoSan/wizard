// System Headers
#include <iostream>

// Project Headers
#include "Globals.h"
#include "Player.h"
#include "events/KeyEvent.h"
#include "imgui.h"
#include "raymath.h"

//-----------------------------------------------------------------------------
Player_t::Player_t()
    : Player_t(
          { ( float ) screenWidth / 2 + 30, ( float ) screenHeight / 2 + 30 } )
{
}

//-----------------------------------------------------------------------------
Player_t::Player_t( Vector2 pos )
{
   playerPosition = { pos.x, pos.y };

   playerTexture = LoadTexture( "spritesheets/wizard01.png" );

   attackTextures[ static_cast<std::size_t>( AttackType::LIGHTNING ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::FIRE ) ] = LoadTexture(
       "resources/attackSprites/magicAttackSprites/10FireBall/Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::WATER ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::EARTH ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );

   activeAttacks.reserve( 32 );

   ANIMATION_WALK_UP_Y   = static_cast<float>( playerTexture.height / 54 ) * 8;
   ANIMATION_WALK_LEFT_Y = static_cast<float>( playerTexture.height / 54 ) * 9;
   ANIMATION_WALK_DOWN_Y = static_cast<float>( playerTexture.height / 54 ) * 10;
   ANIMATION_WALK_RIGHT_Y =
       static_cast<float>( playerTexture.height / 54 ) * 11;

   ANIMATION_IDLE_UP_Y    = static_cast<float>( playerTexture.height / 54 ) * 0;
   ANIMATION_IDLE_LEFT_Y  = static_cast<float>( playerTexture.height / 54 ) * 1;
   ANIMATION_IDLE_DOWN_Y  = static_cast<float>( playerTexture.height / 54 ) * 2;
   ANIMATION_IDLE_RIGHT_Y = static_cast<float>( playerTexture.height / 54 ) * 3;

   frameRec = { 0.0f, 0.0f, ( float ) playerTexture.width / 13,
                static_cast<float>( playerTexture.height / 54 ) };

   frameWidth = playerTexture.width / 13;

   type   = ENTITY_TYPE::PLAYER;
   hitbox = { playerPosition.x + 10, playerPosition.y, 20, 40 };
}

//-----------------------------------------------------------------------------
Player_t::~Player_t()
{
   for ( auto& tex : attackTextures )
   {
      UnloadTexture( tex );
   }
}

//-----------------------------------------------------------------------------
void Player_t::draw()
{
   // DrawTextureRec( playerTexture, frameRec, playerPosition,
   // WHITE );   // Draw part of the texture
   DrawTexturePro( playerTexture, frameRec,
                   { playerPosition.x, playerPosition.y, 40, 40 }, { 0, 0 }, 0,
                   WHITE );

   for ( const auto& atk : activeAttacks )
   {
      if ( atk.active )
      {
         DrawTexturePro( getAttackTexture( atk.type ), atk.sourceRec,
                         { atk.position.x, atk.position.y, 72.0f, 72.0f },
                         { 36.0f, 48.0f }, atk.rotation, WHITE );
#ifdef DEBUG
         DrawRectangleLines( atk.position.x, atk.position.y, 72, 72, RED );
#endif
      }
   }

   activeAttacks.erase(
       std::remove_if( activeAttacks.begin(), activeAttacks.end(),
                       []( const Attack& a ) { return !a.active; } ),
       activeAttacks.end() );

#ifdef DEBUG
   DrawRectangleLines( playerPosition.x + 10, playerPosition.y, 20, 40, RED );
#endif
}

//-----------------------------------------------------------------------------
void Player_t::onEvent( Event_t& e )
{
   EventDispatcher_t dispatcher( e );
   dispatcher.Dispatch<KeyPressedEvent_t>(
       BIND_EVENT_FN( Player_t::handleKeyEvent ) );
}

//-----------------------------------------------------------------------------
bool Player_t::handleKeyEvent( KeyPressedEvent_t& e )
{
   if ( e.getKeyCode() == KEY_SPACE )
   {
      fireCooldown -= GetFrameTime();

      if( fireCooldown <= 0.0f )
      {
         castAttack();
         fireCooldown = FIRE_RATE;
      }
   }

   return false;
}

//-----------------------------------------------------------------------------
void Player_t::castAttack()
{
   if ( activeAttacks.size() >= MAX_ACTIVE_ATTACKS )
   {
      return;
   }

   Vector2 playerCenter = { playerPosition.x + 20.0f,
                            playerPosition.y + 20.0f };
   Vector2 spawnOffset  = Vector2Scale( lastDirection, 10.0f );

   auto& atk    = activeAttacks.emplace_back();
   atk.type     = AttackType::FIRE;
   atk.position = Vector2Add( playerCenter, spawnOffset );
   atk.velocity = Vector2Scale( lastDirection, 200.0f );
   atk.maxFrames = 8;
       //getAttackTexture( atk.type ).width / 8;   // 8 Number of sprites
   atk.sourceRec = { 0, 0, 72, 72 };
   atk.rotation  = Vector2Angle( lastDirection, { 1, 0 } ) * RAD2DEG *
                  -1.0f;   // -1 noetig, da initial das sprite nach rechts zeigt
   atk.active = true;
}

//-----------------------------------------------------------------------------
void Player_t::updateAnimation( float dt )
{
   // Only animate if moving
   if ( velocity.x != 0.0f || velocity.y != 0.0f )
   {
      state = ENTITY_STATE::ACTIVE;
      animTimer += dt;

      // How long each frame should be shown
      float frameDuration = 1.0f / framesSpeed;

      // Advance frame when time is up
      if ( animTimer >= frameDuration )
      {
         animTimer -= frameDuration;   // subtract = smoother than reset
         currentFrame = ( currentFrame + 1 ) % 9;   // 9 frames per walk cycle
         frameRec.x   = ( float ) currentFrame * frameWidth;
      }

      if ( fabsf( velocity.x ) > fabsf( velocity.y ) )
      {
         frameRec.y = ( velocity.x > 0.0f ) ? ANIMATION_WALK_RIGHT_Y
                                            : ANIMATION_WALK_LEFT_Y;
      }
      else
      {
         frameRec.y = ( velocity.y > 0.0f ) ? ANIMATION_WALK_DOWN_Y
                                            : ANIMATION_WALK_UP_Y;
      }

      // Remember direction for idle pose
      lastDirection = Vector2Normalize( velocity );
   }
   else
   {
      state = ENTITY_STATE::IDLE;
      animTimer += dt;
      // How long each frame should be shown
      float frameDuration = 1.0f / framesSpeed;

      if ( animTimer >= frameDuration )
      {
         // 7 Is the amount of sprites i have for the current animation
         currentFrame = ( currentFrame + 1 ) % 7;
         frameRec.x   = ( float ) currentFrame * frameWidth;
         frameRec.y   = ANIMATION_IDLE_DOWN_Y;
         animTimer -= frameDuration;
      }
   }
}

//-----------------------------------------------------------------------------
void Player_t::update( float dt )
{
   Vector2 input{ 0, 0 };
   if ( IsKeyDown( KEY_D ) )
      input.x += 1;
   if ( IsKeyDown( KEY_A ) )
      input.x -= 1;
   if ( IsKeyDown( KEY_S ) )
      input.y += 1;
   if ( IsKeyDown( KEY_W ) )
      input.y -= 1;

   if ( Vector2Length( input ) > 0.1f )
      velocity = Vector2Scale( Vector2Normalize( input ), SPEED );
   else
      velocity = { 0, 0 };

   updateAttacks( dt );
   // Animations
   updateAnimation( dt );
}

//-----------------------------------------------------------------------------
void Player_t::updateAttacks( float dt )
{
   for ( auto& atk : activeAttacks )
   {
      if ( atk.active )
      {
         atk.position.x += atk.velocity.x * GetFrameTime();
         atk.position.y += atk.velocity.y * GetFrameTime();
         atk.timer += GetFrameTime();

         if ( atk.timer >= 0.1f )
         {   // 10 FPS anim
            atk.currentFrame = ( atk.currentFrame + 1 ) % atk.maxFrames;
            atk.sourceRec.x  = atk.currentFrame * 72;
            atk.sourceRec.y  = 0;
            atk.timer        = 0;
         }

         // Deactivate off-screen or on-hit
         if ( atk.position.x > GetRenderWidth() + 100 ||
              atk.position.x < 0 - 100 ||
              atk.position.y > GetRenderHeight() + 100 ||
              atk.position.y < 0 - 100 )
         {
            atk.active = false;
         }
      }
   }
}

//-----------------------------------------------------------------------------
Texture2D& Player_t::getAttackTexture( AttackType type )
{
   return attackTextures[ static_cast<std::size_t>( type ) ];
}

//-----------------------------------------------------------------------------
void Player_t::drawAttackDebugInfo()
{
   if ( !ImGui::CollapsingHeader( "Active Player Attacks" ) )
      return;

   if ( activeAttacks.empty() )
   {
      ImGui::TextColored( ImVec4( 1, 1, 0, 1 ), "No active attacks" );
      return;
   }

   const int columns = 9;
   if ( ImGui::BeginTable( "AttacksTable", columns,
                           ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                               ImGuiTableFlags_Resizable |
                               ImGuiTableFlags_ScrollY ) )
   {
      // Header
      ImGui::TableSetupColumn( "ID", ImGuiTableColumnFlags_WidthFixed, 40 );
      ImGui::TableSetupColumn( "Type", ImGuiTableColumnFlags_WidthFixed, 80 );
      ImGui::TableSetupColumn( "Pos X", ImGuiTableColumnFlags_WidthFixed, 80 );
      ImGui::TableSetupColumn( "Pos Y", ImGuiTableColumnFlags_WidthFixed, 80 );
      ImGui::TableSetupColumn( "Vel X", ImGuiTableColumnFlags_WidthFixed, 80 );
      ImGui::TableSetupColumn( "Vel Y", ImGuiTableColumnFlags_WidthFixed, 80 );
      ImGui::TableSetupColumn( "Frame", ImGuiTableColumnFlags_WidthFixed, 60 );
      ImGui::TableSetupColumn( "Timer", ImGuiTableColumnFlags_WidthFixed, 70 );
      ImGui::TableSetupColumn( "Rotation", ImGuiTableColumnFlags_WidthFixed,
                               80 );
      ImGui::TableHeadersRow();

      // Rows - one per active attack
      for ( size_t i = 0; i < activeAttacks.size(); ++i )
      {
         const auto& atk = activeAttacks[ i ];
         if ( !atk.active )
            continue;

         ImGui::TableNextRow();

         ImGui::TableSetColumnIndex( 0 );
         ImGui::Text( "%zu", i );   // Index as ID

         ImGui::TableSetColumnIndex( 1 );
         switch ( atk.type )
         {
         case AttackType::FIRE:
            ImGui::Text( "Fire" );
            break;
         case AttackType::LIGHTNING:
            ImGui::Text( "Lightning" );
            break;
         case AttackType::WATER:
            ImGui::Text( "Water" );
            break;
         case AttackType::EARTH:
            ImGui::Text( "Earth" );
            break;
         }

         ImGui::TableSetColumnIndex( 2 );
         ImGui::Text( "%.1f", atk.position.x );
         ImGui::TableSetColumnIndex( 3 );
         ImGui::Text( "%.1f", atk.position.y );

         ImGui::TableSetColumnIndex( 4 );
         ImGui::Text( "%.1f", atk.velocity.x );
         ImGui::TableSetColumnIndex( 5 );
         ImGui::Text( "%.1f", atk.velocity.y );

         ImGui::TableSetColumnIndex( 6 );
         ImGui::Text( "%d/%d", atk.currentFrame, atk.maxFrames );

         ImGui::TableSetColumnIndex( 7 );
         ImGui::Text( "%.3f", atk.timer );

         ImGui::TableSetColumnIndex( 8 );
         ImGui::Text( "%.1f°", atk.rotation );
      }

      ImGui::EndTable();
   }

   ImGui::Separator();
   ImGui::Text( "Total active: %zu / %d", activeAttacks.size(),
                MAX_ACTIVE_ATTACKS );
}
