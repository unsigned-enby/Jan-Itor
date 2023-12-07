#ifndef IPFORM_MENU_H
#define IPFORM_MENU_H
#include <array>
#include <ftxui/component/component_base.hpp>
#include "MyCSV.hpp"
#include "IpForm.hpp"
#include "Components.hpp"
using namespace ftxui;
using std::vector;
using std::string;
class IpForm_Menu : public IpForm, public ComponentBase {
   private:
      MyCSV* Csv;
      char Delim = '\0'; 
      string BlackSpace;
      string Removles;
      std::array<bool,3> BSR {false, false, false}; //BlackSpaceRanges
      std::array<bool,3> RR  {false, false, false}; //RemovleRanges
      
      bool* SelectedColumns;
      vector<string> ColumnLables;
   
      Component InputBlackSpace;
      Component RangedBlackSpace;
      Component RangedRemovles;
      Component InputRemovles;
      Component DelimDrop;
      Component ProcessButton;
      Component SplitButton;
      Component JoinButton;
      Component ColumnToggle;
   public:
      IpForm_Menu(MyCSV* csv) : Csv(csv) { //NOLINT

         ColumnLables = Csv->getHeaders();
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
         DelimDrop     = DelimDropDown(&Delim, "Sub-Delim") | size(HEIGHT, LESS_THAN, 6);
         ProcessButton = Button("Clean" , [&] {
            setBlackSpace(BSR, BlackSpace);
            setRemovles  (RR,  Removles);
            for(int i=0; i<ColumnLables.size(); i++) {
               if(SelectedColumns[i]) {
                  Csv->at(i)->subDelim(this->Delim); 
                  cleanColumn(Csv->at(i));
               }
            }
         });
         SplitButton = Button("Split", [&] {
            for(int i=Csv->size()-1; i>=0; i--) {
               if(SelectedColumns[i]) {
                  Csv->splitCol(i);
               }
            }
         });
         JoinButton = Button("Join", [&] {
            vector<bool> selected(Csv->size(), false);
            for(int i=0; i<selected.size(); i++) {
               selected[i] = SelectedColumns[i];
               SelectedColumns[i] = false;
            }
            Csv->joinCols(selected);
         });
         ColumnToggle = ListToggle("Columns", &ColumnLables, &SelectedColumns) | size(HEIGHT, LESS_THAN, 6);
         //navigation configuration
         Add(Container::Vertical({
                        Container::Horizontal({InputBlackSpace, RangedBlackSpace}),
                        Container::Horizontal({InputRemovles,   RangedRemovles}),
                        Container::Horizontal({DelimDrop,       ColumnToggle}),
                        Container::Horizontal({ProcessButton, SplitButton, JoinButton})})); 
      } 
      Element Render() override {
         if (Csv->size() != ColumnLables.size()){
            ColumnLables = Csv->getHeaders();
         }
         return window(text("IpForm Menu"), 
                       vbox(window(hbox(text("Inputs"), filler(), text("Ranges")), 
                                   vbox(hbox(InputBlackSpace->Render(),RangedBlackSpace->Render()),
                                        hbox(InputRemovles->Render(),  RangedRemovles->Render()))),
                            hbox(DelimDrop->Render(), 
                                 filler(), vbox(
                                 window(text("Column"), ColumnToggle->Render()))), 
                            hbox(ProcessButton->Render() | flex_grow, SplitButton->Render() | flex_grow, JoinButton->Render() | flex_grow))
                       );
      }
      bool Focusable () const override {return true;}
};
#endif
