#include <algorithm>
#include <cstring>
#include <iostream>
#include "MyCSV.hpp"
#include <chrono>
using std::string;
using std::vector;
using std::cerr;
//Meta/calculated_info************************************************
void MyCSV::prettyPrintMetaData() const {
   cerr << "Deliminator : >" << Delim            << "<\n";
   cerr << "Quote       : >" << Quote            << "<\n";
   cerr << "ColCount    : "  << this->size()     << "\n";
   cerr << "Input File  : "  << InFile           << "\n";
   cerr << "Output File : "  << OutFile          << "\n";
   cerr << "Headers     : "  << Headers          << "\n";
   cerr << "Column headers:\n";
   for(unsigned int i=0; i<this->size() ; i++) {
         cerr << "   [" << i << "]=>" << Table[i]->header() << "<\n";
   }
   cerr << "RowCounts:\n";
   for(unsigned int i=0; i<this->size() ; i++) {
      cerr << "   [" << i << "]=" << Table[i]->size() << '\n';
   }
}
unsigned int MyCSV::rowCount() {
   if(this->size() <= 0)
      return 0;
   else {
      unsigned int rc = at(0)->size();
      for(MyCol* col : Table) { //sanity check
         if(col->size() != rc) {
            cerr << "Error, column sizes are not identical!\n";
         }
      }
      return rc;
   }
}
//table_I/O_and_parsing_functions*****************************************
void MyCSV::readTable(string inFile) {
   Table.clear();
   if(inFile != "NA")
      InFile = inFile;
   if(InFile.empty()) {
      cerr << "The input file is not set!\n";
      return;
   }
   char* buffer = nullptr;
  
   //read file into memory
   std::ifstream infile(InFile, std::ios::binary);
   if (!infile) {
      cerr << "ERROR OPENING FILE, ";
      cerr << InFile  << " ";
      cerr << "EXITING NOW\n";
      exit(-1); //NOLINT
   }
   infile.seekg(0, std::ios::end);
   unsigned int bufferSize = infile.tellg();
   infile.seekg(0, std::ios::beg);
   buffer = new char[bufferSize+1]; //Plus 1 for end-stop char
   for(int i=0; i<=bufferSize; i++)
      buffer[i] = 0;
   infile.read(buffer, bufferSize);
   infile.close();
  
   //normalize end of buffer
   unsigned int pos = bufferSize;
   buffer[pos--] = 4;   //ascii character code for 'end of transmission'
   while(charClass(buffer[pos]) == RDELIM ||
         buffer[pos] == 0) { buffer[pos--] = 4; }
   char* saveStart = buffer;
   
   //Call addRow once, as parseTable throws an error if it encounters something
   //other than a (RDELIM || EOT). While that could be changed, keeping things as is
   //provides a fair amount of error checking
   Headers ? this->setHeaders(parseRow(buffer)):
             this->addRow(parseRow(buffer));

   parseTable(buffer);
   //TODO: verify saveStart properly releases the associated memory
   delete [] saveStart;
}
MyCSV::CharClass MyCSV::charClass(char ch) const {
   if(ch == Delim)
      return DELIM;
   else if(ch == '\n' || ch == '\r')
      return RDELIM;
   else if(ch == Quote)
      return QUOTE;
   else if(ch == 4)  //ascii char code for end of transmission
      return EOT;
   else
      return CHAR;
}
string MyCSV::parseChar(char* &buffer) {
   char* start = &buffer[0];
   while(true)
      switch(charClass(*buffer)) {
         case EOT:
         case DELIM:
         case RDELIM:
            return {start, buffer};
         default:
            buffer++;
      }
}
string MyCSV::parseQuote(char* &buffer) {
   char* start = &buffer[0];
   while(true)
      switch(charClass(*buffer)) {
         case EOT:
            return string{start, buffer};
         case QUOTE:
            switch(charClass(*++buffer)) { //NOLINT
               case EOT:
               case DELIM:
               case RDELIM:
                  return {start, buffer};
            }
            break;
         default:
            buffer++;
      }
}
vector<string> MyCSV::parseRow(char* &buffer) {
   vector<string> retVec = {""};
   while(true)
      switch(charClass(*buffer)) {
         case EOT:
         case RDELIM:
            return std::move(retVec);
         case DELIM:
            buffer++;
            retVec.push_back({""});
            break;
         case QUOTE: //always a start-quote
            retVec.back() = parseQuote(buffer);
            break;
         case CHAR:
            retVec.back() = parseChar(buffer);
            break;
      }
}
void MyCSV::parseTable(char* buffer) {
   while(true)
      switch(charClass(*buffer)) {
         case EOT:
            return;
         case RDELIM:
            while(charClass(*++buffer) == RDELIM) {
            };
            this->addRow(parseRow(buffer));
            break;
         default:
            cerr << "Error parsing the table, exiting now!\n";
            exit(1);
      }
}
void MyCSV::writeTable(string outfile) {
   if(outfile != "NA")
      OutFile = outfile;
   std::ofstream outFile;
   if(OutFile.empty()) {
      cerr << "The output file is not set!\n";
      return;
   } else {
      outFile.open(OutFile);
      if(!outFile) {
         cerr << "Output file: " << OutFile << " did not open!\n";
         return;
      }
   }
   string line;
   if(Headers) {
      for(unsigned int i=0;i<this->size();i++) {
            line += Table[i]->header();
         line += Delim;
      }
      line.pop_back();
      outFile << line << "\n";
   }
   for (unsigned int i=0;i<rowCount();i++) {
      line.clear();
      for(unsigned int col=0;col<this->size();col++)
         line += Table[col]->at(i) + Delim;
      line.pop_back();
      outFile << line << "\n";
   }
   outFile.close();
}
//element_access_and_iterators****************************************
MyCol* MyCSV::at(unsigned int i) {
   if (i >= this->size()) {
      cerr << "Requested collumn is not available" << "\n";
      return nullptr;
   }
   else {
      return Table[i];
   }
}
MyCol* MyCSV::operator[](unsigned int i) {
   return at(i);
}
MyCol* MyCSV::getCol(string header) {
   MyCol* retCol = nullptr;
   for(MyCol* col : Table) {
      if(col->header() == "NA")
         continue;
      if(col->header() == header) {
         if(retCol) {
            cerr << "WARNING: more than one collumn with the requested"
                      << " header exists! Using the first one found.\n";
            continue;
         } else {
            retCol = col;
   }  }  }
   return retCol;
}
const vector<string*> MyCSV::getRow(unsigned int row) {
   vector<string*> retVec;
   if(row >= this->rowCount() || row < 0)
      return retVec;
   for(MyCol* col : *this)
      retVec.push_back(&col->at(row));
   return retVec;
}
void MyCSV::insert(unsigned int pos, MyCSV &columns) {
   Table.insert(Table.begin()+pos, columns.begin(), columns.end());
   columns.free(0, -1);
}
vector<vector<string>> MyCSV::recVec(bool normal, bool headers) { //record-vector
   vector<vector<string>> recVec;
   
   headers ? recVec.resize(rowCount()+1) :
             recVec.resize(rowCount());
   for(auto &row : recVec)
      row.reserve(this->size());

   for(MyCol* col : *this) {
      auto oldRowItem = col->begin();
      auto newRow     = recVec.begin();
      if(headers) {
         newRow->push_back(col->header());
         newRow++;
      }
      while(newRow != recVec.end()) {
         if(normal || !oldRowItem->empty())
            newRow->push_back(*oldRowItem);
         newRow++; oldRowItem++;
      }
   }
   //this if check isn't really neccassary; it's just here for clarity
   //i.e if(normal), then retTable is at its smallest size
   if(!normal) {
      for(auto &row : recVec) {
         row.shrink_to_fit();
      }
   }
   return std::move(recVec);
}
//element_destruction/release******************************
void MyCSV::free(unsigned int pos, int size) {
   if(size == -1)
      size = this->size();
   Table.erase(Table.begin()+pos, Table.begin()+size);
}
//table_manipulation***************************************
void MyCSV::setHeaders(bool headers) {
   Headers = headers;
   for(MyCol* col : *this) {
      col->header(headers);
   }
}
void MyCSV::setHeaders(vector<string> headers) {
   if(!Headers)
      setHeaders(true);
   if(headers.size() > this->size()) {
      this->resize(headers.size());
   }
   for(unsigned int i=0; i<headers.size() ; i++) {
      this->at(i)->header(headers[i]);
   }
}
vector<string> MyCSV::getHeaders() {
   vector<string> headerVec;
   for(int i=0; i<this->size(); i++) {
      if(Headers) {
         headerVec.push_back(this->at(i)->header());
      } else {
         headerVec.push_back(std::to_string(i++));
      }
   }
   return headerVec;
}
void MyCSV::splitCol(unsigned int column) {
   if(column >= this->size()) {
      cerr << "Error, the requested column is not available!\n";
      return;
   }
   if(!this->at(column)->subDelim()) { //TODO: test that this is working properly once again
      cerr << "Error, a subdelim must be set before spliting a column!\n";
      return;
   }
   MyCol* col = this->at(column);
   char subDelim = *col->subDelim();
   int maxSubItems = 0;
   for(const string &str : *col) {
      int subItems = 0;
      if(!str.empty()) {
         subItems++;
      }
      for(char ch : str) {
         if(ch == subDelim)
            subItems++;
      }
      if(subItems > maxSubItems) {
         maxSubItems = subItems;
      }
   }
   if(maxSubItems <= 1)
      return;
   MyCSV split;
   split.resize(maxSubItems);
   split.setDelim(*col->subDelim());

   for(unsigned int row = 0; row < col->size() ; row++) {
      col->at(row) += 4; //ascii code for EOT
      char* ch = &col->at(row)[0];
      split.addRow(split.parseRow(ch));
   }
   if(Headers) {
      split.setHeaders(Headers);
   }
   if(col->header() != "NA") { //a column may have a header without the tables header flag being set
      int i = 0;
      for(MyCol* c : split) {
         c->header(col->header() + ": " + std::to_string(i++));
      }
   }
   Table.erase(Table.begin()+column);
   this->insert(column, split);
}
//TODO: Gracefully handle empty cell in start column, currently if such is the case,
//      there will be a leading delim. This isn't the biggest issue so I'm leaving it for now.
MyCol* MyCSV::joinCols(unsigned int first, unsigned int last) {
   if(first == last-1) {
      return this->at(first);
   } else if (first == last) { //error check, first should never equal last
      return nullptr;
   }
   unsigned int pos = first;
   auto startCol = this->at(pos++);
   auto curCol   = this->at(pos);
   unsigned int rows = rowCount();
   if(!startCol->subDelim()) {
      startCol->subDelim(' ');
   }
   while(pos < last && curCol) {
      for(int row=0; row<rows ; row++) {
         if(!curCol->at(row).empty()) {
            startCol->at(row) += *startCol->subDelim() + curCol->at(row);
         }
      }
      delete curCol;
      curCol = this->at(++pos);
   }
   Table.erase(this->begin()+first+1, this->begin()+pos);
   return startCol;
}
MyCol* MyCSV::joinCols(vector<bool> cols) {
   int first = -1;
   int last  = -1;
   for(int i=0; i<cols.size() ; i++) {
      if(!cols[i])
         continue;
      if(first == -1) {
         first = i;
         last = i+1;
      } else {
         MyCol* tmpCol = Table[i];
         for(int t=i; t > last ; t--) {
            Table[t] = Table[t-1];
         }
         Table[last++] = tmpCol;
      }
   }
   if(first == -1) {
      cerr << "Error: cannot join columns as none are selected!\n";
      return nullptr;
   }
   return joinCols(first, last);
}
void MyCSV::resize(int cols, int rows) {
   int size = this->size();
   if(cols > size)
      while(cols > size++) {
         MyCol* col = new MyCol;
         col->resize(rowCount());
         col->header(Headers);
         Table.push_back(col);
      }
   else if (cols < size && cols != -1)
      while(cols < size--) {
         Table.back()->clear();
         delete Table.back();
         Table.pop_back();
      }

   if(rows != -1)
      for(MyCol* col : *this) {
         col->resize(rows);
      }
}
void MyCSV::addCol(string header) {
   MyCol* col = new MyCol;
   col->resize(rowCount());
   if(Headers || header != "NA") {
      col->header(header);
   }
   Table.push_back(col);
}
void MyCSV::addCol(MyCol &col) {
   MyCol* c = new MyCol(col);
   Table.push_back(std::move(c));
}
void MyCSV::addCol(MyCol* col) {
   Table.push_back(col);
}
void MyCSV::addRow(vector<string> row) {
   if(row.size() > this->size())
      this->resize(row.size());
   else if(row.size() < this->size())
      row.resize(this->size());

   for(unsigned int i=0; i<this->size(); i++) {
      this->at(i)->emplace_back(row[i]);
   }
}
void MyCSV::addRow() {
   for(MyCol* col : *this) {
      col->push_back("");
   }
}
//*********************************************************
