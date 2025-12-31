#include "PathFindingStrategy.h"
#include "Globals.h"
#include "NpcEnemy.h"
#include "Player.h"
#include "World.h"

#include <cmath>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

// ----------------------------------------------------------------------------
struct Node
{
   int32_t x;
   int32_t y;
   float   g;
   float   h;
   int32_t parentIdx;   // index in nodes vector
};

// ----------------------------------------------------------------------------
static inline int32_t toIndex( int32_t x, int32_t y, int32_t cols )
{
   return y * cols + x;
}

// ----------------------------------------------------------------------------
// Octile heuristic for 8-direction movement (diagonals allowed)
static inline float heuristic( int32_t x1, int32_t y1, int32_t x2, int32_t y2 )
{
   int32_t     dx = std::abs( x1 - x2 );
   int32_t     dy = std::abs( y1 - y2 );
   const float D  = 1.0f;
   const float D2 = std::sqrt( 2.0f );
   int32_t     mn = std::min( dx, dy );
   int32_t     mx = std::max( dx, dy );
   return D * ( mx ) + ( D2 - D ) * mn;
}

// ----------------------------------------------------------------------------
void A_StarStrategy_t::findPath( NpcEnemy_t& npc, const World_t& world,
                                 const Player_t& player ) const
{
   if ( world.worldMap.empty() )
      return;

   // Determine grid dimensions by counting tiles on the first row
   int32_t cols      = 0;
   float   firstRowY = world.worldMap[ 0 ].tileDest.y;
   for ( const auto& t : world.worldMap )
   {
      if ( t.tileDest.y == firstRowY )
         ++cols;
      else
         break;
   }
   if ( cols <= 0 )
      return;

   int32_t rows = static_cast<int32_t>( world.worldMap.size() / cols );

   float tileW = world.worldMap[ 0 ].tileDest.width;
   float tileH = world.worldMap[ 0 ].tileDest.height;

   // Convert world positions to tile coordinates (clamp to grid)
   auto posToTile = [ & ]( const Vector2& pos )
   {
      int32_t tx = static_cast<int32_t>( pos.x / tileW );
      int32_t ty = static_cast<int32_t>( pos.y / tileH );
      if ( tx < 0 )
         tx = 0;
      if ( tx >= cols )
         tx = cols - 1;
      if ( ty < 0 )
         ty = 0;
      if ( ty >= rows )
         ty = rows - 1;
      return std::pair<int32_t, int32_t>( tx, ty );
   };

   auto [ startX, startY ] = posToTile( npc.playerPosition );
   auto [ goalX, goalY ]   = posToTile( player.playerPosition );

   if ( startX == goalX && startY == goalY )
      return;

   int32_t gridSize = cols * rows;

   std::vector<float>   gScore( gridSize,
                                std::numeric_limits<float>::infinity() );
   std::vector<int32_t> parent( gridSize, -1 );
   std::vector<char>    closed( gridSize, 0 );

   // Min-heap by f = g + h
   struct PQItem
   {
      int32_t idx;
      float   f;
   };
   struct Cmp
   {
      bool operator()( PQItem const& a, PQItem const& b ) const
      {
         return a.f > b.f;
      }
   };
   std::priority_queue<PQItem, std::vector<PQItem>, Cmp> open;

   int32_t startIdx   = toIndex( startX, startY, cols );
   gScore[ startIdx ] = 0.0f;
   open.push( { startIdx, heuristic( startX, startY, goalX, goalY ) } );

   // 8-directional neighbors (including diagonals)
   const int32_t dir[ 8 ][ 2 ] = { { 1, 0 }, { -1, 0 }, { 0, 1 },  { 0, -1 },
                                   { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

   bool    found   = false;
   int32_t goalIdx = toIndex( goalX, goalY, cols );

   // Obstacle dilation disabled for now (padding = 0). If NPC is not
   // passing narrow corridors correctly, consider enabling padding based
   // on NPC hitbox size. Using padding=0 avoids over-blocking in current
   // setup.
   int32_t padding = 0;

   auto isBlocked = [ & ]( int32_t tx, int32_t ty )
   {
      // return true if any tile within padding square is COLLISION
      int32_t x0 = std::max( 0, tx - padding );
      int32_t x1 = std::min( cols - 1, tx + padding );
      int32_t y0 = std::max( 0, ty - padding );
      int32_t y1 = std::min( rows - 1, ty + padding );
      for ( int32_t yy = y0; yy <= y1; ++yy )
      {
         for ( int32_t xx = x0; xx <= x1; ++xx )
         {
            int32_t i = toIndex( xx, yy, cols );
            if ( world.worldMap[ i ].tileType == TileType_t::COLLISION )
               return true;
         }
      }
      return false;
   };

   while ( !open.empty() )
   {
      auto cur = open.top();
      open.pop();
      int32_t curIdx = cur.idx;
      if ( closed[ curIdx ] )
         continue;
      closed[ curIdx ] = 1;

      if ( curIdx == goalIdx )
      {
         found = true;
         break;
      }

      int32_t cx = curIdx % cols;
      int32_t cy = curIdx / cols;

      for ( auto& d : dir )
      {
         int32_t nx = cx + d[ 0 ];
         int32_t ny = cy + d[ 1 ];
         if ( nx < 0 || nx >= cols || ny < 0 || ny >= rows )
            continue;
         int32_t nIdx = toIndex( nx, ny, cols );

         // Skip blocked tiles (dilated by NPC size)
         if ( isBlocked( nx, ny ) )
            continue;

         // If moving diagonally, prevent corner cutting: don't allow diagonal
         // movement if either adjacent cardinal tiles are blocked.
         if ( std::abs( d[ 0 ] ) + std::abs( d[ 1 ] ) == 2 )   // diagonal
         {
            int32_t idx1 = toIndex( cx + d[ 0 ], cy, cols );
            int32_t idx2 = toIndex( cx, cy + d[ 1 ], cols );
            if ( world.worldMap[ idx1 ].tileType == TileType_t::COLLISION ||
                 world.worldMap[ idx2 ].tileType == TileType_t::COLLISION )
               continue;
         }

         // cost between neighbours: 1 for orthogonal, sqrt(2) for diagonal
         float moveCost   = ( std::abs( d[ 0 ] ) + std::abs( d[ 1 ] ) == 2 )
                                ? std::sqrt( 2.0f )
                                : 1.0f;
         float tentativeG = gScore[ curIdx ] + moveCost;
         if ( tentativeG < gScore[ nIdx ] )
         {
            gScore[ nIdx ] = tentativeG;
            parent[ nIdx ] = curIdx;
            float f        = tentativeG + heuristic( nx, ny, goalX, goalY );
            open.push( { nIdx, f } );
         }
      }
   }

   if ( !found )
      return;

   // Reconstruct path (as tile indices) from goal to start
   std::vector<int32_t> pathIdx;
   int32_t              cur = goalIdx;
   while ( cur != -1 )
   {
      pathIdx.push_back( cur );
      cur = parent[ cur ];
   }
   std::reverse( pathIdx.begin(), pathIdx.end() );
   // Store the path in the NPC so it can follow it across frames
   npc.pathIndices = std::move( pathIdx );
   npc.pathCursor  = 1;   // Idx 0 would be the start index we are already on...

   if ( npc.pathIndices.size() < 2 )
   {
      npc.pathCursor  = static_cast<int>( npc.pathIndices.size() );
      npc.needsRepath = true;
   }

   npc.repathTimer = 0.0f;
   npc.needsRepath = false;
   TraceLog(LOG_INFO, "A* called");
}
