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

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <queue>
#include <stack>

namespace face2wind
{

#define _TO_B_TREE_NODE(i) static_cast<BinaryTreeNode<T> *>(i)

  template<typename T>
  struct BinaryTreeNode
  {
    BinaryTreeNode():fatherNode(NULL),leftChild(NULL),rightChild(NULL){}
    BinaryTreeNode(const T &data):element(data),fatherNode(NULL),leftChild(NULL),rightChild(NULL){}
    virtual ~BinaryTreeNode(){}
    T element;
    BinaryTreeNode<T> *fatherNode;
    BinaryTreeNode<T> *leftChild;
    BinaryTreeNode<T> *rightChild;
    virtual void showElementName(){std::cout<<element;}
    int deep(){
      int _deep = 1;
      BinaryTreeNode<T> *tmp = fatherNode;
      while(NULL != tmp)
	{
	  //std::cout<<"deep ...."<<_deep<<std::endl;
	  tmp = tmp->fatherNode;
	  ++_deep;
	}
      return _deep;
    }
  };
  
  template<typename T>
  class BinaryTree
  {
  public:
    BinaryTree():rootNode(NULL){}
    virtual ~BinaryTree(){clear();}
    int clear();
    int preOrderTraversal();
    int inOrderTraversal();
    int postOrderTraversal();
    int breadthFirstTraversal(BinaryTreeNode< T >* node);
    void showTree(BinaryTreeNode<T> *node);
    virtual int insert(const T &data) = 0;
    virtual int remove(const T &data) = 0;

  private:
    int doClear(BinaryTreeNode<T> *node);
    int doPreOrderTraversal(BinaryTreeNode<T> *node);
    int doInOrderTraversal(BinaryTreeNode<T> *node);    
    int doPostOrderTraversal(BinaryTreeNode<T> *node);
      
  protected:
    BinaryTreeNode<T> *rootNode;

    bool isLeftChild(BinaryTreeNode<T>* node){
      if(NULL == node)
	return false;
      if(NULL == node->fatherNode)
	return false;
      return (node->fatherNode->leftChild == node);
    }
    bool isRightChild(BinaryTreeNode<T>* node){
      if(NULL == node)
	return false;
      if(NULL == node->fatherNode)
	return false;
      return (node->fatherNode->rightChild == node);
    }
    int rotateLeft(BinaryTreeNode<T>* node);
    int rotateRight(BinaryTreeNode<T>* node);
  };

  template<typename T>
  int BinaryTree<T>::rotateLeft(BinaryTreeNode<T>* node)
  {
    if(NULL == node)
      return 1;
    
    BinaryTreeNode<T> *mRight = node->rightChild;
    if(NULL == mRight)
      return 2;

    if(isLeftChild(node))
    	node->fatherNode->leftChild = mRight;
    else if(isRightChild(node))
    	node->fatherNode->rightChild = mRight;
    else // is root
      BinaryTree<T>::rootNode = mRight;
    mRight->fatherNode = node->fatherNode;
    
    node->rightChild = mRight->leftChild; // node and left child
    if(NULL != mRight->leftChild)
      mRight->leftChild->fatherNode = node;

    mRight->leftChild = node;
    node->fatherNode = mRight;
    return 0;
  }

  template<typename T>
  int BinaryTree<T>::rotateRight(BinaryTreeNode<T>* node)
  {
    if(NULL == node)
      return 1;

    BinaryTreeNode<T> *mLeft = node->leftChild;
    if(NULL == mLeft)
      return 2;

    if(isLeftChild(node))
      node->fatherNode->leftChild = mLeft;
    else if(isRightChild(node))
      node->fatherNode->rightChild = mLeft;
    else // is root
      BinaryTree<T>::rootNode = mLeft;
    mLeft->fatherNode = node->fatherNode;

    node->leftChild = mLeft->rightChild; // node and right child
    if(NULL != mLeft->rightChild)
      mLeft->rightChild->fatherNode = node;

    mLeft->rightChild = node;
    node->fatherNode = mLeft;

    return false;
  }

  template<typename T>
  int BinaryTree<T>::clear()
  {
    doClear(rootNode);
    return 0;
  }

  template<typename T>
  int BinaryTree<T>::doClear(BinaryTreeNode<T> *node)
  {
    if(NULL == node)
      return 0;
    
    if(NULL != node->leftChild)
      doClear(node->leftChild);
    if(NULL != node->rightChild)
      doClear(node->rightChild);
    delete node;
    return 0;
  }

  template<typename T>
  int BinaryTree<T>::preOrderTraversal()
  {
    std::cout<<"pre-Order Traversal:"<<std::endl;
    doPreOrderTraversal(rootNode);
    std::cout<<std::endl;
    return 0;
  }

  template<typename T>
  int BinaryTree<T>::doPreOrderTraversal(BinaryTreeNode<T> *node)
  {
    if(NULL == node)
      return 0;
    
    std::cout<<node->element<<", ";
    doPreOrderTraversal(node->leftChild);
    doPreOrderTraversal(node->rightChild);
    return 0;
  }
    
  template<typename T>
  int BinaryTree<T>::inOrderTraversal()
  {
    std::cout<<"in-Order Traversal:"<<std::endl;
    doInOrderTraversal(rootNode);
    std::cout<<std::endl;
    return 0;
  }

  template<typename T>
  int BinaryTree<T>::doInOrderTraversal(BinaryTreeNode<T> *node)
  {
    if(NULL == node)
      return 0;

    doInOrderTraversal(node->leftChild);
    std::cout<<node->element<<", ";
    doInOrderTraversal(node->rightChild);
    return 0;
  }
    
  template<typename T>
  int BinaryTree<T>::postOrderTraversal()
  {
    std::cout<<"post-Order Traversal:"<<std::endl;
    doPostOrderTraversal(rootNode);
    std::cout<<std::endl;
    return 0;
  }

  template<typename T>
  int BinaryTree<T>::doPostOrderTraversal(BinaryTreeNode<T> *node)
  {
    if(NULL == node)
      return 0;

    doPostOrderTraversal(node->leftChild);
    doPostOrderTraversal(node->rightChild);
    std::cout<<node->element<<", ";
    return 0;
  }

  template<typename T>
  int BinaryTree<T>::breadthFirstTraversal(BinaryTreeNode<T> *node = NULL)
  {
    if(NULL == node)
    {
      if(NULL == BinaryTree<T>::rootNode)
        return 0;
      node = BinaryTree<T>::rootNode;
    }
    std::cout<<"breadth first Traversal:\n";    
    BinaryTreeNode<T> *tmpNode = NULL;
    std::queue<BinaryTreeNode<T> *> nodeQueue;
    nodeQueue.push(node);
    while(!nodeQueue.empty())
    {
      tmpNode = nodeQueue.front();
      nodeQueue.pop();
      std::cout<<tmpNode->element<<", ";
      if(NULL != tmpNode->leftChild)
	nodeQueue.push(tmpNode->leftChild);
      if(NULL != tmpNode->rightChild)
	nodeQueue.push(tmpNode->rightChild);
    }
    std::cout<<std::endl;
    return 0;
  }

  template<typename T>
  void BinaryTree<T>::showTree(BinaryTreeNode<T> *node = NULL)
  {
    if(NULL == node)
    {
      node = BinaryTree<T>::rootNode;
      std::cout<<"show tree :"<<std::endl;
    }
    if(NULL == node)
      return;
    
    std::stack<BinaryTreeNode<T> *> st;
    while(NULL != node || !st.empty() )
    {
	  if(NULL != node->rightChild)
	  {
	    st.push(node);
	    node = node->rightChild;
	  }
	  else
	  {
		int deep = node->deep();
		std::string space;
		while(0 < --deep)
		  space += "   ";
		std::cout<<space;
		node->showElementName();
		std::cout<<std::endl;
		
		node = node->leftChild;
		while(NULL == node && !st.empty() )
		{
		      node = st.top();
		      st.pop();
		      
		      int deep = node->deep();
		      std::string space;
		      while(0 < --deep)
			space += "   ";
		      std::cout<<space;
		      node->showElementName();
		      std::cout<<std::endl;
		    
		      node = node->leftChild;
		}
	  }
    }
    return;
    if(NULL == node)
    {
      node = BinaryTree<T>::rootNode;
      std::cout<<"show tree :"<<std::endl;
    }
    if(NULL == node)
      return;

    if(NULL != node->rightChild)
      showTree(node->rightChild);
    int deep = node->deep();
    std::string space;
    while(0 < --deep)
      space += "   ";
    std::cout<<space;
    node->showElementName();
    std::cout<<std::endl;
    if(NULL != node->leftChild)
      showTree(node->leftChild);
  }
  
} // namespace end

#endif  //BINARY_TREE_H
