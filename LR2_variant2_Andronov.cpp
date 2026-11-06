#include <iostream> // Подключаем стандартный ввод и вывод.
#include <fstream> // Подключаем файловые потоки.
#include <string> // Подключаем тип string.
#include <vector> // Подключаем контейнер vector.
#include <sstream> // Подключаем строковый поток для проверки чисел.
#include <limits> // Подключаем ограничения целочисленных типов.
#include <cstdint> // Подключаем целые типы фиксированного размера.
#include <stdexcept> // Подключаем исключения для сообщений об ошибках.
#ifdef _WIN32 // Проверяем, выполняется ли программа в Windows.
#include <windows.h> // Подключаем настройку кодировки консоли Windows.
#endif // Завершаем условное подключение для Windows.
using namespace std; // Используем имена стандартной библиотеки без префикса std::.

struct Department { // Объявляем структуру «подразделение организации».
    string name; // Храним название подразделения.
    string manager; // Храним ФИО начальника.
    int employees; // Храним общее число сотрудников.
    int onVacation; // Храним число сотрудников в отпуске.
}; // Завершаем описание структуры.

const int MAX_DEPARTMENTS = 10000; // Ограничиваем количество записей для защиты памяти.

int readNumber(const string& prompt, int maximum) { // Вводим число от нуля до maximum.
    string line; // Создаём строку для чтения ввода целиком.
    while (true) { // Повторяем ввод, пока не получим корректное число.
        cout << prompt; // Показываем приглашение ко вводу.
        if (!getline(cin, line)) { // Проверяем, удалось ли прочитать строку.
            throw runtime_error("Ввод данных был прерван."); // Сообщаем об окончании потока ввода.
        } // Завершаем проверку чтения строки.
        istringstream input(line); // Создаём поток для проверки введённой строки.
        long long value = -1; // Готовим переменную для прочитанного числа.
        char extra = '\0'; // Готовим переменную для проверки лишних символов.
        if ((input >> value) && !(input >> extra) && value >= 0 && value <= maximum) { // Проверяем целое число и диапазон.
            return static_cast<int>(value); // Возвращаем допустимое число.
        } // Завершаем проверку корректности числа.
        cout << "Ошибка: введите целое число от 0 до " << maximum << ".\n"; // Просим повторить ввод.
    } // Завершаем цикл повторного ввода.
} // Завершаем функцию чтения целого числа.

string readText(const string& prompt) { // Вводим непустое текстовое поле.
    string value; // Создаём строку для введённого значения.
    while (true) { // Повторяем ввод до получения непустой строки.
        cout << prompt; // Выводим название поля.
        if (!getline(cin, value)) { // Проверяем успешность чтения.
            throw runtime_error("Ввод данных был прерван."); // Сообщаем об окончании ввода.
        } // Завершаем проверку чтения.
        if (value.find_first_not_of(" \t") != string::npos) { // Проверяем наличие видимых символов.
            return value; // Возвращаем введённый текст.
        } // Завершаем проверку непустой строки.
        cout << "Ошибка: поле не должно быть пустым.\n"; // Просим ввести текст повторно.
    } // Завершаем цикл повторного ввода.
} // Завершаем функцию чтения строки.

vector<Department> inputDepartments() { // Создаём вектор подразделений из ввода с клавиатуры.
    int count = readNumber("Количество подразделений: ", MAX_DEPARTMENTS); // Узнаём размер вектора.
    vector<Department> departments; // Создаём пустой вектор структур.
    departments.reserve(count); // Выделяем память под указанное число элементов.
    for (int i = 0; i < count; ++i) { // Вводим сведения о каждом подразделении.
        cout << "\nПодразделение №" << i + 1 << "\n"; // Показываем номер текущей записи.
        Department item; // Создаём очередной объект структуры.
        item.name = readText("Название: "); // Вводим название подразделения.
        item.manager = readText("ФИО начальника: "); // Вводим ФИО начальника.
        item.employees = readNumber("Число сотрудников: ", numeric_limits<int>::max()); // Вводим число сотрудников.
        item.onVacation = readNumber("Сотрудников в отпуске: ", item.employees); // Не допускаем превышения общего числа.
        departments.push_back(item); // Добавляем заполненную структуру в вектор.
    } // Завершаем цикл ввода подразделений.
    return departments; // Возвращаем сформированный вектор.
} // Завершаем функцию ввода объектов.

void printDepartments(const vector<Department>& departments) { // Выводим вектор на экран.
    cout << "\nВсего подразделений: " << departments.size() << "\n"; // Показываем число объектов.
    for (size_t i = 0; i < departments.size(); ++i) { // Перебираем все записи вектора.
        const Department& item = departments[i]; // Получаем текущую структуру по ссылке.
        cout << "\nПодразделение №" << i + 1 << "\n"; // Показываем номер подразделения.
        cout << "Название: " << item.name << "\n"; // Выводим название.
        cout << "ФИО начальника: " << item.manager << "\n"; // Выводим начальника.
        cout << "Число сотрудников: " << item.employees << "\n"; // Выводим штатную численность.
        cout << "Сотрудников в отпуске: " << item.onVacation << "\n"; // Выводим число отпускников.
    } // Завершаем перебор всех подразделений.
} // Завершаем функцию вывода данных.

void writeDepartment(ostream& out, const Department& item, bool binary) { // Записываем одну структуру в поток.
    if (binary) { // Проверяем, выбран ли двоичный режим.
        out.write(item.name.c_str(), static_cast<streamsize>(item.name.size() + 1)); // Пишем название с завершающим нулём.
        out.write(item.manager.c_str(), static_cast<streamsize>(item.manager.size() + 1)); // Пишем ФИО с завершающим нулём.
        int32_t employees = item.employees; // Приводим число сотрудников к 32-битному типу.
        int32_t onVacation = item.onVacation; // Приводим число отпускников к 32-битному типу.
        out.write(reinterpret_cast<const char*>(&employees), sizeof(employees)); // Записываем число сотрудников как байты.
        out.write(reinterpret_cast<const char*>(&onVacation), sizeof(onVacation)); // Записываем число отпускников как байты.
    } else { // Переходим к текстовому формату.
        out << item.name << '\n'; // Записываем название в отдельную строку.
        out << item.manager << '\n'; // Записываем ФИО в отдельную строку.
        out << item.employees << ' ' << item.onVacation << '\n'; // Записываем два числа через пробел.
    } // Завершаем выбор формата записи.
} // Завершаем функцию записи структуры.

bool readDepartment(istream& in, Department& item, bool binary) { // Читаем одну структуру из потока.
    if (binary) { // Проверяем, выбран ли двоичный режим.
        int32_t employees = 0; // Готовим временную переменную для числа сотрудников.
        int32_t onVacation = 0; // Готовим временную переменную для числа отпускников.
        if (!getline(in, item.name, '\0')) return false; // Читаем название до нулевого символа.
        if (!getline(in, item.manager, '\0')) return false; // Читаем ФИО до нулевого символа.
        if (!in.read(reinterpret_cast<char*>(&employees), sizeof(employees))) return false; // Читаем численность из байтов.
        if (!in.read(reinterpret_cast<char*>(&onVacation), sizeof(onVacation))) return false; // Читаем количество отпускников.
        item.employees = employees; // Сохраняем число сотрудников в структуру.
        item.onVacation = onVacation; // Сохраняем число отпускников в структуру.
    } else { // Переходим к чтению текстового формата.
        if (!getline(in, item.name)) return false; // Читаем строку с названием.
        if (!getline(in, item.manager)) return false; // Читаем строку с ФИО начальника.
        if (!(in >> item.employees >> item.onVacation)) return false; // Читаем два целых числа.
        in.ignore(numeric_limits<streamsize>::max(), '\n'); // Пропускаем остаток числовой строки.
    } // Завершаем выбор формата чтения.
    return !item.name.empty() && !item.manager.empty() && item.employees >= 0 && // Проверяем непустые строки и численность.
           item.onVacation >= 0 && item.onVacation <= item.employees; // Проверяем корректность количества отпускников.
} // Завершаем функцию чтения структуры.

void saveDepartments(const string& filename, const vector<Department>& departments, bool binary) { // Сохраняем весь вектор в файл.
    ofstream out(filename, binary ? (ios::out | ios::binary) : ios::out); // Открываем файл в нужном режиме.
    if (!out) throw runtime_error("Не удалось открыть файл для записи: " + filename); // Проверяем открытие файла.
    int32_t count = static_cast<int32_t>(departments.size()); // Определяем количество структур.
    if (binary) { // Проверяем двоичный режим.
        out.write(reinterpret_cast<const char*>(&count), sizeof(count)); // Записываем размер вектора четырьмя байтами.
    } else { // Переходим к текстовому режиму.
        out << count << '\n'; // Записываем число структур текстом.
    } // Завершаем выбор формата заголовка файла.
    for (const Department& item : departments) { // Перебираем все элементы вектора.
        writeDepartment(out, item, binary); // Сохраняем каждую структуру через отдельную функцию.
    } // Завершаем цикл записи структур.
    out.close(); // Закрываем файл и записываем оставшиеся данные из буфера.
    if (!out) throw runtime_error("Ошибка при записи файла: " + filename); // Проверяем результат записи.
} // Завершаем функцию сохранения вектора.

vector<Department> loadDepartments(const string& filename, bool binary) { // Загружаем вектор из файла.
    ifstream in(filename, binary ? (ios::in | ios::binary) : ios::in); // Открываем файл для чтения.
    if (!in) throw runtime_error("Не удалось открыть файл для чтения: " + filename); // Проверяем открытие файла.
    int32_t count = 0; // Готовим переменную для количества структур.
    if (binary) { // Проверяем двоичный режим.
        if (!in.read(reinterpret_cast<char*>(&count), sizeof(count))) { // Пробуем прочитать число структур.
            throw runtime_error("Ошибка чтения заголовка двоичного файла."); // Сообщаем о повреждённом заголовке.
        } // Завершаем проверку двоичного заголовка.
    } else { // Переходим к текстовому заголовку.
        long long textCount = -1; // Готовим число большего диапазона для проверки.
        if (!(in >> textCount) || textCount < 0 || textCount > MAX_DEPARTMENTS) { // Проверяем заголовок.
            throw runtime_error("Ошибка чтения заголовка текстового файла."); // Сообщаем об ошибке заголовка.
        } // Завершаем проверку текстового заголовка.
        count = static_cast<int32_t>(textCount); // Приводим проверенное число к нужному типу.
        in.ignore(numeric_limits<streamsize>::max(), '\n'); // Переходим к следующей строке.
    } // Завершаем выбор формата заголовка.
    if (count < 0 || count > MAX_DEPARTMENTS) { // Проверяем количество элементов из файла.
        throw runtime_error("Недопустимое количество подразделений в файле."); // Отказываемся от неверного размера.
    } // Завершаем проверку количества элементов.
    vector<Department> departments; // Создаём вектор для прочитанных объектов.
    departments.reserve(count); // Резервируем память под нужное количество объектов.
    for (int32_t i = 0; i < count; ++i) { // Последовательно читаем указанное число записей.
        Department item; // Создаём структуру для одной записи.
        if (!readDepartment(in, item, binary)) { // Проверяем успешность чтения очередной структуры.
            throw runtime_error("Ошибка чтения подразделения №" + to_string(i + 1)); // Сообщаем номер повреждённой записи.
        } // Завершаем проверку структуры.
        departments.push_back(item); // Добавляем прочитанное подразделение в вектор.
    } // Завершаем цикл чтения всех записей.
    in.close(); // Закрываем входной файл.
    return departments; // Возвращаем прочитанный вектор.
} // Завершаем функцию загрузки данных.

int main(int argc, char* argv[]) { // Запускаем программу с параметрами командной строки.
#ifdef _WIN32 // Проверяем, запущена ли программа в Windows.
    SetConsoleCP(CP_UTF8); // Настраиваем чтение русских символов в консоли Windows.
    SetConsoleOutputCP(CP_UTF8); // Настраиваем вывод русских символов в консоли Windows.
#endif // Завершаем условную настройку Windows.
    if (argc != 4) { // Проверяем наличие трёх пользовательских параметров.
        cerr << "Использование: " << argv[0] << " <write|read> <text|binary> <имя_файла>\n"; // Показываем синтаксис запуска.
        return 1; // Завершаем программу из-за неверных аргументов.
    } // Завершаем проверку количества параметров.
    string action = argv[1]; // Получаем режим работы: запись либо чтение.
    string format = argv[2]; // Получаем формат файла: текстовый либо двоичный.
    string filename = argv[3]; // Получаем имя файла из командной строки.
    if (action != "write" && action != "read") { // Проверяем команду записи или чтения.
        cerr << "Ошибка: первый параметр должен быть write или read.\n"; // Выводим подсказку по команде.
        return 1; // Завершаем программу с кодом ошибки.
    } // Завершаем проверку команды.
    if (format != "text" && format != "binary") { // Проверяем формат файла.
        cerr << "Ошибка: второй параметр должен быть text или binary.\n"; // Выводим подсказку по формату.
        return 1; // Завершаем программу с кодом ошибки.
    } // Завершаем проверку формата.
    bool binary = (format == "binary"); // Преобразуем название формата в логическую переменную.
    try { // Начинаем блок перехвата ошибок файлов и ввода.
        if (action == "write") { // Выбираем режим записи из консоли в файл.
            vector<Department> departments = inputDepartments(); // Считываем данные всех подразделений.
            saveDepartments(filename, departments, binary); // Сохраняем введённый вектор в файл.
            cout << "\nДанные успешно записаны в файл: " << filename << "\n"; // Сообщаем об успешной записи.
            printDepartments(departments); // Показываем введённые данные в первом запуске.
        } else { // Выбираем режим чтения из файла в консоль.
            vector<Department> departments = loadDepartments(filename, binary); // Загружаем структуры из файла.
            cout << "Данные успешно прочитаны из файла: " << filename << "\n"; // Сообщаем об успешном чтении.
            printDepartments(departments); // Показываем данные во втором запуске.
        } // Завершаем выбор действия.
    } catch (const exception& error) { // Перехватываем ошибки приложения.
        cerr << "Ошибка: " << error.what() << '\n'; // Выводим описание ошибки.
        return 1; // Возвращаем системе код неуспешного завершения.
    } // Завершаем обработку исключений.
    return 0; // Сообщаем об успешном завершении программы.
} // Завершаем главную функцию.
