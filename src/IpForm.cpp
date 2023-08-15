#include <iostream>
#include "IpForm.hpp"
using std::vector;
using std::string;
//********************************************************************
string IpForm::charRange(std::array<bool, 3> ranges) { //NOLINT
   string retString;
   if(ranges[0]) { //a-z
      char ch = 'a';
      while(ch <= 'z')
         retString.push_back(ch++);
   }
   if (ranges[1]) { //A-Z
      char ch = 'A';
      while(ch <= 'Z')
         retString.push_back(ch++);
   }
   if (ranges[2]) { //0-9
      char ch = '0';
      while(ch <= '9')
         retString.push_back(ch++);
   }  
   return retString;
} 
void IpForm::setBlackSpace(std::array<bool, 3> ranges, bool clear) {
   if(clear)
      BlackSpace.clear();
   BlackSpace += charRange(ranges);
}
void IpForm::setRemovles(std::array<bool, 3> ranges, bool clear) {
   if(clear)
      Removles.clear();
   Removles += charRange(ranges);
}
void IpForm::setBlackSpace(string blackSpace, bool clear) {
   if(clear)
      BlackSpace.clear();
   BlackSpace += blackSpace;
}
void IpForm::setRemovles(string removles, bool clear) {
   if(clear)
      Removles.clear();
   Removles += removles;
}
//********************************************************************
void IpForm::removeChars(vector<string> &col) {
   for(string &row : col) {
      //XXX: Check wether row.size is adjusted on each loop
      for(int i=0; i<row.size() ;i++) { //NOLINT
         for(char ch : Removles) {
            if(row[i]==ch) {
               row.erase(i,1);
               i--;
               break;
}  }  }  }  }
void IpForm::cleanColumn(vector<string> &col) {
   removeChars(col); 
   //first checks wether the given char is part of the BlackSpace vec
   //then, if it is not BlackSpace, it checks wether the pos in the current
   //string is imediately after a BlackSpace char (for the sake of sub-delims)
   for(string &row : col) {
      //TODO: make charId enum-based
      int i = 0;
      bool after_target = false;
      while (i < row.size()) {
         int charID = 0;
         for (char ch : BlackSpace) {
            if (row[i] == ch) {
               charID = 1;
               break;
            }
            else {
               if(after_target)
                  charID = 2;
               else
                  charID = 3;
         }  }
         switch (charID) {
            case 1:       //target
               i++;
               after_target = true;
               break;
            case 2:       //after target(which is not a target, hence no break)
               row.insert(i,1,SubDelim);
               i++;
               after_target = false;
            case 3:       //not target
               row.erase(i,1);
               break;
            default:
               std::cerr << "ERROR<ipProcess>\n";
      }  }
      if(!row.empty() && row.back() == SubDelim)
         row.pop_back();
      for(char &ch : row) //NOLINT
         ch = tolower(ch);
}  }
//********************************************************************
