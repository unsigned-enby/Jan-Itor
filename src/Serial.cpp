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
      TargetWords.push_back(temp); //<-member variable
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
   } else if(!RESPs->subDelim()) { //XXX: check wether this results in nothing or a crash
      std::cerr << "Warning, the sub delim for the response column is not set!\n";
   } else {
      subDelim = *RESPs->subDelim();
   }
   
   ParsedResponses.resize(RESPs->size());
   auto newRow = ParsedResponses.begin();
   for(const auto &row : *RESPs) {
      string newRowItem;
      for (const auto &rowItem : row)
         if(rowItem == subDelim) {
            newRow->push_back(newRowItem);
            newRowItem.clear();
         } else {
            newRowItem.push_back(rowItem);
         }
      if(!newRowItem.empty()) {
         newRow->push_back(newRowItem);
      }
      newRow++;
   }
}
vector<string> Serial::uniqWords() {
   vector<string> uniqWords;
   for (const auto &row : ParsedResponses)
      for (const auto &rowItem : row) {
         bool found = false;
         for(const auto &targetItem : TargetWords)
            if (rowItem == targetItem) {
               found = true;
               break;
            }
         if (!found)
            for(const auto &uniqItem : uniqWords)
               if (rowItem == uniqItem) {
                  found = true;
                  break;
               }     
         if (!found) { //if (still) not found
            uniqWords.push_back(rowItem);
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
   
   // first checks for corrected form, then (if found) looks for the second
   // if both are found, then the original is not likely a typo
   int rowIdx = 0;
   for(auto &row : ParsedResponses) {
      for (auto &rowItem : row)
         for(const auto &correction : corrections)
            if (rowItem == correction.first) {
               bool bothFound = false;
               for (const auto &rowItem: row) //rowItem shadows previous declaration
                  if (rowItem == correction.second) {
                     bothFound = true;
                     break;
                  }
               if (!bothFound) {
                  rowItem = correction.second;
                  RowsCorrected.push_back(std::to_string(rowIdx+1) + ":" +
                                          correction.first + "," + correction.second);
               }  
            }
      rowIdx++;
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
      serialPos->resize(-1, RESPs->size());
   }
   
   for(const string &str : TargetWords) {
      serialPos->addCol(str);
   }

   //For each item in a ParsedResponse, check for a match against, target list.
   //If found, assign the position of 'item' in the ParsedResponse to the target PIDslumn
   int rowIdx = 0;
   for(auto &row : ParsedResponses) { //row
      int numIntrusions = 0;
      int targIdx = -1;
      for(const auto &rowItem : row) { //participant responses
         targIdx++;
         bool found = false;
         for(const auto &target : TargetWords)
            if(target == rowItem) {
               std::string &str = serialPos->getCol(target)->at(rowIdx);
               if(!str.empty()) {
                  str += ' ';
               }
               str += std::to_string(targIdx+1);
               found = true;
               break;
            }
         if (!found) {
            numIntrusions++;
         }
      }
      serialPos->getCol("Intrusions")->at(rowIdx) = std::to_string(numIntrusions);
      rowIdx++;
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
      //While there should really be a subDelim already set for RESPs,
      //this is here just incase it gets unset somehow
      subDelim = ' ';
   }
   auto row = ParsedResponses.begin();
   for(auto &newRow : *RESPs) {
      newRow.clear();
      for(const auto &rowItem : *row) {
         newRow += rowItem + subDelim;
      }
      if(!newRow.empty()) {
         newRow.pop_back();
      }
      row++;
   }
}
void Serial::writeLog(string logFile) {
   std::ofstream outfile(logFile);
  
   string msg;
   int size = 0;
   for(const auto &correction : CorrectedWords)
      if(!correction.empty()) {
         size++;
         msg += correction + '\n';
      } else {
         msg += '\n';
      }
   outfile << "CORRECTIONS:" << size << '\n';
   outfile << msg;
   
   
   msg.clear();
   size = 0;
   for(const auto &cRow : RowsCorrected)
      if(!cRow.empty()) {
         size++;
         msg += cRow + '\n';
      } else {
         msg += '\n';
      }
   outfile << "ROWS CORRECTED:" << size << '\n';
   outfile << msg;

   
   outfile.close();
}
