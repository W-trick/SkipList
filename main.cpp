#include <iostream>
#include <string>
#include "skipnode.h"
#include "skiplist.h"

using namespace std;


int main()
{
    SkipList<string,string> skip_list(10);

    for(int i = 0;i < 10; ++i)
    {

        skip_list.InsertNode(to_string(i),to_string(i));
    }
    

    skip_list.DisplayList();

    skip_list.SerchNode(to_string(2));

    skip_list.DeleteNode(to_string(2));

    std::cout<< "size" <<skip_list.SkipSize()<<std::endl;

    skip_list.DisplayList();

    skip_list.dumpFile();

    skip_list.loadFile();
    
    return 0;
}

