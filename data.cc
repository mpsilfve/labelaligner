#include "data.hh"

#include <cassert>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iostream>

Data::Data(void)
{}

Data::Data(const std::string &fn)
{
  std::ifstream in(fn);

  while (in)
    {
      std::string line;
      std::getline(in, line);

      if (line == "")
	{ continue; }

      std::istringstream iss(line);
      std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
	                              std::istream_iterator<std::string>{}};
      assert(tokens.size() > 1);
      data.push_back(LabeledString(tokens.back(), StringVector(tokens.begin(),
							       --tokens.end())));
      //data.back().second.push_back("DUMMY");
    }
}

void Data::append(const Data&another)
{
  for (int i = 0; i < another.size(); ++i)
    { add(another.get(i)); }
}

void Data::add(const LabeledString &s)
{ data.push_back(s); }

int Data::size() const
{ return data.size(); }

const LabeledString &Data::get(int i) const
{ return data[i]; }

void Data::shuffle(void)
{ std::random_shuffle(data.begin(), data.end()); }

#ifdef TEST_data_cc

int main(void)
{}

#endif // TEST_data_cc
