#include <iostream>
#include <array>
#include "DelimDropDown.hpp"
#include "IpFormMenu.hpp"
using namespace ftxui;
using std::vector;
using std::string;
Component IpFormMenu(MyCSV* Csv) {
   class Impl : public IpForm, public ComponentBase {
      private:
         MyCSV* Csv;
         char Delim = '\0'; 
         string BlackSpace;
         string Removles;
         std::array<bool,3> BSR {false, false, false}; //BlackSpaceRanges
         std::array<bool,3> RR  {false, false, false}; //RemovleRanges
         
         int SelectedColumn = 0;
         vector<string> ColumnLables {"NA"};

         Component InputBlackSpace;
         Component RangedBlackSpace;
         Component RangedRemovles;
         Component InputRemovles;
         Component DelimDrop;
         Component ProcessButton;
         Component ColumnToggle;
         bool showDelimDrop = false;
      public:
         Impl(MyCSV* csv) : Csv(csv) { //NOLINT

            InputBlackSpace  = Input(&BlackSpace, "Black Space");
            RangedBlackSpace = Container::Horizontal({
                  Checkbox("a-z", &BSR[0]),
                  Checkbox("A-Z", &BSR[1]),
                  Checkbox("0-9", &BSR[2])
                  });
            InputRemovles  = Input(&Removles, "Removles");
            RangedRemovles = Container::Horizontal({
                  Checkbox("a-z", &RR[0]),
                  Checkbox("A-Z", &RR[1]),
                  Checkbox("0-9", &RR[2])
                  });
            DelimDrop = DelimDropDown(&Delim) | size(HEIGHT, LESS_THAN, 4);
            ColumnToggle = Toggle(&ColumnLables, &SelectedColumn);
            //configColumnToggle();   
            auto buttonAction = [&] {
               setBlackSpace(BlackSpace, true);
               setBlackSpace(BSR);
               setRemovles(Removles, true);
               setRemovles(RR);
               Csv->at(SelectedColumn)->subDelim(Delim); 
               cleanColumn(Csv->at(SelectedColumn));
            };
            ProcessButton = Button("Clean Column", buttonAction);
            //navigation configuration
            Add(Container::Vertical({
                           Container::Horizontal({InputBlackSpace, RangedBlackSpace}),
                           Container::Horizontal({InputRemovles,   RangedRemovles}),
                           Container::Horizontal({DelimDrop, ColumnToggle}),
                           ProcessButton})); 
         } 
         Element Render() override {
            //this is an if->if instead of && so that ColumnLables is not updated imediately after
            //being set to "NA", as "NA" make Csv->size()(i.e 0) != ColumnLabels.size()(i.e 1)
            if(Csv->size() == 0) {
               if (ColumnLables[0] != "NA") {
                  ColumnLables.clear();
                  ColumnLables.push_back("NA");
               }
            }
            else if (Csv->size() != ColumnLables.size()){
               ColumnLables.resize(Csv->size());
               for(int i=0; i < Csv->size(); i++)
                  ColumnLables[i] = '0' + i; //NOLINT
            }
            return window(text("IpForm Menu"), 
                          vbox(window(hbox(text("Inputs"), filler(), text("Ranges")), 
                                      vbox(hbox(InputBlackSpace->Render(),RangedBlackSpace->Render()),
                                           hbox(InputRemovles->Render(),  RangedRemovles->Render()))),
                               hbox(window(text("Sub-Delim"), DelimDrop->Render()), 
                                    filler(), vbox(
                                    window(text("Column"), ColumnToggle->Render()))), 
                               ProcessButton->Render())
                          );
         }
         bool Focusable () const override {return true;}
   };
   return Make<Impl>(Csv);
}
