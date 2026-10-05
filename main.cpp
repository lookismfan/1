/*#include <iostream>
#include <regex>
#include <cmath>
// // TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
float t;
int main() {
     using namespace std;
     for (int i = 1; i < 11; i++) {
         cout << pow(i,(-4/3)) << endl;
         t += pow(i,(-4/3));
     }
     cout << "t: " << t << endl;
     return 0;
     }
*/
/*#include <iostream>
#include <cmath>
using namespace std;
int main() {
    for (int n = 2; n < 7; n += 2) {
        double e = 2.0;
        double s = pow(10.0,-n);
        double t = 1.0;
        int p = 1;
        while (t >= s) {
            p++;
            t /= p;
            e += t;
        }
        cout << "10 ^ -" << n << "e = " << e << endl;
        cout << "p = " << p << endl;
    }
    return 0;
}
*/
#include <iostream>
#include <cmath>
/*using namespace std;
int main() {
    struct Test
    {
        short b; // 2 байта
        char ch1, ch2; // 1 + 1 байт
        int a; // 4 байта
    };
    cout<<sizeof(Test)<<endl;
    return 0;
}
*/
/*
#include <iostream>
#include <cmath>
void student() {
    string stud;
    cin >> stud >> end;
}
using namespace std;
int main() {

    return 0;
}
*/
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <limits>
using namespace std;
// Структура для хранения информации о дисциплине
struct Discipline {
    string name;
    int grade;
};
// Структура для хранения информации о студенте
struct Student {
    string full_name;
    vector<Discipline> subjects;
};
// Ввод целого числа с проверкой
int input_number() {
    int number;
    while (!(cin >> number)) {
        cout << "Ошибка! Нужно ввести число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}
// Ввод данных одной дисциплины
void input_subject(Discipline& subject) {
    cout << "Укажите название предмета: ";
    getline(cin, subject.name);
    cout << "Укажите полученную оценку: ";
    subject.grade = input_number();

    while (subject.grade < 2 || subject.grade > 5) {
        cout << "Оценка должна быть от 2 до 5. Повторите ввод: ";
        subject.grade = input_number();
    }
}
// Ввод данных одного студента
void input_student(Student& stud) {
    int subject_count;
    cout << "Укажите ФИО: ";
    getline(cin, stud.full_name);
    cout << "Сколько дисциплин сдавал студент? ";
    subject_count = input_number();
    while (subject_count < 4) {
        cout << "Количество дисциплин должно быть не меньше 4. Повторите ввод: ";
        subject_count = input_number();
    }
    stud.subjects.resize(subject_count);
    for (int i = 0; i < subject_count; i++) {
        cout << "\nВвод данных по предмету №" << i + 1 << endl;
        input_subject(stud.subjects[i]);
    }
}
// Вывод одной дисциплины
void print_subject(const Discipline& subject) {
    cout << subject.name << ": " << subject.grade << endl;
}
// Вывод информации о студенте
void print_student(const Student& stud) {
    cout << "\nСтудент: " << stud.full_name << endl;
    cout << "Оценки за экзамены:" << endl;

    for (int i = 0; i < stud.subjects.size(); i++) {
        cout << i + 1 << ") ";
        print_subject(stud.subjects[i]);
    }
}
// Проверка: все ли оценки равны 5
bool all_grades_are_five(const Student& stud) {
    for (int i = 0; i < stud.subjects.size(); i++) {
        if (stud.subjects[i].grade != 5) {
            return false;
        }
    }
    return true;
}
int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    int student_count;
    cout << "Введите число студентов в группе: ";
    student_count = input_number();
    while (student_count <= 0) {
        cout << "Количество студентов должно быть больше 0. Повторите ввод: ";
        student_count = input_number();
    }
    vector<Student> group(student_count);
    // Ввод данных
    for (int i = 0; i < student_count; i++) {
        cout << "Данные студента №" << i + 1 << endl;
        input_student(group[i]);
    }
    // Вывод всех введенных данных
    cout << "\n\n СПИСОК СТУДЕНТОВ " << endl;
    for (int i = 0; i < student_count; i++) {
        cout << "\nСтудент №" << i + 1;
        print_student(group[i]);
    }
    // Подсчет студентов со всеми пятерками
    int excellent_count = 0;
    for (int i = 0; i < student_count; i++) {
        if (all_grades_are_five(group[i])) {
            excellent_count++;
        }
    }
    cout << "Число студентов, получивших оценку 5 по всем предметам: "<< excellent_count << endl;
    return 0;
}