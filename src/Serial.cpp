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
   bool bothFound = false;
   int  row = 0;
   for(vector<string> &response : ParsedResponses) {
      for (string &respWord : response) {
         for(int i=0; i < Corrections[0].size() ; i++) {
            bothFound = false;
            // first checks for corrected form, 
            // if (both are) found, then the original is not a likely typo
            // TODO: verify that previous changes to a response will not
            // messup subsequent changes in (response) loop
            if (respWord == Corrections[0][i]) {
               for (const string &rsWd: response) {
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
   if(!Responses) {
      return NULL;
   }
   MyCSV* serialPos = new MyCSV; //NOLINT
   serialPos->addHeader("PID");
   serialPos->addHeader("Intrusions");
   for(const string &str : TargetWords)
      serialPos->addHeader(str);
   const int targCount = TargetWords.size(); //NOLINT
   serialPos->resize(targCount+2); //+2 to account for PID & intrusions field

   serialPos->getCol(0) = *PIDS;
   for(int i=1; i<serialPos->size(); i++) 
      serialPos->getCol(i).resize(Responses->size());

   //For each item in a ParsedResponse, check for a match against, target list.
   //If found, assign the position of 'item' in the ParsedResponse to the target collumn
   for(int row=0; row<ParsedResponses.size(); row++) {
      int numIntrusions = 0;
      const vector<string> &resp = ParsedResponses[row];
      for(int respIdx=0; respIdx < resp.size(); respIdx++) {
         bool found = false;
         for(int targIdx=0; targIdx < targCount; targIdx++) {
            if (TargetWords[targIdx] == resp[respIdx]) {
               found = true;
               if(serialPos->getCol(targIdx+2)[row].empty())
                  serialPos->getCol(targIdx+2)[row] = std::to_string(respIdx + 1);
               else {   
                  string temp = " " + std::to_string(respIdx + 1);
                  serialPos->getCol(targIdx+2)[row].append(temp);
               }   
               break;
            }   
         }
         if (!found) 
            numIntrusions++;
      }   
      serialPos->getCol(1)[row] = std::to_string(numIntrusions);
   }   
   writeLog();
   return serialPos;
}
//********************************************************************
void Serial::parseResp() {
   ParsedResponses.clear();
   ParsedResponses.resize(Responses->size());
   int row = 0;
   for(const string &response : *Responses) {
      string subItem;
      for (const char &ch : response) {
         if(ch != SubDelim) {
            subItem.push_back(ch);
         }
         else {
            ParsedResponses[row].push_back(subItem);
            subItem.clear();
         }
      }
      if(!subItem.empty()) {
         ParsedResponses[row].push_back(subItem);
      }
      row++;
}  }
void Serial::uniqWords() {
   UniqueWords.clear();
   bool found = false;
   for (vector<string> &parsedResp : ParsedResponses) {
      for (string &respWord : parsedResp) {
         found = false;
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
}  }  }  }   
void Serial::updateResponses() {
   for(int i=0; i<Responses->size() ; i++) {
      string &str = Responses->at(i);
      str.clear();
      for(const string &respWord : ParsedResponses[i]) {
         str += respWord + SubDelim;
      }
      if(!str.empty()) {
         str.pop_back();
}  }  }
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
