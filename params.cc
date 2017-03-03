#include "params.hh"

#include <cassert>
#include <iostream>
#include <cmath>
#include <algorithm>

std::string get_pos(const StringVector &v)
{
  for (const std::string &s:v)
    {
      if (s.find("pos=") != std::string::npos)
	{ 
	  return s; 
	}
    }
  return "pos=?";
}

Params::Params(float alpha):
  alpha(alpha),
  segment_count(0),
  segment_label_count(0)
{}

void Params::sample_next(Split &s, Assignment &a, const StringVector &labels)
{
  Split new_split(s);
  new_split.sample_next(labels.size());
  Assignment new_a(labels, new_split, *this);
  // FIXME incr: -1
  //  modify_counts(s,a,-1);

  float new_score = score(new_split, new_a);

  if (new_score / a.get_score() > rand() * 1.0 / RAND_MAX)
    {
      s = new_split;
      a = new_a;
    }

  //modify_counts(s,a,1);
}

float &Params::joint_count(const std::string &substr, const std::string &label)
{ return joint_counts[substr]["LABEL:" + label]; }

float &Params::count(const std::string &str, bool is_label)
{ return counts[is_label ? "LABEL:" + str : str]; }

void Params::init(Data &d)
{
  for (int i = 0; i < d.size(); ++i)
    {
      const std::string &wf = d.get(i).first;
      const StringVector &labels = d.get(i).second;
      std::string pos = get_pos(labels);

      for (int j = 0; j < wf.size() + 1; ++j)
	{
	  for (int k = j + 1; k < wf.size() + 1; ++k)
	    {
	      for (int l = 0; l < labels.size(); ++l)
		{ 
		  joint_count(wf.substr(j, k - j), labels[l]) += 1; 
		  count(labels[l],1) += 1;
		  count(wf.substr(j, k - j),0) += 1;
		}
	    }
	}

      for (int l = 0; l < labels.size(); ++l)
	{ /*
	  for (int z = 0; z < ZEROCOUNT; ++z)
	    {
	      joint_count(ZEROS[z], labels[l]) += 1; 
	      count(labels[l],1) += 1;
	      count(ZEROS[z],0) += 1;
	    }
	  */
	  joint_count("", labels[l]) += 1;
	  count(labels[l],1) += 1;
	  //count(pos,0) += 1;
	  count("",0) += 1;

	  //	  joint_count("Z2", labels[l]) += 1; 
	  //count(labels[l],1) += 1;
	  //count("Z2",0) += 1;

	  comb_count[wf][labels[l]] += 1;
	  segment_label_count += 1;
	}

      split_counts[wf] += 1;
      segment_count += 1;
    }
}

float Params::score(const std::string &split, const std::string &label)
{ return pow(joint_count(split, label),2)/(count(split,0)*count(label,1)); }
/*
float Params::score(const std::string &split, const std::string &label)
{ return (split_counts[split] == 0 ? alpha : split_counts[split])/(segment_count + alpha) *  
    joint_count(split, label)/count(label,1); }

float Params::score(const std::string &split, const std::string &label)
{ return (comb_count[split][label] == 0 ? alpha : comb_count[split][label]) / (segment_label_count + alpha) * pow(joint_count(split, label),2)/(count(split,0)*count(label,1)); }
*/

float Params::score(const Split &s, const Assignment &a)
{ 
  assert(s.size() + 1 == a.size());

  float split_score = 1;

  for (int i = 0; i < s.size(); ++i)
    {
      for (auto l: a.get(i))
	{ split_score *= score(s.get(i), l); }
    }

  for (auto l: a.get(s.size()))
    { split_score *= score("", l); }

  return split_score;
}

void Params::modify_counts(const Split &s, const Assignment &a, float incr)
{
  for (int i = 0; i < s.size(); ++i)
    {
      for (auto l: a.get(i))
	{ 
	  joint_count(s.get(i), l) += incr;
	  count(s.get(i), 0) += incr;
	  count(l, 1) += incr;

	  comb_count[s.get(i)][l] += incr;
	  segment_label_count += incr;
	}
      split_counts[s.get(i)] += incr;
      segment_count =+ incr;
    }

  for (auto l: a.get(s.size()))
    { 
      joint_count("", l) += incr;
      count("", 0) += incr;
      count(l, 1) += incr;

      comb_count[""][l] += incr;
      segment_label_count += incr;
    }

  if (a.get(s.size()).size() > 0)
    { 
      split_counts[""] += incr; 
      segment_count =+ incr;
    }
}

#ifdef TEST_params_cc
#include <map>

int main(void)
{
  Params params;
  Data data;
  for (int i = 0; i < 100; ++i)
    {
      StringVector labels1 = {"N","PLN","IN+ESS"};
      StringVector labels2 = {"N","SGN","IN+ESS"};
      StringVector labels3 = {"N","PLN","GEN"};
      StringVector labels4 = {"N","SGN","GEN"};
      data.add(LabeledString("koirissa",labels1));
      data.add(LabeledString("koirassa",labels2));
      data.add(LabeledString("koirien",labels3));
      data.add(LabeledString("koiran",labels4));
      data.add(LabeledString("kissoissa",labels1));
      data.add(LabeledString("kissassa",labels2));
      data.add(LabeledString("kissojen",labels3));
      data.add(LabeledString("kissan",labels4));
      data.add(LabeledString("palloissa",labels1));
      data.add(LabeledString("pallossa",labels2));
      data.add(LabeledString("pallojen",labels3));
      data.add(LabeledString("pallon",labels4));
    }

  params.init(data);

  Split s("koirissa");
  std::vector<std::string> labels = {"IN+ESS","PLN","N"};
  Assignment a(labels, s, params);

  float max_score = -1;
  std::map<std::string, std::string> m;

  std::cout << params.score("koiri","PLN") << std::endl;
  std::cout << params.score("i","PLN") << std::endl;
  std::cout << params.score("koir","N") << std::endl;
  std::cout << params.score("ssa","N") << std::endl;

  for (int i = 0; i < 1000 * 100; ++i)
    {
      params.sample_next(s,a,labels);
      
      float score = params.score(s,a);
      if (score > max_score)
	{
	  max_score = score;

	  m.clear();
	  for (int j = 0; j < s.size(); ++j)
	    {
	      m[s.get(j)] = "";
	      for (auto l: a.get(j))
		{
		  m[s.get(j)] += l;
		}
	    }
	}
    }

  for (std::map<std::string, std::string>::const_iterator it = m.begin();
       it != m.end();
       ++it)
    {
      std::cout << it->first << ' ' << it->second << std::endl;
    }
  std::cout << max_score << std::endl;
}

#endif // TEST_params_cc
