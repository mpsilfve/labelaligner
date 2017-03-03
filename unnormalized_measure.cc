#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>

#include "unnormalized_measure.hh"

SIPair UnnormalizedMeasure::sample(void) const
{
  assert(not measure.empty());

  float tot = 0;

  for (auto p: measure)
    { tot += p.second; }

  float r = tot * rand() * 1.0 / RAND_MAX;
  float acc = 0;

  const SIPair  * last;

  for (auto p: measure)
    {
      acc += p.second;
      if (acc >= r)
	{ return p.first; }
      last = &(p.first);
    }

  return *last;
}

int UnnormalizedMeasure::empty(void) const
{ return measure.empty(); }

int UnnormalizedMeasure::size(void) const
{ return measure.size(); }

float &UnnormalizedMeasure::get(int k1, const std::string &k2)
{ return measure[SIPair(k1,k2)]; }

void UnnormalizedMeasure::remove_second_matching(const std::string &key)
{
  for(std::map<SIPair, float>::iterator iter = measure.begin(); 
      iter != measure.end(); 
      ) 
    {
      if (iter->first.second == key)
	{ measure.erase(iter++); } 
      else 
	{ ++iter; }
    }
}

void UnnormalizedMeasure::remove_first_matching(int key)
{
  for(std::map<SIPair, float>::iterator iter = measure.begin(); 
      iter != measure.end(); 
      ) 
    {
      if (iter->first.first == key)
	{ measure.erase(iter++); } 
      else 
	{ ++iter; }
    }
}

#ifdef TEST_unnormalized_measure_cc
int main(void)
{
  srand(time(NULL));

  UnnormalizedMeasure m;
  m.get("foo","bar") = 3;
  assert(m.get("foo","bar") == 3);

  m.remove_matching("foo",0);
  m.remove_matching("bar",1);
  assert(m.get("foo","bar") == 3);

  m.remove_matching("bar",0);
  assert(m.get("foo","bar") == 0);

  m.get("foo","foo") = 3;
  m.remove_matching("foo",1);
  assert(m.get("foo","bar") == 0);

  UnnormalizedMeasure n;
  n.get("a","a") = 10;
  n.get("b","b") = 0.01;

  for (int i = 0; i < 10; ++i)
    {
      assert(n.sample() == SIPair("a","a"));
    }
}
#endif // TEST_unnormalized_measure_cc
