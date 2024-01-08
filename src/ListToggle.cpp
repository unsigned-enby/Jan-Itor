#include "Components.hpp"
#include<fstream>

using namespace ftxui;

using std::vector;
using std::string;
//Significant portions of BooleanItem were copied from the ftxui code
//base (files: src/ftxui/component/{menu,checkbox}.cpp)
Component BooleanItem(vector<bool>* states, string* label, int idx) {
class Impl : public ComponentBase {
   private:
      vector<bool>* States;
      string *Label;

      const int Idx;
   public:
      Impl(vector<bool>* states, string* label, int idx)
      :  States(states),
         Label(label),
         Idx(idx) {}
      Element Render() override {
         string retLabel = (Active() ? ">" : " ");
         retLabel += (States->at(Idx) ? ">" : " " ) + *Label;
         Element retElem = text(std::move(retLabel));
         if(Focused()) {
            retElem |= inverted;
         }
         if(Active()) {
            retElem |= bold;
         }
         const bool is_focused = Focused();
         const bool is_active = Active();
         auto focus_management = is_focused ? focus : is_active ? ftxui::select : nothing;
         return retElem | focus_management | reflect(box_);
      }
      bool OnEvent(Event event) override {
         if(!Focused() || !Active()) {
            return false;
         }
         if(event == Event::Character(' ') || event == Event::Return) {
            States->at(Idx) = !States->at(Idx);
            return true;
         }
         return false;
      }
      bool Focusable() const final { return true; };
      Box box_;
};
   return Make<Impl>(states, label, idx);
}

Component ListToggle(vector<string>* choices, vector<bool>* states, int max, bool initState) {
class Impl : public ComponentBase {
   private:
      vector<bool>*   States;
      vector<string>* Choices;
      bool InitState = false;
      int Max = 0;

      int Selected = 0;
      bool Show = false;
      string Label;
      Component List = Container::Vertical({}, &Selected);
      Component Head;

      void remakeList() {
         List->DetachAllChildren();
         States->clear();
         States->resize(Choices->size(), InitState);
         int i = 0;
         for(auto &choice : *Choices) {
            List->Add(BooleanItem(States, &choice, i++));
         }
         Label = "0/" + std::to_string(Max ? Max : Choices->size());
      }
   public:
      Impl(vector<string>* &choices, vector<bool>* &states, int max, bool initState)
       : Choices(choices),
         States(states),
         Max(max) {
         InitState = initState;
         Head = Checkbox(&Label, &Show, {.transform =
            [](const EntryState &s) {
               auto prefix = text(s.state ? "▼ " : "▶ ");
               auto label = text(s.label);
               if (s.active) {
                  label |= bold;
               }
               if (s.focused) {
                  label |= inverted;
               }
               return hbox({prefix, label});
               return text("");
         }});
         remakeList();
         Add(Container::Vertical({Head,List}));
      };
      Element Render() override {
         if(List->ChildCount() != Choices->size()) {
            Show = false;
            remakeList();
            ChildAt(0)->SetActiveChild(Head);
         }
         if(Show) {
            return vbox(Head->Render(), List->Render() | vscroll_indicator | frame);
         } else {
            return Head->Render();
         }
      }
      bool OnEvent(Event event) override {
         if(!Focused() || !ActiveChild()) {
            return false;
         }
         if(event == Event::Character(' ') || event == Event::Return) {
            if(!Show || Head->Focused()) {
               return Head->OnEvent(event);
            }
            List->OnEvent(event);
            
            int numSelected = 0;
            for(bool item : *States) {
               if(item != InitState) {
                  numSelected++;
               }
            }
            
            bool retBool = false;
            if(!Max || Max > Choices->size()) {
               retBool = true;
            } else if(numSelected > Max) {
               List->ActiveChild()->OnEvent(event); //(re)flip the bit
               numSelected -= 1;
               retBool = false;
            } else {
               retBool = true;
            }
            Label = std::to_string(numSelected) + "/" + std::to_string(Max ? Max : Choices->size());
            return retBool;

         } else if(!Show) {
            return false;
         }
         if(ActiveChild()->OnEvent(event)) { //standard handling
            return true;
         } else {
            return false;
         }
      }
      bool Focusable() const override {return true;}
};
   return Make<Impl>(choices, states, max, initState);
}

