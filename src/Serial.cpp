#include <iostream>
#include "Serial.hpp"
vector<string> Serial::readTargetWords(string targFile) {
   TargetWords.clear();
   std::ifstream infile(targFile);
   if(infile.fail()) {
      std::cerr << "The target word file: " << targFile << " did not open properly!\n";
      return TargetWords;
   }
   string temp;
   getline(infile,temp);
   while (infile) {
      TargetWords.push_back(temp);
      getline(infile,temp);
   }
   infile.close();
   return TargetWords;
}
//*******************************************************************
void Serial::makeCorrections(vector<string> correctedWords) {   //NOLINT
   //TODO: Check to see if I can use `array` facilities to join the two below lines,
   for(int i = 0; i<correctedWords.size(); i++) {
      if(!correctedWords[i].empty()) {
         Corrections[0].push_back(UniqueWords[i]);
         Corrections[1].push_back(correctedWords[i]);
      }
   }
   int row = 0;
   for(auto &parsedResponse : ParsedResponses) {
      for (auto &respWord : parsedResponse) {
         for(int i=0; i < Corrections[0].size() ; i++) {
            // first checks for corrected form, 
            // if (both are) found, then the original is not a likely typo
            // TODO: verify that previous changes to a response will not
            // messup subsequent changes in (response) loop
            if (respWord == Corrections[0][i]) {
               bool bothFound = false;
               for (const string &rsWd: parsedResponse) {
                  if (Corrections[1][i] == rsWd) {
                     bothFound = true;
                     break;
               }  }
               if (!bothFound) {
                  respWord = Corrections[1][i];
                  RowsCorrected.push_back(row+1);
            }  }   
      }  }   
      row++;
   }   
   updateResponses();
}
//********************************************************************
MyCSV* Serial::serialize() {   
   if(!Table) {
      return nullptr;
   }
   MyCSV* serialPos = new MyCSV; //NOLINT
   serialPos->addCol(*Table->getCol("PIDS"));
   serialPos->addCol("Intrusions");
   for(const string &str : TargetWords)
      serialPos->addCol(str);

   //For each item in a ParsedResponse, check for a match against, target list.
   //If found, assign the position of 'item' in the ParsedResponse to the target collumn
   int row = -1;
   for(vector<string> &resp : ParsedResponses) { //row
      row++;
      int numIntrusions = 0;
      int respIdx = -1;
      for(const std::string &respItem : resp) { //row items
         respIdx++;
         bool found = false;
         for(const std::string &target : TargetWords) {
            if(target == respItem) {
               found = true;
               std::string &str = serialPos->getCol(target)->at(row);
               if(str.empty()) {
                  str = std::to_string(respIdx+1);
               } else {
                  str += " " + std::to_string(respIdx+1);
               }
               break;
            }
         }
         if (!found) 
            numIntrusions++;
      }   
      serialPos->getCol("Intrusions")->at(row) = std::to_string(numIntrusions);
   }   
   writeLog();
   return serialPos;
}
//********************************************************************
void Serial::parseResp() {
   auto respCol  = Table->getCol("RESPONSES");
   char subDelim = *respCol->subDelim();
   ParsedResponses.clear();
   ParsedResponses.resize(respCol->size());
   auto parsedResponse = ParsedResponses.begin();
   for(const string &response : *respCol) {
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
void Serial::uniqWords() {
   UniqueWords.clear();
   for (const vector<string> &parsedResp : ParsedResponses) {
      for (const string &respWord : parsedResp) {
         bool found = false;
         for(const string &targetWord : TargetWords) {
            if (respWord == targetWord) {                
               found = true;
               break;
         }  }
         if (!found) {
            for(const string &uniqWord : UniqueWords) {    
               if (respWord == uniqWord) {          
                  found = true;                     
                  break;                            
         }  }  }   
         if (!found) {
            UniqueWords.push_back(respWord);
         }  
      }  
   }  
}   
void Serial::updateResponses() {
   //std::vector<std::vector<std::string>>::iterator parsedResponse = ParsedResponses.begin();
   auto respCol = Table->getCol("RESPONSES");
   char subDelim = *respCol->subDelim();
   auto parsedResponse = ParsedResponses.begin();
   for(string &resp : *respCol) {
      resp.clear();
      for(const string &respWord : *parsedResponse) {
         resp += respWord + subDelim;
      }
      if(!resp.empty()) {
         resp.pop_back();
      }  
      parsedResponse++;
}  }
void Serial::writeLog(string logFile) {
   std::ofstream outfile(logFile);
   outfile << "UNIQUE WORDS" << '\n' << UniqueWords.size() << '\n';
   for(const string &str : UniqueWords)
      outfile << str << '\n';
   outfile << "CORRECTED WORDS" << '\n' << Corrections[0].size() << '\n';
   for(int i=0; i<Corrections[0].size() ;i++) 
      outfile << Corrections[0][i] << ',' << Corrections[1][i] << '\n';
   outfile << "ROWS CORRECTED" << '\n' << RowsCorrected.size() << '\n';
   for(const int &row : RowsCorrected)
      outfile << row << '\n';
   outfile.close();
   outfile.clear();
   //TODO: set up output for POS data
}
