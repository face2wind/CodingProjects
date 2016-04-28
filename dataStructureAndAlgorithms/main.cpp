#include <iostream>
#include <string>
#include <vector>

#include <array.hpp>
#include <hashtable.hpp>
#include <list.hpp>
#include <avlTree.hpp>
#include <rbTree.hpp>
#include <sort.hpp>

#include <cstdlib>

int main(int argc, char **argv)
{
  using std::cout;
  using std::endl;
  using std::vector;
  vector<int> data;
  srand(time(NULL));
  for(int i = 0; i < 20; ++i)
    data.push_back(rand()%100);
  std::cout<<"raw array :\n";
  face2wind::showArray(data);
  //face2wind::bubbleSort<int>(data);
  //face2wind::cocktailSort<int>(data);
  //face2wind::oddEvenSort<int>(data);
  //face2wind::combSort<int>(data);
  //face2wind::gnomeSort<int>(data);
  //face2wind::quickSort<int>(data);

  //face2wind::selectionSort<int>(data);
  //face2wind::cycleSort<int>(data);

  //face2wind::insertionSort<int>(data);
  face2wind::shellSort<int>(data);

  //face2wind::radixSort<int>(data);
  face2wind::showArray(data);
  return 0;
}
