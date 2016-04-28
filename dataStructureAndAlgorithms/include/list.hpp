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

#ifndef LIST_H
#define LIST_H

namespace face2wind
{

  template<typename T>
  struct ListItem
  {
    T item;
    ListItem<T> *next;
    ListItem<T> *pre;
  };
  
  template<typename T>
  class List
  {
  public:
    List();
    List(const List<T>& other);
    ~List();
    int deepCopy(const List<T>& other);
    int deepFree();
    List& operator=(const List<T>& other);
    int push_front(const T& element);
    int push_back(const T& element);
    T &pop_front();
    T &pop_back();
    int remove(const T& element);
    int remove_if(bool (*f)(const T&));
    std::size_t size();
    int show() const;
    
  private:
    std::size_t _size;
    ListItem<T> *_first;
    ListItem<T> *_last;

    int doRemoveElement(ListItem<T> *element);
  };

  template<typename T>
  List<T>::List()
    :_size(0),_first(NULL),_last(NULL)
  {
  }

  template<typename T>
  List<T>::List(const List<T>& other)
    :_size(0),_first(NULL),_last(NULL) // use deep copy
  {
    deepCopy(other);
  }

  template<typename T>
  int List<T>::deepCopy(const List<T>& other)
  {
    ListItem<T> *cur = other._first;
    ListItem<T> *tmp = NULL;
    ListItem<T> *pre = NULL;
    while(NULL != cur)
      {
	tmp = new ListItem<T>();
	tmp->item = cur->item;
	if(NULL == cur->pre) // current was head pointer
	  {
	    _first = tmp;
	    tmp->pre = NULL;
	  }
	else
	  {
	    pre->next = tmp;
	  }
	pre = tmp;
	cur = cur->next;
      }
    return 0;
  }

  template<typename T>
  int List<T>::deepFree()
  {
    ListItem<T> *cur = _first;
    ListItem<T> *tmp = NULL;
    while(NULL != cur)
      {
	tmp = cur;
	cur = cur->next;
	delete tmp;
      }
    _first = NULL;
    _last = NULL;
    return 0;
  }
  
  template<typename T>
  List<T>::~List()
  {
    deepFree();
  }

  template<typename T>
  List<T>& List<T>::operator=(const List<T>& other)
  {
    deepFree();
    deepCopy(other);
    return *this;
  }

  template<typename T>
  int List<T>::push_front(const T& element)
  {
    ListItem<T> *tmp = new ListItem<T>();
    tmp->item = element; // do not use pointer copy, operator= replace it
    tmp->pre = NULL;
    tmp->next = _first;
    if(0 == _size)
      _last = tmp;
    else
      _first->pre = tmp;
    _first = tmp;
    ++_size;
    return 0;
  }

  template<typename T>
  int List<T>::push_back(const T& element)
  {
    ListItem<T> *tmp = new ListItem<T>();
    tmp->item = element; // do not use pointer copy, operator= replace it
    tmp->next = NULL;
    tmp->pre = _last;
    if(0 == _size)
      _first = tmp;
    else
      _last->next = tmp;
    _last = tmp;
    ++_size;
    return 0;
  }

  template<typename T>
  T &List<T>::pop_front()
  {
    if(0 == _size)
      {
	//return &static_cast<T *>(NULL);
	throw std::string("pop_front() with size is 0");
      }
    ListItem<T> *tmp = _first;
    if(1 == _size)
      _first = NULL;
    else
      {
	_first = tmp->next;
	_first->pre = NULL;
      }
    --_size;
    return tmp->item;
  }

  template<typename T>
  T &List<T>::pop_back()
  {
    if(0 == _size)
      {
	//return &static_cast<T *>(NULL);
	throw std::string("pop_back() with size is 0");
      }
    
    ListItem<T> *tmp = _last;
    if(1 == _size)
      _last = NULL;
    else
      {
	_last = tmp->pre;
	_last->next = NULL;
      }
    --_size;
    return tmp->item;
  }

  template<typename T>
  int List<T>::remove(const T& element)
  {
    if(0 == _size)
      return 0;
    
    ListItem<T> *cur = _first;
    ListItem<T> *tmp = NULL;
    while(NULL != cur)
      {
	tmp = cur->next;
	if(element == (cur->item))
	  doRemoveElement(cur);
	cur = tmp;
      }
    return 0;
  }

  template<typename T>
  int List<T>::remove_if(bool (*f)(const T&))
  {
    if(0 == _size)
      return 0;
    
    ListItem<T> *cur = _first;
    ListItem<T> *tmp = NULL;
    while(NULL != cur)
      {
	tmp = cur->next;
	if(f(cur->item))
	  doRemoveElement(cur);
	cur = tmp;
      }
    return 0;
  }

  template<typename T>
  int List<T>::doRemoveElement(ListItem<T> *element)
  {
    ListItem<T> *tmp = element;
    if(NULL == tmp->pre) // current delete is head
      {
	if(1 == _size)
	  _first = NULL;
	else
	  {
	    _first = tmp->next;
	    _first->pre = NULL;
	  }
      }
    else if(NULL == tmp->next) // current delete is end
      {
	if(1 == _size)
	  _last = NULL;
	else
	  {
	    _last = tmp->pre;
	    _last->next = NULL;
	  }
      }
    else
      {
	tmp->pre->next = tmp->next;
	tmp->next->pre = tmp->pre;
      }
    --_size;
    delete tmp;
    return 0;
  }
  
  template<typename T>
  std::size_t List<T>::size()
  {
    return _size;
  }

  template<typename T>
  int List<T>::show() const
  {
    ListItem<T> *cur = _first;
    while(NULL != cur)
      {
	std::cout<<cur->item;
	if(NULL != cur->next)
	  std::cout<<",";
	cur = cur->next;
      }
    std::cout<<std::endl;
    return 0;
  }
  
}
#endif // LIST_H

/*

Example:

bool smaller(const int &element)
{
  return (7 > element);
}

int main(int argc, char **argv)
{
  using std::cout;
  using std::endl;

  face2wind::List<int> l;
  l.push_back(4);
  l.push_front(3);
  l.push_back(5);
  l.push_front(2);
  l.show();
  cout<<"pop back = "<<l.pop_back()<<endl;
  l.show();
  cout<<"pop front = "<<l.pop_front()<<endl;
  l.show();
  l.push_back(6);
  l.push_back(7);
  l.push_back(8);
  l.push_back(9);
  l.push_back(10);
  l.show();
  l.remove(8);
  l.remove(5);
  l.show();
  l.remove_if(smaller);
  l.show();
  cout<<"now size is "<<l.size()<<endl;
  return 0;
}

*/
