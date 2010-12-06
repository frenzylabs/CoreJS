// 
//  path.cpp
//  Core
//  
//  Created by Wess Cope on 2010-11-26.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#include "path.h"
using namespace std;

namespace Core
{
   Path::Path(){}
   Path::~Path() {}
   
   size_t Path::LastSlash(string pathname)
   {
      size_t pos = pathname.find_last_of('/');
      if(pos == string::npos)
         pos = pathname.find_last_of('\\');
         
      return pos;
   }
   
   string Path::Normalize(string pathname)
   {
      char *cPath = new char[PATH_MAX];
      realpath(pathname.c_str(), cPath);
      string res(cPath);
      delete[] cPath;
      
      return res;
   }
   
   string Path::Filename(string pathname)
   {
      size_t pos = Path::LastSlash(pathname);
      if(pos != string::npos)
         pathname.erase(pos, pathname.length() - pos);
         
      return pathname;
   }
   
   string Path::Dirname(string pathname)
   {
      size_t pos = Path::LastSlash(pathname);
      if(pos != string::npos)
         pathname.erase(pos, pathname.length() - pos);
         
      return pathname;
   }
   
   bool Path::FileExtension(string pathname, string &resultPath, string &extension)
   {
      vector<string> exts;
      exts.push_back("fm");
      exts.push_back("js");
      string path;      
      
      string fullpath = Path::Normalize(pathname);

      int extFindIndex = fullpath.find_first_of('.', fullpath.find_last_of('/') + 1);
      if(extFindIndex > 0)
      {
         string ext = fullpath.substr(extFindIndex + 1);
         
         vector<string>::iterator it;
         it = find(exts.begin(), exts.end(), ext);
         if(it == exts.end())
            return false;

         if(Path::FileExists(fullpath))
         {
            resultPath = fullpath;
            extension =  ext;

            return true;
         }
      }
      else
      {
         for(int i = 0; i < 2; i++)
         {
            path = fullpath;
            path += ".";
            path += exts[i];

            if(Path::FileExists(path))
            {
               resultPath = path;
               extension =  exts[i];

               return true;
            }
         }         
      }

      return false;
   }
   
   bool Path::IsAbsolute(string pathname)
   {
      return (pathname.length() == 0)? false : (pathname.at(0) == '/');
   }
   
   bool Path::FileExists(string pathname)
   {
      struct stat st;
      if(stat(pathname.c_str(), &st) != 0)
         return false;
         
      if(st.st_mode & S_IFDIR)
         return false;
         
      return true;
   }
   
   bool Path::DirExists(string pathname)
   {
      struct stat st;
      if(stat(pathname.c_str(), &st) != 0)
         return false;
         
      if(st.st_mode & S_IFDIR)
         return true;
         
      return false;
   }
   
   string Path::Cwd()
   {
      char *buffer = getcwd(NULL, 0);
      string result(buffer);
      free(buffer);
      
      return result;
   }
   
   int Path::Chdir(string pathname)
   {
      return chdir(pathname.c_str());
   }
   
}
