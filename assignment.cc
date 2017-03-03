#include "assignment.hh"
#include "unnormalized_measure.hh"
#include "params.hh"

#include <map>
#include <cassert>
#include <algorithm>
#include <iostream>

Assignment::Assignment(const std::vector<std::string> &labels,
		       const Split &s,
		       Params &params):
  assignment(s.size() + 1),
  score(-1),
  labels(&labels),
  s(&s)
{ 
  assert(s.size() - 1 <= labels.size());
  sample(params); 
}

float Assignment::get_score(void) const
{ return score; }

void Assignment::sample(Params &params)
{
  UnnormalizedMeasure m;
  std::map<std::string, int> split_ids;
  std::map<std::string, int> label_ids;
  for (int i = 0; i < s->size() - 1; ++i)
    {
      split_ids[s->get(i)] = i;
      for (int j = 0; j < labels->size(); ++j)
	{ 
	  label_ids[labels->at(j)] = j;
	  m.get(i, labels->at(j)) = 
	    params.score(s->get(i), labels->at(j)); 
	}
    }

  std::vector<bool> used_splits(s->size() - 1); 
  std::vector<bool> used_labels(labels->size()); 
  while (std::find(used_splits.begin(),
		   used_splits.end(), 0) != used_splits.end())
    {
      assert(not m.empty());
      SIPair p = m.sample();
      int split = p.first;
      std::string label = p.second;
      used_splits[split] = 1;
      used_labels[label_ids[label]] = 1;
      m.remove_first_matching(split);
      assignment[split].push_back(label);
      m.remove_second_matching(label);
    }

  for (int i = 0; i < labels->size(); ++i)
    {
      if (used_labels.at(i))
	{ continue; }
      UnnormalizedMeasure m;
      for (int j = 0; j < s->size(); ++j)
	{ m.get(j, labels->at(i)) = 
	    params.score(s->get(j), labels->at(i)); }

      SIPair p = m.sample();
      assignment[p.first].push_back(labels->at(i));
    }
  score = params.score(*s,*this);
}

int Assignment::size(void) const
{ return assignment.size(); }

const std::vector<std::string> &Assignment::get(int i) const
{ return assignment[i]; }

#ifdef TEST_assignment_cc
#include <map>
int main(void)
{
  Params params;
  Data data;
  for (int i = 0; i < 100; ++i)
    {
      data.add(LabeledString("ssa",StringVector(1,"IN+ESS")));
      data.add(LabeledString("i",StringVector(1,"PLN")));
      data.add(LabeledString("koir",StringVector(1,"N")));
    }

  data.add(LabeledString("ssa",StringVector(1,"N")));
  data.add(LabeledString("ssa",StringVector(1,"PLN")));

  data.add(LabeledString("i",StringVector(1,"IN+ESS")));
  data.add(LabeledString("i",StringVector(1,"N")));

  data.add(LabeledString("koir",StringVector(1,"PLN")));
  data.add(LabeledString("koir",StringVector(1,"IN+ESS")));

  params.init(data);

  std::vector<std::string> labels = {"IN+ESS","PLN","N"};
  std::vector<int> boundaries = {0,4,5,8};
  Split s(boundaries,"koirissa");

  std::map<std::string, std::map<std::string, int> > counts;

  for (int i = 0; i < 1000; ++i)
    {
      Assignment a(labels, s, params);
      assert(a.size() == 3);
      assert(a.get(0).size() == 1);
      assert(a.get(1).size() == 1);
      assert(a.get(2).size() == 1);
      counts[s.get(0)][a.get(0)[0]] += 1;
      counts[s.get(1)][a.get(1)[0]] += 1;
      counts[s.get(2)][a.get(2)[0]] += 1;
    }

  assert(counts["koir"]["N"] > counts["koir"]["IN+ESS"]);
  assert(counts["koir"]["N"] > counts["koir"]["PLN"]);

  assert(counts["i"]["PLN"] > counts["i"]["IN+ESS"]);
  assert(counts["i"]["PLN"] > counts["i"]["N"]);

  assert(counts["ssa"]["IN+ESS"] > counts["ssa"]["N"]);
  assert(counts["ssa"]["IN+ESS"] > counts["ssa"]["PLN"]);

  for (int i = 0; i < 3 ; ++i)
    {
      for (int j = 0; j < 3; ++j)
	{
	  std::cerr << s.get(i) << ' ' << labels[j] << ' ' 
		    << counts[s.get(i)][labels[j]] << std::endl;
	}
    }
}

#endif // TEST_assignment_cc
