// ParallelConcurrencyCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PPDemo.h"
int main()
{
    
    std::cout << "Hello World!\n";
    unsigned int test_number = 6;

    switch (test_number)
    {
    case 1:
        za::pp::thread_lifecycle::demo1();
        break;     
    case 2:
        za::pp::detached_thread::demo2();
        break;     
    case 3:
        za::pp::data_race::demo3();
        break;    
    case 4:
        za::pp::mutual_exclusion::demo4();
        break;    
    case 5:
        za::pp::atomic_object::demo5();
        break;    
    case 6:
        za::pp::recursive_mutexx::demo6();
        break;    
    case 7:
        za::pp::try_lockk::demo7();
        break;    
    case 8:
        za::pp::shared_mutexx::demo8();
        break;   
    case 9:
        za::pp::deadlockk::demo9();
        break;    
    case 10:
        za::pp::abandoned_lock::demo10();
        break;    
    case 11:
        za::pp::starvation::demo11();
        break;    
    case 12:
        za::pp::live_lock::demo12();
        break;
    default:
        za::pp::thread_lifecycle::demo1();
        break;
    }

}

/*
* 
* Data Race
* Mutex (Lock)
* Mechanism to implement mutual exclusion
* Only one thread or process can possess at a time
* Limits access to critical section
* Atomic Operations:
* -Execute as a single action, relative to other threads
* -Cannot be interrupted by other concurrent threads
* - Example: Acquiring pencil when taking note from multiple users
* - Remember to release the pencil when done
* 
*/
