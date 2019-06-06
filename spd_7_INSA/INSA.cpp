#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <unistd.h>


using namespace std;

class Task {
public:
    int opNumber, machineNumber, p, r, q, taskNum, orderNum;

    // assignement operator overload
    Task & operator = (const Task& task) {
        this->opNumber = task.opNumber;
        this->p = task.p;
        this->machineNumber = task.machineNumber;
        this->q = task.q;
        this->r = task.r;
        this->taskNum = task.taskNum;
        this->orderNum = task.orderNum;
        return * this;
    }
};

struct compareP {
    bool operator()(const Task& Task1, const Task& Task2)
    {
        if(Task1.p < Task2.p) {
            return true;
        } else if(Task1.p > Task2.p) {
            return false;
        } else if(Task1.p == Task2.p && Task1.taskNum < Task2.taskNum) {
            return false;
        } else {
            return true;
        }
    }
};

Task permutationTable[100][100];
priority_queue<Task, vector<Task>, compareP> taskTableSorted;

void showpq(priority_queue <Task, vector<Task>, compareP> pq)
{
    priority_queue <Task, vector<Task>, compareP> PQ = pq;
    while (!PQ.empty())
    {
        cout << PQ.top().p << " " << PQ.top().machineNumber << " " << PQ.top().opNumber << " " << PQ.top().taskNum << endl;
        PQ.pop();
    }
    cout << endl;
}

void fillRQZeros(int m, int n, int machine, int task) {
    permutationTable[machine][task].r = 0;
    permutationTable[machine][task].q = 0;
}

int findPreOpNum(int opNum, int m, int n, int machine, int task, int orderNum, int p) {
    int lastPreOp = 0;
    for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
            if(permutationTable[i][j].opNumber == opNum && (machine != i || task != j) && permutationTable[i][j].orderNum < orderNum && permutationTable[i][j].p) {
                if(lastPreOp < permutationTable[i][j].q && permutationTable[i][j].r >= p) {
                    lastPreOp =  permutationTable[i][j].q;
                }
            }
        }
    }
    return lastPreOp;
}

void establishRQ(int m, int n, int machine, int task) {
    fillRQZeros(m, n, machine, task);
    if(task == 0 && permutationTable[machine][task].p != 0) {
        permutationTable[machine][task].r = findPreOpNum(permutationTable[machine][task].opNumber, m, n, machine, task, permutationTable[machine][task].orderNum, permutationTable[machine][task].p);
        permutationTable[machine][task].q = permutationTable[machine][task].r + permutationTable[machine][task].p;
    } else if(permutationTable[machine][task].p != 0) {
        permutationTable[machine][task].r = max(permutationTable[machine][task-1].q, findPreOpNum(permutationTable[machine][task].opNumber, m, n, machine, task, permutationTable[machine][task].orderNum, permutationTable[machine][task].p));
        permutationTable[machine][task].q = permutationTable[machine][task].r + permutationTable[machine][task].p;
    }
}

int insertNextTask(int whichTask, int m, int n) {
    bool placeFound = false;
    int i = 0;
    Task current = taskTableSorted.top();
    taskTableSorted.pop();
    current.orderNum = whichTask;
    while(placeFound != true) {
        if(permutationTable[current.machineNumber-1][i].machineNumber == 0) {
            permutationTable[current.machineNumber-1][i] = current;
            establishRQ(m, n, current.machineNumber-1, i);
            placeFound = true;
        }
        i++;
    }
    return current.machineNumber;
}

int countCmax(int n, int m) {
    int current = 0;
    for(int i=0;i<m+1;i++) {
        for(int j=0;j<n+1;j++) {
            if(permutationTable[i][0].p != 0 && permutationTable[i][j].p == 0) {
                if(max(permutationTable[i][j-1].q,permutationTable[i][j-1].r) > current) {
                    current = max(permutationTable[i][j-1].q,permutationTable[i][j-1].r);
                }
            }
        }
    }
    return current;
}

void swapTasks(int id1, int id2, int m) {
    Task tmp;
    tmp = permutationTable[m][id1];
    permutationTable[m][id1] = permutationTable[m][id2];
    permutationTable[m][id2] = tmp;
}

void checkAllPositions(int m, int n, int machine) {
    int lastTask = -1, currentCmax, bestCmax = 9999999, bestPosition;
    bestCmax = countCmax(n, m);
    // cout << bestCmax << endl;
    for(int i=n;i>0;i--) {
        if(permutationTable[machine-1][i-1].p != 0) {
            lastTask = i-1;
            break;
        }
    }
    bestPosition = lastTask;
    if(lastTask != 0) {
        for(int k=lastTask;k>0;k--) {
            swapTasks(k-1, k, machine-1);
            establishRQ(m,n, machine-1, k-1);
            establishRQ(m,n, machine-1, k);
            currentCmax = countCmax(n, m);
            // cout << currentCmax << endl;
            if(currentCmax <= bestCmax) {
                bestCmax = currentCmax;
                bestPosition = k-1;
            }
        }
        for(int j=0;j<bestPosition;j++) {
            swapTasks(j, j+1, machine-1);
            establishRQ(m, n, machine-1, j);
            establishRQ(m, n, machine-1, j+1);
        }
    }
}

int performINSA(int n, int m) {
    int currentMachine, cmax;
    for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
            permutationTable[i][j].machineNumber = 0;
            permutationTable[i][j].p = 0;
            permutationTable[i][j].opNumber = 0;
            permutationTable[i][j].r = -1;
            permutationTable[i][j].q = -1;
        }
    }
    for(int i=0;i<m*n;i++) {
        currentMachine = insertNextTask(i, m, n);
        checkAllPositions(m, n, currentMachine);
    }
    cmax = countCmax(n, m);
    return cmax;
}


int main () {
    auto start = chrono::steady_clock::now();
    ifstream data;
    data.open("data.txt");
    int n, m, taskQuanity, Cmax;
    string str;
    string part1 = "data.", part2 = ":", part3 = "";
    string data1;
    string dataNames[4];
    int finalPermutation[100][100];
    for(int i = 0; i<4;i++) {
        if(i < 10) {
            part3 = "00" + to_string(i);
        } else {
            part3 = "0" + to_string(i);
        }
        data1 = part1 + part3 + part2;
        dataNames[i] = data1;
    }
    for(int i = 0;i<4;i++) {
         while(str != dataNames[i]) {
        //while(str != "data.001:"){
            data >> str;
        }
        data >> str;
        n = stoi(str);
        data >> str;
        m = stoi(str);
        data >> str;
        taskQuanity = stoi(str);
        Task taskTable[taskQuanity-1];
        cout << "Dane dla zestawu " << dataNames[i] << endl;
        int techNum = 0;
        for(int j=0;j<taskQuanity;j++) {
            if(j % n == 0) {
                techNum++;
            }
            if(j % m == 0) {
                data >> str;
            }
            data >> taskTable[j].machineNumber >> taskTable[j].p;
            taskTable[j].opNumber = techNum;
            taskTable[j].taskNum = j+1;
            taskTable[j].q = 0;
            taskTable[j].r = 0;
        }
        priority_queue<Task, vector<Task>, compareP> tmp;
        for(int j=0;j<taskQuanity;j++) {
            taskTableSorted.push(taskTable[j]);
        }
        showpq(taskTableSorted);

        Cmax = performINSA(n, m);
        cout << "Cmax: " << Cmax << endl;
    }

    auto end = chrono::steady_clock::now();
	cout << "Elapsed time in seconds : "
		<< chrono::duration_cast<chrono::seconds>(end - start).count()
		<< " s" << endl;
    return 0;
}
