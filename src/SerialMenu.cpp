#include <algorithm>
#include <iostream>
#include "InputContainer.hpp"
#include "SerialMenu.hpp"
//TODO:
//   - add a button to write the (current) uniq word list to a file
using namespace ftxui;
using std::vector;
using std::string;
Component SerialMenu(MyCSV* Csv) {
   class Impl : public Serial, public ComponentBase {
      private:
         vector<string> ColumnLables {"NA"};
	      std::string TargetFile;
	      std::string SaveFile;
         int RespCol;
         int PID_Col;
         vector<string> UniqWords;
         vector<string> CorrectedWords;
         Component TargetInput;
         Component SaveInput;
         Component RespSel;
         Component PIDToggle;
         Component GetButton;
         Component MakeButton;
         Component SaveButton;
         Component SerializeButton;
         Component CorrectionMenu;
         
         Element TargDisplay = text(""); //placeholder
         void targetDisplay(vector<string> targets) {
            if(targets.empty()) {
               TargDisplay = text("");
               return;
            }
            Elements targDisplay;
            sort(targets.begin(), targets.end());
            string line;
            line.push_back(toupper(targets[0][0]));
            line.append(":-");
            for (string targ : targets) {
               if (toupper(targ[0]) != line[0]) {
                  targDisplay.push_back(text(line));
                  line.clear();
            		line.push_back(toupper(targ[0]));
                  line.append(":-");
               }
               line.append(" " + targ);
            }
            targDisplay.push_back(text(line));
            TargDisplay = window(text("Target Words"), vbox(targDisplay));
         }
      public:
         Impl(MyCSV* csv) { //NOLINT
            Table = csv;
            TargetInput = Input(&TargetFile, "Target File");
            SaveInput   = Input(&SaveFile, "Save File");
            RespSel     = Toggle(&ColumnLables, &RespCol);
            PIDToggle   = Toggle(&ColumnLables, &PID_Col);
            
            GetButton = Button("GetWords", [&] { 
               if(Table->size() == 0 || (RespCol == PID_Col)) {
                  return;
               }
               targetDisplay(readTargetWords(TargetFile));
               Table->at(RespCol)->header(string{"RESPONSES"});
               Table->at(PID_Col)->header(string{"PIDS"});
               
               UniqWords = getUniqWords(Table);
               std::sort(UniqWords.begin(), UniqWords.end());
               CorrectionMenu->OnEvent(Event::Custom);
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
               CorrectionMenu->OnEvent(Event::Custom);
            });
            SaveButton = Button("SaveWords", [&] {
               std::ofstream outFile(SaveFile);
               if(!outFile) {
                  std::cerr << "Error, could not open the save file >" << SaveFile << "< for writing!\n";
                  return;
               }
               outFile << "UNIQUE WORDS: " << UniqWords.size();
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
            
            Add(Container::Horizontal({
                           Container::Vertical({TargetInput,
                                                SaveInput,
                                                RespSel,
                                                PIDToggle,
                                                GetButton,
                                                Container::Horizontal({MakeButton,SaveButton}),
                                                SerializeButton}),
                           CorrectionMenu}));
         }
         Element Render () override { 
            //this is an if->if instead of && so that ColumnLables does not update imediately after 
            //being set to "NA", as "NA" make Csv->size()(i.e 0) != ColumnLabels.size()(i.e 1)
            if(Table->size() == 0) {
               if (ColumnLables[0] != "NA") {
                  ColumnLables.clear();
                  ColumnLables.push_back("NA");
               }
            }
            else if (Table->size() != ColumnLables.size()){
               ColumnLables.resize(Table->size());
               for(int i=0; i < Table->size(); i++)
                  ColumnLables[i] = std::to_string(i); //NOLINT
            }
            return window(text("Serial Positioning & Corrections"), vbox(
                          TargDisplay,
                          hbox(vbox(window(text("Files"), vbox(
                                           TargetInput->Render(), 
                                           SaveInput->Render())),
                                    window(text("Response Column"), RespSel->Render()),
                                    window(text("PID Column"),      PIDToggle->Render()),
                                    hbox(GetButton->Render(), SaveButton->Render()),
                                    MakeButton->Render(), 
                                    SerializeButton->Render()),
                               window(text("Unique Words: " + std::to_string(UniqWords.size())), 
                                    CorrectionMenu->Render()) | size(HEIGHT, EQUAL, 19))
                          ));
         }         
         bool Focusable() const override { return true; };
   };
   return Make<Impl>(Csv);
}
