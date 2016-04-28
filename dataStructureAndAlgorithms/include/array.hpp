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

#ifndef ARRAY_H
#define ARRAY_H

namespace face2wind
{

  template<typename T>
  class Array
  {
  public:
    Array();
    Array(const Array<T>& other);
    Array(const T *other, std::size_t size);
    ~Array();
    Array<T>& operator=(const Array<T>& other);
    T& operator[](std::size_t n) const;
    int push_back(const T &element);
    int push_front(const T &element);
    T pop_back();
    T pop_front();
    int indexOf(const T&element);
    int show() const;

  private:
    std::size_t _size;
    std::size_t _capacity;
    T *_rawArray;
  };

  template<typename T>
  Array<T>::Array()
    :_size(0),_capacity(0),_rawArray(0)
  {
  }

  template<typename T>
  Array<T>::Array(const Array<T>& other)
  {
    _rawArray = new T[other._capacity];
    for(int i = 0; i < other._size; i++)
      _rawArray[i] = other._rawArray[i];
    _size = other._size;
    _capacity = other._capacity;
  }

  template<typename T>
  Array<T>::Array(const T *other, std::size_t size)
  {
    if(NULL == other)
      return;
    _capacity = 1;
    while(size > _capacity)
      _capacity *= 2;
    _rawArray = new T[_capacity];
    for(int i = 0; i < size; i++)
      _rawArray[i] = other[i];
    _size = size;
  }
  
  template<typename T>
  Array<T>::~Array()
  {
    delete []_rawArray;
  }

  template<typename T>
  Array<T>& Array<T>::operator=(const Array<T>& other)
  {
    if(NULL != _rawArray)
      delete []_rawArray;
    _rawArray = new T[other._capacity];
    for(int i = 0; i < other._size; i++)
      _rawArray[i] = other._rawArray[i];
    return *this;
  }

  template<typename T>
  T &Array<T>::operator[](std::size_t n) const
  {
    if(_size <= n)
      throw std::string("Array Error: index out of range");
    //return *static_cast<T *>(NULL);
    return _rawArray[n];
  }

  template<typename T>
  int Array<T>::push_back(const T &element)
  {
    if(0 == _capacity)
      _capacity = 1;
    else if(_size == _capacity)
      _capacity *= 2;
    T *tmpArr = _rawArray;
    _rawArray = new T[_capacity];
    if(0 < _size) // need copy old datas
      {
	for(std::size_t i = 0; i < _size ; i++)
	  _rawArray[i] = tmpArr[i];
      }
    _rawArray[_size++] = element;
    //std::cout<<"after push(), size:"<<_size<<", capacity = "<<_capacity<<std::endl;
    return 0;
  }

  template<typename T>
  int Array<T>::push_front(const T &element)
  {
    if(0 == _capacity)
      _capacity = 1;
    else if(_size == _capacity)
      _capacity *= 2;
    T *tmpArr = _rawArray;
    _rawArray = new T[_capacity];
    if(0 < _size) // need copy old datas
      {
	for(std::size_t i = 0; i < _size ; i++)
	  _rawArray[i+1] = tmpArr[i];
      }
    _rawArray[0] = element;
    _size++;
    //std::cout<<"after push_front(), size:"<<_size<<", capacity = "<<_capacity<<std::endl;
    return 0;
  }

  template<typename T>
  T Array<T>::pop_back()
  {
    if(0 < _size)
      return _rawArray[--_size];
    else
      throw std::string("Array Error: no element for pop()");
  }

  template<typename T>
  T Array<T>::pop_front()
  {
    if(0 < _size)
      {
	T first = _rawArray[0];
	for(std::size_t i = 1; i < _size; ++i) // all elements move one step
	  _rawArray[i-1] = _rawArray[i];
	--_size;
	return first;
      }
    else
      throw std::string("Array Error: no element for pop()");
  }

  template<typename T>
  int Array<T>::indexOf(const T&element)
  {
    int result = -1;
    for(std::size_t i = 0; i < _size ; ++i)
      if(element == _rawArray[i])
	{
	  result = i;
	  break;
	}
    return result;
  }
  
  template<typename T>
  int Array<T>::show() const
  {
    for(std::size_t i = 0; i < _size ; ++i)
      {
	std::cout<<_rawArray[i];
	if( (_size - 1) >  i)
	  std::cout<<",";
      }
    std::cout<<std::endl;
    return 0;
  }
}

#endif // ARRAY_H

/*

  Example:

  int main(int argc, char **argv)
  {
  int a[] = {2,9,4,7,3,8,6,5};
  face2wind::Array<int> arr(a, 8);
  arr.show();
  arr[0] = 13;
  arr.show();
  std::cout<<"pop_front() = "<<arr.pop_front()<<std::endl;
  arr.show();
  std::cout<<"pop_back() = "<<arr.pop_back()<<std::endl;
  arr.show();
  arr.push_front(5);
  arr.show();
  face2wind::Array<int> arr2(arr);
  std::cout<<"arr2 = ";
  arr2.show();
  face2wind::Array<int> arr3 = arr;
  std::cout<<"arr3 = ";
  arr3.show();
  std::cout<<"9 index = "<<arr.indexOf(9)<<std::endl;
  std::cout<<"8 index = "<<arr.indexOf(8)<<std::endl;
  std::cout<<"0 index = "<<arr.indexOf(0)<<std::endl;
  return 0;
  }

*/
