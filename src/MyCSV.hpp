#ifndef JANITOR_H
#define JANITOR_H
#include <vector>
#include <fstream>
#include <string>
using std::vector;
using std::string;
class MyCSV { //NOLINT
   public:
      MyCSV() { setDefaults(); } //NOLINT
      void clear() { 
         Table.clear(); 
         setDefaults(); 
      };
      //Delim & Quote are not set to null bytes so `writeTable` can be used
      //without the need to explicitly set Delim/Quote
      void setDefaults() {
         InFile   = "";
         OutFile  = "";
         Delim    = ';';
         Quote    = '"';
         ColCount = 0;
      }
      //mainly used for instrospection/debugging
      void prettyPrintMetaData () const;
      
      //primary methods
      void readTable  (string inFile  = "NA", bool header=true);
      void writeTable (string outFile = "NA");
      vector<string>& getCol(int i);
      vector<string>& operator[](int i);
      void resize(int size); //number of collumns

      //getters
      char   getDelim   () const {return Delim;};
      string getHeader  (int col) const {return Headers[col];};
      string getInFile  () const {return InFile;};
      string getOutFile () const {return OutFile;};
      char   getQuote   () const {return Quote;};
      char   getSubDelim(int col) const {return SubDelims[col];};
      int    size       () const {return ColCount;};
      int    getRowCount();

      //setters
      void setDelim   (char   delim)   {Delim   = delim;};
      void setOutFile (string outFile) {OutFile = outFile;};
      void setQuote   (char   quote)   {Quote   = quote;};
      void addHeader  (string header);
      void setHeader  (int col, string header);
      void setSubDelim(int col, char subDelim);

   private:
      int    ColCount;
      char   Delim;
      char   Quote;
      string InFile;
      string OutFile;
      vector<char> SubDelims;
      vector<string> Headers;
      vector<vector<string>> Table;

      //helpers
      enum CharClass{DELIM, RDELIM, QUOTE, CHAR};
      CharClass charClass(char &ch) const;
      void makeTable(char* buffer, int bufferSize);
};
#endif
