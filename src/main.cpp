#include <string>
#include <iostream>
#include <vector>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "MyCSV.hpp"
#include "MyCSVMenu.hpp"
#include "IpFormMenu.hpp"
#include "SerialMenu.hpp"
using namespace ftxui;

int main() {
   MyCSV Csv;
   auto screen    = ScreenInteractive::FitComponent();
   auto CsvMen    = MyCSVMenu(&Csv);
   auto IpMen     = IpFormMenu(&Csv);
   auto SerMen    = SerialMenu(&Csv);
   auto container = Container::Horizontal({
                               Container::Vertical({CsvMen, IpMen}),
                               SerMen});
   auto component = CatchEvent(container, [&](Event event) {
         if(event == Event::F1) {
            screen.ExitLoopClosure()();
            return true;
         }
         return false;
   });
   screen.Loop(component);
}
