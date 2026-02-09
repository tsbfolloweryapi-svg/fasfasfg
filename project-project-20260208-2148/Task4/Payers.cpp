#include "Payers.h"
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>  // for runtime_error

// Проверяет успешность предыдущей операции чтения из istringstream
// Выбрасывает исключение, если чтение завершилось с ошибкой
void checkInputFormat(istringstream& iss) {
    if (iss.fail()) throw runtime_error("CSV: �������� ������ ����");
}

// Конструктор: заполняет список тестовыми плательщиками
Payers::Payers() {
    for (int i = 0; i < 15; ++i) {
        list_.push_back(Payer::createFactory(nextId_++));
    }
}

// Добавляет нового плательщика в список (сгенерированный фабрикой)
void Payers::addPayer() {
    list_.push_back(Payer::createFactory(nextId_++));
}

// Удаляет плательщика по идентификатору
void Payers::deleteById(int id) {
    list_.remove_if([id](const Payer& p) { return p.getId() == id; });
}

// Фильтрация: возвращает всех плательщиков с указанным тарифом
list<Payer> Payers::selectByTariff(double tariff) const {
    list<Payer> result;
    copy_if(list_.begin(), list_.end(), back_inserter(result), [tariff](const Payer& p) {
        return eq(p.getTariff(), tariff);
        });
    return result;
}

// Фильтрация: возвращает всех плательщиков с указанной скидкой
list<Payer> Payers::selectByDiscount(int discount) const {
    list<Payer> result;
    copy_if(list_.begin(), list_.end(), back_inserter(result), [discount](const Payer& p) {
        return p.getDiscount() == discount;
        });
    return result;
}

// Фильтрация: возвращает плательщиков, у которых сумма в заданном диапазоне
// Результат дополнительно сортируется по номеру телефона
list<Payer> Payers::selectBySumRange(double low, double high) const {
    list<Payer> result;
    copy_if(list_.begin(), list_.end(), back_inserter(result), [low, high](const Payer& p) {
        double sum = p.calculateSum();
        return sum >= low && sum <= high;
        });
    result.sort([](const Payer& a, const Payer& b) { return a.getPhone() < b.getPhone(); });
    return result;
}

// Фильтрация: возвращает плательщиков с указанным телефоном
list<Payer> Payers::selectByPhone(const string& phone) const {
    list<Payer> result;
    copy_if(list_.begin(), list_.end(), back_inserter(result), [&phone](const Payer& p) {
        return p.getPhone() == phone;
    });
    return result;
}

// Фильтрация: возвращает плательщиков с указанным именем
list<Payer> Payers::selectByName(const string& name) const {
    list<Payer> result;
    copy_if(list_.begin(), list_.end(), back_inserter(result), [&name](const Payer& p) {
        return p.getName() == name;
    });
    return result;
}

// Фильтрация: возвращает плательщиков с указанной датой
list<Payer> Payers::selectByDate(const Date& date) const {
    list<Payer> result;
    copy_if(list_.begin(), list_.end(), back_inserter(result), [&date](const Payer& p) {
        return p.getDate() == date;
    });
    return result;
}

// Подсчет общей суммы всех платежей
double Payers::totalPayments() const {
    double sum = 0.0;
    for (const auto& p : list_) sum += p.calculateSum();
    return sum;
}

// Сортировка: по номеру телефона
void Payers::sortByPhone() {
    list_.sort([](const Payer& a, const Payer& b) { return a.getPhone() < b.getPhone(); });
}

// Сортировка: по времени вызовов по убыванию
void Payers::sortByTimeDescending() {
    list_.sort([](const Payer& a, const Payer& b) { return a.getTimeMin() > b.getTimeMin(); });
}

// Сортировка: по идентификатору
void Payers::sortById() {
    list_.sort([](const Payer& a, const Payer& b) { return a.getId() < b.getId(); });
}

// Сортировка: по имени
void Payers::sortByName() {
    list_.sort([](const Payer& a, const Payer& b) { return a.getName() < b.getName(); });
}

// Сортировка: по сумме платежа по убыванию
void Payers::sortBySumDescending() {
    list_.sort([](const Payer& a, const Payer& b) { return a.calculateSum() > b.calculateSum(); });
}

// Заменяет данные плательщика с указанным id новыми (генерируемыми фабрикой)
void Payers::changePayer(int id) {
    for (auto& p : list_) {
        if (p.getId() == id) {
            p = Payer::createFactory(id);
            return;
        }
    }
    throw runtime_error("���������� �� ������");
}

// Сохранение списка плательщиков в CSV-файл
void Payers::saveToCSV(const string& fname) const {
    ofstream out(fname);
    if (!out.is_open()) throw runtime_error(("�� ������� ������� ���� ��� ������: " + fname).c_str());

    out << "ID,���,�������,�����,������,������,����,�����,���\n";
    for (const auto& p : list_) {
        out << p.getId() << "," << p.getName() << "," << p.getPhone() << "," << p.getTariff() << "," << p.getDiscount()
            << "," << p.getTimeMin() << "," << p.getDate().getDay() << "," << p.getDate().getMonth() << "," << p.getDate().getYear() << "\n";
    }
}

// Загрузка списка плательщиков из CSV-файла
// Примечание: парсинг строгий — ожидается точный формат, иначе выбрасывается исключение
void Payers::loadFromCSV(const string& fname) {
    if (fname.empty()) throw runtime_error("CSV: ������ ��� �����.");

    ifstream in(fname);
    if (!in.is_open()) throw runtime_error(("CSV: �� ������� ������� ���� " + fname).c_str());

    string line;
    if (!getline(in, line)) throw runtime_error("CSV: ������ ������ ���������.");

    // Проверяем заголовок CSV
    {
        istringstream iss(line);
        string token;
        if (!getline(iss, token, ',')) throw runtime_error("CSV: �������� ���������.");
        if (token != "ID") throw runtime_error("CSV: �������� ��������� (��������� ID).");
    }

    list_.clear();
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        Payer p;
        int id, discount, timeMin, d, m, y;
        double tariff;
        string name, phone;

        // Читаем поля в строгом порядке и проверяем формат после каждого чтения
        iss >> id;
        checkInputFormat(iss);
        if (iss.get() != ',') throw runtime_error("CSV: �������� ����������� (��������� ������� ����� ID).");
        getline(iss, name, ',');
        getline(iss, phone, ',');
        iss >> tariff;
        checkInputFormat(iss);
        if (iss.get() != ',') throw runtime_error("CSV: �������� ����������� (��������� ������� ����� ������).");
        iss >> discount;
        checkInputFormat(iss);
        if (iss.get() != ',') throw runtime_error("CSV: �������� ����������� (��������� ������� ����� ������).");
        iss >> timeMin;
        checkInputFormat(iss);
        if (iss.get() != ',') throw runtime_error("CSV: �������� ����������� (��������� ������� ����� �����).");
        iss >> d;
        checkInputFormat(iss);
        if (iss.get() != ',') throw runtime_error("CSV: �������� ����������� (��������� ������� ����� ���).");
        iss >> m;
        checkInputFormat(iss);
        if (iss.get() != ',') throw runtime_error("CSV: �������� ����������� (��������� ������� ����� ������).");
        iss >> y;
        checkInputFormat(iss);

        // Заполняем объект Payer считанными значениями
        p.setId(id);
        p.setName(name);
        p.setPhone(phone);
        p.setTariff(tariff);
        p.setDiscount(discount);
        p.setTimeMin(timeMin);
        p.setDate(d, m, y);

        list_.push_back(p);
        nextId_ = max(nextId_, p.getId() + 1);
    }

    if (list_.empty()) throw runtime_error("CSV: � ����� ��� ������� ��� ��������.");
}