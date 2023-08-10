// ParallelConcurrencyCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "PPDemo.h"
int main()
{
    
    std::cout << "Hello World!\n";

    //if (__cplusplus == 202101L) std::cout << "C++23";
    //else if (__cplusplus == 202002L) std::cout << "C++20";
    //else if (__cplusplus == 201703L) std::cout << "C++17";
    //else if (__cplusplus == 201402L) std::cout << "C++14";
    //else if (__cplusplus == 201103L) std::cout << "C++11";
    //else if (__cplusplus == 199711L) std::cout << "C++98";
    //else std::cout << "pre-standard C++." << __cplusplus;
    //std::cout << "\n";



   
    unsigned int test_number = 25;

    switch (test_number)
    {
    case 1:
        //join
        //parent thread waits for child thread before exiting 
        za::pp::thread_lifecycle::demo1();
        break; 
        //detach 
        //parent and child thread work independently 
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
    case 13:
        za::pp::condition_variablee::demo13();
        break;    
    case 14:
        za::pp::producer_consumer::demo14();
        break;    
    case 15:
        za::pp::semaphore::demo15();
        break;        
    case 16:
        za::pp::race_condition::demo16();
        break;    
    case 17:
        za::pp::barrierr::demo17();
        break;    
    case 18:
        za::pp::latchh::demo18();
        break;    
    case 19:
        za::pp::thread_pool::demo19();
        break;    
    case 20:
        za::pp::futuree::demo20();
        break;    
    case 21:
        za::pp::divide_and_conquer::demo21();
        break;    
    case 22:
        za::pp::measure_speedup::demo22();
        break;    
    case 23:
        za::pp::matrix_multiplyy::demo23();
        break;    
    case 24:
        za::pp::merge_sortt::demo24();
        break;    
    case 25:
        za::pp::download_images::demo25();
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
