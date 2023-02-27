//////////////////////////////////////////////////////////////////////////////////////
// Лабораторная работа 1 по дисциплине ЛОИС
// Выполнена студентом группы 021702
// БГУИР Виноградовой П.С.
// Вариант 2 - Проверить, является ли строка формулой сокращенного языка логики высказываний
// 26.02.2023

#include <vector>
#include <string>

class Formula {
    public:
        virtual std::string toString() = 0;
};

class LogicConstant: public  Formula {
    public:
        LogicConstant(bool input) : constant(input){}
        std::string toString() {
            return constant ? "1" : "0";
        }
    private:
        bool constant;
};

class AtomicFormula: public Formula {
    public:
        AtomicFormula(char input) : letter(input){}
        std::string toString() override {
            return std::string(1, letter);
        }
    private:
        char letter;
};

class UnaryFormula: public Formula {
    public:
        UnaryFormula(Formula* input) : formula(input){}
        virtual std::string getSign() = 0;
        std::string toString() override { 
            return "(" + getSign() + formula->toString() + ")";
        }
    private:
        Formula *formula;
};

class BinaryFormula: public Formula {
    public:
        BinaryFormula(Formula* lFormula, Formula* rFormula) : leftFormula(lFormula), rightFormula(rFormula) {}
        virtual std::string getSign() = 0;
        std::string toString() override { 
            return "(" + leftFormula->toString() + getSign() + rightFormula->toString() + ")";
        }

    private:
        Formula *leftFormula;
        Formula *rightFormula;
};

class Conjunction : public BinaryFormula {
    public:
        Conjunction(Formula* leftFormula, Formula* rightFormula) : BinaryFormula(leftFormula, rightFormula) {}
        std::string getSign() override;
    private:
        std::string sign = "/\\";
};

class Disjunction : public BinaryFormula {
    public:
        Disjunction(Formula* leftFormula, Formula* rightFormula) : BinaryFormula(leftFormula, rightFormula) {}
        std::string getSign() override;
    private:
        std::string sign = "\\/";
};

class Implication : public BinaryFormula {
    public:
        Implication(Formula* leftFormula, Formula* rightFormula) : BinaryFormula(leftFormula, rightFormula) {}
        std::string getSign() override;
    private:
        std::string sign = "->";
};

class Equivalence : public BinaryFormula {
    public:
        Equivalence(Formula* leftFormula, Formula* rightFormula) : BinaryFormula(leftFormula, rightFormula) {}
        std::string getSign() override;
    private:
        std::string sign = "~";
};

class Negation : public UnaryFormula {
    public:
        Negation(Formula* input) : UnaryFormula(input) {}
        std::string getSign() override;
    private:
        std::string sign = "!";
};

class Parser {
    public:
        Formula *parseFormula(std::string inputString);
    private:
        BinaryFormula *parseBinaryFormula(std::string inputString);
        UnaryFormula *parseUnaryFormula(std::string inputString);
        AtomicFormula *parseAtomicFormula(std::string inputString);
        LogicConstant *parseLogicConstant(std::string inputString);
};