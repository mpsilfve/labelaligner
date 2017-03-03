#ifndef HEADER_Assignment_hh
#define HEADER_Assignment_hh

#include <string>
#include <vector>

#include "Split.hh"

typedef std::vector<std::vector<std::string> > LabelAssignment;


class Params;

class Assignment
{
public:
  Assignment(const std::vector<std::string> &labels, 
	     const Split &s,
	     Params &params);

  int size(void) const;
  const std::vector<std::string> &get(int i) const;
  float get_score(void) const;
  void sample(Params &params);

private:
  LabelAssignment assignment;
  float score;
  const std::vector<std::string>  * labels;
  const Split  * s;
};

#endif // HEADER_Assignment_hh
