#include "pathHistory.h"

PathHistory *PathHistory::instance = NULL;

vector<string> PathHistory::getPaths() 
{ 
   return requirePaths;  
}

void PathHistory::addPath(string path) 
{ 
   requirePaths.push_back(path); 
}

PathHistory *PathHistory::Instance() 
{ 
   if(!instance) 
      instance = new PathHistory; 

   return instance;
}
