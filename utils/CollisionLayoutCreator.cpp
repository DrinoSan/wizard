#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

int main()
{
   std::ifstream f( "resources/dungonmapAdjusted.tmx" );

   if ( !f.is_open() )
   {
      std::cerr << "Error opening the file!";
      return 1;
   }

   std::vector<std::vector<std::string>> layout;
   std::vector<std::string>              line;
   std::string                           lineString;
   while ( getline( f, lineString ) )
   {
      const char* head = lineString.data();
      const char* tail = head;
      while ( *head != '\0' )
      {
         if ( *head != ',' )
         {
            ++head;
            continue;
         }

         line.push_back( { tail, head } );

         ++head;
         tail = head;
      }

      layout.push_back( line );
      line.clear();
   }

   for ( const auto& line : layout )
   {
      for ( const auto& c : line )
      {
         std::cout << c << ',';
      }
      std::cout << '\n';
   }

   f.close();
}
