#pragma once

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

// Implementation of list
typedef struct
{
   void*  data;
   size_t size;
   size_t capacity;
   size_t type_size;

} sand_list;

//-----------------------------------------------------------------------------
/// Function to push element e to the end of list l
/// @param: l sand_list is the destination
/// @param: e generic element defined by list type_size which will be pushe to
/// the end of the list
sand_list* sand_list_push_back( sand_list* l, void* e )
{
   if ( l->size < l->capacity )
   {
      // Casting to char* so i can calculate easier
      char* p = ( char* ) l->data;

      // Getting pointer offset where to insert
      p += l->size * l->type_size;

      memcpy( ( void ) p, e, l->type_size );

      l->size++;
      return l;
   }

   // Fresh list
   if ( l->size == 0 )
   {
      // Not checking for calloc return because if this fails everything should burn
      l->capacity = 4;
      l->data = calloc( l->capacity, l->type_size );

      // Casting to char* so i can calculate easier
      char* p = ( char* ) l->data;

      // Getting pointer offset where to insert
      p += l->size * l->type_size;

      memcpy( ( void ) p, e, l->type_size );

      l->size++;
      return l;
   }

   // Need to extend the list and potentially copy stuff
   l->capacity *= 2; // Double the memory
   // Realloc does free the old pointer if a new block is captured
   l->data = realloc( l->data, l->capacity * l->type_size );

   if( l->data == NULL )
   {
      printf( "[sand_lib] Failed to increase memory block in push_back\n" );
      exit(1);
   }

   // Casting to char* so i can calculate easier
   char* p = ( char* ) l->data;

   // Getting pointer offset where to insert
   p += l->size * l->type_size;

   memcpy( ( void ) p, e, l->type_size );

   l->size++;

   return l;
}

//-----------------------------------------------------------------------------
/// Function to create a empty list
/// If memory allocation fails there is no need to continue
/// @param: type_size needed for generic work
/// @return: Pointer to empty list
sand_list* sand_list_create( size_t type_size )
{
   sand_list* l = calloc( 1, type_size );
   if ( l == NULL )
   {
      printf( "[sand_list] cant init allocate memory for list\n" );
      exit( 1 );
   }

   l->size     = 0;
   l->capacity = 0;
}

//-----------------------------------------------------------------------------
void sand_list_free( sand_list* l )
{
   free( l );
}
