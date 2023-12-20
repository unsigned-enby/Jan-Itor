#include <iostream>
#include "Serial.hpp"
/// TODO:
///   - make it an option to include the serial position of all (unique) words, not just the targets
///   - set target words function
///   - maybe: replace the `ParsedResponses` nested-vector with a `MyCSV` object
void Serial::readTargetWords(string targFile) {
   TargetWords.clear();
   if(targFile.empty())
      return;

   std::ifstream infile(targFile);
   if(infile.fail()) {
      std::cerr << "The target word file: " << targFile << " did not open properly!\n";
      return;
   }
   string temp;
   getline(infile,temp);
   while (infile) {
      TargetWords.push_back(temp);
      getline(infile,temp);
   }
   infile.close();
}
//*******************************************************************
void Serial::parseResp() {
   ParsedResponses.clear();
   char subDelim = -1;
   if(!RESPs) {
      std::cerr << "Error: Response column is not set!\n";
      return;
   } else if(!RESPs->subDelim()) {
      std::cerr << "Warning, the sub delim for the response column is not set!\n";
   } else {
      subDelim = *RESPs->subDelim();
   }
   
   ParsedResponses.resize(RESPs->size());
   auto parsedResponse = ParsedResponses.begin();
   for(const string &response : *RESPs) {
      string subItem;
      for (const char &ch : response) {
         if(ch == subDelim) {
            parsedResponse->push_back(subItem);
            subItem.clear();
         }
         else {
            subItem.push_back(ch);
         }
      }
      if(!subItem.empty()) {
         parsedResponse->push_back(subItem);
      }
      parsedResponse++;
   }
}
vector<string> Serial::uniqWords() {
   vector<string> uniqWords;
   for (const vector<string> &parsedResp : ParsedResponses) {
      for (const string &respWord : parsedResp) {
         bool found = false;
         for(const string &targetWord : TargetWords) {
            if (targetWord == respWord) {
               found = true;
               break;
         }  }
         if (!found) {
            for(const string &uniqWord : uniqWords) {
               if (uniqWord == respWord) {
                  found = true;
                  break;
         }  }  }
         if (!found) {
            uniqWords.push_back(respWord);
         }
      }
   }
   return uniqWords;
}
//TODO check for and error-log any instances of corrected-forms that are 
//found in the `UniqueWords` vector
void Serial::makeCorrections_(vector<std::pair<string, string>> corrections) {   //NOLINT
   //for the sake of deliminating multiple invocations of `makeCorrections` 
   if(RowsCorrected.size() != 0 && CorrectedWords.size() != 0) {
      RowsCorrected.push_back("");
      CorrectedWords.push_back("");
   }
   int row = 0;
   // first checks for corrected form,
   // if both are found, then the original is not likely a typo
   for(auto &parsedResponse : ParsedResponses) {
      for (auto &respWord : parsedResponse) {
         for(const auto &correction : corrections) {
            if (respWord == correction.first) {
               bool bothFound = false;
               for (const string &resp: parsedResponse) {
                  if (correction.second == resp) {
                     bothFound = true;
                     break;
               }  }
               if (!bothFound) {
                  respWord = correction.second;
                  RowsCorrected.push_back(
                        std::to_string(row+1) + ":" +
                        correction.first + "," + correction.second
                        );
            }  }
      }  }
      row++;
   }
   for(const auto &correction : corrections) {
      CorrectedWords.push_back(correction.first + ',' + correction.second);
   }
}
//********************************************************************
MyCSV* Serial::serialize() {
   if(!RESPs) {
      std::cerr << "Error: Response column is not set!\n";
      return nullptr;
   }
   MyCSV* serialPos = new MyCSV;
   if(PIDs) {
      serialPos->addCol(*PIDs);
      serialPos->addCol("Intrusions");
   } else { 
      std::cerr << "Warning, PID column is not set!\n";
      serialPos->addCol("Intrusions");
   }
   for(const string &str : TargetWords)
      serialPos->addCol(str);

   //For each item in a ParsedResponse, check for a match against, target list.
   //If found, assign the position of 'item' in the ParsedResponse to the target PIDslumn
   int row = -1;
   for(vector<string> &resp : ParsedResponses) { //row
      row++;
      int numIntrusions = 0;
      int respIdx = -1;
      for(const std::string &respItem : resp) { //participant responses
         respIdx++;
         bool found = false;
         for(const std::string &target : TargetWords) {
            if(target == respItem) {
               std::string &str = serialPos->getCol(target)->at(row);
               if(!str.empty()) {
                  str += ' ';
               }
               str += std::to_string(respIdx+1);
               found = true;
               break;
            }
         }
         if (!found) {
            numIntrusions++;
         }
      }
      serialPos->getCol("Intrusions")->at(row) = std::to_string(numIntrusions);
   }
   writeLog();
   return serialPos;
}
//********************************************************************
void Serial::updateResponses() {
   char subDelim = -1;
   if(RESPs->subDelim()) {
      subDelim = *RESPs->subDelim();
   } else {
      subDelim = ' ';
   }
   auto parsedResponse = ParsedResponses.begin();
   for(string &resp : *RESPs) {
      resp.clear();
      for(const string &respWord : *parsedResponse) {
         resp += respWord + subDelim;
      }
      if(!resp.empty()) {
         resp.pop_back();
      }
      parsedResponse++;
   }
}
void Serial::writeLog(string logFile) {
   std::ofstream outfile(logFile);
  
   int size = 0;
   string msg;
   for(const auto &correction : CorrectedWords) {
      if(!correction.empty()) {
         size++;
         msg += correction + '\n';
      } else {
         msg += '\n';
      }
   }
   outfile << "CORRECTIONS: " << size << '\n';
   outfile << msg;
   
   msg.clear();
   size = 0;
   for(const auto &cRow : RowsCorrected) {
      if(!cRow.empty()) {
         size++;
         msg += cRow + '\n';
      } else {
         msg += '\n';
      }
   }
   outfile << "ROWS CORRECTED: " << size << '\n';
   outfile << msg;

   outfile.close();
}
