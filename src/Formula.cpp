//////////////////////////////////////////////////////////////////////////////////////
// Лабораторная работа 1 по дисциплине ЛОИС
// Выполнена студентом группы 021702
// БГУИР Виноградовой П.С.
// Вариант 2 - Проверить, является ли строка формулой сокращенного языка логики высказываний
// 26.02.2023

#include "Formula.h"
#include <string>

Formula *Parser::parseFormula(std::string inputString) {
    if (auto *formula = parseAtomicFormula(inputString)) {
        return formula;
    }
    if (auto *formula = parseLogicConstant(inputString)) {
        return formula;
    }
    if (auto *formula = parseUnaryFormula(inputString)) {
        return formula;
    }
    if (auto *formula = parseBinaryFormula(inputString)) {
        return formula;
    }
    return nullptr;
}

BinaryFormula *Parser::parseBinaryFormula(std::string inputString) {
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
        Formula *left = nullptr;
        Formula *right = nullptr;
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
                return new Conjunction(left, right);
            case '\\':
                return new Disjunction(left, right);
            case '-':
                return new Implication(left, right);
            case '~':
                return new Equivalence(left, right);
            default:
                return nullptr;       
        }
    }
    return nullptr;
}

UnaryFormula *Parser::parseUnaryFormula(std::string inputString) {
    if (inputString.size() > 3 && inputString[0] == '(' && inputString[inputString.size() - 1] == ')' && inputString [1] == '!') {
        Formula* subFormula = parseFormula(inputString.substr(2, inputString.size() - 3));
        return new Negation(subFormula); 
    }
    return nullptr;
}

AtomicFormula *Parser::parseAtomicFormula(std::string inputString) {
    if (inputString <= "Z" && inputString >= "A") {
        return new AtomicFormula(inputString[0]);
    }  
    return nullptr;
}

LogicConstant *Parser::parseLogicConstant(std::string inputString) {
    if (inputString == "1" || inputString == "0") {
        return new LogicConstant(inputString == "1");
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