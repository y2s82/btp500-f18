// AVLTree.cpp: AVL Tree Implementation in C++
#include <algorithm>
#include <fstream>
#include <iostream>
#include "timer.h"

using namespace std;

const int NUM = 250000;//Max 250000
ofstream outfile;
//int treeHeight=0;

typedef struct EmployeeInfo {
	int salary;
	int age;
	int emplNumber;
	int sin;//search by social insurance number
}EmployeeInfo;

typedef struct node {
	EmployeeInfo empl;
	node* left;
	node* right;
	int height;
}node;

class BST
{
	node* root;
	void makeEmpty(node* t)
	{
		if (t == NULL)
			return;
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}

	int max(int a, int b) {
		if (a > b) return a;
		else return b;
	}

	int min(int a, int b) {
		if (a < b) return a;
		else return b;
	}

	node* insert(EmployeeInfo empl, node* t)
	{
		if (t == NULL)
		{
			t = new node;
			t->empl.salary = empl.salary;
			t->empl.age = empl.age;
			t->empl.emplNumber = empl.emplNumber;
			t->empl.sin = empl.sin;
			t->height = 0;
			t->left = t->right = NULL;
		}
		else if (empl.sin < t->empl.sin)
		{//Go down the left tree
			t->left = insert(empl, t->left);
			if (height(t->left) - height(t->right) == 2)
			{   //Left is higher than right by two
				if (empl.sin < t->left->empl.sin)
					//will go down the left subtree, do right rotate
					t = singleRightRotate(t);
				else

					t = leftRightRotate(t);
			}
		}
		else if (empl.sin > t->empl.sin)
		{//Go down the right tree
			t->right = insert(empl, t->right);
			if (height(t->right) - height(t->left) == 2)
			{//Right is higher than left by two
				if (empl.sin > t->right->empl.sin)
					t = singleLeftRotate(t);//Will go down the right subtree, do left rotate
				else
					t = rightLeftRotate(t);//Will go down the left subtree, do right-left rotate
			}
		}

		t->height = max(height(t->left), height(t->right)) + 1;
		return t;
	}

	node* singleRightRotate(node* &t)
	{//        8
   //    4             12
   // 2     6     10        14    
   //1 3   5 7   9  11    13  15
		node* u = t->left;
   //          8
   //    4(U)          12
   // 2     6     10        14    
   //1 3   5 7   9  11    13  15
		t->left = u->right;
   //          8
   //    6          12
   // 5     7     10        14    
   //            9  11    13  15
   //---------------------------
   //    4(U)
   // 2     6
   //1 3   5 7
		u->right = t;
   //          4
   //    2           8
   // 1     3    6         12    
   //           5 7    10     14
   //                 9  11 13  15
		t->height = max(height(t->left), height(t->right)) + 1;
		u->height = max(height(u->left), t->height) + 1;
		return u;
	}

	node* singleLeftRotate(node* &t)
	{//        8
   //    4             12
   // 2     6     10        14    
   //1 3   5 7   9  11    13  15
		node* u = t->right;
   //          8
   //    4             12(U)
   // 2     6     10        14    
   //1 3   5 7   9  11    13  15
		t->right = u->left;
   //          8
   //    4        10
   // 2     6   9   11    
   //1 3   5 7
   //-------------------------
   //                  12(U)
   //             10        14    
   //            9  11    13  15
		u->left = t;
   //              12
   //       8              14
   //   4      10     13        15    
   // 2   6   9  11
   //1 3 5 7
		t->height = max(height(t->left), height(t->right)) + 1;
		u->height = max(height(t->right), t->height) + 1;
		return u;
	}

	node* rightLeftRotate(node* &t)
	{   //Right rotate the right node, then left rotate the current node
		t->right = singleRightRotate(t->right);
		return singleLeftRotate(t);
	}

	node* leftRightRotate(node* &t)
	{   //Left rotate the left node, then right rotate the current node
		t->left = singleLeftRotate(t->left);
		return singleRightRotate(t);
	}

	node* findMin(node* t)
	{
		if (t == NULL)
			return NULL;
		else if (t->left == NULL)
			return t;
		else
			return findMin(t->left);
	}

	node* findMax(node* t)
	{
		if (t == NULL)
			return NULL;
		else if (t->right == NULL)
			return t;
		else
			return findMax(t->right);
	}

	node* remove(int sin, node* t)
	{
		node* temp;

		// Element not found
		if (t == NULL)
			return NULL;

		// Searching for element
		else if (sin < t->empl.sin)
			t->left = remove(sin, t->left);
		else if (sin > t->empl.sin)
			t->right = remove(sin, t->right);

		// Element found
		// With 2 children
		else if (t->left && t->right)
		{
			temp = findMin(t->right);
			t->empl.sin = temp->empl.sin;
			t->right = remove(t->empl.sin, t->right);
		}
		// With one or zero child
		else
		{
			temp = t;
			if (t->left == NULL)
				t = t->right;
			else if (t->right == NULL)
				t = t->left;
			delete temp;
		}
		if (t == NULL)
			return t;

		t->height = max(height(t->left), height(t->right)) + 1;

		// If node is unbalanced
		// If left node is deleted, right case
		if (height(t->left) - height(t->right) == 2)
		{
			// right right case
			if (height(t->left->left) - height(t->left->right) == 1)
				return singleLeftRotate(t);
			// right left case
			else
				return rightLeftRotate(t);
		}
		// If right node is deleted, left case
		else if (height(t->right) - height(t->left) == 2)
		{
			// left left case
			if (height(t->right->right) - height(t->right->left) == 1)
				return singleRightRotate(t);
			// left right case
			else
				return leftRightRotate(t);
		}
		return t;
	}

	int height(node* t)
	{
		return (t == NULL ? -1 : t->height);
	}

	int getBalance(node* t)
	{
		if (t == NULL)
			return 0;
		else
			return height(t->left) - height(t->right);
	}

	void inorder(node* t)
	{
		if (t == NULL)
			return;
		inorder(t->left);
		outfile << " height:" << t->height << " sin:" << t->empl.sin << " employee number:" << t->empl.emplNumber << " salary:" << t->empl.salary << " age:" << t->empl.age << endl;
		inorder(t->right);
	}

public:
	BST()
	{
		root = NULL;
	}

	void insert(EmployeeInfo empl)
	{
		root = insert(empl, root);
	}

	void remove(int sin)
	{
		root = remove(sin, root);
	}

	void display()
	{
		inorder(root);
		outfile << endl;
	}
	node * GetRoot() {
		return root;
	}
	node * Find(node *node, int sin)
	{
		if (node == NULL)
		{
			/* Element is not found */
			return NULL;
		}
		if (sin > node->empl.sin)
		{
			/* Search in the right sub tree. */
			return Find(node->right, sin);
		}
		else if (sin < node->empl.sin)
		{
			/* Search in the left sub tree. */
			return Find(node->left, sin);
		}
		else
		{
			/* Element Found */
			return node;
		}
	}
};

int main()
{
	BST t;
	int sinArray[NUM];
	for (int i = 0; i < NUM; ++i) sinArray[i] = 100000000 + i;
	random_shuffle(begin(sinArray), end(sinArray));
//	for (int i = 0; i < NUM; ++i) cout << "i:" << i << " sin:" << sinArray[i] << endl;
	for (int i = 0; i < NUM; ++i) {
		EmployeeInfo empl;
		empl.age = 25 + (i * 2) % 45;
		empl.salary = 50000 + (i * 10) % 100000;
		empl.emplNumber = i;
		empl.sin = sinArray[i];
		t.insert(empl);
	}
	outfile.open("inorder.txt");
	cout << "INORDER (NUM=" << NUM << ", height=" << t.GetRoot()->height << "): root node has sin:" << t.GetRoot()->empl.sin << endl << endl;
	outfile << "INORDER (NUM=" << NUM << ", height=" << t.GetRoot()->height << "): root node has sin:" << t.GetRoot()->empl.sin << endl << endl;
	t.display();
	outfile.close();
	node *temp;
	int sin;
	Timer stopwatch;
	do {
		cout << "Enter a SIN to search (-1 to quit):";
		cin >> sin;
		stopwatch.start();
		temp = t.Find(t.GetRoot(), sin);
		stopwatch.stop();
		if (temp == NULL) {
			cout << "SIN " << sin << " not found" << endl;
		}
		else {
			cout << "time:" << stopwatch.currtime() << " height:" << temp->height << " sin:" << temp->empl.sin << " employee number:" << temp->empl.emplNumber << " salary:" << temp->empl.salary << " age:" << temp->empl.age << endl;
			outfile << "time:" << stopwatch.currtime() << " height:" << temp->height << " sin:" << temp->empl.sin << " employee number:" << temp->empl.emplNumber << " salary:" << temp->empl.salary << " age:" << temp->empl.age << endl;
		}
	} while (sin > 0);
	cout << endl << "DONE" << endl << endl;
	//TODO: Look at the remove functions on your own.
}