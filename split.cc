#include "split.hh"

#include <cstdlib>
#include <cassert>

std::vector<std::string> ZEROS =  {"Z1", "Z2"};

Split::Split(const std::string &str):
  str(str),
  pos("")
{
  boundaries.push_back(0);
  boundaries.push_back(str.size());
}

Split::Split(const std::vector<int> &boundaries, const std::string &str):
  str(str),
  boundaries(boundaries)
{}

void Split::set_pos(const std::string &pos)
{ this->pos = pos; }

int Split::size(void) const
//{ return boundaries.size() - 1; }
{ return boundaries.size(); }

std::string Split::get(int i) const
{ 
  if (i < boundaries.size() - 1)
    return str.substr(boundaries[i], boundaries[i+1] - boundaries[i]); 
  return "";
  //  return ZEROS[i + 1 - boundaries.size()];
    //return "Z1";
}

void Split::sample_next(int max_splits)
{
  while (1)
    {
      double random = rand() * 1.0 / RAND_MAX;
     
      if (random < .33 and boundaries.size() < max_splits)
	{ 
	  divide(); 
	  break;
	}    
      else if (random < .67)
	{ 
	  if (boundaries.size() > 2)
	    {
	      join(); 
	      break;
	    }
	}
      else if (boundaries.size() > 2)
	{
	  move();
	  break;
	}     
    }

  for (int i = 0; i < boundaries.size() - 1; ++i)
    { assert(boundaries[i] < boundaries[i+1]); }
}

void Split::divide(void)
{
  // Sample random boundary in the range [0, boundaries.size() - 1].
  int rndind = rand() % (boundaries.size() - 1);
  int split_size = boundaries[rndind + 1] - boundaries[rndind];
  if (split_size > 2)
    {
      int new_boundary = boundaries[rndind] + rand() % (split_size - 2) + 1;
      boundaries.insert(boundaries.begin() + rndind + 1, new_boundary);
    }
  else if (split_size > 1)
    {
      boundaries.insert(boundaries.begin() + rndind + 1, boundaries[rndind] + 1);
    }
}

void Split::join(void)
{
  // Sample random boundary in the range [1, boundaries.size() - 1].
  int rndind = rand() % (boundaries.size() - 2) + 1;
  boundaries.erase(boundaries.begin() + rndind);
}

void Split::move(void)
{
  // Sample random boundary in the range [1, boundaries.size() - 1].
  long rndind = rand();

  rndind =  (rndind % (boundaries.size() - 2)) + 1;
  double r = rand() * 1.0 / RAND_MAX;

  if (r < .5)
    {
      // Move left.
      if (boundaries[rndind] - boundaries[rndind - 1] > 1)
	{ --boundaries[rndind]; }
    }
  else
    {
      // Move right.
      if (boundaries[rndind + 1] - boundaries[rndind] > 1)
	{ ++boundaries[rndind]; }
    }
}

#ifdef TEST_split_cc

class TESTSplit
{
public:
  void divide(Split &s)
  { s.divide(); }
  void join(Split &s)
  { s.join(); }
  void move(Split &s)
  { s.move(); }
};

#include <iostream>
#include <ctime>
#include <set>

int main(void)
{
  Split s("qwerty");
  TESTSplit t;
  srand(time(NULL));
  
  assert(s.size() == 1);
  assert(s.get(0) == "qwerty");
  
  t.divide(s);
  assert(s.size() == 2);
  assert(s.get(0) + s.get(1) == "qwerty");

  for (int i = 0; i < 100; ++i)
    {
      t.move(s);
      assert(s.size() == 2);
      assert(s.get(0) + s.get(1) == "qwerty");
    }

  t.join(s);
  assert(s.size() == 1);
  assert(s.get(0) == "qwerty");
  std::string qwerty = "qwerty";

  std::set<std::string> all_substrs;
  for (int i = 0; i < qwerty.size(); ++i)
    {
      for (int j = i+1; j < qwerty.size()+1; ++j)
	{
	  all_substrs.insert(qwerty.substr(i,j-i));
	}
    }

  std::set<std::string> sampled_substrs;
  for (int i = 0; i < 100000; ++i)
    {
      s.sample_next(100);

      for (int j = 0; j < s.size(); ++j)
	{
	  sampled_substrs.insert(s.get(j));
	}
    }

  assert(all_substrs == sampled_substrs);
}

#endif // TEST_split_cc
