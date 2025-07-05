#pragma once

// System Header
#include <cstdint>

enum OBJECT_TYPE
{
   PLAYER,
   STATIC,
};

class Graphic_t
{
 public:
   Graphic_t( OBJECT_TYPE type_ ) : type{ type_ } {}
   virtual ~Graphic_t();

   virtual void draw()           = 0;
   virtual void handleMovement() = 0;

   OBJECT_TYPE type;
};
