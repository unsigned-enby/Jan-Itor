#include "Components.hpp"
using namespace ftxui;
using std::vector;
using std::string;

Component ListToggle(string label, vector<string>* myVec, bool** states) {
class Impl : public ComponentBase {
   private:
      bool** OutStates;
      vector<string>* Vec;
      
      bool* States;
      bool  Show = false;
      Component List = Container::Vertical({});

      void remakeList() {
         List->DetachAllChildren();
         delete [] States;
         States = new bool[Vec->size()];
         *OutStates = &States[0];
         for(int i=0; i < Vec->size(); i++) {
            States[i] = false;
            List->Add(Checkbox(Vec->at(i), &States[i]));
         }

      }
   public:
      Impl(string label, vector<string>* &myVec, bool** states) : Vec(myVec), OutStates(states){
         States = new bool[Vec->size()];
         for(int i=0; i < Vec->size() ; i++) {
            States[i] = false;
            List->Add(Checkbox(Vec->at(i), &(States)[i]));
         }
         *OutStates = &States[0];
         Add(Collapsible(label, List, &Show));
      };
      Element Render() override {
         if(List->ChildCount() != Vec->size()) {
            remakeList();
         }
         return ActiveChild()->Render() | vscroll_indicator | frame;
      }
      bool OnEvent(Event event) override {
         if(!Focused()) {
            return false;
         } else if(ActiveChild() && ActiveChild()->OnEvent(event)) {
            return true;
         } else {
            return false;
         }
      }
      bool Focusable() const override {return true;}
};

   return Make<Impl>(label, myVec, states);
}
