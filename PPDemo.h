#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <shared_mutex>
#include <array>
#include <boost/thread/latch.hpp>
#include <boost/asio.hpp>
#include <future>
#include <cstdio>
#include <cmath>

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
		namespace condition_variablee
		{
			/*
			* 1. 
			* a. When should a thread typically notify a condition_variable?
			* b. After doing something to change the state associated 
			* with the condition_variable and unlocking the associated mutex
			* 
			* 2. 
			* a. Why would you use the condition_variable's notify_one() 
			* function instead of notify_all()?
			* b. You only need to wake up one waiting thread and it does not matter which one.
			* 
			* 3.
			* a. Which two mechanisms can be used together with a queue to make it thread-safe?
			* b.mutex and condition_variable
			* 
			* 4.
			* Condition variables serve as a holding place for threads to wait 
			* for a certain condition before continuing execution.
			* 5.
			* a. Condition variables work together with which other mechanism to serve as a monitor?
			* b. a mutex
			*/
			void demo13();
		}
		namespace producer_consumer
		{
			/*
			* 1.
			* a. How should the average rates of production and 
			* consumption be related in a producer-consumer architecture?
			* b. The consumption rate should be greater than or equal to the production rate.
			* 
			* 2.
			* a. Which architecture consists of a chained together series of producer-consumer pairs?
			* b. pipeline
			*/
			void demo14();
		}
		namespace semaphoree
		{
			/*
			* 1. 
			* a. Which of these is a common use case for a counting semaphore?
			* b. track the availability of a limited resource
			* 
			* 2.
			* a. What does the semaphore's acquire() function do to the counter value?
			* b. if the counter is positive, decrement its value
			* 
			* 3. 
			* a. Which statement describes the a difference between a binary semaphore and a mutex?
			* b. The binary semaphore can be acquired and released by different threads.
			* 
			* 4.
			* a. In addition to modifying the counter value, what else does calling the 
			* semaphore's release() function do?
			* b. signal another thread waiting to acquire the semaphore.
			* 
			* 5. 
			* a. What does the semaphore's release() function do to the counter value?
			* b. always increment the counter's value
			* 
			*/
			void demo15();
		}
		namespace race_condition
		{
			/*
			* 1. 
			* a. Which scenario creates the potential for a race condition to occur?
			* b. the order in which two threads execute their respective operations will change the output
			* 
			* 2.
			* a. Which of these is responsible for causing a race condition?
			* b. the execution scheduler
			* 
			* 3.
			* A race condition can occur independently of a data race.
			*/
			void demo16();
		}		
		namespace barrierr
		{
			/*
			* 1. 
			* A thread that needs to execute a section of code before the 
			* barrier should call the Barrier's wait(); function after executing the code.
			* 2. 
			* Barriers can be used to control the relative order in which threads execute certain operations
			* 
			*/
			void demo17();
		}
		namespace latchh
		{
			static unsigned int bags_of_chips_ll = 1; // start with one on the list
			static std::mutex pencil_ll;
			static boost::latch fist_bump_ll(10);

			void cpu_work_ll(unsigned long workUnits);
			void barron_shopper_ll();

			void olivia_shopper_ll();
			void demo18();
		}
		namespace thread_pool
		{
			/*
			* computational graph
			* Directed Acyclic Graph(DAG)
			* Critical path = longest path
			* work 
			* span = time in single processor
			* spawn
			* sync
			* fork
			* join
			* Ideal Parallelism = work/span
			* 
			* 1. Thread pool/ To Do List: 
			* a. create and maintain a collection of worker threads
			* b. Reuse existing threads to execute tasks
			* 
			* 2.
			* a. Which of these describes a program's "span"?
			* b. sum of the time for all task nodes along the critical path
			* 
			* 3. 
			* a. Which of these describes a program's "critical path"?
			* b. longest series of sequential operations through the program
			* 
			* 4.
			* a. What does a work-to-span ratio less than one indicate?
			* b. The work-to-span ratio cannot be less than one.
			* 
			* 5. 
			* a. Which function can used to submit tasks to a 
			* Boost C++ Library thread_pool?
			* b. boost::asio::post()
			* 
			* 6. 
			* a. Why are thread pools useful?
			* b. They reuse threads to reduce the overhead that would be 
			* required to create a new, separate thread for every concurrent task.
			* 
			* 7. 
			* a. Which of these describes a program's "work"?
			* b. sum of the time for all task nodes in a computational graph
			* 
			* 8. 
			* a. Why are computational graphs useful?
			* b. They help to identify opportunities for parallel execution.
			*/
			void vegetable_chopper_tp(int vegetable_id);
			void demo19();
		}

		namespace futuree
		{
			/*
			* 1. Future
			* a. Placeholder for a result that will be available later
			* b. Mechanism to access the result of an asynchronous operation
			* c. Read only
			* d. Can do something else while waiting for the result
			* e. future get block execution till future complete
			* 
			* 2. 
			* a. What type of object does the std::async() function return?
			* b.Future
			* 
			* 3. 
			* a. What is the purpose of a future?
			* b. It serves as a placeholder to access a result that may not been computed yet.
			* 
			* 4. 
			*/
			int how_many_vegetables();
			void demo20();
		}
		namespace divide_conquer
		{
			/*
			* 1. Divide the problem into subproblems.
			* 2. Conquer the subproblems by solving them 
			* recursively
			* 3. Combine the solutions to the subproblems
			* 4. Common stucture:
			* a. if "base case" solve problem
			* b. else 
			* - partition problem into "left" and "right" subproblems
			* - solve "left" problem using divide-and-conquer
			* - solve "right" problem using divide-and-conquer
			* - combine solutions to "left" and "right" problems
			* 5. 
			* a. In the parallel divide-and-conquer recursive_sum() 
			* function, what did the program do when it reached the base case?
			* b. compute the sum of the sub-problem using a sequential algorithm
			* 
			* 6.
			* a. What does a divide-and-conquer algorithm do when it reaches the base case?
			* b. Stop subdividing the current problem and solve it.
			*/
			unsigned long long recursive_sum(unsigned int lo, unsigned int hi, unsigned int depth = 0);
			void demo21();
		}
		namespace measure_speedup
		{
			/*
			* 1.
			* a. Throughput = (# tasks)/time
			* b. Number of task executed in a given anmount of time
			* c. expressed in action per unit of time
			*
			* 2.
			* a. Latency = time/task,
			* b. Amount of time to execute a taskc.
			* c. expressed in time
			*
			* 3
			* a. Speedup = (sequential execution time)/(paralell exection time with N workers)
			*
			* 4.
			* a. Amdahl's Law
			* - Overall Speedup = 1/[(1 - P) + P/S]
			* - P = Portion of program tha's parallelizable
			* - S = Speedup of the parallelized portion
			* 5
			* a. Why should you average the execution time across 
			* multiple runs when measuring a program's performance?
			* b. The execution time will vary from run-to-run depending 
			* on how the operating system chooses to schedule your program.
			* 
			* 6. 
			* a. Which of these describes a program's "throughput"?
			* b. number of tasks that can be executed in a certain amount of time
			* 
			* 7. 
			* a. Which of these describes a program's "latency"?
			* b. amount of time a task takes to execute
			* 
			* 8.
			* Increasing the number processors with a fixed problem size 
			* per processor leverages weak scaling to accomplish more work in same time.
			* 
			* 9. 
			* a. Which of these describes a program's "speedup"?
			* b. ratio of sequential execution time to the parallel 
			* execution time with some number of processors
			* 
			* 10.
			* a. If 70% of a program is parallelizable so that using a 4-core 
			* processor will produce a 4x speedup for that portion of the code, 
			* what is the maximum overall speedup the program can achieve?
			* b. 2.1
			* 
			* 11.
			* Increasing the number processors with a fixed total 
			* problem size leverages strong scaling to accomplish same work in less time.
			* 
			* 12.
			* a. What does calculating a program's efficiency 
			* (speedup divided by number of parallel processors) provide an indicator of?
			* b. how well the parallel processing resources are being utilized
			* 
			* 13.
			* Amdahl's Law calculates a(n) upper limit for the overall 
			* speedup that parallelizing a program will achieve.
			*/
			unsigned long long sequential_sum_ms(unsigned int lo, unsigned int hi);
			unsigned long long parallel_sum_ms(unsigned int lo, unsigned int hi, unsigned int depth = 0);
			void demo22();
		}
		
		namespace design_parallel
		{
			/*	 
				Goal: load balancing
			* 1. Partitioning
			* a. Break the problem down into discrete piece of work
			* b. Domain Decomposition
			* c. Functional Decomposition
			* e. Cyclic Decomposition
			* f. Block Decomposition
			* 2. Communication
			* a. Coordinate task execution and share information
			* b. Point to point communication
			* c. Collective Communication
			* d. Synchronous Blocking Communication
			* e. Asynchronous NonBlocking Communication
			* f. Overhead
			* Compute time/resources spent on communication
			* g. Latency
			* Time to send message from A to B
			* h. Bandwidth
			* Amount of data communicated per second (GB/s)
			* 3. Agglomeration
			* a. Combine tasks and replicate data/computation
			* b. Granularity = computation / communication
			* c. Fine-Grained Parallelism:
			* c. Coarsed-Grained Parallelism:
			* 4. Mapping
			* a. Specify where each task will execute
			* 5.
			* a. What is an advantage of using coarse-grained parallelism 
			* with a small number of large tasks?
			* b. high computation-to-communication ratio
			* 
			* 6.
			* a. What is an advantage of using fine-grained parallelism with a large number of small tasks?
			* b. good load-balancing
			* 
			* 7.
			* a. Why does the mapping design stage not apply to applications 
			* written for common desktop operating systems?
			* b. The operating system automatically handles scheduling threads to execute on each processor core.
			* 
			* 8. 
			* a. Why does the partitioning design stage occur before the communication stage?
			* b. You need to know how the problem will be divided in order to assess 
			* the communication needs between individual tasks
			* 
			* 9.
			* a. Which stage of the parallel design process focuses on breaking 
			* the problem down into discrete pieces of work?
			* b. partitioning
			* 
			* 10. 
			* a. Which stage of the parallel design process focuses on specifying where each task will execute?
			* b. mapping
			* 
			* 11.
			* a. Which scenario describes the best use case for a point-to-point communication strategy?
			* b. A small number of tasks need to communicate with each other.
			* 
			* 12.
			* a. Which stage of the parallel design process focuses on coordinating task 
			* execution and how they share information?
			* b. communication
			* 
			* 13.
			* Granularity can be described as the ratio of computation over communication. 
			* 
			* 14. 
			* a. Which stage of the parallel design process focuses on combining tasks 
			* and replicating data or computation as needed to increase program efficiency?
			* b. agglomeration
			* 
			* 15. 
			* 
			*/
		}
		namespace matrix_multiply
		{
			void sequential_matrix_multiply(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C);
			void parallel_worker(long**, size_t, size_t, long**, size_t, size_t, long**, size_t, size_t);
			void parallel_matrix_multiply(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C);
			void parallel_worker(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C, size_t start_row_c, size_t end_row_c);
			void demo23();
		}
		namespace merge_sortt
		{
			void merge(int* array, unsigned int left, unsigned int mid, unsigned int right);
			void sequential_merge_sort(int* array, unsigned int left, unsigned int right);
			/* parallel implementation of merge sort */
			void parallel_merge_sort(int* array, unsigned int left, unsigned int right, unsigned int depth = 0);
			void merge(int* array, unsigned int left, unsigned int mid, unsigned int right);
			void demo24();
		}
		namespace download_image
		{
			/* declarations for download_image helper function */
			size_t download_image(int image_num);
			size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);

			/* sequential implementation of image downloader */
			size_t sequential_image_downloader(int num_images);

			/* parallel implementation of image downloader */
			size_t parallel_image_downloader(int num_images);
			/* helper function to download a single image and return size in bytes */
			size_t download_image(int image_num);

			/* support function for the download_image helper function */
			size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
			void demo25();
		}

	}
}