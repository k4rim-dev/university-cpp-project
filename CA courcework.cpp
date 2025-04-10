#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <locale>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

bool isAlphaString(const string& str) {
    for (char c : str) {
        if (!isalpha(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool isUpper(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'А' && c <= 'Я');
}

bool isLower(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'а' && c <= 'я');
}

void getAlphaString(string& variable) {
    while (true) {
        cin >> variable;

        if (isAlphaString(variable)) {
            if (variable.length() > 20) {
                cout << "Слишком длинный ввод. Попробуйте снова: ";
            }
            else {
                if (isUpper(variable[0]) && all_of(variable.begin() + 1, variable.end(), [](char c) { return isLower(c); })) {
                    break;
                }
                else {
                    cout << "Ошибка: слово должно начинаться с заглавной буквы, а остальные буквы должны быть строчными. Попробуйте снова: ";
                }
            }
        }
        else {
            cout << "Ошибка: поле должно содержать только буквы. Попробуйте снова: ";
        }
    }
}

bool isValidLicense(const string& license) {
    vector<string> validLicenses = { "A", "A1", "B", "B1", "C", "C1", "D", "D1", "BE", "CE", "DE", "C1E", "D1E", "M", "Tm", "Tb" };
    for (const string& valid : validLicenses) {
        if (license == valid) {
            return true;
        }
    }
    return false;
}

template<typename T>
void getInput(T& variable) {
    while (!(cin >> variable) || variable > 1000000 || variable < 0) {
        cout << "Некорректный ввод. Попробуйте снова: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// виртуальный класс Human
class Human {
public:
    virtual ~Human() {}

    virtual string getName() const { return ""; }
    virtual string getSurname() const { return ""; }
    virtual string getPatronymic() const { return ""; }
    virtual double getRate() const { return 0.0; }
    virtual string getPosition() const { return ""; }

    virtual void setName(const string&) {}
    virtual void setSurname(const string&) {}
    virtual void setPatronymic(const string&) {}
    virtual void setRate(double) {}

    virtual void displayInfo() const = 0;
};

// Класс "Электрик"
class Electrician : public Human {
private:
    string name, surname, patronymic;
    double rate;
    string equipment;

public:
    Electrician(string n, string s, string p, double r, const string& eq)
        : name(n), surname(s), patronymic(p), rate(r), equipment(eq) {}

    string getName() const override { return name; }
    string getSurname() const override { return surname; }
    string getPatronymic() const override { return patronymic; }
    double getRate() const override { return rate; }
    string getPosition() const override { return "Электрик"; }

    void setName(const string& n) override { name = n; }
    void setSurname(const string& s) override { surname = s; }
    void setPatronymic(const string& p) override { patronymic = p; }
    void setRate(double r) override {
        if (r < 0.1 || r > 1) {
            throw runtime_error("Ставка должна быть в диапазоне от 0.1 до 1.");
        }
        rate = r;
    }

    void setEquipment(const string& eq) {
        equipment = eq;
    }

    string getEquipment() const {
        return equipment;
    }

    bool fix_wiring() const {
        int randomOutcome = rand() % 4 + 1;
        switch (randomOutcome) {
        case 1:
            cout << "Электрик не обнаружил неисправности.\n";
            break;
        case 2:
            cout << "Электрик проверил проводку - устранил неисправности.\n";
            break;
        case 3:
            cout << "Электрик сейчас не в состоянии выполнять свою работу.\n";
            break;
        case 4:
            cout << "Электрика убило током...\n";
            return true;
        case 5:
            int item = rand() % 3;
            switch (item) {
            case 0: cout << "Электрик обнаружил короткое замыкание.\n"; break;
            case 1: cout << "Электрик обнаружил возгарание проводки.\n"; break;
            case 2: cout << "Электрик случайно обесточил весь город.\n"; break;
            }
            break;
        }
        return false;
    }

    void displayInfo() const override {
        cout << "Электрик: " << name << " " << surname << " " << patronymic << "; Ставка: " << rate
            << "; Оборудование: " << equipment << "\n";
    }

    double calculateSalary(double baseSalary) const {
        return rate * baseSalary;
    }
};

// Класс "Водитель"
class Driver : public Human {
private:
    string name, surname, patronymic;
    double rate;
    vector<string> licenses;
    vector<string> vehicles;

public:
    Driver(string n, string s, string p, double r)
        : name(n), surname(s), patronymic(p), rate(r) {}

    string getName() const override { return name; }
    string getSurname() const override { return surname; }
    string getPatronymic() const override { return patronymic; }
    double getRate() const override { return rate; }
    string getPosition() const override { return "Водитель"; }

    void setName(const string& n) override { name = n; }
    void setSurname(const string& s) override { surname = s; }
    void setPatronymic(const string& p) override { patronymic = p; }
    void setRate(double r) override {
        if (r < 0.1 || r > 1) {
            throw runtime_error("Ставка должна быть в диапазоне от 0.1 до 1.");
        }
        rate = r;
    }

    void addLicense(const string& license) {
        if (find(licenses.begin(), licenses.end(), license) != licenses.end()) {
            throw runtime_error("Эта категория прав уже добавлена.");
        }
        if (!isValidLicense(license)) {
            throw runtime_error("Недействительная категория прав.");
        }
        licenses.push_back(license);
    }

    void removeLicense(int index) {
        if (index >= 0 && index < licenses.size()) {
            licenses.erase(licenses.begin() + index);
        }
    }

    void addVehicle(const string& vehicle) {
        if (find(vehicles.begin(), vehicles.end(), vehicle) != vehicles.end()) {
            throw runtime_error("Это транспортное средство уже добавлено.");
        }
        if (!isAlphaString(vehicle)) {
            throw runtime_error("Наименование транспортного средства должно содержать только буквы.");
        }
        vehicles.push_back(vehicle);
    }

    void removeVehicle(int index) {
        if (index >= 0 && index < vehicles.size()) {
            vehicles.erase(vehicles.begin() + index);
        }
    }

    vector<string> getLicenses() const { return licenses; }

    vector<string> getVehicles() const { return vehicles; }

    void fixVehicle() {
        if (vehicles.empty()) {
            cout << "У водителя еще нет транспортного средства.\n";
            return;
        }
        int outcome = rand() % 3;
        switch (outcome) {
        case 0: cout << "Машину быстро починили.\n"; break;
        case 1: cout << "Серьезная несиправность. Машина не подлежит починке.\n"; break;
        case 2: cout << "Машина в порядке.\n"; break;
        }
    }

    void displayInfo() const override {
        cout << "Водитель: " << name << " " << surname << " " << patronymic << "; Ставка: " << rate << "\n";
        cout << "Категории прав: ";
        for (const auto& license : licenses) {
            cout << license << " ";
        }
        cout << "\nТранспортные средства: ";
        for (const auto& vehicle : vehicles) {
            cout << vehicle << " ";
        }
        cout << "\n";
    }

    double calculateSalary(double baseSalary) const {
        return rate * baseSalary;
    }
};

// Класс "Бухгалтер"
class Accountant : public Human {
private:
    string name, surname, patronymic;
    double rate;
    map<string, double> baseSalaries; // базовые зарплаты для разных должностей

public:
    Accountant(string n, string s, string p, double r)
        : name(n), surname(s), patronymic(p), rate(r) {
        baseSalaries["Электрик"] = 67000;
        baseSalaries["Водитель"] = 71000;
        baseSalaries["Секретарь"] = 87000;
        baseSalaries["Бухгалтер"] = 93000;
        baseSalaries["Директор"] = 115000;
    }

    string getName() const override { return name; }
    string getSurname() const override { return surname; }
    string getPatronymic() const override { return patronymic; }
    double getRate() const { return rate; }
    string getPosition() const { return "Бухгалтер"; }

    void setName(const string& n) override { name = n; }
    void setSurname(const string& s) override { surname = s; }
    void setPatronymic(const string& p) override { patronymic = p; }
    void setRate(double r) override {
        if (r < 0.1 || r > 1) {
            throw runtime_error("Ставка должна быть в диапазоне от 0.1 до 1.");
        }
        rate = r;
    }

    void setBaseSalary(const string& position, double salary) {
        if (salary < 17000) {
            throw runtime_error("Зарплата должна быть выше прожиточного минимума.");
        }
        baseSalaries[position] = salary;
    }

    double getBaseSalary(const string& position) const {
        if (baseSalaries.count(position)) {
            return baseSalaries.at(position);
        }
        return 0.0;
    }

    void displayInfo() const override {
        cout << "Бухгалтер: " << name << " " << surname << " " << patronymic << "; Ставка: " << rate << "\n";
    }

    void displayBaseSalaries() const {
        cout << "Оклад для каждой должности:\n";
        for (const auto& baseSalary : baseSalaries) {
            cout << baseSalary.first << ": " << baseSalary.second << "\n";
        }
    }

    void changeBaseSalary() {
        cout << "Выберите должность для изменения базовой зарплаты:\n";
        cout << "1. Электрик\n";
        cout << "2. Водитель\n";
        cout << "3. Секретарь\n";
        cout << "4. Бухгалтер\n";
        cout << "5. Директор\n";

        int choice;
        getInput(choice);

        string position;
        switch (choice) {
        case 1: position = "Электрик"; break;
        case 2: position = "Водитель"; break;
        case 3: position = "Секретарь"; break;
        case 4: position = "Бухгалтер"; break;
        case 5: position = "Директор"; break;
        default:
            cout << "Некорректный выбор.\n";
            return;
        }

        double newSalary;
        cout << "Введите новую базовую зарплату для должности " << position << ": ";
        getInput(newSalary);

        try {
            setBaseSalary(position, newSalary);
            cout << "Базовая зарплата для должности " << position << " успешно изменена.\n";
        }
        catch (const runtime_error& e) {
            cout << e.what() << endl;
        }
    }

    double calculateSalary(double baseSalary) const {
        return rate * baseSalary;
    }
};

// Класс "Секретарь"
class Secretary : public Human {
private:
    string name, surname, patronymic;
    double rate;

public:
    Secretary(string n, string s, string p, double r)
        : name(n), surname(s), patronymic(p), rate(r) {}

    string getName() const override { return name; }
    string getSurname() const override { return surname; }
    string getPatronymic() const override { return patronymic; }
    double getRate() const override { return rate; }
    string getPosition() const override { return "Секретарь"; }

    void setName(const string& n) override { name = n; }
    void setSurname(const string& s) override { surname = s; }
    void setPatronymic(const string& p) override { patronymic = p; }
    void setRate(double r) override {
        if (r < 0.1 || r > 1) {
            throw runtime_error("Ставка должна быть в диапазоне от 0.1 до 1.");
        }
        rate = r;
    }


    void displayInfo() const override {
        cout << "Секретарь: " << name << " " << surname << " " << patronymic << ", Ставка: " << rate << "\n";
    }

    double calculateSalary(double baseSalary) const {
        return rate * baseSalary;
    }
};

// Класс "Директор"
class Director : public Human {
private:
    string name, surname, patronymic;
    vector<Human*> employees; // Общий контейнер для всех сотрудников
    bool isFileData; // Атрибут для тестового директора

public:
    Director(string n, string s, string p) : name(n), surname(s), patronymic(p) {}
    Director(string n, string s, string p, bool testFlag) : name(n), surname(s), patronymic(p), isFileData(testFlag) {}

    string getName() const override { return name; }
    string getSurname() const override { return surname; }
    string getPatronymic() const override { return patronymic; }
    string getPosition() const override { return "Директор"; }
    bool getIsTestDirector() const { return isFileData; }

    void setName(const string& n) override { name = n; }
    void setSurname(const string& s) override { surname = s; }
    void setPatronymic(const string& p) override { patronymic = p; }

    // Проверка, есть ли сотрудник с данной должностью
    bool hasEmployeeWithPosition(const string& position) const {
        for (const auto& emp : employees) {
            if (emp->getPosition() == position) {
                return true;
            }
        }
        return false;
    }

    // Получение списка сотрудников по должности
    vector<Human*> getEmployeesByPosition(const string& position) const {
        vector<Human*> filteredEmployees;
        for (Human* employee : employees) {
            if (employee->getPosition() == position) {
                filteredEmployees.push_back(employee);
            }
        }
        return filteredEmployees;
    }

    Accountant* getAccountant() const {
        for (Human* emp : employees) {
            if (auto accountant = dynamic_cast<Accountant*>(emp)) {
                return accountant; // Возвращаем указатель на бухгалтера
            }
        }
        return nullptr; // Если бухгалтер не найден
    }

    Secretary* getSecretary() const {
        for (Human* emp : employees) {
            if (auto secretary = dynamic_cast<Secretary*>(emp)) {
                return secretary; // Возвращаем указатель на секретаря
            }
        }
        return nullptr; // Если секретарь не найден
    }

    // Получение всех электриков
    vector<Human*> getElectricians() const {
        return getEmployeesByPosition("Электрик");
    }

    // Получение всех водителей
    vector<Human*> getDrivers() const {
        return getEmployeesByPosition("Водитель");
    }

    // Найм электрика
    void hireElectrician(Electrician* electrician) {
        employees.push_back(electrician);
        cout << "Электрик " << electrician->getName() << " нанят.\n";
    }

    // Найм водителя
    void hireDriver(Driver* driver) {
        employees.push_back(driver);
        cout << "Водитель " << driver->getName() << " нанят.\n";
    }

    // Найм секретаря с проверкой на существование
    void hireSecretary(Secretary* secretary) {
        if (hasEmployeeWithPosition("Секретарь")) {
            cout << "Секретарь уже нанят.\n";
        }
        else {
            employees.push_back(secretary);
            cout << "Секретарь " << secretary->getName() << " нанят.\n";
        }
    }

    // Найм бухгалтера с проверкой на существование
    void hireAccountant(Accountant* accountant) {
        if (hasEmployeeWithPosition("Бухгалтер")) {
            cout << "Бухгалтер уже нанят.\n";
        }
        else {
            employees.push_back(accountant);
            cout << "Бухгалтер " << accountant->getName() << " нанят.\n";
        }
    }

    // Увольнение электрика по имени
    void fireElectrician(const string& electricianName) {
        for (auto it = employees.begin(); it != employees.end(); ) {
            if ((*it)->getPosition() == "Электрик" && (*it)->getName() == electricianName) {
                delete* it; // освобождаем память
                it = employees.erase(it); // удаляем сотрудника и обновляем итератор
                cout << "Электрик " << electricianName << " уволен.\n";
                return; // Выходим из функции, так как Электрик уволен
            }
            else {
                ++it; // Переходим к следующему сотруднику
            }
        }
        cout << "Электрик с именем " << electricianName << " не найден.\n";
    }

    // Увольнение водителя по имени
    void fireDriver(const string& driverName) {
        for (auto it = employees.begin(); it != employees.end(); ) {
            if ((*it)->getPosition() == "Водитель" && (*it)->getName() == driverName) {
                delete* it; // освобождаем память
                it = employees.erase(it); // удаляем сотрудника и обновляем итератор
                cout << "Водитель " << driverName << " уволен.\n";
                return; // Выходим из функции, так как водитель уволен
            }
            else {
                ++it; // Переходим к следующему сотруднику
            }
        }
        cout << "Водитель с именем " << driverName << " не найден.\n";
    }

    // Увольнение секретаря
    void fireSecretary() {
        for (auto it = employees.begin(); it != employees.end(); ) {
            if ((*it)->getPosition() == "Секретарь") {
                delete* it; // освобождаем память
                it = employees.erase(it); // удаляем сотрудника и обновляем итератор
                cout << "Секретарь уволен.\n";
                return; // Выходим из функции, так как секретарь уволен
            }
            else {
                ++it; // Переходим к следующему сотруднику
            }
        }
        cout << "Секретарь не был нанят.\n";
    }

    // Увольнение бухгалтера
    void fireAccountant() {
        for (auto it = employees.begin(); it != employees.end(); ) {
            if ((*it)->getPosition() == "Бухгалтер") {
                delete* it; // освобождаем память
                it = employees.erase(it); // удаляем сотрудника и обновляем итератор
                cout << "Бухгалтер уволен.\n";
                return; // Выходим из функции, так как бухгалтер уволен
            }
            else {
                ++it; // Переходим к следующему сотруднику
            }
        }
        cout << "Бухгалтер не был нанят.\n";
    }

    // Вывод информации о подчинённых
    void displaySubordinates() const {
        cout << "Подчинённые директора " << name << " " << surname << ":\n";
        if (employees.empty()) {
            cout << "Нет подчинённых.\n";
        }
        else {
            int index = 1;
            for (const auto& employee : employees) {
                if (employee != nullptr) { // Проверка указателя
                    cout << index++ << ". ";
                    employee->displayInfo();
                }
                else {
                    cout << "Ошибка: Обнаружен некорректный указатель на сотрудника.\n";
                }
            }
        }
    }

    void displayInfo() const override {
        // Находим бухгалтера среди сотрудников
        Human* accountant = nullptr;
        for (auto* emp : employees) {
            if (emp->getPosition() == "Бухгалтер") {
                accountant = emp;
                break;
            }
        }
        // Если бухгалтер не найден, выводим основную информацию
        if (accountant == nullptr) {
            cout << "Директор: " << name << " " << surname << " " << patronymic << "\n";
        }
        else {
            // Получаем базовый оклад у найденного бухгалтера
            double baseSalary = dynamic_cast<Accountant*>(accountant)->getBaseSalary("Директор");
            cout << "Директор: " << name << " " << surname << " " << patronymic << "; ЗП: " << baseSalary << "\n";
        }
    }

};

// подкласс бухгалтера
class SubclassAccountant : public Accountant {
public:
    void displaySalaries(Director& director) const {
        cout << "Зарплаты сотрудников:\n";

        // Зарплата директора
        double directorSalary = getBaseSalary("Директор");
        cout << director.getName() << " " << director.getSurname() << " " << director.getPosition()
            << ": " << directorSalary << "\n";

        // Зарплата бухгалтера
        double accountantSalary = getRate() * getBaseSalary("Бухгалтер");
        cout << getName() << " " << getSurname() << " " << getPosition() << ": " << accountantSalary << "\n";

        // Зарплата секретаря
        auto secretaries = director.getEmployeesByPosition("Секретарь");
        if (!secretaries.empty()) {
            for (const auto* emp : secretaries) {
                const Secretary* secretary = dynamic_cast<const Secretary*>(emp); 
                if (secretary) {
                    double salary = secretary->getRate() * getBaseSalary("Секретарь");
                    cout << secretary->getName() << " " << secretary->getSurname()
                        << " Секретарь: " << salary << "\n";
                }
            }
        }
        else {
            cout << "Секретарь не найден.\n";
        }

        // Зарплата электриков
        auto electricians = director.getEmployeesByPosition("Электрик");
        if (!electricians.empty()) {
            for (const auto* electrician : electricians) {
                double salary = electrician->getRate() * getBaseSalary("Электрик");
                cout << electrician->getName() << " " << electrician->getSurname()
                    << " Электрик: " << salary << "\n";
            }
        }
        else {
            cout << "Электрики не найдены.\n";
        }

        // Зарплата водителей
        auto drivers = director.getEmployeesByPosition("Водитель");
        if (!drivers.empty()) {
            for (const auto* driver : drivers) {
                double salary = driver->getRate() * getBaseSalary("Водитель");
                cout << driver->getName() << " " << driver->getSurname()
                    << " Водитель: " << salary << "\n";
            }
        }
        else {
            cout << "Водители не найдены.\n";
        }
    }

    void changeEmployeeRate(Director& director) {
        auto secretaries = director.getEmployeesByPosition("Секретарь");
        auto electricians = director.getEmployeesByPosition("Электрик");
        auto drivers = director.getEmployeesByPosition("Водитель");

        // Проверка на наличие сотрудников
        if (secretaries.empty() && electricians.empty() && drivers.empty()) {
            cout << "Некому менять ставку.\n";
            return;
        }

        cout << "Выберите сотрудника для изменения ставки:\n";
        int index = 1;

        // Бухгалтер
        cout << index++ << ". " << getName() << " " << getSurname()
            << " - Бухгалтер, Ставка: " << getRate() << "\n";

        // Секретари
        for (auto* emp : secretaries) {
            Secretary* secretary = dynamic_cast<Secretary*>(emp);
            if (secretary) {
                // Доступ к данным секретаря
                cout << index++ << ". " << secretary->getName() << " " << secretary->getSurname()
                    << " - Секретарь, Ставка: " << secretary->getRate() << "\n";
            }
        }

        // Электрики
        for (auto* Electrician : electricians) {
            cout << index++ << ". " << Electrician->getName() << " " << Electrician->getSurname()
                << " - Электрик, Ставка: " << Electrician->getRate() << "\n";
        }

        // Водители
        for (auto* driver : drivers) {
            cout << index++ << ". " << driver->getName() << " " << driver->getSurname()
                << " - Водитель, Ставка: " << driver->getRate() << "\n";
        }

        int choice;
        cout << "Введите номер сотрудника для изменения ставки: ";
        getInput(choice);

        if (choice > 0 && choice < index) {
            double newRate;
            cout << "Введите новую ставку (от 0 до 1): ";
            getInput(newRate);

            if (newRate < 0 || newRate > 1) {
                cout << "Некорректное значение ставки. Она должна быть в диапазоне от 0 до 1.\n";
                return;
            }

            if (choice == 1) {
                setRate(newRate);
                cout << "Ставка бухгалтера изменена на " << newRate << "\n";
            }

            if (choice <= secretaries.size() + 1 && choice != 1) {
                secretaries[choice - 2]->setRate(newRate);
                cout << "Ставка секретаря изменена на " << newRate << "\n";
            }
            else if (choice <= secretaries.size() + electricians.size() + 1 && choice != 1) {
                electricians[choice - secretaries.size() - 2]->setRate(newRate);
                cout << "Ставка электрика изменена на " << newRate << "\n";
            }
            else if (choice <= secretaries.size() + electricians.size() + drivers.size() + 1 && choice != 1) {
                drivers[choice - secretaries.size() - electricians.size() - 2]->setRate(newRate);
                cout << "Ставка водителя изменена на " << newRate << "\n";
            }
        }
        else {
            cout << "Некорректный выбор.\n";
        }
    }
};

// подкласс секретаря
class SubclassSecretary : public Secretary {
public:
    void displayElectricians(const Director& director) const {
        vector<Human*> electricians = director.getElectricians(); // Используем Human*
        Accountant* accountant = director.getAccountant(); // Добавляем метод в Director

        if (!accountant) {
            cout << "Бухгалтер не назначен.\n";
            return;
        }

        if (electricians.empty()) {
            cout << "Нет электриков для отображения.\n";
            return;
        }

        cout << "+----------------------+----------------------+----------------------+\n";
        cout << "|        Имя           |       Фамилия        |      Зарплата        |\n";
        cout << "+----------------------+----------------------+----------------------+\n";

        for (const auto& emp : electricians) {
            Electrician* electrician = dynamic_cast<Electrician*>(emp); // Приведение типов
            if (electrician) { // Проверяем, что приведение прошло успешно
                double baseSalary = accountant->getBaseSalary("Электрик");
                double salary = electrician->calculateSalary(baseSalary);
                cout << "| " << setw(20) << electrician->getName() << " | "
                    << setw(20) << electrician->getSurname() << " | "
                    << setw(20) << salary << " |\n";
            }
        }

        cout << "+----------------------+----------------------+----------------------+\n";
    }

    void displayDrivers(const Director& director) const {
        vector<Human*> drivers = director.getDrivers(); // Используем Human*
        Accountant* accountant = director.getAccountant(); // Добавляем метод в Director

        if (!accountant) {
            cout << "Бухгалтер не назначен.\n";
            return;
        }

        if (drivers.empty()) {
            cout << "Нет водителей для отображения.\n";
            return;
        }

        cout << "+----------------------+----------------------+----------------------+\n";
        cout << "|        Имя           |       Фамилия        |      Зарплата        |\n";
        cout << "+----------------------+----------------------+----------------------+\n";

        for (const auto& emp : drivers) {
            Driver* driver = dynamic_cast<Driver*>(emp); // Приведение типов
            if (driver) { // Проверяем, что приведение прошло успешно
                double baseSalary = accountant->getBaseSalary("Водитель");
                double salary = driver->calculateSalary(baseSalary);
                cout << "| " << setw(20) << driver->getName() << " | "
                    << setw(20) << driver->getSurname() << " | "
                    << setw(20) << salary << " |\n";
            }
        }

        cout << "+----------------------+----------------------+----------------------+\n";
    }
};

// меню действий для бухгалтера
void accountantActions(Director& director) {
    if (director.getAccountant() == nullptr) {
        cout << "Нет нанятого бухгалтера.\n";
        return;
    }

    Accountant* accountant = director.getAccountant();
    SubclassAccountant* subclassAccountant = static_cast<SubclassAccountant*>(accountant);

    int action;
    do {
        cout << "\nМеню действий для бухгалтера " << accountant->getName() << " " << accountant->getSurname() << ":\n";
        cout << "1. Рассчитать зарплату сотрудников\n";
        cout << "2. Показать оклады по должностям\n";
        cout << "3. Поменять оклад у должности\n";
        cout << "4. Установить ставку для сотрудника\n";
        cout << "5. Вернуться в меню директора\n";
        cout << "Выберите действие: ";
        getInput(action);
        system("cls");


        switch (action) {
        case 1:
            subclassAccountant->displaySalaries(director);
            break;
        case 2:
            accountant->displayBaseSalaries();
            break;
        case 3: {
            accountant->changeBaseSalary();
            break;
        }
        case 4:
            subclassAccountant->changeEmployeeRate(director);
            break;
        case 5:
            cout << "Возврат в меню директора\n";
            break;
        default:
            cout << "Некорректный выбор. Попробуйте снова.\n";
        }
    } while (action != 5);
}

// меню действий для секретаря
void secretaryActions(Director& director) {
    if (director.getSecretary() == nullptr) {
        cout << "Нет нанятого секретаря.\n";
        return;
    }
    Secretary* secretary = director.getSecretary();
    SubclassSecretary* subclassSecretary = dynamic_cast<SubclassSecretary*>(secretary);

    int action;
    do {
        cout << "\nМеню действий для секретаря " << secretary->getName() << " " << secretary->getSurname() << ":\n";
        cout << "1. Показать всех электриков\n";
        cout << "2. Показать всех водителей\n";
        cout << "3. Вернуться в меню директора\n";
        cout << "Выберите действие: ";
        getInput(action);
        system("cls");

        switch (action) {
        case 1:
            subclassSecretary->displayElectricians(director);
            break;
        case 2:
            subclassSecretary->displayDrivers(director);
            break;
        case 3:
            cout << "Возврат в меню директора.\n";
            break;
        default:
            cout << "Некорректный выбор.\n";
        }
    } while (action != 3);
}

// меню действия для водителей
void driverActions(Director& director) {
    if (director.getDrivers().empty()) {
        cout << "Нет нанятых водителей.\n";
        return;
    }

    cout << "Выберите водителя:\n";
    for (size_t i = 0; i < director.getDrivers().size(); ++i) {
        Driver* driver = dynamic_cast<Driver*>(director.getDrivers()[i]); // Приводим к типу Driver*
        if (driver) { // Проверяем, успешно ли произошло приведение
            cout << i + 1 << ". " << driver->getName() << " " << driver->getSurname() << "\n";
        }
        else {
            cout << "Ошибка: Не удалось привести тип к Driver для индекса " << i + 1 << ".\n";
        }
    }

    int driverChoice;
    getInput(driverChoice);
    system("cls");
    if (driverChoice > 0 && driverChoice <= director.getDrivers().size()) {
        Driver* driverPtr = dynamic_cast<Driver*>(director.getDrivers()[driverChoice - 1]); // Приведение к Driver*

        if (driverPtr) { // Проверяем успешность приведения
            Driver& driver = *driverPtr; // Разыменовываем указатель, чтобы получить ссылку на Driver

            int action;
            do {
                cout << "\nМеню действий для водителя " << driver.getName() << " " << driver.getSurname() << ":\n";
                cout << "1. Добавить категорию прав\n";
                cout << "2. Убрать категорию прав\n";
                cout << "3. Добавить транспортное средство\n";
                cout << "4. Убрать транспортное средство\n";
                cout << "5. Вернуться в меню директора\n";
                cout << "Выберите действие: ";
                getInput(action);
                system("cls");

                switch (action) {
                case 1: {
                    string license;
                    cout << "Переключите раскладку на английский.\n";
                    cout << "Введите категорию прав: ";
                    cin >> license;
                    cout << "Верните раскладку на русский.\n";
                    try {
                        driver.addLicense(license);
                        cout << "Категория прав добавлена.\n";
                    }
                    catch (const runtime_error& e) {
                        cout << e.what() << endl;
                    }
                    break;
                }
                case 2: {
                    if (driver.getLicenses().empty()) {
                        cout << "Нет категорий прав для удаления.\n";
                    }
                    else {
                        cout << "Выберите категорию для удаления:\n";
                        for (size_t i = 0; i < driver.getLicenses().size(); ++i) {
                            cout << i + 1 << ". " << driver.getLicenses()[i] << "\n";
                        }
                        int licenseChoice;
                        getInput(licenseChoice);
                        if (licenseChoice > 0 && licenseChoice <= driver.getLicenses().size()) {
                            driver.removeLicense(licenseChoice - 1);
                            cout << "Категория прав удалена.\n";
                        }
                        else {
                            cout << "Некорректный выбор.\n";
                        }
                    }
                    break;
                }
                case 3: {
                    string vehicle;
                    cout << "Введите название транспортного средства (с большой буквы): ";
                    getAlphaString(vehicle);
                    driver.addVehicle(vehicle);
                    cout << "Транспортное средство добавлено.\n";
                    break;
                }
                case 4: {
                    if (driver.getVehicles().empty()) {
                        cout << "Нет транспортных средств для удаления.\n";
                    }
                    else {
                        cout << "Выберите транспортное средство для удаления:\n";
                        for (size_t i = 0; i < driver.getVehicles().size(); ++i) {
                            cout << i + 1 << ". " << driver.getVehicles()[i] << "\n";
                        }
                        int vehicleChoice;
                        getInput(vehicleChoice);
                        if (vehicleChoice > 0 && vehicleChoice <= driver.getVehicles().size()) {
                            driver.removeVehicle(vehicleChoice - 1);
                            cout << "Транспортное средство удалено.\n";
                        }
                        else {
                            cout << "Некорректный выбор.\n";
                        }
                    }
                    break;
                }
                case 5:
                    cout << "Возврат в меню директора.\n";
                    break;
                default:
                    cout << "Некорректный выбор.\n";
                }
            } while (action != 5);
        }
    }
    else {
        cout << "Некорректный выбор.\n";
    }
}

// меню для действий для электриков
void guardActions(Director& director) {
    if (director.getElectricians().empty()) {
        cout << "Нет нанятых электриков.\n";
        return;
    }

    cout << "Выберите электрика:\n";
    for (size_t i = 0; i < director.getElectricians().size(); ++i) {
        Electrician* electrician = dynamic_cast<Electrician*>(director.getElectricians()[i]); // Приведение к типу Electrician*
        if (electrician) { // Проверка на успешность приведения
            cout << i + 1 << ". " << electrician->getName() << " " << electrician->getSurname() << "\n";
        }
        else {
            cout << "Ошибка: Не удалось привести тип к Electrician для индекса " << i + 1 << ".\n";
        }
    }

    int guardChoice;
    getInput(guardChoice);
    system("cls");

    if (guardChoice > 0 && guardChoice <= director.getElectricians().size()) {
        Electrician* guardPtr = dynamic_cast<Electrician*>(director.getElectricians()[guardChoice - 1]); // Приведение к Electrician*

        if (guardPtr) { // Проверяем успешность приведения
            Electrician& Electrician = *guardPtr; // Разыменовываем указатель, чтобы получить ссылку на Electrician

            int action;
            string equipment;
            bool disappeared = false;
            do {
                cout << "\nМеню действий для электрика " << Electrician.getName() << " " << Electrician.getSurname() << ":\n";
                cout << "1. Поменять оборудование\n";
                cout << "2. Совершить починку проводки\n";
                cout << "3. Вернуться в меню директора\n";
                cout << "Выберите действие: ";
                getInput(action);
                system("cls");

                switch (action) {
                case 1:
                    cout << "Введите новое оборудование (с большой буквы): ";
                    getAlphaString(equipment);
                    Electrician.setEquipment(equipment);
                    cout << "Оборудование изменено.\n";
                    break;
                case 2: {
                    disappeared = Electrician.fix_wiring();
                    if (disappeared) { director.fireElectrician(Electrician.getName()); }
                    break;
                }
                case 3:
                    cout << "Возврат в меню директора.\n";
                    break;
                default:
                    cout << "Некорректный выбор. Попробуйте снова.\n";
                }
            } while (action != 3 && !disappeared);
        }
        else {
            cout << "Некорректный выбор.\n";
        }
    }
}

// меню управления для директора
void directorMenu(Director& director) {
    int choice;
    do {
        cout << "\nМеню директора " << director.getName() << ":\n";
        cout << "1. Показать информацию о директоре\n";
        cout << "2. Показать подчинённых\n";
        cout << "3. Нанять сотрудника\n";
        cout << "4. Уволить сотрудника\n";
        cout << "5. Действия для водителя\n";
        cout << "6. Действия для электрика\n";
        cout << "7. Действия для секретаря\n";
        cout << "8. Действия для бухгалтера\n";
        cout << "9. Вернуться в main меню\n";
        cout << "Выберите действие: ";
        getInput(choice);
        system("cls");

        switch (choice) {
        case 1:
            director.displayInfo();
            break;
        case 2:
            if (director.getAccountant() == nullptr && director.getSecretary() == nullptr &&
                director.getElectricians().empty() && director.getDrivers().empty()) {
                cout << "Нет подчинённых.\n";
            }
            else {
                director.displaySubordinates();
            }
            break;
        case 3: {
            int hireChoice;
            cout << "\nКого вы хотите нанять?\n";
            cout << "1. Электрика\n";
            cout << "2. Водителя\n";
            cout << "3. Бухгалтера\n";
            cout << "4. Секретаря\n";
            cout << "Выберите должность: ";
            getInput(hireChoice);

            if (hireChoice == 1) {
                string name, surname, patronymic, equipment;
                double rate;
                cout << "Введите имя электрика: ";
                getAlphaString(name);
                cout << "Введите фамилию электрика: ";
                getAlphaString(surname);
                cout << "Введите отчество электрика: ";
                getAlphaString(patronymic);
                do {
                    cout << "Введите ставку электрика (от 0 до 1): ";
                    getInput(rate);
                    if (rate < 0 || rate > 1) {
                        cout << "Ошибка: ставка должна быть в диапазоне от 0 до 1.\n";
                    }
                } while (rate < 0.1 || rate > 1);
                cout << "Введите оборудование электрика: ";
                getAlphaString(equipment);
                director.hireElectrician(new Electrician(name, surname, patronymic, rate, equipment));
            }
            else if (hireChoice == 2) {
                string name, surname, patronymic;
                double rate;
                cout << "Введите имя водителя: ";
                getAlphaString(name);
                cout << "Введите фамилию водителя: ";
                getAlphaString(surname);
                cout << "Введите отчество водителя: ";
                getAlphaString(patronymic);
                do {
                    cout << "Введите ставку водителя (от 0 до 1): ";
                    getInput(rate);
                    if (rate < 0 || rate > 1) {
                        cout << "Ошибка: ставка должна быть в диапазоне от 0 до 1.\n";
                    }
                } while (rate < 0.1 || rate > 1);

                Driver* newDriver = new Driver(name, surname, patronymic, rate);

                // ввод категорий прав
                char addMore;
                do {
                    string license;
                    cout << "Введите категорию прав (или '0' для завершения ввода): ";
                    cin >> license;
                    if (license == "0") break;
                    try {
                        newDriver->addLicense(license);  // Используем указатель
                    }
                    catch (const runtime_error& e) {
                        cout << e.what() << endl;
                    }
                    cout << "Хотите добавить еще категорию прав? (1 - да/0 - нет): ";
                    cin >> addMore;
                } while (addMore == '1');

                // ввод транспортных средств
                do {
                    string vehicle;
                    cout << "Введите название транспортного средства (или '0' для завершения ввода): ";
                    getAlphaString(vehicle);
                    if (vehicle == "0") break;
                    try {
                        newDriver->addVehicle(vehicle);  // Используем указатель
                    }
                    catch (const runtime_error& e) {
                        cout << e.what() << endl;
                    }
                    cout << "Хотите добавить еще транспортное средство? (1 - да/0 - нет): ";
                    cin >> addMore;
                } while (addMore == '1');

                director.hireDriver(newDriver);
            }
            else if (hireChoice == 3) {
                if (director.getAccountant() == nullptr) {
                    string name, surname, patronymic;
                    double rate;
                    cout << "Введите имя бухгалтера: ";
                    getAlphaString(name);
                    cout << "Введите фамилию бухгалтера: ";
                    getAlphaString(surname);
                    cout << "Введите отчество бухгалтера: ";
                    getAlphaString(patronymic);
                    do {
                        cout << "Введите ставку бухгалтера (от 0 до 1): ";
                        getInput(rate);
                        if (rate < 0 || rate > 1) {
                            cout << "Ошибка: ставка должна быть в диапазоне от 0 до 1.\n";
                        }
                    } while (rate < 0.1 || rate > 1);
                    director.hireAccountant(new Accountant(name, surname, patronymic, rate));
                }
                else {
                    cout << "Бухгалтер уже нанят.\n";
                }
            }
            else if (hireChoice == 4) {
                if (director.getSecretary() == nullptr) {
                    string name, surname, patronymic;
                    double rate;
                    cout << "Введите имя секретаря: ";
                    getAlphaString(name);
                    cout << "Введите фамилию секретаря: ";
                    getAlphaString(surname);
                    cout << "Введите отчество секретаря: ";
                    getAlphaString(patronymic);
                    do {
                        cout << "Введите ставку секретаря (от 0 до 1): ";
                        getInput(rate);
                        if (rate < 0 || rate > 1) {
                            cout << "Ошибка: ставка должна быть в диапазоне от 0 до 1.\n";
                        }
                    } while (rate < 0.1 || rate > 1);
                    director.hireSecretary(new Secretary(name, surname, patronymic, rate));
                }
                else {
                    cout << "Секретарь уже нанят.\n";
                }
            }
            break;
        }
        case 4: {
            if (director.getAccountant() == nullptr && director.getSecretary() == nullptr &&
                director.getElectricians().empty() && director.getDrivers().empty()) {
                cout << "Некого увольнять.\n";
            }
            else {
                Accountant* accountant = director.getAccountant();
                double driverBaseSalary;
                double guardBaseSalary;
                if (accountant) {
                    driverBaseSalary = accountant->getBaseSalary("Водитель");
                    guardBaseSalary = accountant->getBaseSalary("Электрик");
                }
                int fireChoice;
                cout << "\nКого вы хотите уволить?\n";
                cout << "1. Электрика\n";
                cout << "2. Водителя\n";
                cout << "3. Бухгалтера\n";
                cout << "4. Секретаря\n";
                cout << "Выберите должность: ";
                getInput(fireChoice);

                if (fireChoice == 1) {
                    if (director.getElectricians().empty()) {
                        cout << "Нет нанятых электриков.\n";
                    }
                    else {
                        if (accountant) {
                            cout << "+----------------------+----------------------+----------------------+\n";
                            cout << "|        Имя           |       Фамилия        |      Зарплата        |\n";
                            cout << "+----------------------+----------------------+----------------------+\n";

                            for (size_t i = 0; i < director.getElectricians().size(); ++i) {
                                auto emp = director.getElectricians()[i]; // Получаем сотрудника
                                Electrician* electrician = dynamic_cast<Electrician*>(emp); // Приведение типа

                                if (electrician) { // Проверяем, что приведение прошло успешно
                                    double salary = electrician->calculateSalary(guardBaseSalary);
                                    cout << "| " << setw(20) << electrician->getName() << " | "
                                        << setw(20) << electrician->getSurname() << " | "
                                        << setw(20) << salary << " |\n";
                                }
                            }

                            cout << "+----------------------+----------------------+----------------------+\n";
                        }
                        else {
                            cout << "+----------------------+----------------------+\n";
                            cout << "|        Имя           |       Фамилия        |\n";
                            cout << "+----------------------+----------------------+\n";

                            for (size_t i = 0; i < director.getElectricians().size(); ++i) {
                                auto emp = director.getElectricians()[i]; // Получаем сотрудника
                                Electrician* electrician = dynamic_cast<Electrician*>(emp); // Приведение типа

                                if (electrician) { // Проверяем, что приведение прошло успешно
                                    cout << "| " << setw(20) << electrician->getName() << " | "
                                        << setw(20) << electrician->getSurname() << " |\n";
                                }
                            }

                            cout << "+----------------------+----------------------+\n";
                        }

                        int guardIndex;
                        cout << "Выберите электрика для увольнения (номер): ";
                        getInput(guardIndex);
                        if (guardIndex > 0 && guardIndex <= director.getElectricians().size()) {
                            director.fireElectrician(director.getElectricians()[guardIndex - 1]->getName());
                            cout << "Электрик уволен.\n";
                        }
                        else {
                            cout << "Некорректный номер.\n";
                        }
                    }
                }
                else if (fireChoice == 2) {
                    if (director.getDrivers().empty()) {
                        cout << "Нет нанятых водителей.\n";
                    }
                    else {
                        if (accountant) {
                            cout << "+----------------------+----------------------+----------------------+\n";
                            cout << "|        Имя           |       Фамилия        |      Зарплата        |\n";
                            cout << "+----------------------+----------------------+----------------------+\n";

                            for (size_t i = 0; i < director.getDrivers().size(); ++i) {
                                auto emp = director.getDrivers()[i]; // Получаем сотрудника
                                Driver* driver = dynamic_cast<Driver*>(emp); // Приведение типа

                                if (driver) { // Проверяем, что приведение прошло успешно
                                    double salary = driver->calculateSalary(driverBaseSalary);
                                    cout << "| " << setw(20) << driver->getName() << " | "
                                        << setw(20) << driver->getSurname() << " | "
                                        << setw(20) << salary << " |\n";
                                }
                            }

                            cout << "+----------------------+----------------------+----------------------+\n";
                        }
                        else {
                            cout << "+----------------------+----------------------+\n";
                            cout << "|        Имя           |       Фамилия        |\n";
                            cout << "+----------------------+----------------------+\n";

                            for (size_t i = 0; i < director.getDrivers().size(); ++i) {
                                auto emp = director.getDrivers()[i]; // Получаем сотрудника
                                Driver* driver = dynamic_cast<Driver*>(emp); // Приведение типа

                                if (driver) { // Проверяем, что приведение прошло успешно
                                    cout << "| " << setw(20) << driver->getName() << " | "
                                        << setw(20) << driver->getSurname() << " |\n";
                                }
                            }

                            cout << "+----------------------+----------------------+\n";
                        }

                        int driverIndex;
                        cout << "Выберите водителя для увольнения (номер): ";
                        getInput(driverIndex);
                        if (driverIndex > 0 && driverIndex <= director.getDrivers().size()) {
                            director.fireDriver(director.getDrivers()[driverIndex - 1]->getName());
                            cout << "Водитель уволен.\n";
                        }
                        else {
                            cout << "Некорректный номер.\n";
                        }
                    }
                }
                else if (fireChoice == 3) {
                    if (director.getAccountant() == nullptr) {
                        cout << "Бухгалтер не назначен.\n";
                    }
                    else {
                        director.fireAccountant();
                        cout << "Бухгалтер уволен.\n";
                    }
                }
                else if (fireChoice == 4) {
                    if (director.getSecretary() == nullptr) {
                        cout << "Секретарь не назначен.\n";
                    }
                    else {
                        director.fireSecretary();
                        cout << "Секретарь уволен.\n";
                    }
                }
                else {
                    cout << "Некорректный выбор.\n";
                }
            }
            break;
        }

        case 5: {
            driverActions(director);
            break;
        }
        case 6: {
            guardActions(director);
            break;
        }
        case 7: {
            secretaryActions(director);
            break;
        }
        case 8: {
            accountantActions(director);
            break;
        }
        case 9:
            cout << "Возврат в main меню.\n";
            break;
        default:
            cout << "Некорректный выбор. Попробуйте снова.\n";
            break;
        }
    } while (choice != 9);
}

// удаление директора
void removeDirector(vector<Director>& directors, bool& testDataLoaded) {
    if (directors.empty()) {
        cout << "Нет созданных директоров для удаления.\n";
        return;
    }

    cout << "Выберите директора для удаления:\n";
    for (size_t i = 0; i < directors.size(); ++i) {
        cout << i + 1 << ". " << directors[i].getName() << " " << directors[i].getSurname() << "\n";
    }

    int dir_choice;
    getInput(dir_choice);
    system("cls");
    string dirName = directors[dir_choice - 1].getName();
    if (dir_choice > 0 && dir_choice <= directors.size()) {
        // Если удаляемый директор является тестовым, сбрасываем флаг testDataLoaded
        if (directors[dir_choice - 1].getIsTestDirector()) {
            testDataLoaded = false;
        }
        directors.erase(directors.begin() + dir_choice - 1);
        cout << "Директор " << dirName << " удален.\n";
    }
    else {
        cout << "Некорректный выбор.\n";
    }
}

// добавление с файла
void loadDirectorsFromFile(vector<Director>& directors) {
    ifstream inFile("example.txt");
    if (!inFile.is_open()) {
        cout << "Не удалось открыть файл для чтения.\n";
        return;
    }

    string line;
    Director* currentDirector = nullptr;

    while (getline(inFile, line)) {
        istringstream iss(line);
        string position, name, surname, patronymic, equipment, license, vehicle;
        double rate;

        getline(iss, position, ',');
        getline(iss, name, ',');
        getline(iss, surname, ',');
        getline(iss, patronymic, ',');

        if (position == "Директор") {
            // Создаем нового директора
            directors.emplace_back(name, surname, patronymic, true);
            currentDirector = &directors.back();
        }
        else if (currentDirector != nullptr) {
            // Читаем ставку для сотрудников (кроме директора)
            iss >> rate;

            // Добавляем сотрудника к текущему директору в зависимости от роли
            if (position == "Бухгалтер") {
                currentDirector->hireAccountant(new Accountant(name, surname, patronymic, rate));
            }
            else if (position == "Секретарь") {
                currentDirector->hireSecretary(new Secretary(name, surname, patronymic, rate));
            }
            else if (position == "Водитель") {
                currentDirector->hireDriver(new Driver(name, surname, patronymic, rate));
            }
            else if (position == "Электрик") {
                currentDirector->hireElectrician(new Electrician(name, surname, patronymic, rate, "Мультиметр"));
            }
        }
    }
    inFile.close();
    cout << "Данные по умолчанию загружены из файла example.txt.\n";
}

// main меню программы
void mainMenu() {
    vector<Director> directors;
    int choice;
    bool testDataLoaded = false;

    if (!testDataLoaded) { // Проверка, загружены ли тестовые данные
        loadDirectorsFromFile(directors);
        testDataLoaded = true; 
    }
    else {
        cout << "Данные по умолчанию уже были загружены.\n";
    }

    do {
        cout << "\nMain меню:\n";
        cout << "1. Добавить нового директора\n";
        cout << "2. Выбрать директора для управления компанией\n";
        cout << "3. Уволить директора\n";
        cout << "4. Выход\n";
        cout << "Выберите действие: ";
        getInput(choice);
        system("cls");

        switch (choice) {
        case 1: {
            string name, surname, patronymic;
            cout << "Введите имя директора: ";
            getAlphaString(name);
            cout << "Введите фамилию директора: ";
            getAlphaString(surname);
            cout << "Введите отчество директора: ";
            getAlphaString(patronymic);

            directors.push_back(Director(name, surname, patronymic));
            break;
        }
        case 2: {
            if (directors.empty()) {
                cout << "Нет созданных директоров.\n";
            }
            else {
                cout << "Выберите директора:\n";
                for (size_t i = 0; i < directors.size(); ++i) {
                    cout << i + 1 << ". " << directors[i].getName() << " " << directors[i].getSurname() << "\n";
                }
                int dir_choice;
                getInput(dir_choice);
                if (dir_choice > 0 && dir_choice <= directors.size()) {
                    directorMenu(directors[dir_choice - 1]);
                }
                else {
                    cout << "Некорректный выбор.\n";
                }
            }
            break;
        }
        case 3:
            removeDirector(directors, testDataLoaded);
            break;
        case 4:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Некорректный выбор. Попробуйте снова.\n";
            break;
        }
    } while (choice != 4);
}

int main() {
    setlocale(LC_ALL, "RU");
    system("chcp 1251");
    system("cls");



    mainMenu();
    return 0;
}


