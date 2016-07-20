// CSE.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

struct Node {
	string value;
	Node *leftChild = NULL;
	Node *rightSibling = NULL;
};

struct Result {
	string value = "";
	Node *pos = NULL;
};

struct Obj {
	string type = "";
	string value = "";
	int index = 0;
	vector<string> var;
	int cenv = 0;
};

struct e {
	int cur;
	vector<string> i;
	vector<string> v;
	int prev;
};

void fn_E(); void fn_Ew(); void fn_T(); void fn_Ta();
void fn_Tc(); void fn_B(); void fn_Bt(); void fn_Bs();
void fn_Bp(); void fn_A(); void fn_At(); void fn_Af();
void fn_Ap(); void fn_R(); void fn_Rn(); void fn_D();
void fn_Da(); void fn_Dr(); void fn_Db(); void fn_Vb();
void fn_Vl();

stack<Node*> myStack;
int counter = 0;
fstream fs;
string NextToken;
//int N = 0;
vector<vector<Obj*>*> css;
int curDelta = 0;
vector<Obj*>* cs = new vector<Obj*>;
int indexk = 1;
stack<Obj*> control;
stack<Obj*> state;
int si = 0;
int tindex = 1;
int curEnv = 0;
int nextEnv = 1;
vector<e*> mye;
int c = 0;
int printpresent=0;

string keywords[] = { "let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le", "eq", "ne",
"true", "false", "dummy", "within", "and", "rec", "neg", "tau", "function_form", "lambda", "gamma", "nil", "","<true>", "<false>", "<dummy>","<nil>" };

string opers[] = { "->", ">=",  "<=", "**", ".", ",", "|", ">", "<", "+", "-", "*", "/", "@", "(", ")", "=" , "'", "&", "()" };

string allNodes[] = { "tau", "function_form", ",", "and", "lambda" };

string three[] = { "@", "->" };

string trnsf[] = { "let", "where", "wihtin", "rec", "@", "and", "function_form", "lambda" };
//"let", "where", "tau", "->", "not", "neg", "within", "rec", "function_form", "lambda", "aug", 
//"or", "&", "gr", "ge", "ls", "le", "eq", "ne", "+", "-", "*", "/", "**", "and", "@"};

string bOp[] = { "aug", "or", "&", "gr", "ge", "ls", "le", "eq", "ne", "+", "-", "*", "/", "**" };

string uOp[] = { "not", "neg" };

bool uOpe(string s) {
	for (int i = 0; i < (sizeof(uOp) / sizeof(uOp[0])); ++i) {
		if (s.compare(uOp[i]) == 0)
			return 1;
	}
	return 0;
}

bool bOpe(string s) {
	for (int i = 0; i < (sizeof(bOp) / sizeof(bOp[0])); ++i) {
		if (s.compare(bOp[i]) == 0)
			return 1;
	}
	return 0;
}

bool trnsfkey(string s) {
	for (int i = 0; i < (sizeof(trnsf) / sizeof(trnsf[0])); ++i) {
		if (s.compare(trnsf[i]) == 0)
			return 1;
	}
	return 0;
}

bool present_three(string Token) {
	for (int i = 0; i<(sizeof(three) / sizeof(three[0])); ++i) {
		if (Token.compare(three[i]) == 0)
			return 1;
	}
	return 0;
}

bool present_all(string Token) {
	for (int i = 0; i<(sizeof(allNodes) / sizeof(allNodes[0])); ++i) {
		if (Token.compare(allNodes[i]) == 0)
			return 1;
	}
	return 0;
}

bool present(char c, int pl) {
	if (pl == 0) {
		for (int i = 0; i < 20; ++i) {
			if (opers[i].at(pl) == c)
				return 1;
		}
		return 0;
	}
	else if (pl == 1) {
		for (int i = 0; i < 4; ++i) {
			if (opers[i].at(pl) == c)
				return 1;
		}
		return 0;
	}
}

bool isoper(string Token) {
	for (int i = 0; i < 20; ++i) {
		if (Token.compare(opers[i]) == 0)
			return 1;
	}
	return 0;
}

bool isKeyword(string Token) {
	for (int i = 0; i < 30; ++i) {
		if (Token.compare(keywords[i]) == 0)
			return 1;
	}
	return 0;
}

bool integ(string Token) {
	if (Token.length() == 0)
		return 0;
	for (size_t i = 0; i<Token.length(); ++i) {
		if (!isdigit(Token.at(i)))
			return 0;
	}
	return 1;
}

bool ident(string Token) {
	if (Token.length() == 0)
		return 0;
	for (size_t i = 0; i < Token.length(); ++i) {
		if (!(isalnum(Token.at(i)) || Token.at(i) == '_'))
			return 0;
	}
	if (isKeyword(Token) || isoper(Token))
		return 0;
	return 1;
}

bool str(string Token) {
	if (Token.length() == 0)
		return 0;
	if (!(Token.at(0) == '\'' && Token.at(Token.length() - 1) == '\''))
		return 0;
	else
		return 1;
}

bool strEq(const char *arg0, const char *arg1) {		//compare two arguments
	string var0 = (string)arg0;	//converting both the arguments to strings and then using compare
	string var1 = (string)arg1;
	if (var0 == var1)
		return true;
	else
		return false;
}

void printTree(Node *root) {
	if (root != NULL) {
		for (int j = 0; j<counter; ++j) { cout << "."; }
		if (integ(root->value))
			cout << "<INT:" << root->value << ">" << endl;
		else if (str(root->value))
			cout << "<STR:" << root->value << ">" << endl;
		else if (ident(root->value))
			cout << "<ID:" << root->value << ">" << endl;
		else
			cout << root->value << endl;
		counter++;
		printTree(root->leftChild); counter--;
		printTree(root->rightSibling);
	}
}

void buildTree(string s, int i) {
	if (i == 0) {
		Node *temp = new Node;
		temp->value = s;
		myStack.push(temp);
		//if(temp->value=="")
		//	myStack.pop();
	}
	else if (i == 1) {
		Node *temp = myStack.top();
		myStack.pop();
		Node *newHead = new Node;
		newHead->leftChild = temp;
		newHead->value = s;
		myStack.push(newHead);
	}
	else if (i == 2) {
		Node *temp = myStack.top();
		myStack.pop();
		myStack.top()->rightSibling = temp;
		temp = myStack.top();
		myStack.pop();
		Node *newRoot = new Node;
		newRoot->value = s;
		newRoot->leftChild = temp;
		myStack.push(newRoot);
	}
	else {
		if (present_all(s)) {
			vector<Node *> allNodes;
			for (int p = 1; p<i; ++p) {
				allNodes.push_back(myStack.top());
				myStack.pop();
			}
			buildTree(s, 1);
			for (int p = 0; p<allNodes.size(); ++p) {
				Node *trv = myStack.top();
				trv = trv->leftChild;
				for (int p = 0; p<allNodes.size(); ++p) {
					trv->rightSibling = allNodes[allNodes.size() - p - 1];
					trv = trv->rightSibling;
				}
			}
		}
		else if (present_three(s)) {
			vector<Node *> threeNodes;
			for (int p = 1; p<i; ++p) {
				threeNodes.push_back(myStack.top());
				myStack.pop();
			}
			buildTree(s, 1);
			for (int p = 0; p<2; ++p) {
				Node *trv = myStack.top();
				trv = trv->leftChild;
				for (int p = 0; p<2; ++p) {
					trv->rightSibling = threeNodes[threeNodes.size() - p - 1];
					trv = trv->rightSibling;
				}
			}
		}
		else {
			vector<Node*> myNodes;
			for (int p = 2; p<i; ++p) {
				myNodes.push_back(myStack.top());
				myStack.pop();
			}
			buildTree(s, 2);
			for (int p = 0; p<myNodes.size(); ++p) {
				myStack.push(myNodes[myNodes.size() - p - 1]);
				buildTree(s, 2);
			}
		}
	}
	//printTree(myStack.top());
	// cout << "...................." << endl;
}

string scanNextToken() {
	char x;
	x = fs.peek();
	char c = x;
	int i = 0;
	char *myToken = NULL;
	string Token;
	while (!fs.eof()) {
		fs.get(c);
		if (isblank(c) || (c == '\n')) {
			if (myToken == NULL)
				continue;
			else
				break;
		}
		else if (c == '/') {
			if (fs.peek() == '/') {
				fs.get(c);
				while (!(c == '\n')) {
					fs.get(c);
					if (fs.eof()) {
						return "";
					}
					//cout << c << endl;
				}
			}
			else {
				if (myToken == NULL) {
					myToken = new char[2];
					i = 0;
					myToken[i] = c;
					++i;
				}
				break;
			}
			if (myToken == NULL)
				continue;
			else
				break;
		}

		else if (c == '\'') {
			if (myToken == NULL) {
				myToken = new char[100];
				i = 0;
			}
			do {
				myToken[i] = c;
				fs.get(c);
				++i;
				if (c == '\\') {
					//	myToken[i] = c;
					//	++i;
					if (fs.peek() == '\'') {
						fs.get(c);
						myToken[i] = c;
						fs.get(c);
						++i;
					}
				}
			} while (c != '\'');
			myToken[i] = c;
			++i;
			break;
		}

		else if (present(c, 0)) {
			if (myToken == NULL) {
				myToken = new char[3];
				i = 0;
			}
			myToken[i] = c;
			++i;
			if (c == '-' || c == '>' || c == '<' || c == '*') {
				if (present(fs.peek(), 1)) {
					fs.get(c);
					myToken[i] = c;
					++i;
				}
			}
			break;
		}
		else if (isalnum(c) || (c == '_')) {
			if (myToken == NULL) {
				myToken = new char[50];
				i = 0;
			}
			myToken[i] = c;
			++i;
			if (!(isalnum(fs.peek()) || (fs.peek() == '_')))
				break;
			else
				continue;
		}

	}
	if (myToken != NULL)
		myToken[i] = '\0';

	if (myToken == NULL)
		Token = "";
	else
		Token = string(myToken);
	if (Token.compare("print") == 0 || Token.compare("Print") == 0)
		printpresent = 1;
	return Token;
}

void Read(string Token) {
	if (Token.compare(NextToken)) {
		cout << "Expected " << Token << " but found " << NextToken << endl;
		exit(1);
	}
	if (integ(Token) || ident(Token) || str(Token)) {
		buildTree(Token, 0);
	}
	NextToken = scanNextToken();
}

void printAST() {
	NextToken = scanNextToken();
	fn_E();
	//printTree(myStack.top());
}

void fn_E() {
	//cout << "fn_E";
	if (NextToken.compare("let") == 0) {
		Read("let");
		fn_D();
		Read("in");
		fn_E();
		buildTree("let", 2);
		//cout << "LET" << endl;
	}
	else if (NextToken.compare("fn") == 0) {
		Read("fn");
		int N = 0;
		do {
			fn_Vb();
			++N;
		} while (NextToken.compare("(") == 0 || ident(NextToken));
		Read(".");
		fn_E();
		buildTree("lambda", N + 1);
		//cout << "LAMBDA" << endl;
	}
	else
		fn_Ew();
}

void fn_Ew() {
	fn_T();
	if (NextToken.compare("where") == 0) {
		Read("where");
		fn_Dr();
		buildTree("where", 2);
		//cout << "WHERE" << endl;
	}
}

void fn_T() {
	fn_Ta();
	if (NextToken.compare(",") == 0) {
		int N = 0;
		do {
			Read(",");
			fn_Ta();
			++N;
		} while (NextToken.compare(",") == 0);
		buildTree("tau", N + 1);
		//cout << "TAU" << endl;
	}
}

void fn_Ta() {
	fn_Tc();
	if (NextToken.compare("aug") == 0) {
		while (NextToken.compare("aug") == 0) {
			Read("aug");
			fn_Tc();
			buildTree("aug", 2);
		}

		//cout << "AUG" << endl;
	}
}

void fn_Tc() {
	fn_B();
	if (NextToken.compare("->") == 0) {
		Read("->");
		fn_Tc();
		Read("|");
		fn_Tc();
		buildTree("->", 3);
		//cout << "->" << endl;
	}
}

void fn_B() {
	fn_Bt();
	if (NextToken.compare("or") == 0) {
		while (NextToken.compare("or") == 0) {
			Read("or");
			fn_Bt();
			buildTree("or", 2);
		}

		//cout << "OR" << endl;
	}
}

void fn_Bt() {
	fn_Bs();
	if (NextToken.compare("&") == 0) {
		while (NextToken.compare("&") == 0) {
			Read("&");
			fn_Bs();
			buildTree("&", 2);
		}

		// cout << "&" << endl;
	}
}

void fn_Bs() {
	if (NextToken.compare("not") == 0) {
		Read("not");
		fn_Bp();
		buildTree("not", 1);
		// cout << "NOT" << endl;
	}
	else
		fn_Bp();
}

void fn_Bp() {
	fn_A();
	if (NextToken.compare("gr") == 0 || NextToken.compare(">") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("gr", 2);
		// cout << "GR" << endl;
	}
	else if (NextToken.compare("ge") == 0 || NextToken.compare(">=") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("ge", 2);
		// cout << "GE" << endl;
	}
	else if (NextToken.compare("ls") == 0 || NextToken.compare("<") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("ls", 2);
		// cout << "LS" << endl;
	}
	else if (NextToken.compare("le") == 0 || NextToken.compare("<=") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("le", 2);
	}
	else if (NextToken.compare("eq") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("eq", 2);
	}
	else if (NextToken.compare("ne") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("ne", 2);
	}
}

void fn_A() {
	if (NextToken.compare("-") == 0) {
		Read("-");
		fn_At();
		buildTree("neg", 1);
		// cout << "NEG" << endl;
	}
	else if (NextToken.compare("+") == 0) {
		Read("+");
		fn_At();
	}
	else
		fn_At();
	if (NextToken.compare("+") == 0 || NextToken.compare("-") == 0) {
		while (NextToken.compare("+") == 0 || NextToken.compare("-") == 0) {
			string Temp = NextToken;
			Read(NextToken);
			fn_At();
			buildTree(Temp, 2);
			// cout << Temp << endl;
		}
	}
}

void fn_At() {
	fn_Af();
	if (NextToken.compare("*") == 0 || NextToken.compare("/") == 0) {

		while (NextToken.compare("*") == 0 || NextToken.compare("/") == 0) {
			string Temp = NextToken;
			Read(Temp);
			fn_Af();
			buildTree(Temp, 2);
		}
	}
}

void fn_Af() {
	fn_Ap();
	if (NextToken.compare("**") == 0) {
		Read("**");
		fn_Af();
		buildTree("**", 2);
	}
}

void fn_Ap() {
	fn_R();
	if (NextToken.compare("@") == 0) {
		while (NextToken.compare("@") == 0) {
			Read("@");
			if (ident(NextToken)) {
				Read(NextToken);
			}
			fn_R();
			buildTree("@", 3);
			// cout << "@" << endl;
		}
	}
}

void fn_R() {
	fn_Rn();
	if (NextToken.compare("true") == 0 || NextToken.compare("false") == 0 || NextToken.compare("nil") == 0 ||
		NextToken.compare("(") == 0 || NextToken.compare("dummy") == 0 || ident(NextToken)
		|| integ(NextToken) || str(NextToken)) {

		while (NextToken.compare("true") == 0 || NextToken.compare("false") == 0 || NextToken.compare("nil") == 0 ||
			NextToken.compare("(") == 0 || NextToken.compare("dummy") == 0 || ident(NextToken)
			|| integ(NextToken) || str(NextToken)) {
			fn_Rn();
			buildTree("gamma", 2);
		}

		// cout << "GAMMA" << endl;
	}
}

void fn_Rn() {
	if (ident(NextToken) || integ(NextToken) || str(NextToken))
		Read(NextToken);
	else if (NextToken.compare("") == 0) {}
	else if (NextToken.compare("true") == 0) {
		Read(NextToken);
		buildTree("<true>", 0);
	}
	else if (NextToken.compare("false") == 0) {
		Read(NextToken);
		buildTree("<false>", 0);
	}
	else if (NextToken.compare("nil") == 0) {
		Read(NextToken);
		buildTree("<nil>", 0);
	}
	else if (NextToken.compare("dummy") == 0) {
		Read(NextToken);
		buildTree("<dummy>", 0);
	}
	else if (NextToken.compare("(") == 0) {
		Read("(");
		fn_E();
		Read(")");
	}
	else {
		cout << "Error\n" << endl;
		exit(1);
	}
}

void fn_D() {
	fn_Da();
	if (NextToken.compare("within") == 0) {
		Read("within");
		fn_D();
		buildTree("within", 2);
		// cout << "WITHIN" << endl;
	}
}

void fn_Da() {
	fn_Dr();
	if (NextToken.compare("and") == 0) {
		int N = 0;
		do {
			Read("and");
			fn_Dr();
			++N;
		} while (NextToken.compare("and") == 0);
		buildTree("and", N + 1);
		// cout << "AND" << endl;
	}
}

void fn_Dr() {
	if (NextToken.compare("rec") == 0) {
		Read("rec");
		fn_Db();
		buildTree("rec", 1);
	}
	else
		fn_Db();
}

void fn_Db() {
	if (NextToken.compare("(") == 0) {
		Read("(");
		fn_D();
		Read(")");
	}
	else if (ident(NextToken)) {
		Read(NextToken);
		if (ident(NextToken) || NextToken.compare("(") == 0) {
			int N = 0;
			while (ident(NextToken) || NextToken.compare("(") == 0) {
				fn_Vb();
				++N;
			}
			Read("=");
			fn_E();
			buildTree("function_form", N + 2);
			// cout << "FUNCTION_FORM" << endl;
		}
		else {
			if (NextToken.compare(",") == 0) {
				int N = 0;
				while (NextToken.compare(",") == 0) {
					Read(",");
					if (ident(NextToken))
						Read(NextToken);
					else {
						cout << "Error: Expected " << NextToken << endl;
						exit(1);
					}
					++N;
				}
				buildTree(",", N + 1);
			}
			Read("=");
			fn_E();
			buildTree("=", 2);
			// cout << "=" << endl;
		}
	}
}

void fn_Vb() {
	if (ident(NextToken)) {
		Read(NextToken);
	}
	else if (NextToken.compare("(") == 0) {
		Read("(");
		if (NextToken.compare(")") == 0) {
			Read(")");
			buildTree("()", 0);
		}
		else if (ident(NextToken)) {
			//Read(NextToken);
			fn_Vl();
			Read(")");
		}
	}
}

void fn_Vl() {
	if (ident(NextToken))
		Read(NextToken);
	if (NextToken.compare(",") == 0) {
		int N = 0;
		while (NextToken.compare(",") == 0) {
			Read(",");
			if (ident(NextToken))
				Read(NextToken);
			++N;
		}
		buildTree(",", N + 1);
		// cout << "," << endl;
	}
}

void printUsage() {	//If the user enters the command incorrectly or gives the arguments incorrectly etc., this is printed out
	cout << endl;
	cout << "This is how you use it: <ExecutableName> -ast <FileName>";
	cout << "\n" << endl;
	cout << "Name of the executable followed by '-ast' argument followed by the name of the file\n";
}

void transform(string s, Node *position) {
	if (s.compare("let") == 0) {
		position->value = "gamma";							//Rotataion not needed - since similar in structure already to the trnsf
		position->leftChild->value = "lambda";
		Node *temp = position->leftChild->rightSibling;
		position->leftChild->rightSibling = position->leftChild->leftChild->rightSibling;
		position->leftChild->leftChild->rightSibling = temp;
	}
	else if (s.compare("where") == 0) {
		Node *temp1 = position->leftChild->rightSibling;	//Rotation
		position->leftChild->rightSibling = NULL;
		Node *temp2 = position->leftChild;
		position->leftChild = temp1;
		position->leftChild->rightSibling = temp2;

		position->value = "gamma";							//Forming the result nodes
		position->leftChild->value = "lambda";
		Node *temp3 = position->leftChild->rightSibling;
		position->leftChild->rightSibling = position->leftChild->leftChild->rightSibling;
		position->leftChild->leftChild->rightSibling = temp3;
	}
	else if (s.compare("within") == 0) {
		position->value = "=";
		Node *E1 = position->leftChild->leftChild->rightSibling;
		position->leftChild->leftChild->rightSibling = NULL;
		Node *X1 = position->leftChild->leftChild;
		Node *E2 = position->leftChild->rightSibling->leftChild->rightSibling;
		position->leftChild->rightSibling->leftChild->rightSibling = NULL;
		Node *X2 = position->leftChild->rightSibling->leftChild;
		position->leftChild = X2;
		Node *gamma = new Node;
		gamma->value = "gamma";
		Node *lambda = new Node;
		lambda->value = "lambda";
		gamma->leftChild = lambda;
		lambda->rightSibling = E1;
		lambda->leftChild = X1;
		lambda->leftChild->rightSibling = E2;
		position->leftChild->rightSibling = gamma;
	}
	else if (s.compare("rec") == 0) {
		Node *E = position->leftChild->leftChild->rightSibling;
		string x = position->leftChild->leftChild->value;
		position->value = "=";
		position->leftChild = position->leftChild->leftChild;
		Node *trv = position->leftChild;
		trv->rightSibling = new Node;
		trv = trv->rightSibling;
		trv->value = "gamma";
		trv->leftChild = new Node;
		trv = trv->leftChild;
		trv->value = "Y*";
		trv->rightSibling = new Node;
		trv = trv->rightSibling;
		trv->value = "lambda";
		trv->leftChild = new Node;
		trv = trv->leftChild;
		trv->value = x;
		trv->rightSibling = E;
		//trv->leftChild = X;
	}
	else if (s.compare("@") == 0) {
		Node *E2 = position->leftChild->rightSibling->rightSibling;
		position->leftChild->rightSibling->rightSibling = NULL;
		Node *N = position->leftChild->rightSibling;
		position->leftChild->rightSibling = NULL;
		Node *E1 = position->leftChild;
		position->value = "gamma";
		position->leftChild = new Node;
		position->leftChild->value = "gamma";
		position->leftChild->leftChild = N;
		position->leftChild->rightSibling = E2;
		position->leftChild->leftChild->rightSibling = E1;
	}
	else if (s.compare("and") == 0) {
		int count = 1;
		Node *trv = position->leftChild;
		while (trv->rightSibling != NULL) {
			trv = trv->rightSibling;
			++count;
		}
		vector<Node *> identNodes;
		vector<Node *> valNodes;
		for (int i = 0; i < count; ++i) {
			trv = position->leftChild;
			for (int j = 0; j < i; ++j) {
				trv = trv->rightSibling;
			}
			valNodes.push_back(trv->leftChild->rightSibling);
			trv->leftChild->rightSibling = NULL;
			identNodes.push_back(trv->leftChild);
		}
		position->value = "=";
		position->leftChild->value = ",";
		position->leftChild->leftChild = new Node;
		position->leftChild->leftChild->value = identNodes[0]->value;
		trv = position->leftChild->leftChild;
		for (int i = 1; i < count; ++i) {
			trv->rightSibling = new Node;
			trv->rightSibling->value = identNodes[i]->value;
			trv = trv->rightSibling;
		}
		position->leftChild->rightSibling = new Node;
		position->leftChild->rightSibling->value = "tau";
		position->leftChild->rightSibling->leftChild = new Node;
		position->leftChild->rightSibling->leftChild->value = valNodes[0]->value;
		trv = position->leftChild->rightSibling->leftChild;
		for (int i = 1; i < count; ++i) {
			trv->rightSibling = new Node;
			trv->rightSibling = valNodes[i];
			trv = trv->rightSibling;
		}
	}
	else if (s.compare("function_form") == 0) {
		int count = 0;
		Node *trv = position->leftChild;
		while (trv->rightSibling != NULL) {
			trv = trv->rightSibling;
			++count;
		}
		count--;
		trv = position->leftChild;
		for (int i = 0; i < count; ++i) {
			trv = trv->rightSibling;
		}
		Node *E = trv->rightSibling;
		trv->rightSibling = NULL;
		vector<Node *> idNodes;
		for (int i = count; i >0; i--) {
			trv = position->leftChild;
			for (int j = 0; j < i - 1; ++j) {
				trv = trv->rightSibling;
			}
			idNodes.push_back(trv->rightSibling);
			trv->rightSibling = NULL;
		}
		position->value = "=";
		trv = position->leftChild;
		for (int i = count; i > 0; i--) {
			trv->rightSibling = new Node;
			trv->rightSibling->value = "lambda";
			trv->rightSibling->leftChild = idNodes[i - 1];
			trv = trv->rightSibling->leftChild;
		}
		trv->rightSibling = E;
	}
	else if (s.compare("lambda") == 0) {
		int count = 1;
		Node *trv = position->leftChild;
		while (trv->rightSibling != NULL) {
			trv = trv->rightSibling;
			++count;
		}count--;
		trv = position->leftChild;
		for (int i = 0; i < count - 1; ++i) {
			trv = trv->rightSibling;
		}
		Node *E = trv->rightSibling;
		trv->rightSibling = NULL;
		vector<Node *> lidNodes;
		for (int i = count - 1; i > 0; i--) {
			trv = position->leftChild;
			for (int j = 0; j < i - 1; ++j) {
				trv = trv->rightSibling;
			}
			lidNodes.push_back(trv->rightSibling);
			trv->rightSibling = NULL;
		}
		trv = position->leftChild;
		for (int i = count - 1; i >0; i--) {
			trv->rightSibling = new Node;
			trv->rightSibling->value = "lambda";
			trv->rightSibling->leftChild = lidNodes[i - 1];
			trv = trv->rightSibling->leftChild;
		}
		trv->rightSibling = E;
	}
}

Result nextNode(Node *root) {
	Result tempRet;
	if (root->leftChild != NULL) {
		tempRet = nextNode(root->leftChild);
		//return tempRet;
	}
	if (tempRet.value.compare("") == 0) {
		if (root->rightSibling != NULL) {
			tempRet = nextNode(root->rightSibling);
			//return tempRet;
		}
	}
	if (tempRet.value.compare("") == 0) {
		if (trnsfkey(root->value)) {
			if (root->value.compare("lambda") == 0) {
				int count = 1;
				Node *trv = root->leftChild;
				while (trv->rightSibling != NULL) {
					trv = trv->rightSibling;
					++count;
				}count--;
				if (count > 1) {
					tempRet.value = root->value;
					tempRet.pos = root;
				}
			}
			else {
				tempRet.value = root->value;
				tempRet.pos = root;
			}
			return tempRet;
		}
	}
	return tempRet;
}

void subtreeTransformControl() {
	if (myStack.size() == 1) {
		Result res;
		res = nextNode(myStack.top());
		while (res.value.compare("") != 0) {
			//cout << res.value << " : " << res.pos << "\n" << endl;
			transform(res.value, res.pos);
			//res.pos->value = "temp";
			//printTree(myStack.top());
			//cout << "............................." << endl;
			res = nextNode(myStack.top());
		}
	}
}

void populatecss(Node *root) {
	if (root == NULL) {
		return;
	}
	if (root->value.compare("lambda") == 0) {
		Obj *o = new Obj;
		o->value = root->value;
		o->type = "lambda";
		o->index = indexk;
		indexk++;
		if (root->leftChild->value.compare(",") != 0) {
			o->var.push_back(root->leftChild->value);
		}
		else {
			Node *trv = root->leftChild->leftChild;
			o->var.push_back(trv->value);
			while (trv->rightSibling != NULL) {
				trv = trv->rightSibling;
				o->var.push_back(trv->value);
			}
		}
		css[curDelta]->push_back(o);
		cs = new vector<Obj*>;
		curDelta = indexk - 1;
		css.push_back(cs);
		populatecss(root->leftChild->rightSibling); curDelta--;
		populatecss(root->rightSibling);
	}
	else if (root->value.compare("->") == 0) {
		Obj *o = new Obj;
		o->value = "delta2";
		o->type = "delta2";
		o->index = curDelta + 1;
		css[curDelta]->push_back(o);
		o = new Obj;
		o->value = "delta3";
		o->type = "delta3";
		o->index = curDelta + 2;
		css[curDelta]->push_back(o);
		o = new Obj;
		o->value = "beta";
		o->type = "beta";
		css[curDelta]->push_back(o);

		cs = new vector<Obj*>;
		curDelta += 2;
		css.push_back(cs);
		cs = new vector<Obj*>;
		css.push_back(cs);
		populatecss(root->leftChild->rightSibling->rightSibling);
		root->leftChild->rightSibling->rightSibling = NULL;
		curDelta--;
		populatecss(root->leftChild->rightSibling);
		root->leftChild->rightSibling = NULL;
		curDelta--;
		populatecss(root->leftChild);
	}
	else {
		Obj *o = new Obj;
		o->value = root->value;
		if (ident(root->value)) { o->type = "id"; }
		if (integ(root->value)) { o->type = "int"; }
		if (str(root->value)) { o->type = "string"; }
		if (o->value.compare("") == 0) { o->type = o->value; }
		if (o->value.compare("tau") == 0) { o->index = tindex; ++tindex; }
		css[curDelta]->push_back(o);
		populatecss(root->leftChild);
		populatecss(root->rightSibling);
	}
}

void formControlStructs() {
	populatecss(myStack.top());
}

void printControlStructs() {
	cout << "Control Strcutures: " << endl;
	for (int i = 0; i < css.size(); ++i) {
		cout << "Delta" << i << "=> ";
		for (int j = 0; j < css[i]->size(); ++j) {
			if (css[i]->at(j)->value.compare("lambda") == 0 || css[i]->at(j)->value.compare("tau") == 0) {
				cout << css[i]->at(j)->value<< css[i]->at(j)->index << " ";
			}
			else {
				cout << css[i]->at(j)->value << " ";
			}
		}
		cout << endl;
	}
}

void printStack(stack<Obj*> s) {
	for (int i = 0; i < s.size(); ) {
		if (s.top()->value.compare("lambda") == 0 || s.top()->value.compare("e") == 0) {
			cout << s.top()->value << s.top()->index << " ";
		}
		else if (str(s.top()->value)) {
			for (int i = 1; i < s.top()->value.length()-1; ++i) {
				if (!(s.top()->value.at(i) == '\\'))
					cout << s.top()->value.at(i);
				else if (s.top()->value.at(i) == '\\' && s.top()->value.at(i + 1) == 't') { cout << "\t"; ++i; }
				else if (s.top()->value.at(i) == '\\' && s.top()->value.at(i + 1) == 'n') { cout << "\n"; ++i; }
			}
		}
		else {
			cout << s.top()->value;
		}
		s.pop();
	}
	cout << endl;
}

void pushcs(int c) {
	for (int i = 0; i < css[c]->size(); ++i) {
		control.push(css[c]->at(i));
	}
}

void call(string frator, string frand) {
	if (frator.compare("print") == 0 || frator.compare("Print") == 0) {
		//state.pop();
		Obj *o = state.top();
		state.pop();
		o->value = mye[curEnv - 1]->v[0];
		state.push(o);
	}
}

void u(string c, string s) {
	if (c.compare("not") == 0) {
		if (s.compare("true") == 0 || s.compare("True") == 0) {
			Obj *o = new Obj;
			o->value = "false";
			state.push(o);
		}
		else if (s.compare("false") == 0 || s.compare("False") == 0) {
			Obj *o = new Obj;
			o->value = "true";
			state.push(o);
		}
	}
	else if (c.compare("neg") == 0) {
		Obj *o = new Obj;
		int i = stoi(s);
		i = 0 - i;
		string s = to_string(i);
		o->value = s;
		state.push(o);
	}
}

void b(string c, string s0, string s1) {
	if (c.compare("+") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		i = i + j;
		string s = to_string(i);
		Obj *o = new Obj;
		o->value = s;
		state.push(o);
	}
	else if (c.compare("-") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		i = i - j;
		string s = to_string(i);
		Obj *o = new Obj;
		o->value = s;
		state.push(o);
	}
	else if (c.compare("*") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		i = i * j;
		string s = to_string(i);
		Obj *o = new Obj;
		o->value = s;
		state.push(o);
	}
	else if (c.compare("/") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		i = i / j;
		string s = to_string(i);
		Obj *o = new Obj;
		o->value = s;
		state.push(o);
	}
	else if (c.compare("**") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		i = pow(i, j);
		string s = to_string(i);
		Obj *o = new Obj;
		o->value = s;
		state.push(o);
	}
	else if (c.compare("gr") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		Obj *o = new Obj;
		if (i > j)
			o->value = "true";
		else
			o->value = "false";
		state.push(o);
	}
	else if (c.compare("ge") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		Obj *o = new Obj;
		if (i >= j)
			o->value = "true";
		else
			o->value = "false";
		state.push(o);
	}
	else if (c.compare("ls") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		Obj *o = new Obj;
		if (i < j)
			o->value = "true";
		else
			o->value = "false";
		state.push(o);
	}
	else if (c.compare("le") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		Obj *o = new Obj;
		if (i <= j)
			o->value = "true";
		else
			o->value = "false";
		state.push(o);
	}
	else if (c.compare("ne") == 0) {
		int i = stoi(s0);
		int j = stoi(s1);
		Obj *o = new Obj;
		if (i != j)
			o->value = "true";
		else
			o->value = "false";
		state.push(o);
	}
	else if (c.compare("or") == 0) {
		Obj *o = new Obj;
		o->value = "false";
		if (s0.compare("true") == 0 || s1.compare("true") == 0)
			o->value = "true";
		state.push(o);
	}
	else if (c.compare("&") == 0) {
		Obj *o = new Obj;
		o->value = "false";
		if (s0.compare("true") == 0 && s1.compare("true") == 0)
			o->value = "true";
		state.push(o);
	}
}

void applyCSE() {
	if (c > 100)
		return;
	while (!(control.empty())) {
		++c;
		if (control.top()->value.compare("e") == 0) {
			Obj *o = new Obj;
			o = state.top();
			state.pop();
			if (control.top()->index == state.top()->index) {
				control.pop(); state.pop();
				state.push(o);
			}
		}
		else if (bOpe(control.top()->value)) {
			string c0 = control.top()->value;
			control.pop();
			string s0 = state.top()->value;
			state.pop();
			string s1 = state.top()->value;
			state.pop();
			b(c0, s0, s1);
		}
		else if (uOpe(control.top()->value)) {
			string c0 = control.top()->value;
			control.pop();
			string s0 = state.top()->value;
			state.pop();
			u(c0, s0);
		}
		else if (control.top()->value.compare("beta") == 0) {
			control.pop();
			if (state.top()->value.compare("true") == 0) {
				state.pop();
				control.pop();
				int k = control.top()->index;
				control.pop();
				pushcs(k);
			}
			else if (state.top()->value.compare("false") == 0) {
				state.pop();
				int k = control.top()->index;
				control.pop();
				control.pop();
				pushcs(k);
			}
		}
		else if (ident(control.top()->value) || integ(control.top()->value) || str(control.top()->value)) {
			Obj *o;
			o = control.top();
			control.pop();
			state.push(o);
			if (ident(state.top()->value)) {
				if (!(mye.empty())) {
					int size = mye.at(0)->i.size();
					for (int c = 0; c < size; ++c) {
						if (mye.at(curDelta)->i.at(c).compare(state.top()->value)==0) {
							state.top()->value = mye.at(curDelta)->v.at(c);
						}
					}
				}
			}
		}
		else if (control.top()->value.compare("lambda") == 0) {
			Obj *o = control.top();
			control.pop();
			o->cenv = curEnv;
			state.push(o);
		}
		else if (control.top()->value.compare("gamma") == 0 && state.top()->value.compare("lambda") == 0) {
			int k = state.top()->index;
			vector<string> temp = state.top()->var;
			int c = state.top()->cenv;
			control.pop(); state.pop();
			string rand = state.top()->value;
			state.pop();

			e *t = new e;
			t->cur = k;
			t->prev = curEnv;
			t->i = temp;
			t->v.push_back(rand);

			mye.push_back(t);
			curEnv = nextEnv;
			++nextEnv;
			Obj *ev = new Obj;
			ev->value = "e";
			ev->index = curEnv;
			control.push(ev);
			state.push(ev);
			pushcs(k);
		}
		else if (control.top()->value.compare("gamma") == 0 && state.top()->value.compare("lambda") != 0) {
			string oper = state.top()->value;
			state.pop();
			string orand = state.top()->value;
			control.pop();
			//state.pop();
			call(oper, orand);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printUsage();
	}
	else {
		string s = "-ast";
		if (s.compare(argv[1]) == 0) {
			fs.open(argv[2], fstream::in);

			printAST();
		}
		else {
			printUsage();
		}
	}
	//printTree(myStack.top());		//Prints the AST

	//call a control procedure that does the partial standardization and puts the standardized tree on the top of the stack
	subtreeTransformControl();
	printTree(myStack.top());		//prints the partially standardized tree
	
	css.push_back(cs);				//Creates the initial control structure delta0
	
	//Forms the control structures and places them in the vector of vectors 'css'
	formControlStructs();
	
	//printControlStructs();		//Prints all the control structures
	
	Obj *e0 = new Obj;
	e0->value = "e";
	e0->index = 0;
	control.push(e0);			//pushing e0 on to the control stack
	pushcs(0);					//pushing delta0 on to the control stack
	state.push(e0);				//pushing e0 on to the state stack
	//printStack(control);
	//printStack(state);
	applyCSE();
	if(!(state.empty()) && printpresent==1)
		printStack(state);
	return 0;
}



