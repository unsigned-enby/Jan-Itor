#ifndef IpForm_H
#define IpForm_H
#include <vector>
#include <string>
#include <array>
#include "MyCol.hpp"
using std::vector;
using std::string;
/// IpForm uses strings as opposed to vector<char> as the chars passed to setBlackSpace &
/// setRemovles will be in string form, and for the sake of more easily appending the the 
/// member variables BlackSpace and Removles
class IpForm {
   public:
      void setBlackSpace (string chars,               bool clear);
      void setBlackSpace (std::array<bool, 3> ranges, bool clear);
      
      void setRemovles   (string chars,               bool clear);
      void setRemovles   (std::array<bool, 3> ranges, bool clear);
      
      string getBlackSpace () const {return BlackSpace;};
      string getRemovles   () const {return Removles;  };

      void cleanColumn(MyCol* col);
   private:
      string BlackSpace;
      string Removles;
      string charRange(std::array<bool, 3> ranges); 
      void   removeChars(MyCol* col);
};
#endif
