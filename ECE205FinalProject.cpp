//ECE 205 Final Project
#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;

struct Node {
    int value; //Stores either integer value or an operator as an ASCII value
    bool isOp; //True if node is an operator, false if an operand
    Node* left; //Pointer to left child
    Node* right; //Pointer to right child
    //Constructor to initialize a node
    Node(int v, bool op) : value(v), isOp(op), left(nullptr), right(nullptr) {}
};
//Check if a character is an arithmetic operator
bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}
//Returns the precedence level of an operator
//Multiplcation and Division have higher precedence than addition and subtraction
int precedence(char c)
{
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}
//Converts infix expression into a postfix expression
string infixToPostfix(string s) {
    stack<char> st; //Stack used to hold operators and parenthses
    string result; //Stores the final postfix result
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        //Ignore space
        if (c == ' ') continue;
        //If the character is a letter or number, add to the result
        if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9') {
            result += c;
            if (i + 1 >= s.length() || !(s[i + 1] >= '0' && s[i + 1] <= '9')) {
                result += ' ';
            }
        }
        //If the character is an opening parathesis, push onto the stack
        else if (c == '(') {
            st.push('(');
        }
        //If the character is a closing parenthesis the pop operators unitl the matching opening parenthesis is found
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                result += st.top();
                result += ' ';
                st.pop();
            }
            //Remove the opening parenthesis from the stack
            st.pop();
        }
        //If the character is an operator
        else {
            while (!st.empty() && precedence(s[i]) <= precedence(st.top())) {
                result += st.top();
                result += ' ';
                st.pop();
            }
            //Push the current operator onto the stack
            st.push(c);
        }
    }
    //Pop any remaining operators from the stack
    while (!st.empty()) {
        result += st.top();
        result += ' ';
        st.pop();
    }

    return result;
}
//Constructs a binary expression tree from a postfix expression
Node* constructTree(string postfix) {
    stack<Node*> st; //Stack used to store tree nodes
    Node* t, * t1, * t2; //Temporary ndoe pointers

    stringstream ss(postfix);
    string token;

    //Read each token from the postfix expression
    while (ss >> token) {
        //If token is an operator, create an operator node
        if (token.length() == 1 && isOperator(token[0])) {
            t = new Node((int)token[0], true);
            //First popped node becomes right child
            t1 = st.top();
            st.pop();
            //Second popped node becomes the left child
            t2 = st.top();
            st.pop();

            t->right = t1;
            t->left = t2;
            //Push the new subtree into the stack
            st.push(t);
        }
        //If the token is a number, create an operand node
        else {
            int num = stoi(token);
            t = new Node(num, false);
            st.push(t);
        }
    }
    //Last node left on the stack is the root of the tree
    t = st.top();
    st.pop();

    return t;
}
//Perforsm a postorder traversal of the binary tree
void postorderTraversal(Node* root) {
    if (root == nullptr) return;
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    //Print operator or operand depending on the node type
    if (root->isOp)
        cout << (char)root->value << " ";
    else
        cout << root->value << " ";
}

//Prints the binary expression tree
void printTree(Node* root, int space = 0, int height = 6) {
    if (root == nullptr) return;
    //Increase spacing between levels
    space += height;
    //Print the right subtree first
    printTree(root->right, space);
    cout << endl;
    //Print spaces before the current node
    for (int i = height; i < space; i++) cout << " ";
    //Print operator or operand 
    if (root->isOp)
        cout << (char)root->value << "\n";
    else
        cout << root->value << "\n";
    //Print the left subtree
    printTree(root->left, space);
}

//Evaluate a postfix expression using a stack
int evaluatePostfix(string postfix) {
    stack<int> S; //Stack used to store operands
    stringstream ss(postfix); //Reads the postfic token by token
    string token;
    //Read each token from the postfix expression
    while (ss >> token) {
        if (token.length() == 1 && isOperator(token[0])) {
            int op2 = S.top(); S.pop();
            int op1 = S.top(); S.pop();
            int res = 0;

            switch (token[0]) {
            case '+': res = op1 + op2; 
                break;
            case '-': res = op1 - op2; 
                break;
            case '*': res = op1 * op2; 
                break;
            case '/':
                //Prevents division by zero
                if (op2 == 0) throw runtime_error("Division by zero.");
                res = op1 / op2;
                break;
            }
            //Push the result back onto the stack
            S.push(res);
        }
        //If the token is a number, push it onto the stack
        else {
            S.push(stoi(token));
        }
    }
    //The final value left on the stack is the evaluated result
    return S.top();
}

void runExpression(string exp) {
    //Print the original infix expression
    cout << "The program reads an ordinary infix arithmetic expression with integers and operators:" << endl;
    cout << exp << endl;
    //Convert infix expression to postfix
    string postfix = infixToPostfix(exp);
    //Build the binary expression tree from postfix
    Node* r = constructTree(postfix);
    //Print the postfix expression using postorder traversal
    cout << "\na) does a postorder traversal of the binary tree and writes it out as a postfix expression:" << endl;
    postorderTraversal(r);
    cout << endl;
    //Print the binary expression tree
    cout << "\nb) converts it into a binary tree:" << endl;
    printTree(r);
    //Evaluate the postfix expression using a stack
    int result = evaluatePostfix(postfix);
    //Print the evaluted result
    cout << "\nc) uses a stack to evaluate the binary tree expression" << endl;
    cout << result << endl;

    cout << endl;
}

int main() {
    //Sample test expressions for inputs 
    runExpression("( 16 + 2 ) * 5 - 28 / 4");
    runExpression("( 8 + 1 ) * 3 - 8 / 4");
    runExpression("3 + 4 * 2");
    runExpression("( 2 + 3 ) * ( 4 - 1 )");
    return 0;
}