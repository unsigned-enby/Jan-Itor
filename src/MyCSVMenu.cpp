#include "MyCSVMenu.hpp"
#include "DelimDropDown.hpp"
using namespace ftxui;
Component MyCSVMenu(MyCSV* Csv) {
   class Impl : public ComponentBase {
      private:
         MyCSV* Csv;
         string InFile;
         string OutFile;
         char Delim = '\0';
         std::vector<std::string> Quotes {"\"", "'"};
         int SelectedQuote = 0;   

         Component InputFile;
         Component OutputFile;
         Component QuoteToggle;
         Component DelimDrop;
         Component ReadButton;
         Component SaveButton;
      public:
         //Component implimintation
         Impl(MyCSV* csv) : Csv(csv) { //NOLINT
            InputFile   = Input(&InFile,  "Input File")  | border;
            OutputFile  = Input(&OutFile, "Output File") | border;
            QuoteToggle = Toggle(&Quotes, &SelectedQuote);
            DelimDrop   = DelimDropDown(&Delim) | size(HEIGHT, LESS_THAN, 4);
            ReadButton  = Button("Open File", [&] {Csv->setDelim(Delim);
                                                   Csv->setQuote(Quotes[SelectedQuote][0]); 
                                                   Csv->readTable(InFile);});
            SaveButton  = Button("Save File", [&] {Csv->writeTable(OutFile);}); 
            //navigation configuration
            Add(Container::Vertical({
                     Container::Horizontal({InputFile, ReadButton}),
                     Container::Horizontal({OutputFile, SaveButton}),
                     Container::Horizontal({DelimDrop, QuoteToggle}),
                     }));                  
         } 
         //display configuration
         Element Render() override {
            return window(text("CSV Menu"), 
                          vbox(hbox(text("Column count: " + std::to_string(Csv->size())), 
                                    text(" "), filler(),
                                    text("Row Count: " + std::to_string(Csv->getRowCount()))),
                               hbox(InputFile->Render(),  ReadButton->Render()),
                               hbox(OutputFile->Render(), SaveButton->Render()),
                               hbox(window(text("Delimiter"), DelimDrop->Render()), 
                                    filler(), vbox(
                                    window(text("Quote"), QuoteToggle->Render())))
                          ));
         }
         bool Focusable() const override { return true; };
   };
   return Make<Impl>(Csv);
}
