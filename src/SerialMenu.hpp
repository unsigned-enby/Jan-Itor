#ifndef SERIAL_MENU_H
#define SERIAL_MENU_H
#include <algorithm>
#include <iostream>
#include <ftxui/component/component_base.hpp>
#include "MyCSV.hpp"
#include "Serial.hpp"
#include "Components.hpp"
//TODO:
//   - add a button to write the (current) uniq word list to a file
using namespace ftxui;
using std::vector;
using std::string;
class Serial_Menu : public Serial, public ComponentBase {
   private:
      MyCSV* Table;
      string TargetFile;
      string OutFile;
      vector<string> ColumnLables;
      vector<string> Targets;
      vector<string> UniqWords;
      vector<string> CorrectedWords;
      vector<bool>   RESP_Choices;
      vector<bool>   PID_Choices;
      Component TargetInput;
      Component SaveInput;
      Component RESP_Toggle;
      Component PID_Toggle;
      Component Get;
      Component Make;
      Component Save;
      Component Serialize;
      Component CorrectionMenu;
      Component TargDisplay = Collapsible("TargetDisplay", Renderer( [&] {
         if(Targets.empty()) {
            return text("");
         }
         Elements targDisplay;
         sort(Targets.begin(), Targets.end());
         string line;
         line.push_back(toupper(Targets[0][0]));
         line.append(":-");
         for (string targ : Targets) {
            if (toupper(targ[0]) != line[0]) {
               targDisplay.push_back(text(line));
               line.clear();
         		line.push_back(toupper(targ[0]));
               line.append(":-");
            }
            line.append(" " + targ);
         }
         targDisplay.push_back(text(line));
         return window(text("Target Words"), vbox(targDisplay));
      }));
   public:
      Serial_Menu(MyCSV* csv) : Table(csv) { //NOLINT
         ColumnLables = Table->getHeaders();
         TargetInput  = Input(&TargetFile, "Target File");
         SaveInput    = Input(&OutFile,   "Output File");
         RESP_Toggle  = ListToggle(&ColumnLables, &RESP_Choices)   | size(HEIGHT, LESS_THAN, 6);
         PID_Toggle   = ListToggle(&ColumnLables, &PID_Choices, 1) | size(HEIGHT, LESS_THAN, 6);
         //The `InputContainer` class will automatically resize CorrectedWords to that
         //of UniqWords upon change to UniqWords
         CorrectionMenu = InputContainer(&UniqWords, &CorrectedWords); //NOLINT
         
         ///Buttons_begin
         Get = Button("GetWords", [&] { 
            for(int i=0; i<Table->size(); i++) {
               if(PID_Choices[i]) {
                  if(RESP_Choices[i]) {
                     std::cerr << "Warning, a column is selected as both a pid col "
                               << "and a response col. PID will take precedence.\n";
                     RESP_Choices[i] = false;
                  } else {
                     PIDs = Table->at(i);
                  }
                  break;
               }
            }
            RESPs = Table->joinCols(RESP_Choices);
            UniqWords = init(TargetFile);
            //TargetFile.clear();
            
            Targets = getTargetWords();
            std::sort(UniqWords.begin(), UniqWords.end());
            CorrectionMenu->OnEvent(Event::Character("REDRAW"));
         });
         Make = Button("Make corrections", [&] {
            if(CorrectedWords.empty()) {
               return;
            }
            vector<std::pair<string, string>> corrections;
            for(int i=0; i<CorrectedWords.size(); i++) {
               if(CorrectedWords[i].empty()) {
                  continue;
               } else {
                  corrections.push_back({UniqWords[i], CorrectedWords[i]});
               }
            }
            UniqWords = makeCorrections(corrections);
            sort(UniqWords.begin(), UniqWords.end());
            CorrectionMenu->OnEvent(Event::Character("REDRAW"));
         });
         Save = Button("SaveWords", [&] {
            std::ofstream outFile(OutFile);
            if(!outFile) {
               std::cerr << "Error, could not open the save file >" << OutFile << "< for writing!\n";
               return;
            }
            for(string str : UniqWords) 
               outFile << '\n' << str;
            outFile.close();
            OutFile.clear();
         });
         Serialize = Button("Serialize & Save", [&] {
            MyCSV* pos = serialize(); 
            if(pos) {
               pos->writeTable(OutFile);
               delete pos; //NOLINT
            }
            OutFile.clear();
         });
         ///Buttons_end 
         
         Add(Container::Vertical({TargDisplay,
                Container::Horizontal({
                   Container::Vertical({TargetInput,
                                        SaveInput,
                                        Container::Horizontal({RESP_Toggle, PID_Toggle}),
                                        Container::Horizontal({Get,Save}),
                                        Make,
                                        Serialize}),
                   CorrectionMenu})}));
      }
      Element Render () override { 
         if(ColumnLables.size() != Table->size()) {
            ColumnLables = Table->getHeaders();
         }
         return window(text("Serial Positioning & Corrections"), vbox(
                       TargDisplay->Render(),
                       hbox(vbox(window(text("Files"), vbox(
                                        TargetInput->Render(), 
                                        SaveInput->Render())),
                                 hbox(vbox(window(text("Resp Col"), RESP_Toggle->Render())),
                                      vbox(window(text("PID Col"),  PID_Toggle->Render()))),
                                 hbox(Get->Render(), filler(), Save->Render()),
                                 Make->Render(), 
                                 Serialize->Render()),
                            window(text("Unique Words: " + std::to_string(UniqWords.size())), 
                                   CorrectionMenu->Render()) | size(HEIGHT, EQUAL, 15))
                       ));
      }         
      bool Focusable() const override { return true; };
};
#endif
