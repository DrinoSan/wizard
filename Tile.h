#include "raylib.h"

//-----------------------------------------------------------------------------
enum class TileType_t
{
   COLLISION,
   NO_COLLISION
};

//-----------------------------------------------------------------------------
constexpr TileType_t collisionMap[ 11 ][ 12 ] = {
    { TileType_t::COLLISION, TileType_t::COLLISION, TileType_t::COLLISION,
      TileType_t::NO_COLLISION, TileType_t::NO_COLLISION,
      TileType_t::NO_COLLISION, TileType_t::NO_COLLISION, TileType_t::COLLISION,
      TileType_t::COLLISION, TileType_t::COLLISION, TileType_t::NO_COLLISION,
      TileType_t::NO_COLLISION },
    { TileType_t::COLLISION, TileType_t::NO_COLLISION, TileType_t::NO_COLLISION,
      TileType_t::NO_COLLISION, TileType_t::COLLISION, TileType_t::COLLISION,
      TileType_t::NO_COLLISION, TileType_t::NO_COLLISION,
      TileType_t::NO_COLLISION, TileType_t::COLLISION, TileType_t::NO_COLLISION,
      TileType_t::NO_COLLISION },
    { TileType_t::NO_COLLISION, TileType_t::COLLISION, TileType_t::COLLISION,
      TileType_t::NO_COLLISION, TileType_t::NO_COLLISION, TileType_t::COLLISION,
      TileType_t::COLLISION, TileType_t::NO_COLLISION, TileType_t::NO_COLLISION,
      TileType_t::NO_COLLISION, TileType_t::COLLISION,
      TileType_t::COLLISION } };

//-----------------------------------------------------------------------------
class Tile_t : public Graphic_t
{
 public:
   Tile_t( TileType_t type_, int32_t indexInSrc_, Rectangle tileSrc_,
           Rectangle tileDest_ )
       : Graphic_t{ OBJECT_TYPE::STATIC }, tileType{ type_ },
         tileSrc{ tileSrc_ }, tileDest{ tileDest_ }
   {
   }
   ~Tile_t() = default;

   void draw() override {}

   void handleMovement() override {}

   TileType_t tileType;
   Rectangle  tileSrc  = { 0.0f, 0.0f, 16.0f, 16.0f };
   Rectangle  tileDest = { 0.0f, 0.0f, static_cast<float>( 800 / 30.0f ),
                           static_cast<float>( 450 / 20.0f ) };
   int32_t    indexInSrc{};
};
