#ifndef _SORT_HPP_
#define _SORT_HPP_

#include <vector>
#include <iostream>

namespace face2wind{

#define _TEMPLATE_DEFINE_ template<typename T, typename ARR=std::vector<T> >

  template<class T>
  void showArray(const std::vector<T> &array)
  {
    for(typename std::vector<T>::const_iterator it = array.begin();
	it != array.end();++it){
      if(array.begin() != it)
	std::cout<<",";
      std::cout<<*it;
    }
    std::cout<<std::endl;
  }

  /* ==== 交换排序(8) ========================================================
   1 冒泡排序  bubble
   2 鸡尾酒排序（双向冒泡排序）  解决冒泡的“乌龟”
   3 奇偶排序(砖排序) Odd-even sort 类似冒泡，奇数偶数分开判断
   4 梳排序  comb sort 冒泡排序+间隔由大变小
   5 地精排序（侏儒排序）  gnomeSort 试试优化反向冒泡
   6 快速排序  QuickSort 基准<-小于基准的值<-大于基准的值(low)，递归low左边排序，递归low右边排序
   臭皮匠排序 Stooge 排序是一种低效的递归排序算法，甚至慢于冒泡排序，先排序前2/3，再排序后面2/3，最后再排序前2/3
   Bogo排序 既不实用又原始的排序算法，其原理等同将一堆卡片抛起，落在桌上后检查卡片是否已整齐排列好，若非就再抛一次
   */
  
  _TEMPLATE_DEFINE_
  void bubbleSort(ARR &array)
  {
    int size = array.size();
    int swapTimes = 0;
    for(int i = 0; i < size; ++i)
      for(int j = size-1; j > i; --j){
	if(array[j] < array[j-1]){
	  T tmp = array[j];
	  array[j] = array[j-1];
	  array[j-1] = tmp;
	  swapTimes; += 3
	}
      }
    std::cout<<"bubble sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }

  _TEMPLATE_DEFINE_
  void cocktailSort(ARR &array)  // double bubble sort
  {
    int size = array.size();
    int swapTimes = 0;
    int start = 0;
    int end = size-1;
    while(end > start){
      for(int i = start; i < end; ++i){
	if(array[i] > array[i+1]){
	  T tmp = array[i];
	  array[i] = array[i+1];
	  array[i+1] = tmp;
	  swapTimes += 3;
	}
      }
      --end;
      for(int i = end-1; i >= start; --i){
	if(array[i] > array[i+1]){
	  T tmp = array[i];
	  array[i] = array[i+1];
	  array[i+1] = tmp;
	  swapTimes += 3;
	}
      }
      ++start;
    }
    std::cout<<"cocktail sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }

  _TEMPLATE_DEFINE_
  void oddEvenSort(ARR &array)
  {
    int size = array.size();
    int swapTimes = 0;
    bool sorted = false;
    while(!sorted){
      sorted = true;
      for(int i = 0; i < size-1; i+=2)
	if(array[i] > array[i+1]){
	  T tmp = array[i];
	  array[i] = array[i+1];
	  array[i+1] = tmp;
	  swapTimes += 3;
	  sorted = false;
	}
      for(int i = 1; i < size-1; i+=2)
	if(array[i] > array[i+1]){
	  T tmp = array[i];
	  array[i] = array[i+1];
	  array[i+1] = tmp;
	  swapTimes += 3;
	  sorted = false;
	}
    }
    std::cout<<"oddEven sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }
  
  _TEMPLATE_DEFINE_
  void combSort(ARR &array)
  {
    int size = array.size();
    int swapTimes = 0;
    int split = size-1;
    float changeSpeed = 1.5;
    bool hasSwap = true;
    while(hasSwap){
      hasSwap = false;
      for(int i = 0; i < size-split; i += split){
	if(array[i] > array[i+split]){
	  T tmp = array[i];
	  array[i] = array[i+split];
	  array[i+split] = tmp;
	  swapTimes += 3;
	  hasSwap = true;
	}
      }
      if(1 < split){
	split /= changeSpeed;
	hasSwap = true;
      }
    }
    std::cout<<"comb sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }

  _TEMPLATE_DEFINE_
  void gnomeSort(ARR &array)
  {
    int size = array.size();
    int swapTimes = 0;
    int i = 0;
    while(size > i){
      if(0 == i ||array[i-1] <= array[i])
	++i;
      else{
	T tmp = array[i];
	array[i] = array[i-1];
	array[i-1] = tmp;
	swapTimes += 3;
	--i;
      }
    }
    std::cout<<"gnome sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }

  _TEMPLATE_DEFINE_
  int quickSort(ARR &array,int start = 0,int end = 0,int deep = 0)
  {
    int size = array.size();
    int swapTimes = 0;
    if(0 == end)
      end = size-1;
    //std::cout<<"qsort["<<start<<","<<end<<"]\n";
    if(start < end){
      // partition ===========================
      T pivot = array[start];
      int s = start;
      int e = end;
      while(s < e){
	while(s < e && array[e] >= pivot)
	  --e;
	if(s < e){
	  array[s++] = array[e];
	  ++swapTimes;
	}
	while(s < e && array[s] <= pivot)
	  ++s;
	if(s < e){
	  array[e--] = array[s];
	  ++swapTimes;
	}
      }
      array[s] = pivot;
      // partition end =======================
      swapTimes += 2;
      int pivotIndex = s;
      if(start+1 < pivotIndex)
	swapTimes += quickSort<T>(array,start,pivotIndex-1,deep+1);
      if(pivotIndex+1 < end)
	swapTimes += quickSort<T>(array,pivotIndex+1,end, deep+1);
    }
    if(0 == deep)
      std::cout<<"gnome sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
    return swapTimes;
  }
  
  /* ==== 选择排序(8) ========================================================
   1 选择排序  顺序从剩下的集合里选择最小/大的数放在前/后面
   2 循环（圈）排序 Cycle sort  最小覆盖
   堆排序  HeapSort
   */


  _TEMPLATE_DEFINE_
  void selectionSort(ARR &array) // not stable
  {
    int size = array.size();
    int swapTimes = 0;
    for(int i = 0; i < size ; ++i){
      int minIndex = i;
      for(int j = i+1; j < size; ++j)
	if(array[minIndex] > array[j])
	  minIndex = j;
      if(i != minIndex){
	T tmp = array[i];
	array[i] = array[minIndex];
	array[minIndex] = tmp;
	swapTimes += 3;
      }
    }
    std::cout<<"selection sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }

  // like you take bus, and some of people did not sat their own seat
  // then tell them from seat 1 to N, sit on their own seat,one by one
  _TEMPLATE_DEFINE_
  void cycleSort(ARR &array)
  {
    int size = array.size();
    int swapTimes = 0;
    for(int i = 0; i < size-1; ++i){
      int targetIndex = i;
      for(int j = i+1; j < size; j++)
	if(array[i] > array[j])
	  ++targetIndex;

      if(i == targetIndex)
	continue;
      
      while(array[i] == array[targetIndex])
	++targetIndex;
      T tmp = array[i];
      array[i] = array[targetIndex];
      array[targetIndex] = tmp;
      swapTimes += 3;

      while(i != targetIndex){
	targetIndex = i;
	for(int k = targetIndex + 1 ; k < size ; ++k)
	  if(array[i] > array[k])
	    ++targetIndex;

	if(i != targetIndex){
	  while(array[i] == array[targetIndex])
	    ++targetIndex;
	  T tmp = array[i];
	  array[i] = array[targetIndex];
	  array[targetIndex] = tmp;
	  swapTimes += 3;
	}
      }
    }
    std::cout<<"cycle sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }

  /* ==== 插入排序(8) =======================================================
   1 插入排序  InsertionSort  变种（二分查找）
   2 希尔排序  间隔逐渐缩小的插入排序，插入排序的改良
   */

  _TEMPLATE_DEFINE_
  void insertionSort(ARR &array) // stable
  {
    int size = array.size();
    int swapTimes = 0;
    for(int i = 1; i < size-1; ++i){
      T tmp = array[i+1];
      int j = i+1;
      while(0 < j && array[j-1] > tmp){
	array[j] = array[j-1];
	++swapTimes;
	--j;
      }
      array[j] = tmp;
      swapTimes += 2;
    }
    std::cout<<"insertion sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }

  _TEMPLATE_DEFINE_
  void shellSort(ARR &array) // stable
  {
    int size = array.size();
    int swapTimes = 0;
    int gap = size/2;
    while(0 < gap){
      for(int i = 0; i < size-gap; i+=gap){
	T tmp = array[i+gap];
	int j = i + gap;
	while(0 < j && array[j-gap] > tmp){
	  array[j] = array[j-gap];
	  ++swapTimes;
	  j -= gap;
	}
	array[j] = tmp;
	swapTimes += 2;
      }
      gap /= 2;
    }
    std::cout<<"insertion sort :"<<"(move "<<swapTimes<<" times)"<<std::endl;
  }


  /* ==== 分布排序(8) =======================================================

   基数排序  radix sort 根据比较数值范围而定（位数小的，用LSD，位数大的用MSD）
   桶排序  bucket sort 基数排序的一类，针对不同类型分桶方案不同
   鸽笼排序  Pigeonhole sort
   */
  
  _TEMPLATE_DEFINE_
  void radixSort(ARR &array, int digitNum = 2, bool isLSD = true)
  {
    int size = array.size();
    int swapTimes = 0;
    if(isLSD){
      typename std::vector<std::vector<T> > container(10);
      typename std::vector<T> realArr;
      typename std::vector<T> copy = array;
      int curDigit = 1;
      while(curDigit <= digitNum){
	container.clear();
	int digitMax = 1; // gat max gigit num
	for(int i = 0; i < curDigit; ++i){
	  digitMax *= 10;
	}
	for(int i = 0; i < size; ++i){
	  int radix = copy[i]%digitMax;
	  radix = radix*10/digitMax;
	  container[radix].push_back(copy[i]);
	}
	realArr.clear();
	for(typename std::vector<std::vector<T> >::iterator it
	      = container.begin(); it != container.end(); ++it){
	  for(typename std::vector<T>::iterator cit = it->begin();
	      cit != it->end(); ++cit){
	    realArr.push_back(*cit);
	  }
	}
	copy = realArr;
	++curDigit;
      }
      for(int i = 0; i < size; ++i)
	array[i] = copy[i];
    }else{


    }
    std::cout<<"radix sort :"<<"(swap "<<swapTimes<<" times)"<<std::endl;
  }

  /* ==== 交换排序(8) ========================================================
     归并排序  MergeSort
  */  


}

#endif
