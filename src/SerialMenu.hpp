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
      vector<string> ColumnLables;
      vector<string> Targets;
      bool* RespChoices;
      bool* PID_Choices;
      std::string TargetFile;
      std::string SaveFile;
      vector<string> UniqWords;
      vector<string> CorrectedWords;
      Component TargetInput;
      Component SaveInput;
      Component RespSel;
      Component PIDSel;
      Component GetButton;
      Component MakeButton;
      Component SaveButton;
      Component SerializeButton;
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
         TargetInput = Input(&TargetFile, "Target File");
         SaveInput   = Input(&SaveFile, "Save File");
         RespSel     = ListToggle("Responses", &ColumnLables, &RespChoices) | size(HEIGHT, LESS_THAN, 6);
         PIDSel      = ListToggle("PID",       &ColumnLables, &PID_Choices) | size(HEIGHT, LESS_THAN, 6);
         
         GetButton = Button("GetWords", [&] { 
            MyCol* pidCol  = nullptr;
            MyCol* respCol = nullptr;
            vector<bool> responses(Table->size(), false);
            for(int i=0; i<Table->size(); i++) {
               if(PID_Choices[i]) {
                  if(pidCol) {
                     std::cerr << "Warning, more than one pid column selected "
                               << "using the first one found.\n";
                  } else if(responses[i]) {
                     std::cerr << "Warning, a column is selected as both a pid col "
                               << "and a response col. PID will take precedence.\n";
                     responses[i] = false;
                  } else {
                     pidCol = Table->at(i);
                     continue;
                  }
               }
               responses[i] = RespChoices[i];
            }
            respCol = Table->joinCols(responses);
            Targets = readTargetWords(TargetFile);
            UniqWords = getUniqWords(respCol, pidCol);
            std::sort(UniqWords.begin(), UniqWords.end());
            CorrectionMenu->OnEvent(Event::Character("REDRAW"));
         });
         MakeButton = Button("Make corrections", [&] {
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
         SaveButton = Button("SaveWords", [&] {
            std::ofstream outFile(SaveFile);
            if(!outFile) {
               std::cerr << "Error, could not open the save file >" << SaveFile << "< for writing!\n";
               return;
            }
            for(string str : UniqWords) 
               outFile << '\n' << str;
            outFile.close();
            SaveFile.clear();
         });
         SerializeButton = Button("Serialize & Save", [&] {
            MyCSV* pos = serialize(); 
            if(pos) {
               pos->writeTable(SaveFile);
               delete pos; //NOLINT
            }
            SaveFile.clear();
         });
         //The `InputContainer` class will automatically resize CorrectedWords to that
         //of UniqWords upon change to UniqWords
         CorrectionMenu = InputContainer(&UniqWords, &CorrectedWords); //NOLINT
         
         Add(Container::Vertical({TargDisplay,
               Container::Horizontal({
                        Container::Vertical({TargetInput,
                                             SaveInput,
                                             RespSel,
                                             PIDSel,
                                             Container::Horizontal({GetButton,SaveButton}),
                                             MakeButton,
                                             SerializeButton}),
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
                                 window(text("Response Column"), RespSel->Render()),
                                 window(text("PID Column"),      PIDSel->Render()),
                                 hbox(GetButton->Render(), SaveButton->Render()),
                                 MakeButton->Render(), 
                                 SerializeButton->Render()),
                            window(text("Unique Words: " + std::to_string(UniqWords.size())), 
                                 CorrectionMenu->Render()) | size(HEIGHT, EQUAL, 19))
                       ));
      }         
      bool Focusable() const override { return true; };
};
#endif
