#include "Components.hpp"
#include <ftxui/dom/table.hpp>
using namespace ftxui;
Component TablePreview(MyCSV* table) {
   class Impl : public ComponentBase {
      private:
         MyCSV* Table;
         bool Show = false;
         int Y = 0;
         int X = 0;
         Dimensions dim;
         Element savedElement;
         
         Component ShowButton;
         Component TableRenderer;
         Component TableWindow;
         void makeTable() {
            if(Table->size() == 0) {
               savedElement = text("");
               return;
            }
            auto table = ftxui::Table(Table->recVec(true, true));
            table.SelectColumns(0, -1).Separator();
            table.SelectRows(0, -1).Separator(HEAVY);
            savedElement = table.Render();
            savedElement->ComputeRequirement();
            dim.dimx = savedElement->requirement().min_x;
            dim.dimy = savedElement->requirement().min_y;
         }
      public:
         Impl(MyCSV* table) : Table(table) {
            makeTable();
            ShowButton = Button("Preview Table", [&] { makeTable() ; Show = true; });
            TableRenderer = Renderer([&] { return  savedElement | 
                                                   vscroll_indicator | 
                                                   focusPosition(X, Y) | 
                                                   frame; });
            TableWindow = Window({
                  .inner = TableRenderer,
                  .title = "Table Preview: Use h/j/k/l (+Shift) to navigate. Press q or Esc to exit.",
                  .width = 2000, //arbitrarily large sizes
                  .height = 2000,
                  });
            Add(Modal(ShowButton, TableWindow, &Show));
         }
         Element Render() override {
            if(!Show) {
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
                     Y<dim.dimy    ? Y+=1  : Y=dim.dimy;
                     return true;
                  case 'J':
                     Y<dim.dimy-10 ? Y+=10 : Y=dim.dimy;
                     return true;
                  case 'k':
                     Y>0  ? Y-=1  : Y=0;
                     return true;
                  case 'K':
                     Y>10 ? Y-=10 : Y=0;
                     return true;
                  case 'l':
                     X<dim.dimx    ? X+=1  : X=dim.dimx;
                     return true;
                  case 'L':
                     X<dim.dimx-10 ? X+=10 : X=dim.dimx;
                     return true;
                  case 'h':
                     X>0  ? X-=1  : X=0;
                     return true;
                  case 'H':
                     X>10 ? X-=10 : X=0;
                     return true;
                  case 'q':
                     Show = false;
                     ShowButton->TakeFocus();
                     return true;
                  default:
                     return false;
               }
            } else if(event == Event::Escape) {
               Show = false;
               ShowButton->TakeFocus();
               return true;
            }
            return false;
         }
         bool Focusable() const override { return true; }
   };
   return Make<Impl>(table);
}
