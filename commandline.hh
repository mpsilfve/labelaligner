#ifndef HEADER_commandline_hh
#define HEADER_commandline_hh

#include <map>
#include <string>

union Arg
{
  int n;
  float f;
  const char * str;
};

typedef std::map<std::string, Arg> ArgMap;

ArgMap getargs(int argc, const char * argv[]);

#endif // HEADER_commandline_hh
