#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "MyCSV.hpp"

ftxui::Component DelimDropDown(char* delim, std::string title); 

ftxui::Component ListToggle(std::vector<std::string>* choices,
                            std::vector<bool>*        states,
                            int  max = 0, //no/infinite max
                            bool initState = false);

ftxui::Component InputContainer(std::vector<std::string>* inVec, 
                                std::vector<std::string>* outVec);

ftxui::Component TablePreview(MyCSV* table);
#endif
