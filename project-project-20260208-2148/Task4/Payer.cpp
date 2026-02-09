#include "Payer.h"
#include "Utils.h"
#include <sstream>

// ����������� ���������� ����������� ���������� Payer
// ���������: используется для генерации тестовых данных в приложении
// �������: не использует ввод пользователя, генерирует случайные значения
// Создает и возвращает заполненный объект Payer для тестирования
// Используется при генерации начального набора данных и при изменениях
Payer Payer::createFactory(int id) {
    Payer p;
    p.setId(id);
    // �������� ������� (текстовые поля) — формируем простую заглушку
    // Заполняем поля заглушечными значениями
    p.setName("�������" + to_string(getRand(1, 20)));
    // Номер в формате 062-xxx (соответствует заданию)
    p.setPhone("062-" + to_string(getRand(121, 333)));
    // Тариф и скидка — вещественные/integer значения в разумных пределах
    p.setTariff(getRand(1.0, 5.0));
    p.setDiscount(getRand(0, 20));
    p.setTimeMin(getRand(10, 200));
    // Дата — год в заданном диапазоне
    p.setDate(getRand(1, 28), getRand(1, 12), getRand(2025, 2027));
    return p;
}

string Payer::toString() const {
    // Возвращаем компактное строковое представление Payer для отладки/логов
    // Формирует строковое представление плательщика (состояние всех полей)
    ostringstream oss;
    oss << "ID: " << getId() << ", ���: " << getName() << ", �������: " << getPhone()
        << ", �����: " << fixed << setprecision(2) << getTariff() << ", ������: " << getDiscount()
        << ", ������: " << getTimeMin() << ", ����: " << getDate().toString() << ", �����: " << calculateSum();
    return oss.str();
}