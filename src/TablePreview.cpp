#include "Components.hpp"
#include <ftxui/dom/table.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
using namespace ftxui;
Component TablePreview(MyCSV* tab) {
   class Impl : public ComponentBase {
      private:
         MyCSV* Tab;
         int y = 0;
         int x = 0;
         bool show = false;
         Element savedElement;
         Component ShowButton;
         Component TableRenderer;
         Component TableWindow;
         void drawTable() {
            if(Tab->size() == 0) {
               savedElement = text("");
               return;
            }
            auto table = Table(Tab->recVec());
            auto tableSelection = table.SelectAll();
            tableSelection.Separator(DASHED);
            tableSelection.DecorateAlternateRow(bgcolor(Color::DeepSkyBlue3Bis));
            savedElement = table.Render();
         }
      public:
         Impl(MyCSV* tab) : Tab(tab) {
            drawTable();
            ShowButton = Button("Preview Table", [&] { drawTable() ; show = true; }) | flex_shrink;
            TableRenderer = Renderer([&] { 
                  return  savedElement |  vscroll_indicator | focusPosition(x, y) | frame; });
            TableWindow = Window({
                  .inner = TableRenderer,
                  .title = "Table Preview",
                  .width = 2000,
                  .height = 2000,
                  });
            Add(Modal(ShowButton, TableWindow, &show));
         }
         Element Render() override {
            if(!show) {
               return vbox(ShowButton->Render());
            } else {
               TableWindow->TakeFocus();
               return TableWindow->Render();
            }
         }
         bool OnEvent(Event event) override {
            if(!Focused())
               return false;
            if(ActiveChild() && ActiveChild()->OnEvent(event))
               return true;
            if(event.is_character()) {
               switch(event.character()[0]) {
                  case 'j':
                     y+=1;
                     return true;
                  case 'J':
                     y += 10;
                     return true;
                  case 'k':
                     if(y>0)
                        y -= 1;
                     return true;
                  case 'K':
                     if( y > 10)
                        y -= 10;
                     return true;
                  case 'l':
                     x += 1;
                     return true;
                  case 'L':
                     x += 10;
                     return true;
                  case 'h':
                     if(x>0)
                        x -= 1;
                     return true;
                  case 'H':
                     if(x > 10)
                        x -= 10;
                     return true;
                  case 'q':
                     show = false;
                     ShowButton->TakeFocus();
                     return true;
                  default:
                     return false;
               }
            } else if(event == Event::Escape) {
               show = false;
               ShowButton->TakeFocus();
               return true;
            }
            return false;
         }
         bool Focusable() const override { return true; }
   };
   return Make<Impl>(tab);
}
