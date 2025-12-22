#include <iostream>

#include "raylib.h"

//-----------------------------------------------------------------------------
#define PRINT_RECT( rec ) printRect( rec, __FILE__, __LINE__ )

//-----------------------------------------------------------------------------
inline void printRect( const Rectangle& rect, const std::string& file,
                       const int64_t lineNum )
{
#ifdef DEBUG
   std::cout << "[RECT] x.pos <" << rect.x << "> y.pos <" << rect.y
             << "> width <" << rect.width << "> rect.height <" << rect.height
             << "> - File " << file << " Line " << lineNum << "\n";
#endif
}

//-----------------------------------------------------------------------------
inline void printStringln( const std::string& msg )
{
#ifdef DEBUG
   std::cout << msg << "\n";
#endif
}

//-----------------------------------------------------------------------------
inline void printString( const std::string& msg )
{
#ifdef DEBUG
   std::cout << msg;
#endif
}

//-----------------------------------------------------------------------------
inline void drawTileWithNumber( const Rectangle& tileDest, int n )
{
   DrawRectangleLines( tileDest.x, tileDest.y, tileDest.width, tileDest.height,
                       GREEN );
   std::string nStr       = std::to_string( n );
   int         fontSize   = 10;
   Color       textColor  = ( n == 1 ) ? RED : WHITE;
   int         textWidth  = MeasureText( nStr.c_str(), fontSize );
   int         textHeight = fontSize;
   int         textX      = tileDest.x + ( tileDest.width - textWidth ) / 2;
   int         textY      = tileDest.y + ( tileDest.height - textHeight ) / 2;
   DrawText( nStr.c_str(), textX, textY, fontSize, textColor );
}

//-----------------------------------------------------------------------------
inline void drawGrid( const std::vector<Tile_t>& tiles )
{
   BeginDrawing();
   ClearBackground( RAYWHITE );
   for ( const auto& tile : tiles )
   {
      // drawTileWithNumber( tile.tileDest, tile.n );
      drawTileWithNumber( tile.tileDest, 1 );
   }
   EndDrawing();
}
