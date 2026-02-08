#include "App.h"
#include "Utils.h"

// хелпер для вывода
static void printVectorVisual(const vector<float>& v, const string& title = "", int perRow = 8) {
    if (!title.empty()) cout << title << " (size=" << v.size() << "):\n";
    const int w = 10;
    for (size_t i = 0; i < v.size(); ++i) {
        cout << setw(3) << i << ": " << fixed << setprecision(4) << setw(w) << v[i];
        if ((i + 1) % perRow == 0 || i + 1 == v.size()) cout << "\n"; else cout << " ";
    }
}

// App: реализация функций для задания 1

App::App() : binFile_("task1_vector.bin") {
    // создаём вектор размера [12,30] с псевдослучайными значениями
    int n = getRand(12, 30);
    v_.assign(n, 0.0f);
    for (auto &x : v_) x = getRand(-100.0f, 100.0f);
}

// Чтение одного числа с проверкой формата ввода (согласно правилам)
float App::readFloatFromCin(const string &prompt) {
    // kept for compatibility but not used in automated run
    cout << prompt;
    float value;
    if (!(cin >> value)) throw std::runtime_error("Invalid input");
    return value;
}

void App::saveToBinary() const {
    ofstream out(binFile_, ios::binary | ios::out | ios::trunc);
    if (!out.is_open()) throw std::runtime_error("Не удалось открыть файл для записи");

    size_t n = v_.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(n));
    if (n) out.write(reinterpret_cast<const char*>(v_.data()), n * sizeof(float));
    if (out.fail()) throw std::runtime_error("Ошибка записи в бинарный файл");
}

void App::loadFromBinary() {
    ifstream in(binFile_, ios::binary | ios::in);
    if (!in.is_open()) throw std::runtime_error("Не удалось открыть файл для чтения");

    size_t n = 0;
    in.read(reinterpret_cast<char*>(&n), sizeof(n));
    if (in.fail()) throw std::runtime_error("Ошибка чтения размера из бинарного файла");

    vector<float> tmp;
    if (n) {
        tmp.resize(n);
        in.read(reinterpret_cast<char*>(tmp.data()), n * sizeof(float));
        if (in.fail()) throw std::runtime_error("Ошибка чтения данных из бинарного файла");
    }

    v_.swap(tmp);
}

void App::run() {
    init();
    // Сохраняем исходный вектор в бинарный файл перед обработками
    saveToBinary();

    // Автоматически генерируем интервал [a,b]
    float a = getRand(-50.0f, 50.0f);
    float b = a + getRand(0.0f, 100.0f);
    if (a > b) swap(a, b);
    cout << "Автогенерированный интервал: a=" << a << " b=" << b << "\n";

    // Выполнение шагов по очереди
    step_countNegative();
    step_countOutsideInterval(a, b);
    step_sumBeforeFirstMin();
    step_sumBetweenFirstMinAndFirstMax();
    step_selectNegatives();
    step_selectOutsideInterval(a, b);
    step_sortDescending();
    // Восстанавливаем исходный вектор из файла перед следующими операциями
    loadFromBinary();
    step_sortByAbsAscending();
    loadFromBinary();
    step_moveOutsideIntervalToEnd(a, b);

    // Восстанавливаем вектор из бинарного файла после всех обработок
    loadFromBinary();
    cout << "Вектор восстановлен из бинарного файла, размер=" << v_.size() << "\n";
}

// основные методы
void App::step_countNegative() {
    const auto negCount = count_if(v_.begin(), v_.end(), [](float x){ return x < 0.0f; });
    cout << "1) Количество отрицательных элементов: " << negCount << "\n";
}

void App::step_countOutsideInterval(float a, float b) {
    const auto notInInterval = count_if(v_.begin(), v_.end(), [a,b](float x){ return x < a || x > b; });
    cout << "2) Количество элементов вне [a,b]: " << notInInterval << "\n";
}

void App::step_sumBeforeFirstMin() {
    const auto itMin = min_element(v_.begin(), v_.end());
    const float sumBeforeMin = (itMin == v_.begin() || itMin == v_.end()) ? 0.0f : accumulate(v_.begin(), itMin, 0.0f);
    cout << "3) Сумма перед первым минимальным элементом: " << sumBeforeMin << "\n";
}

void App::step_sumBetweenFirstMinAndFirstMax() {
    const auto itMin = min_element(v_.begin(), v_.end());
    const auto itMax = max_element(v_.begin(), v_.end());
    float sumBetween = 0.0f;
    if (itMin != v_.end() && itMax != v_.end() && itMin != itMax) {
        auto first = min(itMin, itMax);
        auto last = max(itMin, itMax);
        if (distance(first, last) > 1) sumBetween = accumulate(next(first), last, 0.0f);
    }
    cout << "4) Сумма между первым min и первым max: " << sumBetween << "\n";
}

void App::step_selectNegatives() {
    vector<float> negatives;
    copy_if(v_.begin(), v_.end(), back_inserter(negatives), [](float x){ return x < 0.0f; });
    cout << "5) Отобранные отрицательные элементы (count=" << negatives.size() << ":\n";
    if (!negatives.empty()) printVectorVisual(negatives, string());
}

void App::step_selectOutsideInterval(float a, float b) {
    vector<float> outside;
    copy_if(v_.begin(), v_.end(), back_inserter(outside), [a,b](float x){ return x < a || x > b; });
    cout << "6) Элементы вне [a,b] (count=" << outside.size() << ":\n";
    if (!outside.empty()) printVectorVisual(outside, string());
}

void App::step_sortDescending() {
    sort(v_.begin(), v_.end(), greater<float>());
    cout << "7) Вектор после сортировки по убыванию:\n";
    printVectorVisual(v_, string());
}

void App::step_sortByAbsAscending() {
    sort(v_.begin(), v_.end(), [](float lhs, float rhs){ return fabs(lhs) < fabs(rhs); });
    cout << "8) Вектор после сортировки по возрастанию модулей:\n";
    printVectorVisual(v_, string());
}

void App::step_moveOutsideIntervalToEnd(float a, float b) {
    stable_partition(v_.begin(), v_.end(), [a,b](float x){ return x >= a && x <= b; });
    cout << "9) Перенос элементов вне [a,b] в конец (внутри-интервал в начале):\n";
    printVectorVisual(v_, string());
}
