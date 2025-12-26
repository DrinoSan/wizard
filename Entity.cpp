#include "Entity.h"

// ----------------------------------------------------------------------------
void Entity_t::drawLifeBar() const
{
   const float barWidth   = 30.0f;   // Width of the bar in pixels
   const float barHeight  = 5.0f;    // Height of the bar
   const float barOffsetY = -5.0f;

   float healthPercent = ( float ) lifePoints / 100.0f;
   healthPercent       = Clamp( healthPercent, 0.0f, 1.0f );

   Color barColor = ( healthPercent >= 0.2f ) ? GREEN : RED;

   float spriteWidth =
       40.0f;   // This is hardcoded but it is based on DrawTexutrePro when
                // drawing the sprite of the player. This will need to be
                // changed when using different sprites...

   float barCenterX =
       playerPosition.x + spriteWidth / 2.0f;   // Center of sprite

   Vector2 barPos = { barCenterX - barWidth / 2.0f,
                      playerPosition.y + barOffsetY };

   DrawRectangle( barPos.x, barPos.y, barWidth, barHeight, DARKGRAY );

   DrawRectangle( barPos.x, barPos.y, barWidth * healthPercent, barHeight,
                  barColor );

   // Outline
   DrawRectangleLines( barPos.x, barPos.y, barWidth, barHeight, BLACK );
}
