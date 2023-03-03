//////////////////////////////////////////////////////////////////////////////////////
// Лабораторная работа 1 по дисциплине ЛОИС
// Выполнена студентом группы 021702
// БГУИР Виноградовой П.С.
// Вариант 2 - Проверить, является ли строка формулой сокращенного языка логики высказываний
// 26.02.2023

#include "Formula.h"
#include <memory>
#include <string>

std::shared_ptr<Formula> Parser::parseFormula(std::string inputString) {
    if (auto formula = parseAtomicFormula(inputString)) {
        return formula;
    }
    if (auto formula = parseLogicConstant(inputString)) {
        return formula;
    }
    if (auto formula = parseUnaryFormula(inputString)) {
        return formula;
    }
    if (auto formula = parseBinaryFormula(inputString)) {
        return formula;
    }
    return nullptr;
}

std::shared_ptr<BinaryFormula> Parser::parseBinaryFormula(std::string inputString) {
    if (inputString.size() > 4 && inputString[0] == '(' && inputString[inputString.size() - 1] == ')' ) {
        int position = 2;
        int brackets = 0;
        if (inputString[1] == '(') {
            brackets++;
        }
        while (position < inputString.size() - 1 && brackets != 0) {
            if (inputString[position] == '(') {
                brackets++;
            }
            if (inputString[position] == ')') {
                brackets--;
            }
            position++;
        }
        if (brackets != 0) {
            return nullptr;
        }
        std::shared_ptr<Formula> left = nullptr;
        std::shared_ptr<Formula> right = nullptr;
        if(position > inputString.size() - 3) {
            return nullptr;
        }
        std::string twoCharSign = inputString.substr(position, 2);
        if (twoCharSign == "/\\" || twoCharSign == "\\/" || twoCharSign == "->") {
            left = parseFormula(inputString.substr(1, position -1));
            right = parseFormula(inputString.substr(position + 2, inputString.size() - position - 3));
        }
        if (inputString[position] == '~') {
            left = parseFormula(inputString.substr(1, position -1));
             right = parseFormula(inputString.substr(position + 1, inputString.size() - position - 2));
        }
        if (!left || !right) {
            return nullptr;
        }
        switch (inputString[position]) {
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

std::shared_ptr<UnaryFormula> Parser::parseUnaryFormula(std::string inputString) {
    if (inputString.size() > 3 && inputString[0] == '(' && inputString[inputString.size() - 1] == ')' && inputString [1] == '!') {
        int position = 2;
        int brackets = 0;
        if (inputString[0] == '(') {
            brackets++;
        }
        while (position < inputString.size()  && brackets != 0) {
            if (inputString[position] == '(') {
                brackets++;
            }
            if (inputString[position] == ')') {
                brackets--;
            }
            position++;
        }
        if (brackets != 0) {
            return nullptr;
        }
        std::shared_ptr<Formula> subFormula = parseFormula(inputString.substr(2, inputString.size() - 3));
        if (subFormula) {
            return std::make_shared<Negation>(subFormula); 
        }
    }
    return nullptr;
}

std::shared_ptr<AtomicFormula> Parser::parseAtomicFormula(std::string inputString) {
    if (inputString.size() == 1 && inputString <= "Z" && inputString >= "A") {
        return std::make_shared<AtomicFormula>(inputString[0]);
    }  
    return nullptr;
}

std::shared_ptr<LogicConstant> Parser::parseLogicConstant(std::string inputString) {
    if (inputString.size() == 1 && inputString == "1" || inputString == "0") {
        return std::make_shared<LogicConstant>(inputString == "1");
    }
    return nullptr;
}

std::string Conjunction::getSign() {
    return sign;
}

std::string Disjunction::getSign() {
    return sign;
}

std::string Equivalence::getSign() {
    return sign;
}

std::string Implication::getSign() {
    return sign;
}

std::string Negation::getSign() {
    return sign;
}