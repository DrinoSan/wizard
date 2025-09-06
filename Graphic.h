#pragma once

// System Header
#include <cstdint>

// Project Header
#include "events/Event.h"
#include "events/KeyEvent.h"

//-----------------------------------------------------------------------------
class Graphic_t
{
 public:
   Graphic_t() = default;
   virtual ~Graphic_t();

   virtual void draw() = 0;
};
