//////////////////////////////////////////////////////////////////////////////////////
// Лабораторная работа 2 по дисциплине ЛОИС
// Выполнена студентом группы 021702
// БГУИР Виноградовой П.С.
// Вариант 9 - Построить СДНФ для заданной формулы сокращенного языка логики
// высказываний. 27.03.2023

#include "src/Formula.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <ostream>
#include <set>
#include <string>

void testing_mode()
{
    while (true)
    {
        std::cout << "-----------------------------------\n";
        std::cout << "1. Add formula to testing database\n2. Test\n3. back\n";
        std::string choice1;
        std::cin >> choice1;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice1 == "3")
        {
            break;
        }
        else if (choice1 == "1")
        {
            std::ofstream file_o("../../formulas.txt", std::ios::app);
            std::string   addingFormula;
            std::cout << "-----------------------------------\n";
            std::cout << "Enter formula:\n";
            std::getline(std::cin, addingFormula);
            if (file_o.is_open())
            {
                file_o << addingFormula << std::endl;
            }
            file_o.close();
        }
        else if (choice1 == "2")
        {
            std::ifstream file("../../formulas.txt");
            std::cout
                << "-----------------------------------------------------------"
                   "-----\n";
            std::cout << "Whether the string is a reduced propositional logic "
                         "formula?\nWrite the answer in the following format:\n"
                         "y - is a formula\nn - is not a formula\n";
            if (file.is_open())
            {
                std::string       line;
                std::vector<bool> answerVector;
                while (std::getline(file, line))
                {
                    std::cout << line << std::endl;
                    std::string answer;
                    std::cin >> answer;
                    Parser parser;
                    auto   formula = parser.parseFormula(line);
                    if ((formula && answer == "y") ||
                        (!formula && answer == "n"))
                    {
                        std::cout << "True\n";
                        answerVector.push_back(1);
                    }
                    else
                    {
                        std::cout << "False\n";
                        answerVector.push_back(0);
                    }
                }
                int countResult = 0;
                for (int i = 0; i < answerVector.size(); i++)
                {
                    if (answerVector[i] == 1)
                    {
                        countResult++;
                    }
                }
                std::cout << "Result: " << countResult << "/"
                          << answerVector.size() << std::endl;
                file.close();
            }
        }
        else
        {
            std::cout << "Unexpected answer!\n";
        }
    }
}

void checking_statements_mode()
{
    while (true)
    {
        std::cout << "-----------------------------------\n";
        std::cout << "2. back\nEnter the statement: \n";
        std::string str;
        std::getline(std::cin, str);
        if (str == "2")
        {
            break;
        }
        else
        {
            Parser parser;
            auto   formula = parser.parseFormula(str);
            if (formula)
            {
                std::cout << formula->toString() << " is a formula"
                          << std::endl;
            }
            else
            {
                std::cout << "Not a formula\n";
            }
        }
    }
}

void calculating_mode()
{
    while (true)
    {
        std::cout << "-----------------------------------\n";
        std::cout << "3. back\nEnter the formula: \n";
        std::string str;
        std::getline(std::cin, str);
        if (str == "3")
        {
            break;
        }
        else
        {
            TruthTable  table;
            Parser      parser;
            std::string resultPDNF;
            if (auto formula = parser.parseFormula(str))
            {
                std::vector<bool> res = table.calculate(formula);
                if (res.size() == 0)
                {
                    std::cout << "Not a formula\n";
                }
                else
                {
                    for (int i = 0; i < res.size(); i++)
                    {
                        std::cout << res[i] << " ";
                    }
                    std::cout << std::endl;
                }
            }
            else
            {
                std::cout << "Not a formula\n";
            }
        }
    }
}

void building_mode()
{
    while (true)
    {

        std::cout << "-----------------------------------\n";
        std::cout << "After 17 variables it takes more than 5 minutes to build "
                     "PDNF\n";
        std::cout << "3. back\nEnter the formula: \n";
        std::string str;
        std::getline(std::cin, str);
        if (str == "3")
        {
            break;
        }
        else
        {
            TruthTable  table;
            std::string result = table.buildPDNF(str);
            if (result == "")
            {
                std::cout << "Can't build the PDNF" << std::endl;
            }
            else
            {
                std::ofstream file_o("../pdnf_1.txt", std::ios::out);
                if (file_o.is_open())
                {
                    file_o << result;
                }
                file_o.close();
                std::set<char> variables;
                for (int i = 0; i < str.size(); i++)
                {
                    if (isalpha(str[i]))
                    {
                        variables.emplace(str[i]);
                    }
                }
                std::cout << result << std::endl;
                int letters = (pow(2, variables.size()) - 1) * variables.size();
                float ratio = float(result.size() - letters) / float(letters);
                std::cout << ratio << std::endl;
            }
        }
    }
}

int main()
{

    while (true)
    {
        std::cout << "----------------------------\n";
        std::cout << "Choose the mode :" << std::endl
                  << "1. Build PDNF" << std::endl
                  << "2. Calculate formula" << std::endl
                  << "3. exit\n";
        std::string choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == "3")
        {
            return 0;
        }
        else if (choice == "1")
        {
            building_mode();
        }
        else if (choice == "2")
        {
            calculating_mode();
        }
        else
        {
            std::cout << "Unexpected answer!\n";
        }
    }
    return 0;
}