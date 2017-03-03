#ifndef HEADER_unnormalized_measure_hh
#define HEADER_unnormalized_measure_hh

#include <map>
#include <string>
#include <utility>

typedef std::pair<int,std::string> SIPair;

class UnnormalizedMeasure
{
public:
  SIPair sample(void) const;
  float &get(int k1, const std::string &k2);
  void remove_first_matching(int key);
  void remove_second_matching(const std::string &key);
  int empty(void) const;
  int size(void) const;
private:
  std::map<SIPair, float> measure;
};

#endif // HEADER_unnormalized_measure_hh
