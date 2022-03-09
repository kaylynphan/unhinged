#include "MemberDatabase.h"
#include <iostream>

using namespace std;

int main() {
    MemberDatabase mdb;
    if (!mdb.LoadDatabase("testmembers.txt"))
    {
        cout << "Error loading file" << endl;
        return 1;
    } else {
        cout << mdb.toString() << endl;
    }
}