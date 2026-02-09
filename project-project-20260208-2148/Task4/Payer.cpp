#include "Payer.h"
#include "Utils.h"
#include <sstream>

// ??????????? ?????????? ??????????? ?????????? Payer
// ?????????: используетс€ дл€ генерации тестовых данных в приложении
// ???????: не использует ввод пользовател€, генерирует случайные значени€
// —оздает и возвращает заполненный объект Payer дл€ тестировани€
// »спользуетс€ при генерации начального набора данных и при изменени€х
Payer Payer::createFactory(int id) {
    Payer p;
    p.setId(id);
    // ???????? ??????? (текстовые пол€) Ч формируем простую заглушку
    // «аполн€ем пол€ заглушечными значени€ми
    p.setName("???????" + to_string(getRand(1, 20)));
    // Ќомер в формате 062-xxx (соответствует заданию)
    p.setPhone("062-" + to_string(getRand(121, 333)));
    // “ариф и скидка Ч вещественные/integer значени€ в разумных пределах
    p.setTariff(getRand(1.0, 5.0));
    p.setDiscount(getRand(0, 20));
    p.setTimeMin(getRand(10, 200));
    // ƒата Ч год в заданном диапазоне
    p.setDate(getRand(1, 28), getRand(1, 12), getRand(2025, 2027));
    return p;
}

string Payer::toString() const {
    // ¬озвращаем компактное строковое представление Payer дл€ отладки/логов
    // ‘ормирует строковое представление плательщика (состо€ние всех полей)
    ostringstream oss;
    oss << "ID: " << getId() << ", ???: " << getName() << ", ???????: " << getPhone()
        << ", ?????: " << fixed << setprecision(2) << getTariff() << ", ??????: " << getDiscount()
        << ", ??????: " << getTimeMin() << ", ????: " << getDate().toString() << ", ?????: " << calculateSum();
    return oss.str();
}