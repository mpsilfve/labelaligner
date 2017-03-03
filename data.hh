#ifndef HEADER_data_hh
#define HEADER_data_hh

#include <string>
#include <vector>
#include <utility>

typedef std::vector<std::string> StringVector;
typedef std::pair<std::string, StringVector> LabeledString;

class Data
{
public:
  Data(void);
  Data(const std::string &fn);
  void append(const Data&another);
  int size(void) const;
  const LabeledString &get(int i) const;
  void add(const LabeledString &s);
  void shuffle(void);
private:
  typedef std::vector<LabeledString> DataVector;
  
  DataVector data;
};

#endif // HEADER_data_hh
