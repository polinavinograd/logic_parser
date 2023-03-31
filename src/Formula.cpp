//////////////////////////////////////////////////////////////////////////////////////
// Лабораторная работа 1 по дисциплине ЛОИС
// Выполнена студентом группы 021702
// БГУИР Виноградовой П.С.
// Вариант 9 - Построить СДНФ для заданной формулы сокращенного языка логики
// высказываний. 27.03.2023

#include "Formula.h"
#include <algorithm>
#include <math.h>
#include <memory>
#include <set>
#include <string>

std::vector<bool> decimalToBinary(int decimal, const std::set<char>& variables)
{
    std::vector<bool> binary_num;
    for (int i = 0; i < variables.size(); i++)
    {
        binary_num.insert(binary_num.begin(), decimal % 2);
        decimal /= 2;
    }
    return binary_num;
}

std::shared_ptr<Formula> Parser::parseFormula(std::string inputString)
{
    if (auto formula = parseAtomicFormula(inputString))
    {
        return formula;
    }
    if (auto formula = parseLogicConstant(inputString))
    {
        return formula;
    }
    if (auto formula = parseUnaryFormula(inputString))
    {
        return formula;
    }
    if (auto formula = parseBinaryFormula(inputString))
    {
        return formula;
    }
    return nullptr;
}

std::shared_ptr<BinaryFormula> Parser::parseBinaryFormula(
    std::string inputString)
{
    if (inputString.size() > 4 && inputString[0] == '(' &&
        inputString[inputString.size() - 1] == ')')
    {
        int position = 2;
        int brackets = 0;
        if (inputString[1] == '(')
        {
            brackets++;
        }
        while (position < inputString.size() - 1 && brackets != 0)
        {
            if (inputString[position] == '(')
            {
                brackets++;
            }
            if (inputString[position] == ')')
            {
                brackets--;
            }
            position++;
        }
        if (brackets != 0)
        {
            return nullptr;
        }
        std::shared_ptr<Formula> left  = nullptr;
        std::shared_ptr<Formula> right = nullptr;
        if (position > inputString.size() - 3)
        {
            return nullptr;
        }
        std::string twoCharSign = inputString.substr(position, 2);
        if (twoCharSign == "/\\" || twoCharSign == "\\/" || twoCharSign == "->")
        {
            left  = parseFormula(inputString.substr(1, position - 1));
            right = parseFormula(inputString.substr(
                position + 2, inputString.size() - position - 3));
        }
        if (inputString[position] == '~')
        {
            left  = parseFormula(inputString.substr(1, position - 1));
            right = parseFormula(inputString.substr(
                position + 1, inputString.size() - position - 2));
        }
        if (!left || !right)
        {
            return nullptr;
        }
        switch (inputString[position])
        {
            case '/':
                return std::make_shared<Conjunction>(left, right);
            case '\\':
                return std::make_shared<Disjunction>(left, right);
            case '-':
                return std::make_shared<Implication>(left, right);
            case '~':
                return std::make_shared<Equivalence>(left, right);
            default:
                return nullptr;
        }
    }
    return nullptr;
}

std::shared_ptr<UnaryFormula> Parser::parseUnaryFormula(std::string inputString)
{
    if (inputString.size() > 3 && inputString[0] == '(' &&
        inputString[inputString.size() - 1] == ')' && inputString[1] == '!')
    {
        int position = 2;
        int brackets = 0;
        if (inputString[0] == '(')
        {
            brackets++;
        }
        while (position < inputString.size() && brackets != 0)
        {
            if (inputString[position] == '(')
            {
                brackets++;
            }
            if (inputString[position] == ')')
            {
                brackets--;
            }
            position++;
        }
        if (brackets != 0)
        {
            return nullptr;
        }
        std::shared_ptr<Formula> subFormula =
            parseFormula(inputString.substr(2, inputString.size() - 3));
        if (subFormula)
        {
            return std::make_shared<Negation>(subFormula);
        }
    }
    return nullptr;
}

std::shared_ptr<AtomicFormula> Parser::parseAtomicFormula(
    std::string inputString)
{
    if (inputString.size() == 1 && inputString <= "Z" && inputString >= "A")
    {
        return std::make_shared<AtomicFormula>(inputString[0]);
    }
    return nullptr;
}

std::shared_ptr<LogicConstant> Parser::parseLogicConstant(
    std::string inputString)
{
    if (inputString.size() == 1 && inputString == "1" || inputString == "0")
    {
        return std::make_shared<LogicConstant>(inputString == "1");
    }
    return nullptr;
}

std::string Conjunction::getSign()
{
    return sign;
}

std::string Disjunction::getSign()
{
    return sign;
}

std::string Equivalence::getSign()
{
    return sign;
}

std::string Implication::getSign()
{
    return sign;
}

std::string Negation::getSign()
{
    return sign;
}

std::shared_ptr<Formula> UnaryFormula::getFormula()
{
    return formula;
}

std::shared_ptr<Formula> BinaryFormula::getLeft()
{
    return leftFormula;
}

std::shared_ptr<Formula> BinaryFormula::getRight()
{
    return rightFormula;
}

bool Negation::calculate(const std::map<char, bool>& correlation)
{
    return !(getFormula()->calculate(correlation));
}

bool Conjunction::calculate(const std::map<char, bool>& correlation)
{
    return (getLeft()->calculate(correlation) &&
            getRight()->calculate(correlation));
}

bool Disjunction::calculate(const std::map<char, bool>& correlation)
{
    return (getLeft()->calculate(correlation) ||
            getRight()->calculate(correlation));
}

bool Equivalence::calculate(const std::map<char, bool>& correlation)
{
    return (getLeft()->calculate(correlation) ==
            getRight()->calculate(correlation));
}

bool Implication::calculate(const std::map<char, bool>& correlation)
{
    return (!(getLeft()->calculate(correlation)) ||
            (getRight()->calculate(correlation)));
}

bool LogicConstant::calculate(const std::map<char, bool>& correlation)
{
    return constant;
};

bool AtomicFormula::calculate(const std::map<char, bool>& correlation)
{
    value = correlation.at(letter);
    return value;
}

// bool LogicConstant::getConstant() { return constant; }

std::vector<bool> TruthTable::calculate(std::shared_ptr<Formula> formula)
{
    std::vector<bool>    result;
    std::string          str = formula->toString();
    std::set<char>       variables;
    std::map<char, bool> correlation;
    for (int i = 0; i < str.size(); i++)
    {
        if (isalpha(str[i]))
        {
            variables.emplace(str[i]);
        }
    }
    for (int j = 0; j < pow(2, variables.size()); j++)
    {
        std::vector<bool>        binary_num = decimalToBinary(j, variables);
        std::set<char>::iterator it         = variables.begin();
        for (int i = 0; i < binary_num.size() && it != variables.end(); i++)
        {
            correlation[*it] = (binary_num[i]);
            it++;
        }
        result.emplace_back(formula->calculate(correlation));
    }

    return result;
}

std::string TruthTable::buildPDNF(const std::string& inputString)
{
    Parser                   parser;
    std::shared_ptr<Formula> resultPDNF = nullptr;
    std::set<char>           variables;

    if (auto formula = parser.parseFormula(inputString))
    {
        for (int i = 0; i < inputString.size(); i++)
        {
            if (isalpha(inputString[i]))
            {
                variables.emplace(inputString[i]);
            }
        }
        std::vector<bool> table = calculate(formula);
        if (variables.size() == 0)
        {
            table.emplace_back(table[0]);
            variables.insert('A');
        }
        for (int i = 0; i < table.size(); i++)
        {
            if (table[i])
            {
                std::vector<bool> binary_num = decimalToBinary(i, variables);
                std::shared_ptr<Formula> currentConjunction = nullptr;
                std::set<char>::iterator it                 = variables.begin();

                for (int j = 0; j < binary_num.size() && it != variables.end();
                     j++)
                {
                    std::shared_ptr<Formula> temp_formula =
                        std::make_shared<AtomicFormula>(*it);

                    if (!binary_num[j])
                    {
                        temp_formula = std::make_shared<Negation>(temp_formula);
                    }

                    if (currentConjunction)
                    {
                        currentConjunction = std::make_shared<Conjunction>(
                            temp_formula, currentConjunction);
                    }
                    else
                    {
                        currentConjunction = temp_formula;
                    }
                    it++;
                }
                if (resultPDNF)
                {
                    resultPDNF = std::make_shared<Disjunction>(
                        currentConjunction, resultPDNF);
                }
                else
                {
                    resultPDNF = currentConjunction;
                }
            }
        }
    }
    if (!resultPDNF)
        return "";
    return resultPDNF->toString();
}
