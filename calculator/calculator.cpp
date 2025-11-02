#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class MySimpleVector {
private:
    double data[4];    
    size_t currentSize; 

public:

    MySimpleVector() : currentSize(0) {}

    void push_back(double value) {
        if (currentSize >= 4) {
            throw out_of_range("Вектор переполнен");
        }
        data[currentSize] = value;
        currentSize++;
    }

    double get(size_t index) const {
        if (index >= currentSize) {
            throw out_of_range("Индекс за пределами вектора");
        }
        return data[index];
    }

    size_t size() const {
        return currentSize;
    }
};

double stringToDouble(const string& str) {
    double result = 0.0;
    double sign = 1.0;
    size_t i = 0;

    // Обработка знака (если есть)
    if (str[0] == '-') {
        sign = -1.0;
        i++;
    } else if (str[0] == '+') {
        i++;
    }

    // Обработка целой части
    while (i < str.length() && isdigit(str[i])) {
        result = result * 10.0 + (str[i] - '0');
        i++;
    }

    // Обработка дробной части (если есть)
    if (i < str.length() && str[i] == '.') {
        i++;
        double fraction = 0.0;
        double power = 0.1;
        while (i < str.length() && isdigit(str[i])) {
            fraction += (str[i] - '0') * power;
            power /= 10.0;
            i++;
        }
        result += fraction;
    }

    // Проверка на остаток строки
    if (i < str.length()) {
        throw invalid_argument("Неверный числовой формат: " + str);
    }

    return sign * result;
}

// Функция для выполнения операции
double calculate(const string& operation, MySimpleVector& operands) {
    if (operation == "summa") {
        double result = 0;
        for (size_t i = 0; i < operands.size(); ++i) {
            result += operands.get(i);
        }
        return result;
    } else if (operation == "difference") {
        if (operands.size() < 2) {
            throw invalid_argument("Для вычитания необходимо минимум 2 операнда");
        }
        double result = operands.get(0);
        for (size_t i = 1; i < operands.size(); ++i) {
            result -= operands.get(i);
        }
        return result;
    } else {
        throw invalid_argument("Неподдерживаемая операция: " + operation);
    }
}

int main(int argc, char* argv[]) {
    // Проверка наличия аргументов
    if (argc < 4) {
        cerr << "Использование: calculator -o <operation> <operand1> <operand2> ... <operandN>" << endl;
        cerr << "  operation: summa (суммирование) или difference (вычитание)" << endl;
        cerr << "  operands: от 2 до 4 числовых операндов" << endl;
        return 1;
    }

    // Получение операции
    string operation;
    MySimpleVector operands; 

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 < argc) {
                operation = argv[i + 1];
                i++; 
            } else {
                cerr << "Ошибка: необходимо указать операцию после -o" << endl;
                return 1;
            }
        } else {
            // Преобразование аргумента в число (double)
            try {
                double operand = stringToDouble(arg);
                operands.push_back(operand); 
            } catch (const invalid_argument& e) {
                cerr << "Ошибка: " << e.what() << endl;
                return 1;
            }
        }
    }

    // Проверка количества операндов
    if (operands.size() < 2 || operands.size() > 4) {
        cerr << "Ошибка: необходимо указать от 2 до 4 операндов" << endl;
        return 1;
    }

    try {
        double result = calculate(operation, operands);
        cout << "Результат: " << result << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
