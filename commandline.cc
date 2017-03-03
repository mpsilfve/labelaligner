#include "commandline.hh"

#include <sstream>
#include <iostream>
#include <cstdlib>

int get_int(const std::string &s, const std::string &prefix)
{
  std::istringstream in(s.substr(prefix.size()));
  int i;
  in >> i;
  return i;
}

float get_float(const std::string &s, const std::string &prefix)
{
  std::istringstream in(s.substr(prefix.size()));
  float f;
  in >> f;
  return f;
}

const char * get_string(const char * s, const std::string &prefix)
{return s + prefix.size();}

ArgMap getargs(int argc, const char * argv[])
{
  ArgMap m;

  m["alpha"].f = .5;
  m["train_iters"].n = 1000;
  m["test_iters"].n = 1000;
  m["epochs"].n = 5;

  for (int i = 1; i < argc; ++i)
    {
      std::string arg_and_val(argv[i]);
      
      if (arg_and_val.find("--train_file=") == 0)
	{ m["train_file"].str = get_string(argv[i],"--train_file="); }
      else if (arg_and_val.find("--test_file=") == 0)
	{ m["test_file"].str = get_string(argv[i],"--test_file="); }
      else if (arg_and_val.find("--epochs=") == 0)
	{ m["epochs"].n = get_int(arg_and_val,"--epochs="); }
      else if (arg_and_val.find("--alpha=") == 0)
	{ m["alpha"].f = get_float(arg_and_val,"--alpha="); }
      else if (arg_and_val.find("--train_iters=") == 0)
	{ m["train_iters"].n = get_int(arg_and_val,"--train_iters="); }
      else if (arg_and_val.find("--test_iters=") == 0)
	{ m["test_iters"].n = get_int(arg_and_val,"--test_iters="); }
      else
	{
	  std::cerr << "Invalid commandline argument " << arg_and_val << std::endl;
	  exit(1);
	}
    }
  return m;
}

#ifdef TEST_commandline_cc
#include <cassert>

int main()
{
  int argc = 3;
  const char * argv[] = {"foo", "--file=foo","--alpha=0.5","--epochs=3"};

  ArgMap m = getargs(argc,argv);

  assert(std::string(m["file"].str) == "foo");
  assert(m["alpha"].f == .5);
  assert(m["epochs"].n == 3);
}

#endif // TEST_commandline_cc
