// 
//  path.h
//  Core
//  
//  Created by Wess Cope on 2010-11-26.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#ifndef __path_h__
#define __path_h__

#include "common.h"
#include <map>
#include <algorithm>

using namespace std;

namespace Core
{
   class Path
   {
   public:
      Path();
      ~Path();

      static size_t LastSlash(string pathname);
      static string Normalize(string pathname);
      static string Filename(string pathname);
      static string Dirname(string pathname);
      static bool FileExtension(string pathname, string &resultPath, string &extension);
      static bool IsAbsolute(string pathname);
      static bool FileExists(string pathname);
      static bool DirExists(string pathname);
      static string Cwd();
      static int Chdir(string pathname);

   };
}

#endif