#ifndef INPUT_MENU_H
#define INPUT_MENU_H
#include <vector>
#include <string>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "IpForm.hpp"
   ftxui::Component InputContainer(std::vector<std::string>* inVec,
                                   std::vector<std::string>* outVec);
#endif
