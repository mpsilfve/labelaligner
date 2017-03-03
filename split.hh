#ifndef HEADER_split_hh
#define HEADER_split_hh

#include <vector>
#include <string>

#define ZEROCOUNT 1
extern std::vector<std::string> ZEROS;

class Split
{
public:
  Split(const std::string &str);
  Split(const std::vector<int> &boundaries, const std::string &str);
  int size(void) const;
  std::string get(int i) const;
  void sample_next(int max_splits);
  void set_pos(const std::string &pos);

private:
  std::string str;
  std::string pos;
  std::vector<int> boundaries;

  void divide(void);
  void join(void);
  void move(void);
  
  friend class TESTSplit;
};

#endif // HEADER_split_hh
