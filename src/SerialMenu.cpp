#include <algorithm>
#include <iostream>
#include "InputContainer.hpp"
#include "SerialMenu.hpp"
using namespace ftxui;
using std::vector;
Component SerialMenu(MyCSV* Csv) {
   class Impl : public Serial, public ComponentBase {
      private:
         MyCSV* Csv; //NOLINT
         vector<string> ColumnLables {"NA"};
	      std::string TargetFile;
	      std::string SaveFile;
         int RespCol;
         int PID_Col;
         vector<string> UniqWords;
         vector<string> CorrectedWords;
         Component TargetInput;
         Component SaveInput;
         Component RespToggle;
         Component PIDToggle;
         Component GetButton;
         Component MakeButton;
         Component SerializeButton;
         Component CorrectionMenu;

         Element TargDisplay = text(""); //placeholder
         int numCols = 0;
         
         void configTargDisplay(vector<string> targets) {
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
         Impl(MyCSV* csv) : Csv(csv) { //NOLINT
            std::function<void()> makeAction = [&] {
               if(!CorrectedWords.empty()) {
                  makeCorrections(CorrectedWords);
               }
            };
            std::function<void()> serializeAction = [&] {
               MyCSV* pos = serialize(); 
               if(pos) {
                  pos->writeTable(SaveFile);
                  delete pos; //NOLINT
               }
            };
            std::function<void()> getAction = [&] { 
               if(!TargetFile.empty()) {
                  configTargDisplay(readTargetWords(TargetFile));
               }
               setPIDS(&Csv->getCol(PID_Col));
               SubDelim  = Csv->getSubDelim(RespCol);
               UniqWords = setResponses(&Csv->getCol(RespCol));
            };
            TargetInput     = Input(&TargetFile, "Target File");
            SaveInput       = Input(&SaveFile, "Save File");
            RespToggle      = Toggle(&ColumnLables, &RespCol);
            PIDToggle       = Toggle(&ColumnLables, &PID_Col);
            GetButton       = Button("GetWords", getAction);
            MakeButton      = Button("Make corrections", makeAction);
            SerializeButton = Button("Serialize & Save", serializeAction);
            //The `InputContainer` class will automatically resize CorrectedWords to that
            //of UniqWords upon change to UniqWords
            CorrectionMenu  = InputContainer(&UniqWords, &CorrectedWords); //NOLINT
            
            Add(Container::Horizontal({
                           Container::Vertical({TargetInput,
                                                SaveInput,
                                                RespToggle,
                                                PIDToggle,
                                                GetButton,
                                                MakeButton,
                                                SerializeButton}),
                           CorrectionMenu}));
         }
         Element Render () override { 
            //this is an if->if instead of && so that ColumnLables does not update imediately after 
            //being set to "NA", as "NA" make Csv->size()(i.e 0) != ColumnLabels.size()(i.e 1)
            if(Csv->size() == 0) {
               if (ColumnLables[0] != "NA") {
                  ColumnLables.clear();
                  ColumnLables.push_back("NA");
               }
            }
            else if (Csv->size() != ColumnLables.size()){
               ColumnLables.resize(Csv->size());
               for(int i=0; i < Csv->size(); i++)
                  ColumnLables[i] = '0' + i; //NOLINT
            }
            int uniqWordCount = 0;
            if(!UniqWords.front().empty())
               uniqWordCount = UniqWords.size();
            return window(text("Serial Positioning & Corrections"), vbox(
                          TargDisplay,
                          hbox(vbox(window(text("Files"), vbox(
                                           TargetInput->Render(), 
                                           SaveInput->Render())),
                                    window(text("Response Column"), RespToggle->Render()),
                                    window(text("PID Column"),      PIDToggle->Render()),
                                    GetButton->Render(),
                                    MakeButton->Render(), 
                                    SerializeButton->Render()),
                               window(text("Unique Words: " + std::to_string(uniqWordCount)), 
                                      CorrectionMenu->Render()) | size(HEIGHT, EQUAL, 19))
                          ));
         }         
         bool Focusable() const override { return true; };
   };
   return Make<Impl>(Csv);
}
