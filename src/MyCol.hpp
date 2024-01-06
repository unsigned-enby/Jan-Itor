#ifndef MYCOL_H
#define MYCOL_H
#include <vector>
#include <string>
#include <iostream>
using std::vector;
using std::string;
class MyCol {
   //friend class MyCSV;
   private:
      vector<string> Column;
      string Header  = "NA";
      char* SubDelim = nullptr;
   public:
      MyCol() {
         Header   = "NA";
         SubDelim = nullptr;
      }
      MyCol(MyCol& col) {
         Column = col.Column;
         Header = col.Header;
         if(col.SubDelim) {
            SubDelim = new char;
            *SubDelim = *col.SubDelim;
         }
      }
      MyCol(string header, char* subDelim = nullptr) {
         Header = header;
         subDelim ? 
            SubDelim = new char(*subDelim):
            SubDelim = nullptr;
      }
      ~MyCol() {
         clear();
      }
      void clear() {
         Column.clear();
         if(SubDelim) {
            delete SubDelim;
            SubDelim = nullptr;
         }
         Header = "NA";
      }
      void push_back(string field) { 
         Column.push_back(field); 
      }
      void emplace_back(string &field) { 
         Column.emplace_back(field); //should std::move be used here?
      }
      void resize (uint size) { 
         if(size == Column.size())
            return;
         Column.resize(size); 
      }
      
      string& operator[] (int i) { return Column[i]; }
      string& at (uint i) { return Column[i]; }
      vector<string>::iterator begin() {return Column.begin();}
      vector<string>::iterator end  () {return Column.end();}
      string& back() {return Column.back();}
      
      uint size () const {return Column.size();}

      string header() { return Header; }
      void header(string header) {
         Header = header;
      }
      void header(bool header)   {
         header ? Header = "" : Header = "NA";
      }
      
      char const* subDelim() const { return SubDelim; }
      void subDelim(char subDelim) {
         if(!SubDelim) {
            SubDelim = new char;
            *SubDelim = -1;
         }
         *SubDelim = subDelim;
      }
      void  subDelim(bool subDelim) { //mainly for unseting the subDelim
         if(subDelim == (bool)SubDelim)
            return;
         if(subDelim) {
            SubDelim  = new char;
            *SubDelim = -1;
         } else {
            delete SubDelim;
            SubDelim = nullptr;
         }
      }
};
#endif
