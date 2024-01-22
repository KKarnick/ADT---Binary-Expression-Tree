
#include "ExpressionTree.h"

template <typename DataType>
ExprTree<DataType>::ExprTreeNode::ExprTreeNode ( char elem, ExprTreeNode *leftPtr, ExprTreeNode *rightPtr ){
	   //set all node values to passed parameters
	dataItem = elem;
	left = leftPtr;
	right = rightPtr;
}

template <typename DataType>
ExprTree<DataType>::ExprTree (){
	//create empty tree
	root = 0;
}

template <typename DataType>
ExprTree<DataType>::ExprTree(const ExprTree& source){
	root = source.root;
	    //call private function
	copyHelper(root);
}

template <typename DataType>
ExprTree<DataType>& ExprTree<DataType>::operator=(const ExprTree& source){
	if (this != &source) {  //ensure not already pointing to same tree
		clear();  //clear current tree (deallocate memory)
		root = source.root;
		   //call private function
		copyHelper(root);
	}
	       //return reference to current tree in either case 
	return this;
}

template <typename DataType>
ExprTree<DataType>::~ExprTree (){
	clear();  //deallocate space for current nodes
}

template <typename DataType>
void ExprTree<DataType>::build (){
	//start by clearing current tree if one exists
	clear();
	    //call private function
	buildHelper(root);
}

template <typename DataType>
void ExprTree<DataType>::expression () const{
	  //needs to output infix (regularly used math expressions)
	      //call private function
	exprHelper(root);
}

template <typename DataType>
DataType ExprTree<DataType>::evaluate() const throw (logic_error){
	   //returns evaluation of expression  (calls private function)
	if (isEmpty())
		throw logic_error("Tree is empty.");
	else 
		return evalHelper(root);
	

}

template <typename DataType>
void ExprTree<DataType>::clear (){
	  //call private function
	clearHelper(root);
	  //declare tree as empty (root = null)
	root = 0;
}

template <typename DataType>
void ExprTree<DataType>::commute(){
	  //switches left & right children at every node except leaves
	  //call private function
	commuteHelper(root);
}

template <typename DataType>
bool ExprTree<DataType>::isEquivalent(const ExprTree& source) const{
	   //evaluates if trees are same, NOT result!
	
	if (this == &source)  //same tree
		return true;
	else if (isEmpty() && source.isEmpty())  //both empty trees (so equivalent as 0=0)
		return true;
	else {
		  //call private function to recursively traverse trees and check for equivalence
		return isEquivHelper(root, source.root);
	}
}

template <typename DataType>
bool ExprTree<DataType>::isEmpty() const{
	if (root == 0)
		return true;
	else
		return false;
}

#include "show8.cpp"

template <typename DataType>
void ExprTree<DataType>::copyHelper(ExprTreeNode*& p) {
	   //starts with p being equal to root; if not null, then creates new node to add to "new" tree
	if (p != 0) {
		p = new ExprTreeNode(p->dataItem, p->left, p->right);
		    //traverse subtrees and recursively add nodes
		copyHelper(p->left);
		copyHelper(p->right);
	}
}

template<typename DataType>
void ExprTree<DataType>::buildHelper(ExprTreeNode*& p) {
	char ch; //input (can be operand (number only here) or operator) 
	   //get input from user (node value)
	cin >> ch;  
	p = new ExprTreeNode(ch, 0, 0);
	  /*identify type of input
	    if it is a digit, have reached leaf and can return
		if it is an operator, continue down subtrees until leaves are established in tree
		*/
	if (!isdigit(ch)) {
		buildHelper(p->left);
		buildHelper(p->right);
	}
}
/*
template<>
void ExprTree<bool>::buildHelper(ExprTreeNode*& p) {
	char ch; //input (can be operand (number only here) or operator) 
   //get input from user (node value)
	cin >> ch;
	p = new ExprTreeNode(ch, 0, 0);
	//identify type of input
	//  if it is a digit, have reached leaf and can return
	 // if it is an operator, continue down subtrees until leaves are established in tree
	  
	if (!isdigit(ch)) {
		buildHelper(p->left);
		buildHelper(p->right);
	}
}
*/
template <typename DataType>
void ExprTree<DataType>::exprHelper(ExprTreeNode *p) const {
	//check if node is null
	if (p != 0){
		if (!isdigit(p->dataItem)) { //if an operator
			cout << "(";  //open expression
			exprHelper(p->left); //traverse to left subtree
			cout << p->dataItem;   //output dataItem at node (operator)
			exprHelper(p->right);  //traverse right subtree
			cout << ")";    //close expression
		}
		else
			cout << p->dataItem;  //output dataItem (operand)
    }
}

template <typename DataType>
float ExprTree<DataType>::evalHelper(ExprTreeNode* p) const {
	float endResult;   //will hold final evaluation result
	float leftSide;   //will hold left val
	float rightSide;   //will hold right val

	if (isdigit(p->dataItem))  //if numerical, cast to appropriate type
		endResult = float(p->dataItem - '0');  //cast to DataType used
	else {  //else it's an operator
		    //recursively get values of each subtree
		leftSide = evalHelper(p->left);  
		rightSide = evalHelper(p->right);
		    //evaluate w/given operator
		if (p->dataItem == '-')
			endResult = (leftSide - rightSide);
		else if(p->dataItem == '+')
			endResult = (leftSide + rightSide);
		else if (p->dataItem == '*')
			endResult = (leftSide * rightSide);
		else if (p->dataItem == '/') 
			endResult = (leftSide / rightSide);
			
	}
	return endResult;  //return operand or expression evaluation to caller (recursively)
}
/*template<>
bool ExprTree<bool>::evalHelper(ExprTreeNode* p) const {
	bool endResult;   //will hold final evaluation result
	bool leftSide;   //will hold left val
	bool rightSide;   //will hold right val

	if (isdigit(p->dataItem))  //if numerical, cast to appropriate type
		endResult = bool(p->dataItem);  //cast to DataType used
	else {  //else it's an operator
			//recursively get values of each subtree
		leftSide = evalHelper(p->left);
		if (p->dataItem == '!')
			return !leftSide;
		rightSide = evalHelper(p->right);
		//evaluate w/given operator
		if (p->dataItem == '-')
			endResult = (leftSide || rightSide);
		else if (p->dataItem == '+')
			endResult = (leftSide && rightSide);
		else if (p->dataItem == '^')
			endResult = (leftSide != rightSide);
	}
	return endResult;  //return operand or expression evaluation to caller (recursively)
}
*/
template <typename DataType>
bool ExprTree<DataType>::isEquivHelper(const ExprTreeNode* r, const ExprTreeNode* o) const {
	//case 1: both nodes are null
	if (r == 0 && o == 0)
		return true;
	//case 2: one is null and other is not
	else if ((r == 0 && o != 0) || (r != 0 && o == 0))
		return false;
	//case 3: dataVals not same
	else if (r->dataItem != o->dataItem)
		return false;
	//case 4: nodes dataItems are equiv (and not null) -- compare values
	else {
		bool eval1 = false;  //temp needed in case equiv is checked
		        //check left & right subtrees
		eval1 = ( isEquivHelper(r->left, o->left) && isEquivHelper(r->right, o->right) );
		if (eval1 == false) {
			   //check for commutable operators
			if (r->dataItem == '*' || r->dataItem == '+') {
				//check if equiv by commute property
				eval1 = (isEquivHelper(r->left, o->right) && isEquivHelper(r->right, o->left));
			}
		}
		return eval1;
	}
}

template <typename DataType>
void ExprTree<DataType>::clearHelper(ExprTreeNode* p) {
	//recursively traverse tree and delete nodes
	if (p != nullptr) {
		clearHelper(p->left);
		clearHelper(p->right);
		delete p;
	}
	//returns when left & right of leaves have been evaluated to be null
}

template <typename DataType>
void ExprTree<DataType>::commuteHelper(ExprTreeNode* p) {
	    //pointer to hold value during swap
	ExprTreeNode* temp;
	if (p != 0) {
		//case 1: leaf node (no children) 
		if ((p->left == 0) && (p->right == 0))
			return;
		//case 2: only left child is null
		else if ((p->left == 0) && (p->right != 0)) {
			p->left = p->right;
			p->right = 0;
		}
		//case 3: only right child is null
		else if ((p->left != 0) && (p->right = 0)) {
			p->right = p->left;
			p->left = 0;
		}
		//case 4: both children not null
		else {
			temp = p->left;
			p->left = p->right;
			p->right = temp;
		}
	}
}