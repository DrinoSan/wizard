#include <iostream>

#include "raylib.h"

//-----------------------------------------------------------------------------
#define PRINT_RECT( rec ) \
   printRect( rec, __FILE__, __LINE__ )

inline void printRect( const Rectangle& rect, const std::string& file, const int64_t lineNum  )
{
#ifdef DEBUG
   std::cout << "[RECT] x.pos <" << rect.x << "> y.pos <" << rect.y
             << "> width <" << rect.width << "> rect.height <" << rect.height
             << "> - File " << file << " Line " << lineNum << "\n";
#endif
}
