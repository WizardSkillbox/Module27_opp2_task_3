#include <iostream>
#include <string>
#include <cassert>

#include <random_int_generator.h>

using namespace std;

enum TaskTypes_e {
    TASK_TYPE_NONE,
    TASK_TYPE_FIRST = 1,
    TASK_TYPE_A = TASK_TYPE_FIRST,
    TASK_TYPE_B,
    TASK_TYPE_C,
    TASK_TYPE_LAST = TASK_TYPE_C
};

class Worker {
    TaskTypes_e task = TASK_TYPE_NONE;
public:

    void AssignTask(TaskTypes_e inTask) {
        assert(inTask >= TASK_TYPE_FIRST && task <= TASK_TYPE_LAST);
        task = inTask;
    }
};

class Manager {
    Worker **pWorkers;
    int numOfWorkers;
    int groupIndex;
    int numOfFreeWorkers;

public:
    Manager(int inGroupIndex, int inNumOfWorkers)
            : groupIndex(inGroupIndex),
              numOfWorkers(inNumOfWorkers), numOfFreeWorkers(inNumOfWorkers) {
        assert(inNumOfWorkers > 0);
        assert(inGroupIndex >= 0);

        pWorkers = new Worker *[numOfWorkers];
        for (int i = 0; i < numOfWorkers; ++i) {
            pWorkers[i] = new Worker();
        }
    }

    ~Manager() {
        for (int i = 0; i < numOfWorkers; ++i) {
            delete pWorkers[i];
        }
        delete[] pWorkers;
    }

    void ObtainVectorOfTasks(int vectorNum) {
        if (!numOfFreeWorkers) {
            return;
        }

        IntRandom generator = IntRandom(vectorNum + groupIndex);
        int numOfTasks = generator.generateRandom(1, numOfWorkers);

        if ((numOfFreeWorkers - numOfTasks) < 0) {
            numOfTasks = numOfFreeWorkers;
        }

        for (int i = numOfWorkers - numOfFreeWorkers; i < numOfWorkers - numOfFreeWorkers + numOfTasks; ++i) {
            int taskType = generator.generateRandom(TASK_TYPE_FIRST, TASK_TYPE_LAST);
            pWorkers[i]->AssignTask((TaskTypes_e) taskType);
        }
        numOfFreeWorkers -= numOfTasks;
    }

    int GetNumberOfFreeWorkers() const {
        cout << "Group " << groupIndex << ": Free workers " << numOfFreeWorkers << endl;
        return numOfFreeWorkers;
    }

};

class Director {
    Manager **pManagers;
    int numOfGroups;

    static int GetIntFromStream(const string &message, int min, int max) {
        int num;
        bool isCorrect = false;
        while (!isCorrect) {
            cout << message << ":";
            isCorrect = (!!(cin >> num)) && num >= min && num <= max;
            if (!isCorrect) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error! Please enter correct number" << endl;
            }
        }
        return num;
    }

public:
    Director(int maxGroups, int maxWorkers) {
        numOfGroups = GetIntFromStream("Enter the number of groups", 1, maxGroups);
        pManagers = new Manager *[numOfGroups];
        for (int i = 0; i < numOfGroups; ++i) {
            string message = "Group " + to_string(i + 1) + ". Enter the number of workers";
            int numOfWorkers = GetIntFromStream(message, 1, maxWorkers);
            pManagers[i] = new Manager(i, numOfWorkers);
        }
    }

    ~Director() {
        for (int i = 0; i < numOfGroups; ++i) {
            delete pManagers[i];
        }
        delete[] pManagers;
    }

    void IssueDirective() {
        int vector = GetIntFromStream("Enter vector value", -INT32_MAX, INT32_MAX);
        for (int i = 0; i < numOfGroups; ++i) {
            pManagers[i]->ObtainVectorOfTasks(vector);
        }
    }

    int GetNumberOfFreeWorkers() {
        int result = 0;
        for (int i = 0; i < numOfGroups; ++i) {
            result += pManagers[i]->GetNumberOfFreeWorkers();
        }
        return result;
    }
};

int main() {
    auto director = new Director(10, 10);

    while(director->GetNumberOfFreeWorkers()) {
        director->IssueDirective();
    }
    delete director;
    return 0;
}
