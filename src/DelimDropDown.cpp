#include "Components.hpp"
using namespace ftxui;
using std::vector;
using std::string;


Component DelimDropDown(char* delim, string title) {
class Impl : public ComponentBase {
   private:
      char* Delim;
      std::vector<std::string> DelimChoices {"TAB", "VTAB",
         "SPACE", "NULL", ";", ",", ":", "%", "#", "$", "&", "@", "?"};
      string Label = DelimChoices[0];
      string Title;
      int DelimIdx = 0;
      bool Show = false;
      Component radioBox;
   public:
      Impl(char* delim, string title) : Delim(delim), Title(title) {
         *Delim = '\t';
         RadioboxOption Rbo;
         Rbo.selected = &DelimIdx;
         Rbo.entries = &DelimChoices;
         Rbo.on_change = [&] {
            Label = DelimChoices[DelimIdx];
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
         };
         radioBox = Radiobox(Rbo);
         Add(radioBox);
      }
      Element Render() override {
         //"▶" "▼"
         Element retElem;
         if(Show) {
            retElem = vbox(text("▼ " + Label),
                  radioBox->Render() | vscroll_indicator | frame);
         } else {
            retElem = text("▶ " + Label); //might need to do -1 if not using window
            if(Focused()) {
               retElem = retElem | inverted ; //might need to do -1 if not using window
            }
         }
         return vbox(window(text(Title), retElem));
      }
      bool OnEvent(Event event) override {
         if(!Focused() && !ActiveChild()) {
            return false;
         }
         if(event == Event::Character(' ') || event == Event::Return) {
            if(Show)
               radioBox->OnEvent(event);
            Show = !Show;
            return true;
         } else if(!Show) {
            return false;
         }
         if(ActiveChild()->OnEvent(event)) {
            return true;
         }
         return true; //make event handling greedy iff !Show
      }
      bool Focusable() const override { return true; }
};
   return Make<Impl>(delim, title);
}

//"▶" "▼"

