#include "pch.h"
#include "Utils.h"
#include "MenuItem.h"
#include "Menu.h"
#include "App.h"

int main() {
    init(L"Task2: ????????? ??????");

    App app;

    // Перечень команд меню и их обработка
    enum Commands : int {
        CMD_FREQ_WORDS = 1001,
        CMD_FREQ_LETTERS,
        CMD_SWAP_LINES,
        CMD_CAPITALIZE,
        CMD_ORDER_BY_LEN,
        CMD_ORDER_WORDS_IN_LINES
    };

    vector<MenuItem> items = {
        MenuItem(CMD_FREQ_WORDS, "????????? ??????? ????"),
        MenuItem(CMD_FREQ_LETTERS, "????????? ??????? ????"),
        MenuItem(CMD_SWAP_LINES, "???????? ?????? ???????"),
        MenuItem(CMD_CAPITALIZE, "Capitalize ?????"),
        MenuItem(CMD_ORDER_BY_LEN, "??????????? ?? ?????"),
        MenuItem(CMD_ORDER_WORDS_IN_LINES, "??????????? ????? ? ???????"),
        MenuItem(Menu::CMD_QUIT, "?????")
    };

    Menu menu(COORD{ 5, 5 }, items, mainColor, infoColor);

    while (true) {
        try {
            cls();
            showNavBarMessage(hintColor, "Task2: ????????? ??????");

            int cmd = menu.navigate();
            if (cmd == Menu::CMD_QUIT) break;

            switch (cmd) {
            case CMD_FREQ_WORDS: app.doFreqDictWords(); break;
            case CMD_FREQ_LETTERS: app.doFreqDictLetters(); break;
            case CMD_SWAP_LINES: app.doSwapLines(); break;
            case CMD_CAPITALIZE: app.doWordsCapitalize(); break;
            case CMD_ORDER_BY_LEN: app.doOrderByLen(); break;
            case CMD_ORDER_WORDS_IN_LINES: app.doOrderWordsInLines(); break;
            }
        }
        catch (exception& ex) {
            int x = 12, y = 8;
            cout << color(errColor)
                << pos(x, y) << setw(W) << " "
                << pos(x, y + 1) << setw(W) << " "
                << pos(x, y + 2) << setw(W) << left << "    [??????]"
                << pos(x, y + 3) << setw(W) << " "
                << pos(x, y + 4) << setw(W) << ("    "s + ex.what())
                << pos(x, y + 5) << setw(W) << " "
                << pos(x, y + 6) << setw(W) << " "
                << pos(0, 20) << right << color(mainColor);
            getKey("");
        }
    }

    cls();
    return 0;
}
