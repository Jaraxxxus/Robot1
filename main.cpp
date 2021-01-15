#include "Robots.h"
#include <Windows.h>
//#include "optionparser.h"
#include <iostream>
#include <string>
#include <vector>



int main(int argc, char* argv[] )
{
    std::string cheme;
    argc -= (argc > 0); argv += (argc > 0); // skip program name argv[0] if present
    if (argc > 0)
      cheme = argv[0];
    else
        cheme = "";
    
    
    
    
   

    Game game;
    game.Execute(cheme);


}