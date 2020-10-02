#include <iostream>
#include <fstream>
#include <stdio.h>
#include<string>
#include "RedBlackTree.h"

using namespace std;

void VisualizeRBTDot(string dotString){
    ofstream myfile;
    myfile.open ("rbt.dot");
    myfile << dotString;
    myfile.close();
    system("dot -Tpng rbt.dot -o rbt.png");
    remove("rbt.dot");
}

int main (){
    RedBlackTree* rbt = new RedBlackTree();
   	cout << "Enter node. 0 to quit" << endl;
   	int choice;
   	cin >> choice;
   	while(choice != 0){
        rbt->Insert(choice);
        VisualizeRBTDot(rbt->CreateDotString());
        cout << "Enter node. 0 to quit" << endl;
        cin >> choice;
   	}

}
