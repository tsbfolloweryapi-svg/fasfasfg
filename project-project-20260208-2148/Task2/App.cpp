#include "App.h"
#include "Utils.h"
#include <locale>
#include <cctype>
#include <algorithm>

App::App() : App("test.txt") {}
App::App(const string& fileName) : fileName_(fileName) {}

// Вывод содержимого текстового файла построчно с нумерацией
void App::viewText(const string& title, const string& fname) {
    cout << "     " << title << "\n"
        << "     +" << setfill('-') << setw(112) << "-" << "+"  // ????????? ?? 8
        << setfill(' ') << "\n";

    fstream fs(fname, ios::in);
    if (!fs.is_open()) {
        throw exception(("?????? ???????? ????? " + fname + " ??? ??????").c_str());
    }
    fs.imbue(locale(".1251"));

    int row = 1;
    string line;
    while (getline(fs, line)) {
        cout << " " << setfill('0') << setw(3) << row++ << setfill(' ')
            << " | " << left << setw(110) << line << right << " |\n";  // ????????? ?? 8
    }
    fs.close();
    cout << "     +" << setfill('-') << setw(112) << "-" << "+"
        << setfill(' ') << "\n";
}

// Вывод словаря частот в виде колонок
void App::show(const string& title, const map<string, double>& freqDict) {
    cout << "    " << title << "\n" << setprecision(3);

    int counter = 1;
    for (const auto& item : freqDict) {
        cout << "    " << left << setw(20) << item.first << right
            << setw(5) << item.second << " || ";
        if (counter++ % 3 == 0) {
            cout << "\n";
        }
    }
    cout << "\n";
}

// Сбор и отображение частоты встречаемости слов в файле
void App::doFreqDictWords() {
    cls();
    viewText("???? " + fileName_ + ", ????? ??? ?????????:", fileName_);
    getKey("\n??????? ??? ???????????? ???????");

    try {
        map<string, double> freqDict = TextProcessor::makeWordsFrequencyDict(fileName_);
        show("\n\n    ????????? ??????? ???? ??????, ???? " + fileName_ + ":", freqDict);
    }
    catch (const exception& ex) {
        cout << color(errColor) << "??????: " << ex.what() << color(mainColor) << "\n";
    }

    getKey("\n??????? ??? ???????????...");
}

// Сбор и отображение частоты встречаемости букв в файле
void App::doFreqDictLetters() {
    cls();
    viewText("???? " + fileName_ + ", ????? ??? ?????????:", fileName_);
    getKey("\n??????? ??? ???????????? ???????");

    try {
        map<string, double> freqDict = TextProcessor::makeLettersFrequencyDict(fileName_);
        show("\n\n    ????????? ??????? ???? ??????, ???? " + fileName_ + ":", freqDict);
    }
    catch (const exception& ex) {
        cout << color(errColor) << "??????: " << ex.what() << color(mainColor) << "\n";
    }

    getKey("\n??????? ??? ???????????...");
}

// Чтение файла, обмен пар строк и запись результата в новый файл
void App::doSwapLines() {
    cls();
    viewText("???? " + fileName_ + ", ????? ??? ?????????:", fileName_);
    getKey("\n??????? ??? ????? ?????");

    try {
        fstream in(fileName_, ios::in);
        if (!in.is_open()) throw runtime_error(("?????? ???????? " + fileName_).c_str());
        in.imbue(locale(".1251"));

        vector<string> lines;
        string line;
        while (getline(in, line)) {
            lines.push_back(line);
        }
        in.close();

        for (size_t i = 0; i + 1 < lines.size(); i += 2) {
            swap(lines[i], lines[i + 1]);
        }

        string outFile = "swap.txt";
        ofstream out(outFile);
        if (!out.is_open()) throw runtime_error(("?????? ?????? ? " + outFile).c_str());
        out.imbue(locale(".1251"));
        for (const auto& l : lines) {
            out << l << "\n";
        }
        out.close();

        cout << "?????? ????????, ????????? ? " << outFile << "\n";
        cls();
        viewText("????????? ? " + outFile + ":", outFile);
    }
    catch (const exception& ex) {
        cout << color(errColor) << "??????: " << ex.what() << color(mainColor) << "\n";
    }

    getKey("\n??????? ??? ???????????...");
}

// Преобразование первой буквы каждого слова в верхний регистр
void App::doWordsCapitalize() {
    cls();
    viewText("???? " + fileName_ + ", ????? ??? ?????????:", fileName_);
    getKey("\n??????? ??? capitalize");

    try {
        fstream in(fileName_, ios::in);
        if (!in.is_open()) throw runtime_error(("?????? ???????? " + fileName_).c_str());
        in.imbue(locale(".1251"));

        vector<string> lines;
        string line;
        while (getline(in, line)) {
            lines.push_back(line);
        }
        in.close();

        vector<string> newLines;
        for (auto l : lines) {
            stringstream ss(l);
            string newL, word;
            while (ss >> word) {
                if (!word.empty()) {
                    word[0] = toupper(static_cast<unsigned char>(word[0]));
                    transform(word.begin() + 1, word.end(), word.begin() + 1, [](unsigned char c) { return tolower(c); });
                }
                newL += word + " ";
            }
            if (!newL.empty()) newL.pop_back();
            newLines.push_back(newL);
        }

        string outFile = "capitalize.txt";
        ofstream out(outFile);
        if (!out.is_open()) throw runtime_error(("?????? ?????? ? " + outFile).c_str());
        out.imbue(locale(".1251"));
        for (const auto& l : newLines) {
            out << l << "\n";
        }
        out.close();

        cout << "????? ? capitalize ???????, ????????? ? " << outFile << "\n";
        cls();
        viewText("????????? ? " + outFile + ":", outFile);
    }
    catch (const exception& ex) {
        cout << color(errColor) << "??????: " << ex.what() << color(mainColor) << "\n";
    }

    getKey("\n??????? ??? ???????????...");
}

// Сортировка строк по длине и запись результата в файл
void App::doOrderByLen() {
    cls();
    viewText("???? " + fileName_ + ", ????? ??? ?????????:", fileName_);
    getKey("\n??????? ??? ?????????????? ?? ?????");

    try {
        fstream in(fileName_, ios::in);
        if (!in.is_open()) throw runtime_error(("?????? ???????? " + fileName_).c_str());
        in.imbue(locale(".1251"));

        vector<string> lines;
        string line;
        while (getline(in, line)) {
            lines.push_back(line);
        }
        in.close();

        sort(lines.begin(), lines.end(), [](const string& a, const string& b) { return a.length() < b.length(); });

        string outFile = "orderByLen.txt";
        ofstream out(outFile);
        if (!out.is_open()) throw runtime_error(("?????? ?????? ? " + outFile).c_str());
        out.imbue(locale(".1251"));
        for (const auto& l : lines) {
            out << l << "\n";
        }
        out.close();

        cout << "?????? ??????????? ?? ?????, ????????? ? " << outFile << "\n";
        cls();
        viewText("????????? ? " + outFile + ":", outFile);
    }
    catch (const exception& ex) {
        cout << color(errColor) << "??????: " << ex.what() << color(mainColor) << "\n";
    }

    getKey("\n??????? ??? ???????????...");
}

// Сортировка слов внутри каждой строки (лексикографически, без учёта регистра)
void App::doOrderWordsInLines() {
    cls();
    viewText("???? " + fileName_ + ", ????? ??? ?????????:", fileName_);
    getKey("\n??????? ??? ?????????????? ???? ? ???????");

    try {
        fstream in(fileName_, ios::in);
        if (!in.is_open()) throw runtime_error(("?????? ???????? " + fileName_).c_str());
        in.imbue(locale(".1251"));

        vector<string> lines;
        string line;
        while (getline(in, line)) {
            lines.push_back(line);
        }
        in.close();

        vector<string> newLines;
        for (const auto& l : lines) {
            vector<string> words = TextProcessor::splitBySpace(l);
            sort(words.begin(), words.end(), [](const string& a, const string& b) {
                string la = a, lb = b;
                transform(la.begin(), la.end(), la.begin(), [](unsigned char c) { return tolower(c); });
                transform(lb.begin(), lb.end(), lb.begin(), [](unsigned char c) { return tolower(c); });
                return la < lb;
                });
            string newL;
            for (const auto& w : words) {
                newL += w + " ";
            }
            if (!newL.empty()) newL.pop_back();
            newLines.push_back(newL);
        }

        string outFile = "orderliness.txt";
        ofstream out(outFile);
        if (!out.is_open()) throw runtime_error(("?????? ?????? ? " + outFile).c_str());
        out.imbue(locale(".1251"));
        for (const auto& l : newLines) {
            out << l << "\n";
        }
        out.close();

        cout << "????? ? ??????? ??????????? ?? ????????, ????????? ? " << outFile << "\n";
        cls();
        viewText("????????? ? " + outFile + ":", outFile);
    }
    catch (const exception& ex) {
        cout << color(errColor) << "??????: " << ex.what() << color(mainColor) << "\n";
    }

    getKey("\n??????? ??? ???????????...");
}