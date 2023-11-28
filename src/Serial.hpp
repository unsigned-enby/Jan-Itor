#ifndef CEREAL_H
#define CEREAL_H
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include "MyCSV.hpp"
using std::vector;
using std::string;

class Serial {
   public:
      vector<string> getUniqWords(MyCSV* table) {
         Table = table;
         parseResp();
         uniqWords();
         return UniqueWords;
      }
      vector<string> readTargetWords(string targFile);
      void   makeCorrections(vector<string> correctedWords);
      MyCSV* serialize();

   private:
      //internals   
      MyCSV* Table = nullptr;
      vector<vector<string>> ParsedResponses;
      
      vector<string> TargetWords;
      vector<string> UniqueWords;
      vector<int>    RowsCorrected;
      std::array<vector<string>, 2> Corrections;

      //helpers
      void parseResp      ();
      void uniqWords      ();
      void updateResponses();
      void writeLog(string logFile = "Serial.log");
};
#endif
