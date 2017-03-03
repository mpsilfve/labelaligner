#ifndef HEADER_params_hh
#define HEADER_params_hh

#include <string>
#include <unordered_map>

#include "split.hh"
#include "data.hh"
#include "assignment.hh"

std::string get_pos(const StringVector &v);

class Params
{
public:  
  Params(float alpha);
  void sample_next(Split &s, Assignment &a, const StringVector &labels);
  void init(Data &d);
  float score(const std::string &split, const std::string &label);
  float score(const Split &s, const Assignment &a);
  float &joint_count(const std::string &str, const std::string &label);
  float &count(const std::string &str, bool is_label);
  void modify_counts(const Split &s, const Assignment &a, float incr);

private:
  std::unordered_map<std::string, std::unordered_map<std::string, float> > joint_counts;
  std::unordered_map<std::string,float> counts;
  std::unordered_map<std::string,float> split_counts;
  std::unordered_map<std::string, std::unordered_map<std::string, float> > comb_count;
  float segment_label_count;
  float alpha;
  int segment_count;

};

#endif // HEADER_params_hh
