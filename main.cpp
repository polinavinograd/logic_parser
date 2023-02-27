//////////////////////////////////////////////////////////////////////////////////////
// Лабораторная работа 1 по дисциплине ЛОИС
// Выполнена студентом группы 021702
// БГУИР Виноградовой П.С.
// Вариант 2 - Проверить, является ли строка формулой сокращенного языка логики
// высказываний 26.02.2023

#include "src/Formula.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>


int main() {
  
  while (true) {
    std::cout << "----------------------------\n";
    std::cout << "Choose the mode :" << std::endl
              << "1. Testing knowledge mode" << std::endl
              << "2. Checking statements mode" << std::endl
              << "exit\n";
    std::string choice;
    std::cin >> choice;
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
    if (choice == "exit") {
      return 0;

    } else if (choice == "1") {
      while (true) {
        std::cout << "-----------------------------------\n";
        std::cout << "1. Add formula to testing database\n2. Test\nback\n";
        std::string choice1;
        std::cin >> choice1;
        std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
        if (choice1 == "back") {
          break;
        } else if (choice1 == "1") {
          std::ofstream file_o("../../formulas.txt", std::ios::app);
          std::string addingFormula;
          std::cout << "-----------------------------------\n";
          std::cout << "Enter formula:\n";
          std::getline(std::cin, addingFormula);
          //std::cin >> addingFormula;
          if (file_o.is_open()) {
            file_o << addingFormula << std::endl;
          }
          file_o.close();
        } else if (choice1 == "2") {
          std::ifstream file("../../formulas.txt");
          std::cout << "----------------------------------------------------------------\n";
          std::cout << "Whether the string is a reduced propositional logic "
                       "formula?\nWrite the answer in the following format:\n"
                       "y - is a formula\nn - is not a formula\n";
          if (file.is_open()) {
            std::string line;
            std::vector<bool> answerVector;
            while (std::getline(file, line)) {
              std::cout << line << std::endl;
              std::string answer;
              std::cin >> answer;
              Parser parser;
              auto formula = parser.parseFormula(line);
              if ((formula && answer == "y") || (!formula && answer == "n")) {
                std::cout << "True\n";
                answerVector.push_back(1);
              } else {
                std::cout << "False\n";
                answerVector.push_back(0);
              }
            }
            int countResult = 0;
            for (int i = 0; i < answerVector.size(); i++) {
              if (answerVector[i] == 1) {
                countResult++;
              }
            }
            std::cout << "Result: " << countResult << "/" << answerVector.size()
                      << std::endl;
            file.close();
          }
        } else {
          std::cout << "Unexpected answer!\n";
        }
      }
    } else if (choice == "2") {
      while (true) {
        std::cout << "-----------------------------------\n";
        std::cout << "back\nEnter the statement: \n";
        std::string str;
        std::getline(std::cin, str);

        // std::cin >> str;
        if (str == "back") {
          break;
        } else {
          if (str != "")
          {
            Parser parser;
            auto formula = parser.parseFormula(str);
            if (formula) {
              std::cout << formula->toString() << " is a formula" << std::endl;
            } else {
              std::cout << "NO\n";
            }
          }
          else{
            std::cout << "NO\n";
          }
        }
      }
    } else {
      std::cout << "Unexpected answer!\n";
    }
  }
  return 0;
}