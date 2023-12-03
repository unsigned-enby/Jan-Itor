#ifndef IpForm_H
#define IpForm_H
#include <vector>
#include <string>
#include <array>
#include "MyCol.hpp"
using std::vector;
using std::string;
class IpForm {
   public:
      void setBlackSpace (string blackSpace,          bool clear = false);
      void setBlackSpace (std::array<bool, 3> ranges, bool clear = false);
      void setBlackSpace (std::array<bool, 3> ranges, string removles);
      
      void setRemovles   (string removles,            bool clear = false);
      void setRemovles   (std::array<bool, 3> ranges, bool clear = false);
      void setRemovles   (std::array<bool, 3> ranges, string removles);
      
      string getBlackSpace () const {return BlackSpace;};
      string getRemovles   () const {return Removles;  };

      void cleanColumn(MyCol* col);
   private:
      string BlackSpace;
      string Removles;
      string charRange(std::array<bool, 3> ranges); 
      void removeChars(MyCol* col);
};
#endif
