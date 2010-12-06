#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class PathHistory
{
private:
   PathHistory(){};
   static PathHistory *instance;
   
   vector<string> requirePaths;
   
public:
   vector<string> getPaths();
   void addPath(string path);
   static PathHistory *Instance();
};