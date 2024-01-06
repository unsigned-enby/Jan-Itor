#ifndef MYCSV_MENU_H
#define MYCSV_MENU_H
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/table.hpp>
#include "MyCSV.hpp"
#include "Components.hpp"
using namespace ftxui;
using std::vector;
using std::string;
class MyCSV_Menu : public ComponentBase {
   private:
      MyCSV* Csv;
      string InFile;
      string OutFile;
      char Delim = '\0';
      vector<string> Quotes {"\"", "'"};
      int SelectedQuote = 0;   

      bool ShowTable = false;
      Component InputFile;
      Component OutputFile;
      Component QuoteToggle;
      Component DelimDrop;
      Component Preview;
      Component ReadButton;
      Component SaveButton;
   public:
      //Component implimintation
      MyCSV_Menu(MyCSV* csv) : Csv(csv) { //NOLINT
         InputFile   = Input(&InFile,  "Input File")  | border;
         OutputFile  = Input(&OutFile, "Output File") | border;
         QuoteToggle = Toggle(&Quotes, &SelectedQuote);
         DelimDrop   = DelimDropDown(&Delim, "Delimiter") | size(HEIGHT, LESS_THAN, 6);
         Preview     = TablePreview(Csv);
         ReadButton  = Button("Open File", [&] {Csv->setDelim(Delim);
                                                Csv->setQuote(Quotes[SelectedQuote][0]); 
                                                Csv->readTable(InFile);});
         SaveButton  = Button("Save File", [&] {Csv->setDelim(Delim);
                                                Csv->setQuote(Quotes[SelectedQuote][0]); //not currently used
                                                Csv->writeTable(OutFile);
                                                OutFile.clear();}); 
         //navigation configuration
         Add(Container::Vertical({
                  Container::Horizontal({InputFile, ReadButton}),
                  Container::Horizontal({OutputFile, SaveButton}),
                  Container::Horizontal({DelimDrop, Preview, QuoteToggle}),
                  }));
      } 
      //display configuration
      Element Render() override {
         return window(text("CSV Menu"),
                       vbox(hbox(text("Column count: " + std::to_string(Csv->size())), 
                                 text(" "), filler(),
                                 text("Row Count: "    + std::to_string(Csv->rowCount()))),
                            hbox(InputFile->Render(),  ReadButton->Render()),
                            hbox(OutputFile->Render(), SaveButton->Render()),
                            hbox(DelimDrop->Render(), filler(), 
                                 Preview->Render(),   vbox(
                                 window(text("Quote"), QuoteToggle->Render())))
                       ));
      }
      bool Focusable() const override { return true; };
};
#endif
