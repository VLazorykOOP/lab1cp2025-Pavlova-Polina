#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <windows.h>
#include <locale.h>

using namespace std;

// Змінна для відстеження алгоритму
int usedAlgorithm = 0;

// Базовий клас винятків
class Exception {
public:
    virtual void Message() const {
        cout << "Виникла помилка" << endl;
    }
};

// Винятки пов'язані з файлами
class FileException : public Exception {
protected:
    string filename;
public:
    FileException(const string& name) : filename(name) {}
    void Message() const override {
        cout << "Помилка при роботі з файлом: " << filename << endl;
    }
};

class FileNotFound : public FileException {
public:
    FileNotFound(const string& name) : FileException(name) {}
    void Message() const override {
        cout << "Файл не знайдено: " << filename << endl;
    }
};

// Винятки пов'язані з математичними обчисленнями
class MathException : public Exception {
protected:
    double value;
public:
    MathException(double val) : value(val) {}
    void Message() const override {
        cout << "Математична помилка при значенні: " << value << endl;
    }
    double getValue() const { return value; }
};

class DivisionByZero : public MathException {
public:
    DivisionByZero(double val) : MathException(val) {}
    void Message() const override {
        cout << "Ділення на нуль при значенні: " << value << endl;
    }
    // Наслідуємо метод getValue() від базового класу
};

class OutOfRangeValue : public MathException {
public:
    OutOfRangeValue(double val) : MathException(val) {}
    void Message() const override {
        cout << "Значення поза допустимим діапазоном: " << value << endl;
    }
};

// Функції для алгоритму 2
double Tbl2(double x) {
    if (x > 10 || x < -10) {
        throw OutOfRangeValue(x);
    }

    ifstream file("dat1.dat");
    if (!file) {
        throw FileNotFound("dat1.dat");
    }

    double xi, yi, xi1, yi1, y = 0;
    file >> xi1 >> yi1;

    if (xi1 == x) {
        y = yi1;
    }
    else {
        while (!file.eof()) {
            xi = xi1; yi = yi1;
            file >> xi1;
            if (file.eof()) break;
            file >> yi1;

            if (xi < x && x < xi1) {
                y = yi + (yi1 - yi) * (x - xi) / (xi1 - xi);
                break;
            }
            if (xi1 == x) {
                y = yi1;
                break;
            }
        }
    }

    file.close();
    return y;
}

double Krl2(double x, double y, double z) {
    try {
        if (x > 0 && y <= 1) {
            if (z == 0) {
                throw DivisionByZero(z);
            }
            return Tbl2(x) + Tbl2(y) / z;
        }
        else if (y > 1) {
            if (x == 0) {
                throw DivisionByZero(x);
            }
            return Tbl2(y) + Tbl2(z) / x;
        }
        else {
            if (y == 0) {
                throw DivisionByZero(y);
            }
            return Tbl2(z) + Tbl2(x) / y;
        }
    }
    catch (DivisionByZero& e) {
        // Обробка помилки ділення на нуль згідно умови завдання (Алгоритм 2, п.4)
        if (x <= 0) {
            cout << "Krl2: Повертаємо [y+z] при x <= 0" << endl;
            return floor(y + z);
        }
        else if (y <= 0 || z <= 0) {
            cout << "Krl2: Повертаємо x+1 при y <= 0 або z <= 0" << endl;
            return x + 1;
        }
        return 0; // Не повинно досягатися
    }
}

double Nrl2(double x, double y) {
    try {
        double temp = sqrt(x * x + y * y);
        if (temp == 0) {
            throw DivisionByZero(temp);
        }

        if (x > y) {
            return 0.42 * Krl2(x / temp, y / temp, x / temp);
        }
        else {
            return 0.57 * Krl2(y / temp, x / temp, y / temp);
        }
    }
    catch (DivisionByZero& e) {
        // Обробка виняткової ситуації ділення на нуль (Алгоритм 2, п.3.1)
        cout << "Nrl2: Помилка ділення на нуль - повертаємо 0.05" << endl;
        return 0.05;
    }
}

double Grl2(double x, double y, double z) {
    try {
        double temp = x + y;

        if (x + y >= z) {
            temp += 0.3 * Nrl2(x, z) + 0.7 * Nrl2(y, z);
        }
        else {
            temp += 1.3 * Nrl2(x, z) - 0.3 * Nrl2(y, z);
        }

        return temp;
    }
    catch (DivisionByZero& e) {
        // Передаємо виняток далі для обробки в Krl2
        throw;
    }
    catch (OutOfRangeValue& e) {
        // Вихід за межі допустимого діапазону
        cout << "Grl2: Значення поза допустимим діапазоном" << endl;
        return x + y; // Просте значення за замовчуванням
    }
    catch (...) {
        // Інші помилки
        throw;
    }
}

// Функція для отримання значення з таблиці (Алгоритм 1)
double Tbl(double x) {
    if (x > 10 || x < -10) {
        throw OutOfRangeValue(x);
    }

    ifstream file("dat1.dat");
    if (!file) {
        throw FileNotFound("dat1.dat");
    }

    double xi, yi, xi1, yi1, y = 0;
    file >> xi1 >> yi1;

    if (xi1 == x) {
        y = yi1;
    }
    else {
        while (!file.eof()) {
            xi = xi1; yi = yi1;
            file >> xi1;
            if (file.eof()) break;
            file >> yi1;

            if (xi < x && x < xi1) {
                y = yi + (yi1 - yi) * (x - xi) / (xi1 - xi);
                break;
            }
            if (xi1 == x) {
                y = yi1;
                break;
            }
        }
    }

    file.close();
    return y;
}

// Функції для алгоритму 1
double Krl(double x, double y, double z) {
    try {
        if (x > 0 && y <= 1) {
            if (z == 0) {
                throw DivisionByZero(z);
            }
            return Tbl(x) + Tbl(y) / z;
        }
        else if (y > 1) {
            if (x == 0) {
                throw DivisionByZero(x);
            }
            return Tbl(y) + Tbl(z) / x;
        }
        else {
            if (y == 0) {
                throw DivisionByZero(y);
            }
            return Tbl(z) + Tbl(x) / y;
        }
    }
    catch (DivisionByZero& e) {
        // Обробка помилки ділення на нуль згідно умови завдання (Алгоритм 1, п.4)
        if (x <= 0) {
            cout << "Krl: Повертаємо [y+z] при x <= 0" << endl;
            return floor(y + z);
        }
        else if (y <= 0 || z <= 0) {
            cout << "Krl: Повертаємо x+1 при y <= 0 або z <= 0" << endl;
            return x + 1;
        }
        return 0; // Не повинно досягатися
    }
}

double Nrl(double x, double y) {
    if (x > y) {
        return 0.42 * Krl(x, y, x);
    }
    else {
        return 0.57 * Krl(y, x, y) - 0.42 * Krl(y, y, y);
    }
}

double Grl(double x, double y, double z) {
    double temp = floor(x + y);

    // Перевірка умови переходу до алгоритму 2
    if (floor(x + y) == floor(z)) {
        throw z / 2.0;
    }

    if (x + y >= z) {
        temp += 0.4 * Nrl(y, x) + 0.6 * Nrl(z, y);
    }
    else {
        temp += 1.4 * Nrl(x, y) - 0.4 * Nrl(z, y * Nrl(y, 1));
    }

    return temp;
}

// Функція обчислення за алгоритмом 2
double algorithm2(double x, double y, double z) {
    usedAlgorithm = 2;
    return x * Grl2(x, y, z) + y * Grl2(y, x, z) + z * Grl2(z, x, y) - x * y * z * Grl2(y, x, z);
}

// Функція обчислення за алгоритмом 3
double algorithm3(double x, double y, double z) {
    usedAlgorithm = 3;
    return 1.3498 * x + 2.2362 * y - 2.348 * x * y * z;
}

// Основна функція обчислення за алгоритмом 1
double fun(double x, double y, double z) {
    try {
        usedAlgorithm = 1; // Використовується алгоритм 1
        return x * Grl(x, y, z) + y * Grl(y, z, x) + z * Grl(z, x, y);
    }
    catch (double d) {
        // Якщо з Grl було викинуто число (z/2), переходимо до алгоритму 2
        z = d;
        return algorithm2(x, y, z);
    }
}

// Функція для встановлення української мови в консолі
void SetUkrainianConsole() {
    // Метод 1: Встановлення кодової сторінки Windows 1251
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Метод 2: Встановлення локалі
    setlocale(LC_ALL, "Ukrainian_Ukraine.1251");
}

int main() {
    // Встановлення підтримки української мови
    SetUkrainianConsole();

    // Створення тестового файлу dat1.dat
    ofstream outdat("dat1.dat");
    if (outdat.is_open()) {
        outdat << -10 << "\t" << 23.5 << endl;
        outdat << -5 << "\t" << 12.4 << endl;
        outdat << 0 << "\t" << 10.1 << endl;
        outdat << 5 << "\t" << 6.87 << endl;
        outdat << 10 << "\t" << 1.21 << endl;
        outdat.close();
    }

    double x, y, z, f;

    cout << "Введіть значення x y z: ";
    cin >> x >> y >> z;

    try {
        // Спроба обчислити за алгоритмом 1
        f = fun(x, y, z);
        cout << "Обчислення успішно виконано за алгоритмом " << usedAlgorithm << endl;
    }
    catch (OutOfRangeValue& e) {
        e.Message();
        // Перехід до алгоритму 2 з z = x/2 (згідно п.5.2 Алгоритму 1)
        z = e.getValue() / 2.0;
        f = algorithm2(x, y, z);
        cout << "Перехід до алгоритму " << usedAlgorithm << " через вихід за межі діапазону" << endl;
    }
    catch (FileNotFound& e) {
        e.Message();
        // Перехід до алгоритму 3 (згідно п.5.1 Алгоритму 1)
        f = algorithm3(x, y, z);
        cout << "Перехід до алгоритму " << usedAlgorithm << " через проблему з файлом" << endl;
    }
    catch (DivisionByZero& e) {
        e.Message();
        // Дивимося, яке значення викликало помилку
        double errorValue = e.getValue();
        if (errorValue == 0) {
            cout << "Значення, що викликало помилку: 0" << endl;
            f = x + 1; // Згідно з умовою завдання
        }
        else {
            f = floor(y + z); // Згідно з умовою завдання
        }
        cout << "Обробка ділення на нуль, результат: " << f << endl;
    }
    catch (...) {
        cout << "Невідома помилка" << endl;
        // Переходимо до алгоритму 3
        f = algorithm3(x, y, z);
        cout << "Перехід до алгоритму " << usedAlgorithm << " через невідому помилку" << endl;
    }

    cout << "Результат: " << f << endl;

    return 0;
}