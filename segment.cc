#include <iostream>
#include <vector>
#include <ctime>
#include <cassert>

#include "params.hh"
#include "split.hh"
#include "assignment.hh"
#include "commandline.hh"

typedef std::vector<Split> SplitVector;
typedef std::vector<Assignment> AssignmentVector;

int main(int argc, const char * argv[])
{
  ArgMap m = getargs(argc,argv);
  int N = m["epochs"].n;
  int train_iters = m["train_iters"].n;
  int test_iters = m["test_iters"].n;
  float alpha = m["alpha"].f;
  const char * train_fn = m["train_file"].str;
  const char * test_fn = m["test_file"].str;

  std::cerr << "Train File: " << train_fn << std::endl;
  std::cerr << "Test File: " << test_fn << std::endl;
  std::cerr << "Epochs: " << N << std::endl;
  std::cerr << "Alpha: " << alpha << std::endl;
  std::cerr << "train_iters: " << train_iters << std::endl;
  std::cerr << "test_iters: " << test_iters << std::endl;

  srand(time(NULL));

  std::cerr << "Read data." << std::endl;
  Data traindata(train_fn);
  Data testdata(test_fn);
  traindata.append(testdata);
  traindata.shuffle();

  std::cerr << "Initialize parameters." << std::endl;
  Params params(alpha);
  params.init(traindata);

  SplitVector splits;
  AssignmentVector assignments;

  for (int i = 0; i < traindata.size(); ++i)
    {
      Split s(traindata.get(i).first);
      s.set_pos(get_pos(traindata.get(i).second));
      Assignment a(traindata.get(i).second, s, params);
      splits.push_back(s);
      assignments.push_back(a);
    }

  std::cerr << "Train" << std::endl;

  for (int pass = 0; pass < N; ++pass)
    {
      std::cerr << "Epoch " << pass + 1 << " of " << N << '\r'; 
      for (int i = 0; i < traindata.size(); ++i)
	{
	  float max_score = -1;
	  Split max_split(traindata.get(i).first);
	  Assignment max_assignment(traindata.get(i).second, max_split, params);
	  
	  for (int j = 0; j < train_iters; ++j)
	    {
	      params.modify_counts(splits[i],assignments[i],-1);
	      params.sample_next(splits[i],assignments[i], traindata.get(i).second);
	      params.modify_counts(splits[i],assignments[i],1);
	      
	      float score = params.score(splits[i],assignments[i]);
	      
	      if (score > max_score)
		{
		  max_score = score;
		  max_split = splits[i];
		  max_assignment = assignments[i];
		}
	    }
	  
	  splits[i] = max_split;
	  assignments[i] = max_assignment;	  
	}
    }
  std::cerr << std::endl;
  std::cerr << "Test" << std::endl;
  
  splits.clear();
  assignments.clear();

  for (int i = 0; i < testdata.size(); ++i)
    {
      Split s(testdata.get(i).first);
      Assignment a(testdata.get(i).second, s, params);
      s.set_pos(get_pos(testdata.get(i).second));
      splits.push_back(s);
      assignments.push_back(a);
    }

  for (int i = 0; i < testdata.size(); ++i)
    {
      std::cerr << "String " << i + 1 << " of " << testdata.size() << '\r'; 

      float max_score = -1;
      Split max_split(testdata.get(i).first);
      Assignment max_assignment(testdata.get(i).second, max_split, params);
	  
      for (int j = 0; j < test_iters; ++j)
	{
	  params.modify_counts(splits[i],assignments[i],-1);
	  params.sample_next(splits[i],assignments[i], testdata.get(i).second);
	  params.modify_counts(splits[i],assignments[i],1);
	  
	  float score = params.score(splits[i],assignments[i]);
	  
	  if (score > max_score)
	    {
	      max_score = score;
	      max_split = splits[i];
	      max_assignment = assignments[i];
	    }
	}

      for (int j = 0; j < max_split.size(); ++j)
	{
	   std::cout << (j == 0 ? "" : " ") << max_split.get(j); 

	  for (int k = 0; k < max_assignment.get(j).size(); ++k)
	    {
	      std::string l = max_assignment.get(j)[k];
	      std::cout << (k == 0 ? '/' : ',') << l;
	    }
	}

      //StringVector zlabels;
      //      for (int j = max_split.size() - ZEROCOUNT; j < max_split.size(); ++j)
      //for (auto l: max_assignment.get(j))
      //{ zlabels.push_back(l); }

      //if (zlabels.size())
      //{
      //std::cout << " \"\""; 
      //for (int j = 0; j < zlabels.size(); ++j)
      //{ std::cout << (j == 0 ? '/' : ',') << zlabels[j]; } 
      //}

      std::cout << std::endl;
    }
  std::cerr << std::endl;

}

