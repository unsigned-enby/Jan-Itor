#include "Components.hpp"
using namespace ftxui;
using std::string;
using std::vector;

Component InputContainer(vector<string>* inVec, vector<string>* outVec) { 
class Impl : public ComponentBase {
   private:
      vector<string>* InVec;
      vector<string>* OutVec;
      Element savedElement;
      //Component child = Container::Vertical({}); //placeholder
      void configChild() {
         DetachAllChildren();
         OutVec->clear();
         if(InVec->empty()) {
            savedElement = text("");
            return;
         }
         OutVec->resize(InVec->size());   
         //TODO: test to see if this is more or less effiecient that having a 'child' member object
         Components inputs;
         for(int i = 0; i < InVec->size() ; i++) {
            inputs.emplace_back(Input(&OutVec->at(i), &InVec->at(i)));
         }
         Add(Container::Vertical(std::move(inputs)));
         savedElement = ActiveChild()->Render();
      }
   public:
      Impl(vector<string>* inVec, vector<string>* outVec) : InVec(inVec), 
                                                            OutVec(outVec) {
         configChild();
      }
      Element Render() override {
         if(ActiveChild() && ActiveChild()->Focused()) {
            savedElement = ActiveChild()->Render();
         }
         return savedElement | vscroll_indicator | frame;
      }
      ///reconfigures child when OnEvent is manually called with 'Event::Custom'
      bool OnEvent(Event event) override {
         if(event == Event::Character("REDRAW")) {
            configChild();
            return true;
         }
         if (!Focused()) {
            return false;
         }
         if (ActiveChild() && ActiveChild()->OnEvent(event)) {
            return true;
         }
         return false;
      }
      bool Focusable () const override { return true; }
};

   return Make<Impl>(std::move(inVec), std::move(outVec));
}
