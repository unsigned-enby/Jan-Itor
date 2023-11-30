#include "DelimDropDown.hpp"
using namespace ftxui;
using std::vector;
using std::string;

Component DelimDropDown(char* delim) {
   class Impl : public ComponentBase {
      private:
         char* Delim;
         
         std::vector<std::string> DelimChoices {"TAB", "VTAB",
            "SPACE", "NULL", ";", ",", ":", "%", "#", "$", "&", "@", "?"};
         string Delimiter = DelimChoices[0];
         int DelimIdx  = 0;
         bool Show = false;
   
         Component collapsible;
         Component radioBox;
      public:
         Impl(char* delim) : Delim(delim) {
            *Delim = '\t';
            auto Rbo = RadioboxOption::Simple();
            Rbo.on_change = [&] { 
               switch(DelimIdx) {
                  case 0:
                     *Delim = '\t';
                     break;
                  case 1:
                     *Delim = '\v';
                     break;
                  case 2:
                     *Delim = ' ';
                     break;
                  case 3:
                     *Delim = '\0';
                     break;
                  default:
                     *Delim = DelimChoices[DelimIdx][0];
                     break;
               }
               if(Delimiter != DelimChoices[DelimIdx]) {
                  Delimiter = DelimChoices[DelimIdx];
               }
            };
            radioBox = Radiobox(&DelimChoices, &DelimIdx, Rbo) | vscroll_indicator | frame;
            collapsible = Collapsible(&Delimiter, radioBox, &Show);
            Add(collapsible);
         }
         bool OnEvent(Event event) override {
            bool retBool = false;
            if(event == Event::Return) {
               if(Show) {
                  retBool = radioBox->OnEvent(event);
               } else {
                  retBool = true;
               }
               Show = !Show;
            } else {
               retBool = ChildAt(0)->OnEvent(event);
            }
            if(Show) {
               radioBox->TakeFocus();
            }
            else {
               collapsible->ChildAt(0)->ChildAt(0)->TakeFocus();
            }
            return retBool;
         }
         bool Focusable() const override { return true; }
   };

   return Make<Impl>(delim);
}
