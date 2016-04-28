/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  dream <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef RBTREE_NODE_H
#define RBTREE_NODE_H

#include "binarySearchTree.hpp"

namespace face2wind
{
  template<typename T>
  struct RBTreeNode : public BinaryTreeNode<T>
  {
    char color;

    RBTreeNode():color('r'){}
    RBTreeNode(const T &data):BinaryTreeNode<T>(data),color('r'){}
    virtual ~RBTreeNode(){}
    virtual void showElementName(){
      std::cout<<BinaryTreeNode<T>::element<<"("<<color<<")";
    }
  };
  
  template<typename T>
  class RBTree : public BinarySearchTree<T>
  {
  protected:
    bool isRed(RBTreeNode<T>* node){
      if(NULL == node)
	return false;
      return node->color=='r';
    }
    bool isBlack(RBTreeNode<T>* node){
      if(NULL == node)
	return false;
      return node->color=='b';
    }
    bool isRed(BinaryTreeNode<T>* node){
      RBTreeNode<T>* rbNode = static_cast<RBTreeNode<T>* >(node);
      return isRed(rbNode);
    }
    bool isBlack(BinaryTreeNode<T>* node){
      RBTreeNode<T>* rbNode = static_cast<RBTreeNode<T>* >(node);
      return isBlack(rbNode);
    }
    void copyColor(BinaryTreeNode<T>* src, BinaryTreeNode<T>* dsc){
      if(NULL == src || NULL == dsc)
	return;
      static_cast<RBTreeNode<T>* >(dsc)->color = static_cast<RBTreeNode<T>* >(src)->color;
    }
    void setRed(RBTreeNode<T>* node){
      if(NULL == node)
	return;
      node->color='r';
    }
    void setBlack(RBTreeNode<T>* node){
      if(NULL == node)
	return;
      node->color='b';
    }
    void setRed(BinaryTreeNode<T>* node){
      RBTreeNode<T>* rbNode = static_cast<RBTreeNode<T>* >(node);
      setRed(rbNode);
    }
    void setBlack(BinaryTreeNode<T>* node){
      RBTreeNode<T>* rbNode = static_cast<RBTreeNode<T>* >(node);
      setBlack(rbNode);
    }

    bool isLeftChild(RBTreeNode<T>* node){
      BinaryTreeNode<T> *bNode = static_cast<BinaryTreeNode<T> *>(node);
      return BinaryTree<T>::isLeftChild(bNode);
    }
    bool isRightChild(RBTreeNode<T>* node){
      BinaryTreeNode<T> *bNode = static_cast<BinaryTreeNode<T> *>(node);
      return BinaryTree<T>::isRightChild(bNode);
    }

    int rotateLeft(RBTreeNode<T>* node){
      BinaryTreeNode<T>* rbNode = static_cast<BinaryTreeNode<T>* >(node);
      return BinaryTree<T>::rotateLeft(rbNode);
    }
    int rotateRight(RBTreeNode<T>* node){
      BinaryTreeNode<T>* rbNode = static_cast<BinaryTreeNode<T>* >(node);
      return BinaryTree<T>::rotateRight(rbNode);
    }

    BinaryTreeNode<T> *getBrother(BinaryTreeNode<T> *node){
      if(NULL == node)
	return NULL;
      if(NULL == node->fatherNode)
	return NULL;
      if(node == node->fatherNode->leftChild)
	return node->fatherNode->rightChild;
      if(node == node->fatherNode->rightChild)
	return node->fatherNode->leftChild;
      return NULL;
    }
    
    void fixInsert(RBTreeNode<T>* node);
    void fixRemove(BinaryTreeNode<T>* node);

  public:
    RBTree(){}
    virtual ~RBTree(){}

    virtual int insert(const T &data);
    virtual int remove(const T &data);
  };

  template<typename T>
  int RBTree<T>::insert(const T &data)
  {
    RBTreeNode<T> *newNode = new RBTreeNode<T>(data);
    //newNode->element = data;
    if(NULL == BinaryTree<T>::rootNode)
      {
	BinaryTree<T>::rootNode = newNode;
	//std::cout<<"insert "<<data<<" into root\n";
      }
    else
      BinarySearchTree<T>::doInsertNode(newNode, BinaryTree<T>::rootNode);
    fixInsert(newNode);
    return 0;
  }

  template<typename T>
  void RBTree<T>::fixInsert(RBTreeNode<T>* node)
  {
    if(NULL == node)
      return;

    // current insert was root node , set it to black and return
    if(node == BinaryTree<T>::rootNode)
      {
	setBlack(node);
	return;
      }
    
    // father is black , tree are stil RB tree , do nothing
    if(isBlack(node->fatherNode))
      return;

    // then father node is red ....
    
    // uncle is red node ( father aleady red node) ,
    // set father and uncle to Black , and set grandfather to Red
    // fix the grandfather node
    if(isRed( getBrother(node->fatherNode) ) ) 
      {
	setBlack(node->fatherNode);
	setBlack(getBrother(node->fatherNode));
	setRed(node->fatherNode->fatherNode);
	fixInsert(static_cast<RBTreeNode<T>*>(node->fatherNode->fatherNode));
	return;
      }
    else // uncle is black node or NULL
      {
	//node and father node was all left child ,
	//set father to Black and grandfather to Red, 
	//and right rotate the grandfather node
	if(BinaryTree<T>::isLeftChild(node) && BinaryTree<T>::isLeftChild(node->fatherNode) )
	  {
	    setBlack(node->fatherNode);
	    setRed(node->fatherNode->fatherNode);
	    BinaryTree<T>::rotateRight(node->fatherNode->fatherNode);
	  }
	//node and father node was all right child ,
	//set father to Black and grandfather to Red,
	//and left rotate the grandfather node
	else if(BinaryTree<T>::isRightChild(node) && BinaryTree<T>::isRightChild(node->fatherNode) )
	  {
	    setBlack(node->fatherNode);
	    setRed(node->fatherNode->fatherNode);
	    BinaryTree<T>::rotateLeft(node->fatherNode->fatherNode);
	  }
	//node was left child and father was right,
	//set node to Black and set grandfather to Red
	//then right rotate the father, at last left rotate the father
	else if(BinaryTree<T>::isLeftChild(node) && BinaryTree<T>::isRightChild(node->fatherNode) )
	  {
	    setBlack(node);
	    setRed(node->fatherNode->fatherNode);
	    BinaryTree<T>::rotateRight(node->fatherNode);
	    BinaryTree<T>::rotateLeft(node->fatherNode);
	  }
	//node was right child and father was left,
	//set node to Black and set grandfather to Red 
	//then left rotate the father, at last right rotate the father
	else if(BinaryTree<T>::isRightChild(node) && BinaryTree<T>::isLeftChild(node->fatherNode) )
	  {
	    setBlack(node);
	    setRed(node->fatherNode->fatherNode);
	    BinaryTree<T>::rotateLeft(node->fatherNode);
	    BinaryTree<T>::rotateRight(node->fatherNode);
	  }
      }
    // always set root to black to keep the Red-Black Tree
    setBlack(BinaryTree<T>::rootNode);
  }
  
#define _TO_RBTREE_NODE(i) static_cast<RBTreeNode<T> *>(i)

  template<typename T>
  int RBTree<T>::remove(const T &data)
  {

    RBTreeNode<T> *node = _TO_RBTREE_NODE(BinarySearchTree<T>::findNode(data));
    if(NULL == node)
      return 1;
    
    RBTreeNode<T> *needDeleteNode = node;
    RBTreeNode<T> *deleteNodeSuccessor = NULL;
    if(NULL == node->leftChild || NULL == node->rightChild)
      needDeleteNode = node;
    else  // after this needDeleteNode has no right child
      needDeleteNode = _TO_RBTREE_NODE(BinarySearchTree<T>::maxTreeNode(node->leftChild));
    deleteNodeSuccessor = _TO_RBTREE_NODE(needDeleteNode->leftChild);
    if(NULL != deleteNodeSuccessor)
      deleteNodeSuccessor->fatherNode = needDeleteNode->fatherNode;
    if(NULL == needDeleteNode->fatherNode)
      BinaryTree<T>::rootNode = deleteNodeSuccessor;
    else if(BinaryTree<T>::isLeftChild(needDeleteNode))
      needDeleteNode->fatherNode->leftChild = deleteNodeSuccessor;
    else
      needDeleteNode->fatherNode->rightChild = deleteNodeSuccessor;
    
    if(node != needDeleteNode)
      node->element = needDeleteNode->element;
      
    if(isBlack(needDeleteNode))
      fixRemove(_TO_B_TREE_NODE(deleteNodeSuccessor));

    return 0;
  }

  template<typename T>
  void RBTree<T>::fixRemove(BinaryTreeNode<T>* node)
  {
    BinaryTreeNode<T>* tmpNode;
    while(node != BinaryTree<T>::rootNode && isBlack(node))
      {
	tmpNode = getBrother(node);
	if(BinaryTree<T>::isLeftChild(node)) {
	    if(isRed(tmpNode)) { // brother is red node
	      setBlack(tmpNode);
	      setRed(node->fatherNode);
	      BinaryTree<T>::rotateLeft(node->fatherNode);
	      tmpNode = getBrother(node); // get new brother
	    }
	    if(isBlack(tmpNode->leftChild) && isBlack(tmpNode->rightChild)) {
	      setRed(tmpNode);
	      node = node->fatherNode;
	    }
	    else if(isBlack(tmpNode->rightChild)) { // brother's left child is red
	      setBlack(tmpNode->leftChild);
	      setRed(tmpNode);
	      BinaryTree<T>::rotateRight(tmpNode);
	      tmpNode = getBrother(node);
	    }
	    else { // brother's child all red
	      _TO_RBTREE_NODE(tmpNode)->color =
		_TO_RBTREE_NODE(node->fatherNode)->color;
	      setBlack(node->fatherNode);
	      setBlack(tmpNode->rightChild);
	      BinaryTree<T>::rotateLeft(node->fatherNode);
	      node = BinaryTree<T>::rootNode;
	    }
	}
	else //same as then clause with "right" and "left" exchanged
	  {
	    if(isRed(tmpNode)) {
	      setBlack(tmpNode);
	      setRed(node->fatherNode);
	      BinaryTree<T>::rotateRight(node->fatherNode);
	      tmpNode = getBrother(node);
	    }
	    if(isBlack(tmpNode->rightChild) &&
	       isBlack(tmpNode->leftChild)) {
	      setRed(tmpNode);
	      node = node->fatherNode;
	    }
	    else if(isBlack(tmpNode->leftChild)) {
	      setBlack(tmpNode->rightChild);
	      setRed(tmpNode);
	      BinaryTree<T>::rotateLeft(tmpNode);
	      tmpNode = getBrother(node);
	    }
	    else {
	      _TO_RBTREE_NODE(tmpNode)->color =
		_TO_RBTREE_NODE(node->fatherNode)->color;
	      setBlack(node->fatherNode);
	      setBlack(tmpNode->leftChild);
	      BinaryTree<T>::rotateRight(node->fatherNode);
	      node = BinaryTree<T>::rootNode;
	    }
	  }
      }
  }

} // namespace end

#endif // RBTREE_NODE_H

/*
example:

void removeNode(face2wind::BinaryTree<int> &a, int num)
{
  a.remove(num);
  std::cout<<"after remove "<<num<<" :"<<std::endl;
  a.showTree();
}

int main(int argc, char **argv)
{
  using std::cout;
  using std::endl;
  using std::vector;
  vector<int> data;
  face2wind::RBTree<int> a;
  srand((unsigned)time(0));
  for(int i = 0; i < 20 ; ++i){
    int num = rand()%100;
    data.push_back(num);
    a.insert(num);
  }
  a.showTree();
  for(int i = 0; i < 5 ; ++i)
    removeNode(a,data[i*2]);
  
  return 0;
}
*/
