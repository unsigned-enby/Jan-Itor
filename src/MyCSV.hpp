#ifndef JANITOR_H
#define JANITOR_H
#include <vector>
#include <fstream>
#include <string>
#include "MyCol.hpp"
class MyCSV {
   public:
      MyCSV()  { 
         setDefaults(); 
      }
      ~MyCSV() { clear(); }
      void setDefaults() {
         Delim    = ';';
         Quote    = '"';
         Headers  = true;
         InFile   = "";
         OutFile  = "";
      }
      void clear() { 
         for(MyCol* col : Table) {
            col->clear();
            delete col;
         }
         Table.clear(); 
         setDefaults(); 
      };
      ///meta/calculated info
      void prettyPrintMetaData () const;
      unsigned int size     () const {return Table.size();}; //column count
      unsigned int rowCount ();
      
      ///table I/O & parsing functions
      void readTable  (string inFile  = "NA");
      void writeTable (string outFile = "NA");
     
      ///element access and iterators
      MyCol* at(unsigned int i);
      MyCol* operator[](unsigned int i);
      MyCol* getCol(string header);
      const vector<string*> getRow(unsigned int row);
      vector<MyCol*>::iterator begin() { return Table.begin(); }
      vector<MyCol*>::iterator end()   { return Table.end();   }
      void insert(unsigned int pos, MyCSV &columns);
      vector<vector<string>> recVec(bool normal, bool includeHeaders); //record vector
     
      ///element destruction/release
      void free(unsigned int pos, int size);
      
      ///table-maninpulation
      void setHeaders(bool headers);
      void setHeaders(vector<string> headers);
      vector<string> getHeaders(); 
      void splitCol(unsigned int col);
      MyCol* joinCols(unsigned int first, unsigned int last);
      MyCol* joinCols(vector<bool> cols);
      void resize(int cols, int rows = -1); //number of collumns/row count
      void addCol(string header = "NA");
      void addCol(MyCol &col);
      void addCol(MyCol* col);
      void addRow(vector<string> row);
      void addRow();

      ///memeber getters
      string getInFile  () const {return InFile;};
      string getOutFile () const {return OutFile;};
      char   getDelim () const {return Delim;};
      char   getQuote () const {return Quote;};

      ///member setters
      void setOutFile (string outFile) {OutFile = outFile;};
      void setDelim   (char   delim)   {Delim   = delim;};
      void setQuote   (char   quote)   {Quote   = quote;};

   private:
      char Delim;
      char Quote;
      bool Headers = true;
      string InFile;
      string OutFile;
      vector<MyCol*> Table;
      
      ///helpers
      enum CharClass{DELIM, RDELIM, QUOTE, EOT, CHAR};
      CharClass charClass(char ch) const;
      
      ///Parsers
      ///atomic parsers
      string parseChar (char* &buffer);
      string parseQuote(char* &buffer);
      ///'object' parsers
      vector<string> parseRow(char* &buffer);
      void parseTable(char* buffer);

};
#endif
