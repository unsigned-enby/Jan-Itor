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
      //columns
      MyCol* RESPs = nullptr;
      MyCol* PIDs  = nullptr;
   public:
      vector<string> init(string targFile) {
         this->clear();
         readTargetWords(targFile);
         parseResp();
         return uniqWords();
      }
      vector<string> makeCorrections(vector<std::pair<string,string>> corrections) {
         if(!RESPs)
            return {};
         makeCorrections_(std::move(corrections));
         updateResponses();
         return uniqWords();
      }
      //void setColumns(MyCol* responses, MyCol* pids) {
      //   this->clear();
      //   RESPs = responses;
      //   PIDs  = pids;
      //}
      vector<string> getTargetWords() { return TargetWords; }
      MyCSV* serialize();
   private:
      //internals   
      vector<vector<string>> ParsedResponses;
      vector<string> TargetWords;
      
      //log vectors
      vector<string> CorrectedWords;
      vector<string> RowsCorrected;

      //helpers
      void clear() {
         TargetWords.clear();
         ParsedResponses.clear();
         CorrectedWords.clear();
         RowsCorrected.clear();
      }

      void readTargetWords(string targFile);
      void parseResp      ();
      void updateResponses();
      vector<string> uniqWords();
      void makeCorrections_(vector<std::pair<string,string>> corrections);
      void writeLog(string logFile = "Serial.log");
};
#endif
