#ifndef IpForm_H
#define IpForm_H
#include <vector>
#include <string>
#include <array>
using std::vector;
using std::string;
class IpForm {
   public:
      void setBlackSpace (std::array<bool, 3> ranges, bool clear = false);
      void setRemovles   (std::array<bool, 3> ranges, bool clear = false);
      void setBlackSpace (string blackSpace, bool clear = false);
      void setRemovles   (string removles,   bool clear = false);
      void setSubDelim(char subDelim) { SubDelim = subDelim; };
      
      string getBlackSpace () const { return BlackSpace; };
      string getRemovles   () const { return Removles; };
      char   getSubDelim   () const { return SubDelim; };

      void cleanColumn(vector<string> &col);
   private:
      string BlackSpace;
      string Removles;
      char   SubDelim;
      string charRange(std::array<bool, 3> ranges); 
      void removeChars(vector<string> &col);
};
#endif
