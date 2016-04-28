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

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include "binaryTree.hpp"

namespace face2wind
{

  template<typename T>
  class BinarySearchTree : public BinaryTree<T>
  {

  public:
    BinarySearchTree(){}
    virtual ~BinarySearchTree(){}
    
    virtual int insert(const T &data);
    virtual int remove(const T &data);
    BinaryTreeNode<T> *findNode(const T &data);

  protected:
    int doInsertNode(BinaryTreeNode<T> *newNode, BinaryTreeNode<T> *node);
    int doRemove(BinaryTreeNode<T> *rNode, BinaryTreeNode<T> *node);
    BinaryTreeNode<T> *doFindNode(const T &data, BinaryTreeNode<T> *node);
    BinaryTreeNode<T> *minTreeNode(BinaryTreeNode<T>* node);
    BinaryTreeNode<T> *maxTreeNode(BinaryTreeNode<T>* node);
  };

  template<typename T>
  int BinarySearchTree<T>::insert(const T &data)
  {
    BinaryTreeNode<T> *newNode = new BinaryTreeNode<T>(data);
    if(NULL == BinaryTree<T>::rootNode)
      {
	BinaryTree<T>::rootNode = newNode;
	//std::cout<<"insert "<<data<<" into root\n";
      }
    else
      doInsertNode(newNode, BinaryTree<T>::rootNode);
    return 0;
  }

  template<typename T>
  int BinarySearchTree<T>::doInsertNode(BinaryTreeNode<T> *newNode,
					BinaryTreeNode<T> *node)
  {
    if(NULL == node || NULL == newNode)
      return 1;
    if(node->element == newNode->element)
      return 0;
    else if(node->element < newNode->element)
      {
	if(NULL != node->rightChild)
	  doInsertNode(newNode, node->rightChild);
	else
	  {
	    node->rightChild = newNode;
	    newNode->fatherNode = node;
	    //std::cout<<"insert "<<newNode->element
	    //<<" into right child of "<<node->element<<std::endl;
	  }
      }
    else
      {
	if(NULL != node->leftChild)
	  doInsertNode(newNode, node->leftChild);
	else
	  {
	    node->leftChild = newNode;
	    newNode->fatherNode = node;
	    //std::cout<<"insert "<<newNode->element
	    //<<" into left child of "<<node->element<<std::endl;
	  }
      }
    return 0;
  }
  
  template<typename T>
  int BinarySearchTree<T>::remove(const T &data)
  {
    BinaryTreeNode<T> *node = findNode(data);
    if(NULL == node)
      return 1;
    else
      return doRemove(node,BinaryTree<T>::rootNode);
  }

  template<typename T>
  int BinarySearchTree<T>::doRemove(BinaryTreeNode<T> *rNode,
				    BinaryTreeNode<T> *node)
  {
    if(NULL == rNode)
      return 1;
    BinaryTreeNode<T> *tmpNode = NULL;
    if(NULL == rNode->leftChild && NULL == rNode->rightChild) // is leaf node
      {
	//std::cout<<"1111"<<std::endl;
	if(rNode == BinaryTree<T>::rootNode) // is root
	    BinaryTree<T>::rootNode = NULL;
	else if(rNode->fatherNode->leftChild == rNode)
	    rNode->fatherNode->leftChild = NULL;
	else // (rNode->fatherNode->rightChild == rNode)
	    rNode->fatherNode->rightChild = NULL;
	delete rNode;
      }
    else if(NULL != rNode->leftChild && NULL != rNode->rightChild)
      // has left son and right son
      {
	//std::cout<<"2222"<<std::endl;
	tmpNode = minTreeNode(rNode->rightChild);
	T tmpElement = tmpNode->element;
	tmpNode->element = rNode->element;
	rNode->element = tmpElement;
	doRemove(tmpNode, node);
      }
    else
      {
	//std::cout<<"3333"<<std::endl;
	tmpNode = rNode->leftChild;
	if(NULL == tmpNode)
	  tmpNode = rNode->rightChild;
	if(NULL != tmpNode)
	  {
	    tmpNode->fatherNode = rNode->fatherNode;
	    if(rNode == rNode->fatherNode->leftChild)
	      rNode->fatherNode->leftChild = tmpNode;
	    else
	      rNode->fatherNode->rightChild = tmpNode;
	    delete rNode;
	  }
      }
    return 0;
  }
  
  template<typename T>
  BinaryTreeNode<T> *BinarySearchTree<T>::findNode(const T &data)
  {
    return doFindNode(data, BinaryTree<T>::rootNode);
  }

  template<typename T>
  BinaryTreeNode<T> *BinarySearchTree<T>::doFindNode(const T &data,
						     BinaryTreeNode<T> *node)
  {
    if(NULL == node)
      return NULL;
    
    if(node->element == data)
      return node;
    else if(node->element < data)
      return doFindNode(data, node->rightChild);
    else
      return doFindNode(data, node->leftChild);
  }

  template<typename T>
  BinaryTreeNode<T> *BinarySearchTree<T>::minTreeNode(BinaryTreeNode<T>* node)
  {
    if(NULL == node)
      return NULL;
    while(NULL != node->leftChild)
      node = node->leftChild;
    return node;
  }

  template<typename T>
  BinaryTreeNode<T> *BinarySearchTree<T>::maxTreeNode(BinaryTreeNode<T>* node)
  {
    if(NULL == node)
      return NULL;
    while(NULL != node->rightChild)
      node = node->rightChild;
    return node;
  }
}

#endif // BINARY_SEARCH_TREE_H
