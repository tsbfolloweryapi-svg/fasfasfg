#include "pch.h"
#include "Utils.h"
#include "MenuItem.h"
#include "Menu.h"
#include "App.h"

// Точка входа приложения: инициализация и главный цикл обработки команд
// Инициализирует окружение, формирует меню и вызывает методы App по командам
int main() try {
    init(L"Task4: �������� ����");

    App app;

    // Commands
    enum Commands : int {
        CMD_ADD = 1001,
        CMD_REMOVE,
        CMD_FILTER_TARIFF,
        CMD_FILTER_DISCOUNT,
        CMD_FILTER_SUM_RANGE,
        CMD_SORT_ID,
        CMD_SORT_NAME,
        CMD_SORT_SUM_DESC,
        CMD_CHANGE,
        CMD_SAVE,
        CMD_LOAD
    };

    vector<MenuItem> items = {
        MenuItem(CMD_ADD, "�������� ��������"),
        MenuItem(CMD_REMOVE, "������� �� ID"),
        MenuItem(CMD_FILTER_TARIFF, "����� �� ������"),
        MenuItem(CMD_FILTER_DISCOUNT, "����� �� ������"),
        MenuItem(CMD_FILTER_SUM_RANGE, "����� �� ����� � ���������"),
        MenuItem(CMD_SORT_ID, "���� �� ID"),
        MenuItem(CMD_SORT_NAME, "���� �� ���"),
        MenuItem(CMD_SORT_SUM_DESC, "���� �� ����� ����."),
        MenuItem(CMD_CHANGE, "�������� ��������"),
        MenuItem(CMD_SAVE, "Save CSV"),
        MenuItem(CMD_LOAD, "Load CSV"),
        MenuItem(Menu::CMD_QUIT, "�����")
    };

    Menu menu(COORD{ 5, 5 }, items, mainColor, infoColor);

    while (true) {
        cls();
        showNavBarMessage(hintColor, "Task4: ��������");

        int cmd = menu.navigate();
        if (cmd == Menu::CMD_QUIT) break;

        switch (cmd) {
        case CMD_ADD: app.doAddPayer(); break;
        case CMD_REMOVE: app.doDeleteById(); break;
        case CMD_FILTER_TARIFF: app.doSelectByTariff(); break;
        case CMD_FILTER_DISCOUNT: app.doSelectByDiscount(); break;
        case CMD_FILTER_SUM_RANGE: app.doSelectBySumRange(); break;
        case CMD_SORT_ID: app.doSortById(); break;
        case CMD_SORT_NAME: app.doSortByName(); break;
        case CMD_SORT_SUM_DESC: app.doSortBySumDescending(); break;
        case CMD_CHANGE: app.doChangePayer(); break;
        case CMD_SAVE: app.doSaveToCSV(); break;
        case CMD_LOAD: app.doLoadFromCSV(); break;
        }

        getKey("\n������� ��� �����������...");
    }

    cls();
    return 0;
}
catch (exception& ex) {
    int x = 12, y = 8;
    cout << color(errColor)
        << pos(x, y) << setw(W) << " "
        << pos(x, y + 1) << setw(W) << " "
        << pos(x, y + 2) << setw(W) << left << "    [������]"
        << pos(x, y + 3) << setw(W) << " "
        << pos(x, y + 4) << setw(W) << ("    "s + ex.what())
        << pos(x, y + 5) << setw(W) << " "
        << pos(x, y + 6) << setw(W) << " "
        << pos(0, 20) << right << color(mainColor);
    getKey("");
}