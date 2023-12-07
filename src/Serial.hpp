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
   public:
      vector<string> getUniqWords(MyCol* respCol, MyCol* pidCol) {
         Responses = respCol;
         PIDs = pidCol;
         parseResp();
         return uniqWords();
      }
      vector<string> readTargetWords(string targFile); //both sets and returns target words
      vector<string> makeCorrections(vector<std::pair<string,string>> corrections); //returns uniqWords()
      MyCSV* serialize();

   private:
      //columns
      MyCol* Responses = nullptr;
      MyCol* PIDs = nullptr;
      //internals   
      vector<vector<string>> ParsedResponses;
      vector<string> TargetWords;
      
      //log vectors
      vector<string> CorrectedWords;
      vector<string> RowsCorrected;

      //helpers
      void parseResp      ();
      void updateResponses();
      vector<string> uniqWords();
      void writeLog(string logFile = "Serial.log");
};
#endif
