#include "InputContainer.hpp"
#include <iostream>
#include <functional>
using namespace ftxui;
using std::string;
using std::vector;

Component InputContainer(vector<string>* inVec, vector<string>* outVec) {
   class Impl : public ComponentBase {
      private:
         vector<string>* InVec;
         vector<string>* OutVec;
         Component child = Container::Vertical({}); //placeholder
         void configChild() {
            child->DetachAllChildren();
            OutVec->clear();
            OutVec->resize(InVec->size());   
            child = Container::Vertical({});
            if(InVec->empty()) {
               child->Add(Renderer([] { return text(""); })); //prevents 'Empty container' being displayed
               return;
            }
            //TODO: check to see if creating a Component Vector that is then 
            //added to `child` would be more efficient
            for(int i = 0; i < InVec->size() ; i++) {
               child->Add(Input(&OutVec->at(i), &InVec->at(i)));
            }
            child |= vscroll_indicator;
            child |= frame;
         }
      public:
         Impl(vector<string>* inVec, vector<string>* outVec) : InVec(inVec), 
                                                               OutVec(outVec) {
            configChild();
         }
         Element Render() override {
            return child->Render();
         }
         ///reconfigures child when OnEvent is manually called with 'Event::Custom'
         bool OnEvent(Event event) override {
            if(event == Event::Custom)
               configChild();
            return child->OnEvent(event);
         }
         bool Focusable () const override { return true; }
   };
   return Make<Impl>(inVec, outVec);
}
