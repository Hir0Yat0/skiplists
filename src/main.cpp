#include<iostream>
#include<stdlib.h>

#include <map>

#include "node.hpp"
#include "skiplists.hpp"

int main(){
    
    std::cout << "Hello World!" << "\n";
    
    skiplists::SkipLists skiplists{};
    skiplists.insertOrReplace(1,1);
    skiplists.insertOrReplace(1,2);
    skiplists.printSkipLists();
    std::cout << "size = " << skiplists.size() << "\n";

    return 0;
}
