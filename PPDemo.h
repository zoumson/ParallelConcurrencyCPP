#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <shared_mutex>
#include <array>

namespace za
{
	namespace pp
	{
		namespace thread_lifecycle
		{
			void chef_olivia();
			void demo1();
		}
		namespace detached_thread
		{
			/**
			 * Barron finishes cooking while Olivia cleans
			 */
			//garbage collector
			//when process terminates all data storage are claimed by os
			void kitchen_cleaner();
			void demo2();
		}		
		namespace data_race
		{
			/*
			* Two shoppers adding items to a shared notepad
			* 1. 
			* a. In the example program to demonstrate a data race, 
			* why did the data race only occur when each of the threads
			* were incrementing a shared variable a large number of time?
			* b. The large number of write operations on the shared 
			* variable provided more opportunities for the data race to occur.
			* 2.
			* a. Which of these scenarios does NOT have the potential for a data race?
			* b. Two threads are both reading the same shared variable.
			* 3.
			* a. Why can potential data races be hard to identify?
			* b. The data race may not always occur during execution to cause a problem.
			*/
			static unsigned int garlic_count_dr = 0;
			void shopper();
			void demo3();

		}
		namespace mutual_exclusion
		{
			/*
			* Two shoppers adding items to a shared notepad
			* 1.
			* a. How many threads can possess a mutex at the same time?
			* b. 1
			* 2.
			* a. What does it mean to protect a critical section of code with mutual exclusion?
			* b. Prevent multiple threads from concurrently executing in the critical section.
			* 
			* 3. 
			* a. What happens if ThreadA calls the lock() function 
			* on a mutex that is already possessed by ThreadB?
			* b. hreadA will block and wait until ThreadB calls the unlock() method.
			*/
			static unsigned int garlic_count_me = 0;
			static std::mutex pencil_me;
			void shopper();
			void demo4();

		}		
		namespace atomic_object
		{
			/*
			* Two shoppers adding items to a shared notepad
			* 1. 
			* Using the ++ operator to increment a variable executes 
			* as multiple instructions at the lowest level.
			* 
			* 2. 
			* a. What makes atomic objects unique?
			* b. Their values are accessed or modified as a single, 
			* uninterrupted operation which makes them "thread-safe."
			*/
			static std::atomic<unsigned int> garlic_count_ao(0);
			void shopper();
			void demo5();

		}
		namespace recursive_mutexx
		{
			/*
			* Two shoppers adding items to a shared notepad
			* 1.
			* A recursive_mutex can be locked multiple times by the same thread.
			* 2.
			* a. How many times must a thread unlock a recursive_mutex 
			* before another thread can acquire it?
			* b. as many times as that thread locked it
			*/
			static unsigned int garlic_count_rm = 0;
			static unsigned int potato_count_rm = 0;
			static std::recursive_mutex pencil_rm;
			void add_garlic();

			void add_potato();

			void shopper();
			void demo6();

		}
		namespace try_lockk
		{
			/*
			* Two shoppers adding items to a shared notepad
			* 1.
			* a. What is the difference between the try_lock() and 
			* regular lock() functions in C++?
			* b. try_lock() will not block execution if the mutex is 
			* already taken by another thread.
			* 2. 
			* a. Why is the try_lock() function useful?
			* b. It enables a thread to execute alternate 
			* operations if the mutex it needs to acquire is already taken.
			* 
			* 3. 
			* a. What happens when a thread calls the try_lock() function 
			* on a mutex that is NOT currently locked by another thread?
			* b. The function immediately locks the mutex and returns true.
			*/
			static unsigned int items_on_notepad_tl = 0;
			static std::mutex pencil_tl;
			void shopper(const char* name);
			void demo7();
		}
		namespace shared_mutexx
		{
			/*
			* Two shoppers adding items to a shared notepad
			* 1.
			* a. Which of these scenario describes the best use case for using a shared_mutex?
			* b. Lots of threads need to read the value of a shared variable, 
			* but only a few thread need to modify its value.
			* operations if the mutex it needs to acquire is already taken.
			* 
			* 2.
			* a. What is the maximum number of threads that can have 
			* exclusive ownership of a shared_mutex at the same time?
			* b. 1
			* 
			* 3. 
			* a. What is the maximum number of threads that can have 
			* shared ownership of a shared_mutex at the same time?
			* b. no limit
			* 
			* 4.
			* a. How many threads can take shared ownership of a 
			* shared_mutex while another thread has exclusive ownership of it?
			* b. 0
			* 
			*/
			static char WEEKDAYS_sm[7][10] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
			static int today_sm = 0;
			static std::shared_mutex marker_sm;

			void calendar_reader(const int id);
			void calendar_writer(const int id);
			void demo8();
		}
		namespace deadlockk
		{
			
			/*
			* Each member is waiting for another member to take action
			* 1. Which of these is a possible strategy to prevent deadlocks 
			* when multiple threads will need to acquire multiple locks?
			* 2. 
			* The Dining Philosophers scenario demonstrates the deadlock 
			* situation that can occur when multiple threads need(s) to acquire  multiple locks.
			*/
			static int sushi_count_d = 5000;
			void philosopher(std::mutex& first_chopstick, std::mutex& second_chopstick);
			void demo9();
		}
		namespace abandoned_lock
		{
			
			/*
			* 1. 
			* a. What happens when a thread terminates while still holding onto a mutex?
			* b. The mutex will be stuck in the locked state forever and no other threads will be able to acquire it.
			* 
			* 2.
			* a. Why does using a std::scoped_lock protect against a thread terminating while holding onto a mutex?
			* b. When the thread terminates, the scoped_lock object is destroyed, and the mutex is automatically released.
			*/
			static int sushi_count_al = 5000;
			void philosopher_al(std::mutex& chopsticks);
			void demo10();
		}
		namespace starvation
		{
			/*
			* 1. 
			* a. Only the lowest priority threads will be at risk of resource starvation.
			* b. False
			* 
			* 2. Starvation occurs when a thread is perpetually denied resources due to competition with other threads
			*/
			static int sushi_count_ss = 5000;
			void philosopher_ss(std::mutex& chopsticks);
			void demo11();
		}		
		namespace live_lock
		{
			/*
			* 1.
			* a. Which of these is a possible strategy to prevent 
			* deadlocks when multiple threads will need to acquire multiple locks?
			* b. Prioritize the locks so that all threads will acquire them in the same relative order.
			* 
			* 2.
			* a. Which of these is a possible strategy to resolve a livelock between multiple threads?
			* b. Implement a randomized mechanism to determine which thread goes first.
			* 
			* 3. Unlike during a deadlock, the threads in a livelock scenario 
			* are actively executing without making useful progress.
			* 4. The threads in your program are clearly not making progress. 
			* a. How might you determine if it is due to a deadlock or a livelock?
			* b. Use the Resource Monitor to investigate the program's CPU usage to see if it is actively executing.
			*/
			
			static int sushi_count_ll = 5000;
			void philosopher_ll(std::mutex& first_chopstick, std::mutex& second_chopstick);
			void demo12();
		}
	}
}