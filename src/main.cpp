#include <string>
#include <iostream>
#include <vector>
#include <cstdio>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include "MyCSV.hpp"
#include "MyCSVMenu.hpp"
#include "IpFormMenu.hpp"
#include "SerialMenu.hpp"
using namespace ftxui;

int main() {
   freopen("error.log", "w", stderr);
   auto screen = ScreenInteractive::FitComponent();
   MyCSV* Csv = new MyCSV;
   auto CsvMen    = Make<MyCSV_Menu>(Csv);
   auto IpMen     = Make<IpForm_Menu>(Csv);
   auto SerMen    = Make<Serial_Menu>(Csv);
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
