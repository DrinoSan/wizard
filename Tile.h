#include "raylib.h"

//-----------------------------------------------------------------------------
enum class TileType_t
{
   COLLISION,
   DAMAGE,
   FREEZ,
   NO_COLLISION
};

//-----------------------------------------------------------------------------
class Tile_t : public Graphic_t
{
 public:
   Tile_t( TileType_t type_, int32_t indexInSrc_, Rectangle tileSrc_,
           Rectangle tileDest_, int32_t i = 0 )
       : tileType{ type_ }, tileSrc{ tileSrc_ }, tileDest{ tileDest_ }
   {
   }
   ~Tile_t() = default;

   void draw() override {}

   // @TODO: Check if this will be needed
   // void onEvent( Event_t& e ) override {}

   // Just the index
   int32_t i;
   TileType_t tileType;
   Rectangle  tileSrc  = { 0.0f, 0.0f, 16.0f, 16.0f };
   Rectangle  tileDest = { 0.0f, 0.0f, static_cast<float>( 800 / 30.0f ),
                           static_cast<float>( 450 / 20.0f ) };
   int32_t    indexInSrc{};
};
