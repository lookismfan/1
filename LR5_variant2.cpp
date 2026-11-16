#include <iostream> // Подключаем стандартные потоки ввода и вывода.
#include <fstream> // Подключаем файловые потоки.
#include <sstream> // Подключаем чтение отдельной строки как потока.
#include <string> // Подключаем класс строки.
#include <iomanip> // Подключаем quoted, fixed и setprecision.
#include <stdexcept> // Подключаем стандартные исключения.
#include <cmath> // Подключаем проверку конечности числа isfinite.
#include <utility> // Подключаем функцию перемещения move.
#include <tuple> // Подключаем tie для сравнения нескольких полей.
#include <functional> // Подключаем стандартные хеш-функции.
class Deposit { // Описываем банковский вклад как самостоятельный объект.
private: // Запрещаем прямое изменение полей извне.
    std::string name_; // Храним название вклада.
    double amount_ = 0.0; // Храним неотрицательную сумму вклада.
    std::string currency_ = "RUB"; // Храним обозначение валюты.
    double rate_ = 0.0; // Храним годовую процентную ставку.
public: // Открываем доступ к операциям класса.
    Deposit() = default; // Создаём корректный пустой объект для последующего присваивания.
    Deposit(std::string name, double amount, std::string currency, double rate) // Объявляем конструктор с параметрами.
        : name_(std::move(name)), amount_(amount), // Переносим название и сохраняем сумму.
          currency_(std::move(currency)), rate_(rate) { // Переносим валюту и сохраняем ставку.
        if (name_.empty() || currency_.empty()) { // Проверяем обязательные строковые поля.
            throw std::invalid_argument("Пустое название или валюта"); // Сообщаем о некорректных данных.
        } // Завершаем проверку строк.
        if (!std::isfinite(amount_) || !std::isfinite(rate_) || amount_ < 0 || rate_ < 0) { // Проверяем числовые поля.
            throw std::invalid_argument("Сумма и ставка должны быть конечными и неотрицательными"); // Отклоняем неверные числа.
        } // Завершаем проверку чисел.
    } // Завершаем конструктор с параметрами.
    Deposit(const Deposit&) = default; // Копируем поля стандартным способом.
    Deposit(Deposit&&) noexcept = default; // Перемещаем строки и переносим числовые поля.
    Deposit& operator=(const Deposit&) = default; // Выполняем копирующее присваивание по полям.
    Deposit& operator=(Deposit&&) noexcept = default; // Выполняем перемещающее присваивание по полям.
    ~Deposit() = default; // Освобождаем ресурсы строк автоматически.
    bool operator<(const Deposit& other) const { // Задаём порядок объектов по сумме вклада.
        return std::tie(amount_, name_, currency_, rate_) // При равной сумме учитываем остальные поля.
             < std::tie(other.amount_, other.name_, other.currency_, other.rate_); // Сравниваем кортежи лексикографически.
    } // Завершаем оператор сравнения.
    bool operator==(const Deposit& other) const { // Проверяем полное совпадение вкладов.
        return std::tie(amount_, name_, currency_, rate_) // Собираем поля текущего объекта.
            == std::tie(other.amount_, other.name_, other.currency_, other.rate_); // Сравниваем все четыре поля.
    } // Завершаем оператор равенства.
    friend std::ostream& operator<<(std::ostream& out, const Deposit& value) { // Объявляем вывод объекта в любой поток.
        const auto flags = out.flags(); // Сохраняем исходные флаги потока.
        const auto precision = out.precision(); // Сохраняем исходную точность вывода.
        out << std::quoted(value.name_) << ' ' << std::fixed << std::setprecision(2) // Выводим название и задаём два знака после точки.
            << value.amount_ << ' ' << std::quoted(value.currency_) << ' ' << value.rate_; // Выводим сумму, валюту и ставку.
        out.flags(flags); // Восстанавливаем исходные флаги потока.
        out.precision(precision); // Восстанавливаем исходную точность.
        return out; // Возвращаем поток для цепочки операций вывода.
    } // Завершаем оператор вывода.
    friend struct DepositHash; // Разрешаем хеш-функтору читать закрытые поля.
}; // Завершаем описание класса.
struct DepositHash { // Описываем хеш-функтор для неупорядоченного множества.
    std::size_t operator()(const Deposit& value) const { // Возвращаем хеш банковского вклада.
        std::size_t result = std::hash<std::string>{}(value.name_); // Вычисляем хеш названия.
        const auto combine = [&result](std::size_t part) { // Создаём функцию объединения хешей.
            result ^= part + 0x9e3779b9u + (result << 6) + (result >> 2); // Добавляем очередной хеш к результату.
        }; // Завершаем функцию объединения.
        combine(std::hash<double>{}(value.amount_)); // Учитываем сумму вклада.
        combine(std::hash<std::string>{}(value.currency_)); // Учитываем обозначение валюты.
        combine(std::hash<double>{}(value.rate_)); // Учитываем годовую ставку.
        return result; // Возвращаем итоговый хеш.
    } // Завершаем вызов функтора.
}; // Завершаем описание хеш-функтора.
Deposit parseDeposit(const std::string& line) { // Преобразуем одну строку файла в объект.
    std::istringstream input(line); // Создаём поток для разбора строки.
    std::string name, currency; // Подготавливаем строковые поля.
    double amount = 0.0, rate = 0.0; // Подготавливаем числовые поля.
    if (!(input >> std::quoted(name) >> amount >> std::quoted(currency) >> rate)) { // Читаем четыре поля записи.
        throw std::runtime_error("Ожидаются название, сумма, валюта и ставка"); // Отклоняем неполную или неверную запись.
    } // Завершаем проверку чтения.
    input >> std::ws; // Пропускаем пробелы в конце записи.
    if (!input.eof()) { // Проверяем отсутствие лишних данных.
        throw std::runtime_error("Лишние данные после ставки"); // Отклоняем запись с дополнительными полями.
    } // Завершаем проверку конца строки.
    return Deposit(name, amount, currency, rate); // Создаём проверенный объект вклада.
} // Завершаем разбор строки.
template <typename Consumer> // Принимаем действие, выполняемое над каждым вкладом.
void readDeposits(Consumer consume) { // Читаем исходные данные из input.txt.
    std::ifstream input("input.txt"); // Открываем входной файл в рабочей папке.
    if (!input) { // Проверяем успешность открытия.
        throw std::runtime_error("Не удалось открыть input.txt"); // Сообщаем об отсутствии доступа к файлу.
    } // Завершаем проверку открытия.
    std::string line; // Создаём буфер строки.
    std::size_t number = 0; // Считаем строки для сообщений об ошибках.
    while (std::getline(input, line)) { // Последовательно читаем строки до конца файла.
        ++number; // Увеличиваем номер текущей строки.
        if (line.find_first_not_of(" \t\r") == std::string::npos) { // Определяем пустую строку.
            continue; // Пропускаем строку без данных.
        } // Завершаем проверку пустой строки.
        try { // Дополняем возможную ошибку номером строки.
            consume(parseDeposit(line)); // Передаём прочитанный вклад вызывающей программе.
        } catch (const std::exception& error) { // Перехватываем ошибку обработки записи.
            throw std::runtime_error("Строка " + std::to_string(number) + ": " + error.what()); // Уточняем место ошибки.
        } // Завершаем обработку ошибки записи.
    } // Завершаем чтение строк.
    if (input.bad()) { // Проверяем аппаратную ошибку чтения.
        throw std::runtime_error("Ошибка чтения input.txt"); // Сообщаем о сбое чтения.
    } // Завершаем проверку потока.
} // Завершаем чтение файла.
template <typename Container> // Разрешаем вывод разных контейнеров одним шаблоном.
void printContainer(std::ostream& out, const std::string& title, const Container& data) { // Выводим заголовок и содержимое.
    out << title << " (" << data.size() << ")\n"; // Показываем название и число элементов.
    for (const Deposit& value : data) { // Перебираем объекты без лишнего копирования.
        out << value << '\n'; // Используем перегруженный оператор вставки в поток.
    } // Завершаем перебор элементов.
    out << '\n'; // Отделяем блок вывода пустой строкой.
} // Завершаем вывод контейнера.
void demonstrateSpecialMembers(std::ostream& out) { // Демонстрируем специальные функции класса.
    Deposit original("Демонстрационный", 1000.0, "RUB", 5.0); // Вызываем конструктор с параметрами.
    Deposit copied(original); // Вызываем конструктор копирования.
    Deposit moved(std::move(copied)); // Вызываем конструктор перемещения.
    Deposit assigned; // Вызываем конструктор по умолчанию.
    assigned = original; // Вызываем копирующее присваивание.
    Deposit moveAssigned; // Создаём ещё один объект по умолчанию.
    moveAssigned = std::move(assigned); // Вызываем перемещающее присваивание.
    out << "Проверка копирования и перемещения: " // Начинаем строку проверки.
        << ((moved == original && moveAssigned == original) ? "OK" : "FAIL") << '\n'; // Проверяем сохранение значений.
    out << "Оператор вывода: " << moved << "\n\n"; // Демонстрируем вывод объекта.
} // Завершаем демонстрацию специальных функций.
#include <set> // Подключаем упорядоченное множество.
#include <unordered_set> // Подключаем неупорядоченное множество.
int main() { // Определяем точку входа программы ЛР5.
    try { // Обрабатываем ошибки работы программы.
        std::set<Deposit> ordered; // Создаём множество с порядком, заданным operator<.
        std::unordered_set<Deposit, DepositHash> unordered; // Создаём множество с собственным хеш-функтором.
        std::size_t records = 0; // Считаем все записи входного файла, включая дубликаты.
        readDeposits([&](const Deposit& value) { // Задаём действия над каждым прочитанным вкладом.
            ordered.insert(value); // Добавляем объект в упорядоченное множество.
            unordered.insert(value); // Добавляем тот же объект в хеш-множество.
            ++records; // Увеличиваем число обработанных записей.
        }); // Завершаем загрузку обоих множеств.
        std::ofstream output("output.txt"); // Создаём файл для сохранения контрольного вывода.
        if (!output) { // Проверяем доступность выходного файла.
            throw std::runtime_error("Не удалось создать output.txt"); // Сообщаем об ошибке открытия.
        } // Завершаем проверку файла.
        const auto report = [&](std::ostream& out) { // Определяем единый отчёт для экрана и файла.
            demonstrateSpecialMembers(out); // Показываем работу специальных функций класса.
            out << "Прочитано записей: " << records << '\n'; // Выводим количество входных записей.
            out << "Повторных записей: " << records - ordered.size() << "\n\n"; // Выводим количество полных дубликатов.
            printContainer(out, "Set по возрастанию суммы", ordered); // Печатаем отсортированные уникальные объекты.
            printContainer(out, "Unordered_set без гарантии порядка", unordered); // Печатаем объекты хеш-множества.
        }; // Завершаем определение контрольного отчёта.
        report(std::cout); // Выводим объекты на экран по условию задания.
        report(output); // Дополнительно сохраняем те же результаты в файл.
        output.flush(); // Завершаем буферизованную запись.
        if (!output) { // Проверяем отсутствие ошибки записи.
            throw std::runtime_error("Ошибка записи output.txt"); // Сообщаем о сбое записи.
        } // Завершаем проверку выходного потока.
        return 0; // Возвращаем код успешного выполнения.
    } catch (const std::exception& error) { // Перехватываем стандартные исключения.
        std::cerr << "Ошибка: " << error.what() << '\n'; // Печатаем причину ошибки.
        return 1; // Возвращаем код ошибки.
    } // Завершаем обработку исключений.
} // Завершаем программу ЛР5.
