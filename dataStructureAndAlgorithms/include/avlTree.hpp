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

#ifndef AVL_TREE_NODE_H
#define AVL_TREE_NODE_H

#include "binarySearchTree.hpp"

namespace face2wind
{

#define _TO_AVL_TREE_NODE(i)  static_cast<AVLTreeNode<T>*>(i)
  
  template<typename T>
  struct AVLTreeNode : public BinaryTreeNode<T>
  {
    AVLTreeNode(){}
    AVLTreeNode(const T &data):BinaryTreeNode<T>(data){}
    virtual ~AVLTreeNode(){}

    int hight(){
      int l = 0;
      int r = 0;
      if(NULL != BinaryTreeNode<T>::leftChild)
	l = _TO_AVL_TREE_NODE(BinaryTreeNode<T>::leftChild)->hight();
      if(NULL != BinaryTreeNode<T>::rightChild)
	r = _TO_AVL_TREE_NODE(BinaryTreeNode<T>::rightChild)->hight();
      return l>r?
	l+1:
	r+1;
    }
    int balanceFactor(){
      int l = 0;
      int r = 0;
      if(NULL != BinaryTreeNode<T>::leftChild)
        l = _TO_AVL_TREE_NODE(BinaryTreeNode<T>::leftChild)->hight();
      if(NULL != BinaryTreeNode<T>::rightChild)
        r = _TO_AVL_TREE_NODE(BinaryTreeNode<T>::rightChild)->hight();
      return l-r;
    }
  };

  template<typename T>
  class AVLTree : public BinarySearchTree<T>
  {
  protected:
    int doInsertNode(AVLTreeNode<T> *newNode, AVLTreeNode<T> *node);
    
  public:
    AVLTree(){}
    virtual ~AVLTree(){}

    virtual int insert(const T &data);
    virtual int remove(const T &data);
  protected:
    BinaryTreeNode<T>* doRemove(BinaryTreeNode< T >* rNode, BinaryTreeNode< T >* node);
  protected:
    int rebalance(AVLTreeNode< T >* node);
  };
  
  template<typename T>
  int AVLTree<T>::insert(const T &data)
  {
    AVLTreeNode<T> *newNode = new AVLTreeNode<T>(data);
    if(NULL == BinaryTree<T>::rootNode){
      BinaryTree<T>::rootNode = newNode;
    }
    else
      {
	BinarySearchTree<T>::doInsertNode(newNode, _TO_AVL_TREE_NODE(BinaryTree<T>::rootNode) );
	rebalance(newNode);
      }
    return 0;
  }
  
  template<typename T>
  int AVLTree<T>::doInsertNode(AVLTreeNode<T> *newNode,
			       AVLTreeNode<T> *node)
  {
    if(node->element == newNode->element)
      return 0;
    
    if(node->element < newNode->element)
      {
	if(NULL != node->rightChild)
	  doInsertNode(newNode, _TO_AVL_TREE_NODE(node->rightChild));
	else{
	  node->rightChild = newNode;
	  newNode->fatherNode = _TO_B_TREE_NODE(node);
	}
	if(-2 == node->balanceFactor()){
	  if(newNode->element < node->rightChild->element){ // RL =========
	    BinaryTree<T>::rotateRight(node->rightChild);
	    BinaryTree<T>::rotateLeft(node);
	  }
	  else // RR ==============
	    BinaryTree<T>::rotateLeft(node);
	}
      }
    else
      {
	if(NULL != node->leftChild)
	  doInsertNode(newNode, _TO_AVL_TREE_NODE(node->leftChild));
	else{
	  node->leftChild = newNode;
	  newNode->fatherNode = _TO_B_TREE_NODE(node);
	}
	if(2 == node->balanceFactor()){
	  if(newNode->element < node->leftChild->element) // LL ======
	    BinaryTree<T>::rotateRight(node);
	  else{ // LR ==========
	    BinaryTree<T>::rotateLeft(node->rightChild);
	    BinaryTree<T>::rotateRight(node);
	  }
	}
      }
    return 0;
  }
  
  template<typename T>
  int AVLTree<T>::remove(const T &data)
  {
    AVLTreeNode<T> *node = static_cast<AVLTreeNode<T>*>(BinarySearchTree<T>::findNode(data));
    if(NULL == node)
      return 1;
    
    rebalance(_TO_AVL_TREE_NODE(doRemove(node, BinaryTree<T>::rootNode) ) );
    return 0;
  }
  
  template<typename T>
  BinaryTreeNode<T> * AVLTree<T>::doRemove(BinaryTreeNode< T >* rNode, BinaryTreeNode< T >* node)
  {
    if(NULL == rNode)
      return NULL;
    BinaryTreeNode<T> *tmpNode = NULL;
    BinaryTreeNode<T> *deleteFather = NULL; // need delete node's fatherNode
    deleteFather = rNode->fatherNode;
    if(NULL == rNode->leftChild && NULL == rNode->rightChild) // is leaf node
      {
	if(rNode == BinaryTree<T>::rootNode) // is root
	  BinaryTree<T>::rootNode = NULL;
	else if(rNode->fatherNode->leftChild == rNode)
	  rNode->fatherNode->leftChild = NULL;
	else // (rNode->fatherNode->rightChild == rNode)
	  rNode->fatherNode->rightChild = NULL;
	delete rNode;
      }
    else if(NULL != rNode->leftChild && NULL != rNode->rightChild)
      {
	tmpNode = BinarySearchTree<T>::minTreeNode(rNode->rightChild);
	T tmpElement = tmpNode->element;
	tmpNode->element = rNode->element;
	rNode->element = tmpElement;
	return doRemove(tmpNode, node);
      }
    else
      {
	tmpNode = rNode->leftChild;
	if(NULL == tmpNode)
	  tmpNode = rNode->rightChild;
	if(NULL != tmpNode)
	  {
	    tmpNode->fatherNode = rNode->fatherNode;
	    if(NULL != rNode->fatherNode)
	      {
		if(rNode == rNode->fatherNode->leftChild)
		  rNode->fatherNode->leftChild = tmpNode;
		else
		  rNode->fatherNode->rightChild = tmpNode;
	      }
	    delete rNode;
	  }
      }
    return deleteFather;
  }
  
  template<typename T>
  int AVLTree<T>::rebalance(AVLTreeNode<T> *node)
  {
    if(NULL == node)
      return 0;
   
    bool isleft = BinaryTree<T>::isLeftChild(_TO_B_TREE_NODE(node));
    while(NULL != node)
      {
	int balanceFactor = node->balanceFactor();
	if(2 == balanceFactor)
	  {
	    if(isleft) // LL
	      {
		BinaryTree<T>::rotateRight(node);
	      }
	    else // RL
	      {
		BinaryTree<T>::rotateRight(node->leftChild);
		BinaryTree<T>::rotateLeft(node);
	      }
	  }
	else if(-2 == balanceFactor)
	  {
	    if(isleft) // LR
	      {
		BinaryTree<T>::rotateLeft(node->rightChild);
		BinaryTree<T>::rotateRight(node);
	      }
	    else // RR
	      {
		BinaryTree<T>::rotateLeft(node);
	      }
	  }
	node = _TO_AVL_TREE_NODE(node->fatherNode);
	isleft = BinaryTree<T>::isLeftChild(_TO_B_TREE_NODE(node));
      }
    return 0;
  }

} // end name space

#endif // AVL_TREE_NODE_H
