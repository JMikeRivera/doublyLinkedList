#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

//Define la estructura para la entrada que se vaya a realizar
//Se guardan los atributos de mes, dia, hora, direccion_ip y razon
struct LogEntry {
    string month;
    int day;
    string time;
    string ipAddress;
    string reason;
};

//Se define un nodo para la entrada 
struct LogEntryNode {
    LogEntry entry;
    LogEntryNode* next;

    LogEntryNode(const LogEntry& logEntry) : entry(logEntry), next(nullptr) {}
};

class LinkedList {
public:
    LinkedList() : head(nullptr) {}

    void addLogEntry(const LogEntry& entry) {
        LogEntryNode* newNode = new LogEntryNode(entry);
        newNode->next = head;
        head = newNode;
    }

    void printLogEntries() {
        LogEntryNode* current = head;
        while (current != nullptr) {
            cout << "Month: " << current->entry.month << endl;
            cout << "Day: " << current->entry.day << endl;
            cout << "Time: " << current->entry.time << endl;
            cout << "IP Address: " << current->entry.ipAddress << endl;
            cout << "Reason: " << current->entry.reason << endl;
            cout << "=========================" << endl;
            current = current->next;
        }
    }

    void sortLogEntries() {
        head = mergeSort(head);
    }

    int binarySearch(const string& targetIP) {
        int index = 0;
        LogEntryNode* current = head;

        while (current != nullptr) {
            if (current->entry.ipAddress == targetIP) {
                return index; //En caso que se encuentre la direccion ip
            }

            current = current->next;
            index++;
        }

        return -1; // En caso de que no se encuentre
    }

    void printLogEntriesInRange(const string& startIP, const string& endIP) {
        LogEntryNode* current = head;
        bool inRange = false;

        ofstream outputFile("output.txt");
        if (!outputFile.is_open()) {
            cerr << "Failed to create the output file." << endl;
            return;
        }

        while (current != nullptr) {
            if (current->entry.ipAddress == startIP) {
                inRange = true;
            }
            //Se imprimen los atributos de la clase
            if (inRange) {
                outputFile << "Month: " << current->entry.month << endl;
                outputFile << "Day: " << current->entry.day << endl;
                outputFile << "Time: " << current->entry.time << endl;
                outputFile << "IP Address: " << current->entry.ipAddress << endl;
                outputFile << "Reason: " << current->entry.reason << endl;
                outputFile << "=========================" << endl;
            }

            if (current->entry.ipAddress == endIP) {
                break;
            }

            current = current->next;
        }

        outputFile.close();
        cout << "Output file 'output.txt' created with log entries in the specified range." << endl;
    }

    ~LinkedList() {
        while (head != nullptr) {
            LogEntryNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

private:
    LogEntryNode* head;
        //Se prepara el programa para MergSort
    LogEntryNode* merge(LogEntryNode* left, LogEntryNode* right) {
        LogEntryNode dummyNode(LogEntry{"", 0, "", "", ""});
        LogEntryNode* current = &dummyNode;

        while (left != nullptr && right != nullptr) {
            if (left->entry.ipAddress <= right->entry.ipAddress) {
                current->next = left;
                left = left->next;
            } else {
                current->next = right;
                right = right->next;
            }
            current = current->next;
        }

        current->next = (left != nullptr) ? left : right;
        return dummyNode.next;
    }
    //Se aplica el mergSort a la entrada del nodo

    LogEntryNode* mergeSort(LogEntryNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        LogEntryNode* slow = head;
        LogEntryNode* fast = head->next;
        while (fast != nullptr) {
            fast = fast->next;
            if (fast != nullptr) {
                fast = fast->next;
                slow = slow->next;
            }
        }

        LogEntryNode* left = head;
        LogEntryNode* right = slow->next;
        slow->next = nullptr;

        left = mergeSort(left);
        right = mergeSort(right);

        return merge(left, right);
    }
};

int main() {
    ifstream file("bitacora.txt");
    if (!file.is_open()) {
        cerr << "Failed to open the database file." << endl;
        return 1;
    }
        //Se utilizan getters para obtener la informacion en las funciones anteriores
    vector<LogEntry> logEntries;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        LogEntry entry;
        iss >> entry.month >> entry.day >> entry.time >> entry.ipAddress;
        getline(iss, entry.reason);
        logEntries.push_back(entry);
    }

    LinkedList logList;
    for (const LogEntry& entry : logEntries) {
        logList.addLogEntry(entry);
    }

    logList.sortLogEntries();

    cout << "Enter the starting IP address: "; //Pide al usuario la direccion inicial
    string startIP;
    cin >> startIP;

    cout << "Enter the ending IP address: "; //Pide al usuario la direccion final
    string endIP;
    cin >> endIP;

    logList.printLogEntriesInRange(startIP, endIP); //Se imprime la entrada

    return 0;
}