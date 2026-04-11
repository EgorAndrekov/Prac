#include "IterableObject.h"
#include "String.h"
#include "List.h"
#include "StringList.h"
#include <iostream>

using namespace std;

const int MAX_OBJECTS = 100;
IterableObject* objects[MAX_OBJECTS];
int objCount = 0;

void addObject(IterableObject* obj) {
    if (objCount < MAX_OBJECTS) {
        objects[objCount++] = obj;
    } else {
        cout << "Нельзя добавить больше объектов." << endl;
        delete obj;
    }
}

void removeObject(int index) {
    if (index < 0 || index >= objCount) {
        cout << "Неверный индекс." << endl;
        return;
    }
    delete objects[index];
    for (int i = index; i < objCount - 1; ++i) {
        objects[i] = objects[i + 1];
    }
    --objCount;
}

void listObjects() {
    if (objCount == 0) {
        cout << "Нет объектов." << endl;
        return;
    }
    for (int i = 0; i < objCount; ++i) {
        cout << i << ": ";
        if (dynamic_cast<String*>(objects[i]) && !dynamic_cast<StringList*>(objects[i]))
            cout << "String";
        else if (dynamic_cast<List<int>*>(objects[i]))
            cout << "List<int>";
        else if (dynamic_cast<List<double>*>(objects[i]))
            cout << "List<double>";
        else if (dynamic_cast<List<string>*>(objects[i]))
            cout << "List<string>";
        else if (dynamic_cast<StringList*>(objects[i]))
            cout << "StringList";
        else
            cout << "Unknown";
        cout << " (length=" << objects[i]->length() << ")" << endl;
    }
}

void testContains() {
    cout << "\n=== ТЕСТ contains через указатель на IterableObject ===" << endl;
    for (int i = 0; i < objCount; ++i) {
        cout << "Объект " << i << ": ";
        bool res1 = objects[i]->contains("a");
        bool res2 = objects[i]->contains("123");
        cout << "contains(\"a\")=" << (res1 ? "true" : "false")
             << ", contains(\"123\")=" << (res2 ? "true" : "false") << endl;
    }
}

void showMenu() {
    cout << "\nМеню:\n"
         << "1. Создать String\n"
         << "2. Создать List<int>\n"
         << "3. Создать List<double>\n"
         << "4. Создать List<string>\n"
         << "5. Создать StringList\n"
         << "6. Показать все объекты\n"
         << "7. Вызвать contains на объекте\n"
         << "8. Запустить тест (contains для всех)\n"
         << "9. Удалить объект\n"
         << "10. Выход\n"
         << "Выберите: ";
}

int main() {
    int choice;
    while (true) {
        showMenu();
        cin >> choice;
        if (choice == 10) break;

        switch (choice) {
            case 1: {
                string val;
                cout << "Введите строку: ";
                cin >> val;
                addObject(new String(val));
                break;
            }
            case 2: {
                cout << "Создаётся пустой List<int>. Добавьте элементы (0 - закончить):\n";
                List<int>* lst = new List<int>();
                int x;
                while (cin >> x && x != 0) {
                    lst->add(x);
                }
                cin.clear();
                addObject(lst);
                break;
            }
            case 3: {
                cout << "Создаётся пустой List<double>. Добавьте элементы (0 - закончить):\n";
                List<double>* lst = new List<double>();
                double x;
                while (cin >> x && x != 0) {
                    lst->add(x);
                }
                cin.clear();
                addObject(lst);
                break;
            }
            case 4: {
                cout << "Создаётся пустой List<string>. Добавьте элементы (END - закончить):\n";
                List<string>* lst = new List<string>();
                string x;
                while (cin >> x && x != "END") {
                    lst->add(x);
                }
                cin.clear();
                addObject(lst);
                break;
            }
            case 5: {
                string str;
                cout << "Введите строку для StringList: ";
                cin >> str;
                StringList* sl = new StringList(str);
                cout << "Добавьте строки в список (END - закончить):\n";
                string elem;
                while (cin >> elem && elem != "END") {
                    sl->addString(String(elem));
                }
                cin.clear();
                addObject(sl);
                break;
            }
            case 6:
                listObjects();
                break;
            case 7: {
                int idx;
                cout << "Введите индекс объекта: ";
                cin >> idx;
                if (idx < 0 || idx >= objCount) {
                    cout << "Неверный индекс." << endl;
                    break;
                }
                string key;
                cout << "Введите ключ для поиска: ";
                cin >> key;
                bool res = objects[idx]->contains(key);
                cout << "Результат: " << (res ? "true" : "false") << endl;
                break;
            }
            case 8:
                testContains();
                break;
            case 9: {
                int idx;
                cout << "Введите индекс объекта для удаления: ";
                cin >> idx;
                removeObject(idx);
                break;
            }
            default:
                cout << "Неверный выбор." << endl;
        }
    }
    for (int i = 0; i < objCount; ++i) {
        delete objects[i];
    }
    return 0;
}