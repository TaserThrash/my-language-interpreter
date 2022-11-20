// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <windows.h>
#include <ctime>

using namespace std;

vector<int> lines = {};
vector<string> instructions = {};
vector<int> arguments = {};
vector<int> nexts = {};
vector<int> stack = {};

int getInt() {
    int x = 0;
    while (true) {
        cin >> x;
        if (cin.fail()) {
            cin.clear();
            continue;
        }
        break;
    }
    return x;
}

int pos = 0;

BOOL scpt(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int t;

int runLine(int x) {
    int arg = arguments[x];
    int next = nexts[x];
    string ins = instructions[x];
    if (ins == "push")
        stack.push_back(arg);
    if (ins == "goto") {
        if (arg >= 0 && arg < stack.size()) {
            pos = arg;
        }
        else {
            cout << "invalid stack location";
            return 0;
        }
    }
    if (ins == "setconst") {
        if (arg >= 0 && arg < stack.size()) {
            stack[pos] = arg;
        }
        else {
            cout << "invalid stack location";
            return 0;
        }
    }
    if (ins == "setreg") {
        if (arg >= 0 && arg < stack.size()) {
            stack[pos] = stack[arg];
        }
        else {
            cout << "invalid stack location";
            return 0;
        }
    }
    if (ins == "addnum") {
        stack[pos] += arg;
    }
    if (ins == "addreg") {
        stack[pos] += stack[arg];
    }
    if (ins == "subnum") {
        stack[pos] -= arg;
    }
    if (ins == "subreg") {
        stack[pos] -= stack[arg];
    }
    if (ins == "multnum") {
        stack[pos] *= arg;
    }
    if (ins == "multreg") {
        stack[pos] *= stack[arg];
    }
    if (ins == "divnum") {
        stack[pos] /= arg;
    }
    if (ins == "divreg") {
        stack[pos] /= stack[arg];
    }
    if (ins == "jez") {
        if (stack[pos] == 0) {
            next = arg;
        }
    }
    if (ins == "jgz") {
        if (stack[pos] > 0) {
            next = arg;
        }
    }
    if (ins == "jlz") {
        if (stack[pos] < 0) {
            next = arg;
        }
    }
    if (ins == "print") {
        if (arg >= 0 && arg < stack.size()) {
            cout << stack[arg] << "\n";
        }
        else {
            cout << "invalid stack location";
            return 0;
        }
    }
    if (ins == "plot") {
       scpt(stack[arg], stack[arg + 1]);
        cout << '@';
    }
    if (ins == "rnd") {
        stack[pos] = rand() % arg;
    }
    if (ins == "input") {
        cout << "\ninsert a number\n";
        stack[arg] = getInt();
    }
    if (ins == "cls") {
        system("cls");
    }
    
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == next) {
            return i;
        }
    }

    if (nexts[x] == 0) {
        cout << "terminated program\n";
        stack.clear();
        pos = 0;
        return 0;
    }

    cout << "invalid next line";
    return 0;
}

void run() {
    int x = lines[0];
    int j = 0;
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] < x) {
            j = i;
            x = lines[i];
        }
    }

    int a = runLine(j);

    while (a != 0) {
        a = runLine(a);
        
        for (int i = 0; i < 10; i++){
            if (GetAsyncKeyState(VK_ESCAPE)) {
                cout << "terminated program\n";
                stack.clear();
                pos = 0;
                return;
            }

            Sleep(125 / 10);
        }
    }

}

void list() {
    for (int i = 0; i < lines.size(); i++) {
        cout << lines[i] << " ";
        cout << instructions[i] << " ";
        cout << arguments[i] << " ";
        cout << nexts[i] << "\n";
    }
}

string instruct = ("[line] [command] [value] [next line] \n"
"\"push\" pushes value to the stack \n \"print\" outputs the value of the stack at position value \n"
" \"goto\" sets the pointer pos to the value \n"
"\"jez\" goes to line number value if the value being pointed to is zero \"jgz\" positive \"jlz\" negative \n"
"add, sub, div or mult plus const adds, subtracts, divides, or multiplies the number pointed to by value at reg by the stack location value \n \"plot\" places a # at with an x of she stack value at position value y at the next");

int main()
{
    srand(time(0));
    cout << "0 for run -1 for list -2 for clear -3 for help \n ";
    
    while (true) {
        //cout << "line number: ";
        int x = getInt();
        if (x == 0) {
            run();
            continue;
        }
        if (x == -1) {
            list();
            continue;
        }
        if (x == -2) {
            system("cls");
            continue;
        }
        if (x == -3) {
            cout << instruct;
        }

        for (int i = 0; i < lines.size(); i++) {
            if (lines[i] == x) {
                lines.erase(lines.begin() + i);
                instructions.erase(instructions.begin() + i);
                arguments.erase(arguments.begin() + i);
                nexts.erase(nexts.begin() + i);
            }
        }

        for (int i = lines.size() - 1; i > 1; i--) {
            if (lines[i] < lines[i - 1]) {
                int a = lines[i];
                int b = lines[i - 1];
                lines[i - 1] = a;
                lines[i] = b;
                string c = instructions[i];
                string d = instructions[i - 1];
                instructions[i - 1] = c;
                instructions[i] = d;
                a = arguments[i];
                b = arguments[i - 1];
                arguments[i - 1] = a;
                arguments[i] = b;
                a = nexts[i];
                b = nexts[i - 1];
                nexts[i - 1] = a;
                nexts[i] = b;
            }
        }

        lines.push_back(x);
        string s;
        //cout << "instruction: ";
        cin >> s;
        instructions.push_back(s);

        //cout << "argument: ";
        arguments.push_back(getInt());
        //cout << "next line number: ";
        nexts.push_back(getInt());
    }
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
