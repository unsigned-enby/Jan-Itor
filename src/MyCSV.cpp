#include <iostream>
#include "MyCSV.hpp"
using std::vector;
using std::string;
//********************************************************************
void MyCSV::prettyPrintMetaData() const {
   std::cerr << "Deliminator : >" << Delim           << "<"   << '\n';
   std::cerr << "Quote       : >" << Quote           << "<"   << '\n';
   std::cerr << "ColCount    : "  << ColCount        << '\n';
   std::cerr << "RowCount    : "  << Table[0].size() << '\n';
   std::cerr << "Input File  : "  << InFile          << '\n';
   std::cerr << "Output File : "  << OutFile         << '\n';
}
//********************************************************************
vector<string>& MyCSV::getCol(int i) {
   if (i >= ColCount) { 
      std::cerr << "ERROR: Requested collumn is not available" << "\n";
      exit(1); //NOLINT
   }
   else //NOLINT
      return Table[i];
}
vector<string>& MyCSV::operator[](int i) {
   return getCol(i);
}
int MyCSV::getRowCount() {
   if(ColCount <= 0)
      return 0;
   else 
      return getCol(0).size();
}
//********************************************************************
void MyCSV::resize(int size) {
   if(size == ColCount) {
      return;
   }
   if(size > ColCount) {
      Table.resize(size);
      while(ColCount < size) 
         Table[ColCount++].resize(Table[0].size());
      return;
   }
   if(size < ColCount) {
      while(ColCount > size) 
         Table[ColCount--].clear();
      Table.resize(ColCount);
      return;
   }
}
//********************************************************************
MyCSV::CharClass MyCSV::charClass(char &ch) const {
   if(ch == Delim) {
      return DELIM;
   }
   else if(ch == '\n' || ch == '\r') { 
      return RDELIM;
   }
   else if(ch == Quote) {
      return QUOTE;
   }
   else {
      return CHAR;
   }
}
void MyCSV::readTable(string inFile, bool header) {
   if(inFile != "NA")
      InFile = inFile;
   if(InFile.empty()) {
      std::cerr << "The input file is not set!\n";
      return;
   }
   char* buffer = nullptr;
   std::fstream infile(InFile);
   if (!infile) {
      std::cerr << "ERROR OPENING FILE, "; 
      std::cerr << InFile  << " ";
      std::cerr << "EXITING NOW\n";
      exit(-1); //NOLINT
   }
   //It might be more effiecient, or at least cleaner, to use getline here,
   //but this method allows for handling multichar row-delimiters   
   if (header) {
      Headers.clear();
      char ch = 0;
      while(charClass(ch) != RDELIM) {
         string str; 
         ch = infile.get();
         while(charClass(ch) != DELIM && charClass(ch) != RDELIM) {
            str.push_back(ch);
            ch = infile.get();
         }
         addHeader(str);
      }
      ch = infile.peek();
      while(charClass(ch) == RDELIM) {
         infile.get();
         ch = infile.peek();
      }
   }
   int startPos = (int)infile.tellg(); 
   infile.seekg(0, std::ios::end);
   int bufferSize = (int)infile.tellg(); 
   bufferSize -= startPos;
   infile.seekg(startPos, std::ios::beg);   
   buffer = new char[bufferSize]; //NOLINT
   infile.read(buffer, bufferSize);   
   infile.close();   
   makeTable(buffer, bufferSize);
   delete [] buffer; //NOLINT
}
void MyCSV::makeTable(char* buffer, int bufferSize) {
   ColCount = 0;
   Table.clear();
   int col = 0;
   int pos = 0;
   Table.emplace_back();
   string str;
   while(pos < bufferSize) {
      switch(charClass(buffer[pos])) {
         case QUOTE:
            //upon encountering a quote, skip foward until a quote-[r]delim sequence is found
            //then incriment position so the next iteration of the parent loop handles
            //the [r]delim
            pos++;
            while(pos < bufferSize) { //a safer `while(true)`
               if(charClass(buffer[pos]) == QUOTE &&
                 (charClass(buffer[pos+1]) == DELIM || charClass(buffer[pos+1]) == RDELIM))
                     break;
               str.push_back(buffer[pos++]);
            }
            pos++;
            break;
         case CHAR:
            while(charClass(buffer[pos]) == CHAR) {
               str.push_back(buffer[pos]);
               pos++;
            }
            break;
         case DELIM:
            Table[col++].push_back(str);
            str.clear();
            if(col >= ColCount) {
               ColCount++;
               Table.emplace_back();
            }
            pos++;
            break;
         case RDELIM: 
            //TODO: test to see if an `addrow` method would speed this up at all
            Table[col].push_back(str);
            str.clear();
            if(ColCount == 0 && col == 0)
               ColCount++;
            col = 0;
            while(charClass(buffer[pos]) == RDELIM) //handle multi byte row delims(i.e \r\n)
               pos++;
            break;
         default:
            std::cerr << "There was an error proccessing the input file!\n";
            exit(-1); //NOLINT
            break;
      }
   }
   
   //Verify
   int rowCount = getCol(0).size();
   for(int i=1; i<ColCount ; i++) {
      if(getCol(i).size() != rowCount) {
         std::cerr << "There was an error proccessing the input file!\n";
         exit(-1);
      }
   }

   SubDelims.resize(ColCount, 0);
}
void MyCSV::setSubDelim(int col, char subDelim) {
   if(col >= ColCount)
      return;
   else
      SubDelims[col] = subDelim;
}
void MyCSV::setHeader(int col, string header) {
   if(col >= ColCount)
      return;
   else
      Headers[col] = header;
}
void MyCSV::addHeader(string header) {
   //TODO: test wether incrimenting ColCount here breaks anything
   //ColCount++;
   Headers.push_back(header);
}
//********************************************************************
void MyCSV::writeTable(string outfile) {
   if(outfile != "NA") 
      OutFile = outfile;
   
   std::ofstream outFile;
   if(OutFile.empty()) {
      std::cerr << "The output file is not set!\n";
      return;
   } else {
      outFile.open(OutFile);
      if(!outFile) {
         std::cerr << "Output file: " << OutFile << " did not open!\n";
         return;
      }
   }
   
   string line;
   if(!Headers.empty()) {
      for(string header : Headers) {
         line += header + Delim;
      }
      line.pop_back();
      outFile << line << "\n";
   }
   for (int i=0;i<Table[0].size();i++) {   
      line.clear();
      for(int col=0;col<ColCount;col++)
         line += Table[col][i] + Delim;
      line.pop_back();
      outFile << line << "\n";
   } 
   outFile.close();
}  
//********************************************************************
