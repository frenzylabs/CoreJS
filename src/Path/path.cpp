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
   /**
    * Class construct method
    */
   Path::Path(){}
   
   /**
    * Class deconstruct method
    */
   Path::~Path() {}
   
   /**
    * Method find the last slash of path
    *
    * @param {string} path to get last slash
    */
   size_t Path::LastSlash(string pathname)
   {
      size_t pos = pathname.find_last_of('/');
      if(pos == string::npos)
         pos = pathname.find_last_of('\\');
         
      return pos;
   }
   
   /**
    * Take a path string and normalize (., ..).
    *
    * @param {string} path to use.
    */
   string Path::Normalize(string pathname)
   {
      char *cPath = new char[PATH_MAX];
      realpath(pathname.c_str(), cPath);
      string res(cPath);
      delete[] cPath;
      
      return res;
   }
   
   /**
    * Grab the filename from a path
    *
    * @param {string} path with file.
    */
   string Path::Filename(string pathname)
   {
      size_t pos = Path::LastSlash(pathname);
      if(pos != string::npos)
         pathname.erase(pos, pathname.length() - pos);
         
      return pathname;
   }
   
   /**
    * Get the directory (last directory) from a path
    *
    * @param {string} path to parse for directory name
    */
   string Path::Dirname(string pathname)
   {
      size_t pos = Path::LastSlash(pathname);
      if(pos != string::npos)
         pathname.erase(pos, pathname.length() - pos);
         
      return pathname;
   }
   
   /**
    * Test a path for file and add/resolve file's extension
    *
    * @param {string} path to file
    * @param {string} reference to the final path with extension
    * @param {string} reference to extension string to populate with resulting extension.
    */
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
   
   /**
    * Test if the given path is absolute.
    *
    * @param {string} path to test
    */
   bool Path::IsAbsolute(string pathname)
   {
      return (pathname.length() == 0)? false : (pathname.at(0) == '/');
   }
   
   /**
    * Test to see if a file exists
    *
    * @param {string} path with filename
    */
   bool Path::FileExists(string pathname)
   {
      struct stat st;
      if(stat(pathname.c_str(), &st) != 0)
         return false;
         
      if(st.st_mode & S_IFDIR)
         return false;
         
      return true;
   }
   
   /**
    * Test to see if a directory exists
    *
    * @param {string} path to test
    */
   bool Path::DirExists(string pathname)
   {
      struct stat st;
      if(stat(pathname.c_str(), &st) != 0)
         return false;
         
      if(st.st_mode & S_IFDIR)
         return true;
         
      return false;
   }
   
   /**
    * Path method to return a string of the current working directory
    */
   string Path::Cwd()
   {
      char *buffer = getcwd(NULL, 0);
      string result(buffer);
      free(buffer);
      
      return result;
   }
   
   /**
    * Change current working directory to path given
    *
    * @param {string} path to change to.
    */
   int Path::Chdir(string pathname)
   {
      return chdir(pathname.c_str());
   }
   
}
