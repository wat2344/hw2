#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class Node {
public:
    int data;
    Node* next;
    Node* prev;

    Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

class LinkedList {
protected:
    Node* head;
    Node* tail;
    int count;
    int type;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}
    virtual ~LinkedList() {
        clear();
    }

    virtual void add(int value) = 0;
    virtual void print() = 0;
    virtual vector<int> find(int value) = 0;
    virtual bool removeFirst(int value) = 0;
    virtual bool removeAll(int value) = 0;
    virtual void clear() = 0;
    virtual void writeToFile(const string& filename) = 0;
    virtual void readFromFile(const string& filename) = 0;

    int getType() const { return type; }
    int getCount() const { return count; }
};

class SinglyLinkedList : public LinkedList {
public:
    SinglyLinkedList() { type = 1; }

    void add(int value) override {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        count++;
    }

    void print() override {
        Node* current = head;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    vector<int> find(int value) override {
        vector<int> positions;
        Node* current = head;
        int pos = 1;
        while (current) {
            if (current->data == value) {
                positions.push_back(pos);
            }
            current = current->next;
            pos++;
        }
        return positions;
    }

    bool removeFirst(int value) override {
        if (!head) return false;

        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            count--;
            return true;
        }

        Node* current = head;
        while (current->next && current->next->data != value) {
            current = current->next;
        }

        if (current->next) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            count--;
            return true;
        }
        return false;
    }

    bool removeAll(int value) override {
        bool removed = false;
        while (removeFirst(value)) {
            removed = true;
        }
        return removed;
    }

    void clear() override {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    void writeToFile(const string& filename) override {
        ofstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }
        file << "> ";
        Node* current = head;
        while (current) {
            file << current->data << " ";
            current = current->next;
        }
        file.close();
    }

    void readFromFile(const string& filename) override {
        if (count > 0) {
            cout << "Ошибка: список уже существует!" << endl;
            return;
        }

        ifstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }

        string typeMarker;
        file >> typeMarker;
        if (typeMarker != ">") {
            cout << "Неверный формат файла!" << endl;
            return;
        }

        int value;
        while (file >> value) {
            add(value);
        }
        file.close();
    }
};

class DoublyLinkedList : public LinkedList {
public:
    DoublyLinkedList() { type = 2; }

    void add(int value) override {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void print() override {
        Node* current = head;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    vector<int> find(int value) override {
        vector<int> positions;
        Node* current = head;
        int pos = 1;
        while (current) {
            if (current->data == value) {
                positions.push_back(pos);
            }
            current = current->next;
            pos++;
        }
        return positions;
    }

    bool removeFirst(int value) override {
        if (!head) return false;

        Node* current = head;
        while (current && current->data != value) {
            current = current->next;
        }

        if (!current) return false;

        if (current == head) {
            head = current->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;
        } else if (current == tail) {
            tail = current->prev;
            tail->next = nullptr;
        } else {
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }

        delete current;
        count--;
        return true;
    }

    bool removeAll(int value) override {
        bool removed = false;
        while (removeFirst(value)) {
            removed = true;
        }
        return removed;
    }

    void clear() override {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        count = 0;
    }

    void writeToFile(const string& filename) override {
        ofstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }
        file << "<> ";
        Node* current = head;
        while (current) {
            file << current->data << " ";
            current = current->next;
        }
        file.close();
    }

    void readFromFile(const string& filename) override {
        if (count > 0) {
            cout << "Ошибка: список уже существует!" << endl;
            return;
        }

        ifstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }

        string typeMarker;
        file >> typeMarker;
        if (typeMarker != "<>") {
            cout << "Неверный формат файла!" << endl;
            return;
        }

        int value;
        while (file >> value) {
            add(value);
        }
        file.close();
    }
};

class CircularSinglyLinkedList : public LinkedList {
public:
    CircularSinglyLinkedList() { type = 3; }

    void add(int value) override {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            head->next = head;
        } else {
            Node* current = head;
            while (current->next != head) {
                current = current->next;
            }
            current->next = newNode;
            newNode->next = head;
        }
        count++;
    }

    void print() override {
        if (!head) return;

        Node* current = head;
        int i = 0;
        while (i < count) {
            cout << current->data << " ";
            current = current->next;
            i++;
        }
        cout << endl;
    }

    vector<int> find(int value) override {
        vector<int> positions;
        if (!head) return positions;

        Node* current = head;
        int pos = 1;
        int i = 0;
        while (i < count) {
            if (current->data == value) {
                positions.push_back(pos);
            }
            current = current->next;
            pos++;
            i++;
        }
        return positions;
    }

    bool removeFirst(int value) override {
        if (!head) return false;

        Node* current = head;
        Node* prev = nullptr;
        int i = 0;

        while (i < count) {
            if (current->data == value) break;
            prev = current;
            current = current->next;
            i++;
        }

        if (i == count) return false;

        if (current == head) {
            if (count == 1) {
                delete head;
                head = nullptr;
            } else {
                Node* last = head;
                while (last->next != head) {
                    last = last->next;
                }
                head = head->next;
                last->next = head;
                delete current;
            }
        } else {
            prev->next = current->next;
            delete current;
        }

        count--;
        return true;
    }

    bool removeAll(int value) override {
        bool removed = false;
        while (removeFirst(value)) {
            removed = true;
        }
        return removed;
    }

    void clear() override {
        if (!head) return;

        Node* current = head->next;
        while (current != head) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
        head = nullptr;
        count = 0;
    }

    void writeToFile(const string& filename) override {
        ofstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }
        file << ">0 ";
        if (head) {
            Node* current = head;
            int i = 0;
            while (i < count) {
                file << current->data << " ";
                current = current->next;
                i++;
            }
        }
        file.close();
    }

    void readFromFile(const string& filename) override {
        if (count > 0) {
            cout << "Ошибка: список уже существует!" << endl;
            return;
        }

        ifstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }

        string typeMarker;
        file >> typeMarker;
        if (typeMarker != ">0") {
            cout << "Неверный формат файла!" << endl;
            return;
        }

        int value;
        while (file >> value) {
            add(value);
        }
        file.close();
    }
};

class CircularDoublyLinkedList : public LinkedList {
public:
    CircularDoublyLinkedList() { type = 4; }

    void add(int value) override {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            head->next = head;
            head->prev = head;
        } else {
            Node* last = head->prev;
            last->next = newNode;
            newNode->prev = last;
            newNode->next = head;
            head->prev = newNode;
        }
        count++;
    }

    void print() override {
        if (!head) return;

        Node* current = head;
        int i = 0;
        while (i < count) {
            cout << current->data << " ";
            current = current->next;
            i++;
        }
        cout << endl;
    }

    vector<int> find(int value) override {
        vector<int> positions;
        if (!head) return positions;

        Node* current = head;
        int pos = 1;
        int i = 0;
        while (i < count) {
            if (current->data == value) {
                positions.push_back(pos);
            }
            current = current->next;
            pos++;
            i++;
        }
        return positions;
    }

    bool removeFirst(int value) override {
        if (!head) return false;

        Node* current = head;
        int i = 0;

        while (i < count) {
            if (current->data == value) break;
            current = current->next;
            i++;
        }

        if (i == count) return false;

        if (count == 1) {
            delete head;
            head = nullptr;
        } else {
            Node* prevNode = current->prev;
            Node* nextNode = current->next;

            prevNode->next = nextNode;
            nextNode->prev = prevNode;

            if (current == head) {
                head = nextNode;
            }

            delete current;
        }

        count--;
        return true;
    }

    bool removeAll(int value) override {
        bool removed = false;
        while (removeFirst(value)) {
            removed = true;
        }
        return removed;
    }

    void clear() override {
        if (!head) return;

        Node* current = head->next;
        while (current != head) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
        head = nullptr;
        count = 0;
    }

    void writeToFile(const string& filename) override {
        ofstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }
        file << "<>0 ";
        if (head) {
            Node* current = head;
            int i = 0;
            while (i < count) {
                file << current->data << " ";
                current = current->next;
                i++;
            }
        }
        file.close();
    }

    void readFromFile(const string& filename) override {
        if (count > 0) {
            cout << "Ошибка: список уже существует!" << endl;
            return;
        }

        ifstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }

        string typeMarker;
        file >> typeMarker;
        if (typeMarker != "<>0") {
            cout << "Неверный формат файла!" << endl;
            return;
        }

        int value;
        while (file >> value) {
            add(value);
        }
        file.close();
    }
};

LinkedList* createList(int type) {
    switch (type) {
        case 1: return new SinglyLinkedList();
        case 2: return new DoublyLinkedList();
        case 3: return new CircularSinglyLinkedList();
        case 4: return new CircularDoublyLinkedList();
        default: return nullptr;
    }
}

bool parseArguments(int argc, char* argv[], int& type, int& count, vector<int>& elements) {
    type = 0;
    count = 0;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-t" && i + 1 < argc) {
            type = atoi(argv[++i]);
            if (type < 1 || type > 4) {
                cout << "Неверный тип списка!" << endl;
                return false;
            }
        } else if (arg == "-c" && i + 1 < argc) {
            count = atoi(argv[++i]);
            if (count < 0) {
                cout << "Неверное количество элементов!" << endl;
                return false;
            }
        } else if (arg == "-e" && i + 1 < argc) {
            string elementsStr = argv[++i];
            stringstream ss(elementsStr);
            string token;

            while (getline(ss, token, ',')) {
                elements.push_back(stoi(token));
            }
        } else {
            cout << "Неизвестный параметр: " << arg << endl;
            return false;
        }
    }

    if (type == 0) {
        cout << "Не указан тип списка!" << endl;
        return false;
    }

    if (count != elements.size()) {
        cout << "Количество элементов не соответствует указанному значению!" << endl;
        return false;
    }

    return true;
}

void showMenu() {
    cout << "\n========== МЕНЮ ==========" << endl;
    cout << "1. Добавить элемент" << endl;
    cout << "2. Вывести список" << endl;
    cout << "3. Найти элемент" << endl;
    cout << "4. Удалить элемент (первое вхождение)" << endl;
    cout << "5. Удалить элемент (все вхождения)" << endl;
    cout << "6. Удалить весь список" << endl;
    cout << "7. Записать список в файл" << endl;
    cout << "8. Считать список из файла" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

int main(int argc, char* argv[]) {
    LinkedList* list = nullptr;

    if (argc > 1) {
        int type, count;
        vector<int> elements;

        if (parseArguments(argc, argv, type, count, elements)) {
            list = createList(type);

            if (list) {
                for (int val : elements) {
                    list->add(val);
                }
                cout << "Список создан успешно. Тип: " << type
                     << ", элементов: " << list->getCount() << endl;
            }
        } else {
            return 1;
        }
    }

    if (!list) {
        cout << "Список не создан через аргументы. Создайте список вручную." << endl;
        cout << "Выберите тип списка (1-4): ";
        int type;
        cin >> type;

        list = createList(type);
        if (!list) {
            cout << "Неверный тип списка!" << endl;
            return 1;
        }
    }

    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                int value;
                cout << "Введите значение: ";
                cin >> value;
                list->add(value);
                cout << "Элемент добавлен." << endl;
                break;
            }
            case 2: {
                cout << "Содержимое списка: ";
                list->print();
                break;
            }
            case 3: {
                int value;
                cout << "Введите значение для поиска: ";
                cin >> value;
                vector<int> positions = list->find(value);
                if (positions.empty()) {
                    cout << "Элемент не найден." << endl;
                } else {
                    cout << "Элемент найден на позициях: ";
                    for (int pos : positions) {
                        cout << pos << " ";
                    }
                    cout << endl;
                }
                break;
            }
            case 4: {
                int value;
                cout << "Введите значение для удаления: ";
                cin >> value;
                if (list->removeFirst(value)) {
                    cout << "Первое вхождение элемента удалено." << endl;
                } else {
                    cout << "Элемент не найден." << endl;
                }
                break;
            }
            case 5: {
                int value;
                cout << "Введите значение для удаления: ";
                cin >> value;
                if (list->removeAll(value)) {
                    cout << "Все вхождения элемента удалены." << endl;
                } else {
                    cout << "Элемент не найден." << endl;
                }
                break;
            }
            case 6: {
                list->clear();
                cout << "Список очищен." << endl;
                break;
            }
            case 7: {
                string filename;
                cout << "Введите имя файла: ";
                cin >> filename;
                list->writeToFile(filename);
                cout << "Список записан в файл." << endl;
                break;
            }
            case 8: {
                string filename;
                cout << "Введите имя файла: ";
                cin >> filename;
                list->readFromFile(filename);
                break;
            }
            case 0: {
                cout << "Выход из программы." << endl;
                break;
            }
            default: {
                cout << "Неверный выбор!" << endl;
                break;
            }
        }
    } while (choice != 0);

    delete list;
    return 0;
}