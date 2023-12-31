#include <iostream>
#include <random>

using namespace std;

template <typename T>
class DLinkedList {
private:
    struct Node {
        T data;
        int index;
        Node* next;
        Node* prev;

        Node(const T& value, const int& index) : data(value), index(index), next(nullptr), prev(nullptr) {}
    };
    Node* _head;
    Node* _tail;

public:
    DLinkedList() : _head(nullptr), _tail(nullptr) {}

    DLinkedList(const DLinkedList& other) : _head(nullptr), _tail(nullptr) {
        Node* current = other._head;
        push_tail(current->data, current->index);
        current = current->next;
        while (current != other._tail->next) {
            push_tail(current->data, current->index);
            current = current->next;
        }
    }
    T generateRandom(random_device& randomDevice) {
        mt19937_64 generator(randomDevice());
        if constexpr (numeric_limits<T>::is_integer)
        {
            uniform_int_distribution<T> dice(-10, 10);
            return dice(generator);
        }
        else
        {
            uniform_real_distribution<T> dice(-10.0, 10.0);
            return dice(generator);
        }
    }
    DLinkedList(int size, const T& min, const T& max) : _head(nullptr), _tail(nullptr) {
        random_device randomDevice;
        
        for (int i = 0; i < size; ++i) {
            T value = generateRandom(randomDevice);
            push_tail(value,i);
        }
    }

    ~DLinkedList() {
        while (_head != _tail) {
            auto ptr = _head;
            _head = _head->next;
            delete ptr;
        }
        _head = _tail = nullptr;
    }

    DLinkedList& operator=(const DLinkedList& other) {
        if (this != &other) {
            DLinkedList temp(other);
            swap(temp._head, _head);
            swap(temp._tail, _tail);
        }
        return *this;
    }

    void push_tail(const T& value, const int& index) {
        Node* new_node = new Node(value,index);
        if (!_head) {
            _head = new_node;
            _head->next = _head;
            _head->prev = _head;
            _tail = _head;
        }
        else {
            _tail->next = new_node;
            new_node->prev = _tail;
            new_node->next = _head;
            _head->prev = new_node;
            _tail = new_node;
        }
    }

    void push_tail(const DLinkedList& other) {
        if (!other._head) {
            throw length_error("List is empty");
        }
        Node* current = other._head;
        while (current != other._tail) {
            push_tail(current->data, current->index);
            current = current->next;
        }
        push_tail(other._tail->data, other._tail->index);
    }

    void push_head(const T& value, const int& index) {
        Node* new_node = new Node(value,index);
        if (!_head) {
            _head = new_node;
            _tail = new_node;
        }
        else {
            new_node->next = _head;
            new_node->prev = _tail;
            _tail->next = new_node;
            _head->prev = new_node;
            _head = new_node;
        }
    }

    void push_head(const DLinkedList& other) {
        if (!other._head) {
            throw length_error("List is empty");
        }
        Node* current = other._tail;
        while (current != other._head) {
            push_head(current->data, current->index);
            current = current->prev;
        }
        push_head(other._head->data, other._head->index);
    }

    void pop_head() {
        if (_head) {
            Node* temp = _head;

            if (_head == _tail) {
                _head = nullptr;
                _tail = nullptr;
            }
            else {
                _head = _head->next;
                _head->prev = _tail;
                _tail->next = _head;
            }

            delete temp;
        }
    }

    void pop_tail() {
        if (_tail) {
            Node* temp = _tail;

            if (_head == _tail) {
                _head = nullptr;
                _tail = nullptr;
            }
            else {
                _tail = _tail->prev;
                _tail->next = _head;
                _head->prev = _tail;
            }

            delete temp;
        }
    }

    void delete_node(const T& value) {
        Node* current = _head;
        Node* start_node = _head;
        do {
            if (current->data == value) {
                if (current == start_node) {
                    start_node = current->next;
                }
                if (current == _head) {
                    _head = current->next;
                }
                if (current == _tail) {
                    _tail = current->prev;
                }
                current->prev->next = current->next;
                current->next->prev = current->prev;
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            else {
                current = current->next;
            }
        } while (current != start_node);
    }

    T operator[](int index) const {
        Node* tmp = _head;
        int n = 0;
        while (n != index) {
            tmp = tmp->next;
            n++;
        }
        return tmp->data;
    }

    T& operator[](int index) {
        Node* tmp = _head;
        int n = 0;
        while (n != index) {
            tmp = tmp->next;
            n++;
        }
        return tmp->data;
    }
    size_t get_size() {
        if (!_head) {
            throw length_error("List is empty");
        }
        size_t size = 1;
        _head = _head->next;
        while (_head != _tail) {
            ++size;
            _head = _head->next;
        }
        return size;
    }
    int get_index(int index) {
        if (!_head) {
            throw length_error("List is empty");
        }
        Node* tmp = _head;
        for (int i = 0; i < index; ++i, tmp = tmp->next);
        return tmp->index;
    }
    friend ostream& operator<<(ostream& os, const DLinkedList<T>& list) {
        Node* current = list._head;
        if (!current) {
            os << "Empty List";
        }
        else {
            do {
                os << current->data << " ";
                current = current->next;
            } while (current != list._tail->next);
        }
        return os;
    }
};
template<typename T>
T& polynomial(DLinkedList<T>& List, T x) {
    T sum = 0;
    List.delete_node(sum);
    for (int i = 0; i < List.get_size(); ++i) {
        sum += List[i] * pow(x, List.get_index(i));
    }
    return sum;
}

int main() {
    DLinkedList<int> list1;
    list1.push_tail(1,6);
    list1.push_tail(2,5);
    list1.push_tail(2,4);
    list1.push_tail(8,3);
    list1.push_tail(2,2);
    list1.push_head(4,1);
    list1.push_head(5,0);
    DLinkedList<int> list2 = list1;
    DLinkedList<int> list3(list1);
    list3 = list2;
    DLinkedList<int> list4(5, 1, 10);
    cout << "First list: " << list1 << endl;
    cout << "Assignment: " << list2 << endl;
    cout << "Copying: " << list3 << endl;
    cout << "Random: " << list4 << endl << endl;
    cout << "Remove from the head: " << endl;
    list4.pop_head();
    cout << "First: " << list4 << endl;
    list4.pop_head();
    cout << "Second: " << list4 << endl;
    list4.pop_head();
    cout << "Third: " << list4 << endl << endl;
    list1.delete_node(2);
    cout << "Delete Nodes from first(2): " << list1 << endl << endl;
    DLinkedList<int> list5(5, 1, 8);
    cout << "Original: " << list5 << endl;
    cout << "Remove from the tail: " << endl;
    list5.pop_tail();
    cout << "First: " << list5 << endl;
    list5.pop_tail();
    cout << "Second: " << list5 << endl;
    list5.pop_tail();
    cout << "Third: " << list5 << endl << endl;
    cout << "First list: " << list1 << endl;
    cout << "First index of first list: " << list1[0] << endl;
    list1[2] = 10;
    cout << "Changed third index of first list: " << list1[2] << endl;
    cout << "First list: " << list1 << endl << endl;
    DLinkedList<int> list6(4, 1, 7);
    cout << "Another list: " << list6 << endl;
    list1.push_head(list6);
    cout << "First list with another in head: " << list1 << endl;
    cout << "Second list: " << list2 << endl;
    list2.push_tail(list6);
    cout << "Second list with another in tail: " << list2 << endl << endl;
    cout << endl << endl << "floating point numbers:" << endl;
    //////////////////////////////////////////////////////////////////////
    DLinkedList<double> dlist1;
    dlist1.push_tail(1.5,6);
    dlist1.push_tail(2.2,5);
    dlist1.push_tail(2.3,4);
    dlist1.push_tail(8.4,3);
    dlist1.push_tail(2.5,2);
    dlist1.push_head(4.7,1);
    dlist1.push_head(5.1,0);
    DLinkedList<double> dlist2 = dlist1;
    DLinkedList<double> dlist3(dlist1);
    dlist3 = dlist2;
    DLinkedList<double> dlist4(5, 1.0, 10.0);
    cout << "First list: " << dlist1 << endl;
    cout << "Assignment: " << dlist2 << endl;
    cout << "Copying: " << dlist3 << endl;
    cout << "Random: " << dlist4 << endl << endl;
    cout << "Remove from the head: " << endl;
    dlist4.pop_head();
    cout << "First: " << dlist4 << endl;
    dlist4.pop_head();
    cout << "Second: " << dlist4 << endl;
    dlist4.pop_head();
    cout << "Third: " << dlist4 << endl << endl;
    dlist1.delete_node(2.3);
    cout << "Delete Nodes from first(2.3): " << dlist1 << endl << endl;
    DLinkedList<double> dlist5(5, 1.0, 8.0);
    cout << "Original: " << dlist5 << endl;
    cout << "Remove from the tail: " << endl;
    dlist5.pop_tail();
    cout << "First: " << dlist5 << endl;
    dlist5.pop_tail();
    cout << "Second: " << dlist5 << endl;
    dlist5.pop_tail();
    cout << "Third: " << dlist5 << endl << endl;
    cout << "First list: " << dlist1 << endl;
    cout << "First index of first list: " << dlist1[0] << endl;
    dlist1[2] = 10.5;
    cout << "Changed third index of first list: " << dlist1[2] << endl;
    cout << "First list: " << dlist1 << endl << endl;
    DLinkedList<double> dlist6(4, 1.0, 7.0);
    cout << "Another list: " << dlist6 << endl;
    dlist1.push_head(dlist6);
    cout << "First list with another in head: " << dlist1 << endl;
    cout << "Second list: " << dlist2 << endl;
    dlist2.push_tail(dlist6);
    cout << "Second list with another in tail: " << dlist2 << endl << endl;

    double result1 = polynomial(dlist6, 3.0);
    cout << "polynomial another list with x=3: " << result1;
    return 0;
}