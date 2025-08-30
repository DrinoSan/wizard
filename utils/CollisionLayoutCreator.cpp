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

         // @TODO: Make this better currently i assu below 49 is a tile which
         // should have a collision
         std::string element{ tail, head };
         char        tileType{ 'C' };
         if ( std::stoi( element ) > 42 )
         {
            tileType = 'N';
         }

         line.push_back( element + "-" + tileType );

         ++head;
         tail = head;
      }

      layout.push_back( line );
      line.clear();
   }

   std::ofstream out( "tilemapWithTypes.sand" );

   for ( const auto& line : layout )
   {
      for ( const auto& c : line )
      {
         out << c + ',';
         std::cout << c << ',' << '\t';
      }
      std::cout << '\n';
   }

   out.close();
   f.close();
}
