#ifndef CEREAL_H
#define CEREAL_H
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include "MyCSV.hpp"
using std::vector;
using std::string;

//TODO create a one-shot Unique words vector for loging that does
//not change upon invocation of makeCorrections
class Serial {
   protected:
      MyCSV* Table = nullptr;
   public:
      vector<string> getUniqWords(MyCSV* table = nullptr) {
         if(!Table && !table)
            return {};
         if(!Table)
            Table = table;
         parseResp();
         return uniqWords();
      }
      vector<string> readTargetWords(string targFile); //both sets and returns target words
      vector<string> uniqWords();
      
      vector<string> makeCorrections(vector<std::pair<string,string>> corrections);
      MyCSV* serialize();

   private:
      //internals   
      vector<vector<string>> ParsedResponses;
      vector<string> TargetWords;
      
      //log vectors
      vector<string> CorrectedWords;
      vector<string> RowsCorrected;

      //helpers
      void parseResp      ();
      void updateResponses();
      void writeLog(string logFile = "Serial.log");
};
#endif
