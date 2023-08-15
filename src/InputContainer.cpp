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
         //SavedStart is a unique identifier, nothing more
         string* SavedStart;
         Component child = Container::Vertical({}); //placeholder
         void configChild() {
            SavedStart = InVec->data();
            OutVec->clear();
            OutVec->resize(InVec->size());   
            child->DetachAllChildren();
            child = Container::Vertical({});
            if(InVec->front().empty()) {
               child->Add(Renderer([] { return text(""); }));
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
            if(InVec->size() == 0) {
               InVec->emplace_back();
            }
            configChild();
         }
         Element Render() override {
            if(SavedStart != InVec->data()) { 
               configChild();
            }
            return child->Render();
         }
         bool OnEvent(Event event) override {
            return child->OnEvent(event);
         }
         bool Focusable () const override { return true; }
   };
   return Make<Impl>(inVec, outVec);
}
