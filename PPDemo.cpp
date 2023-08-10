#include "PPDemo.h"

namespace za
{
	namespace pp
	{
		namespace thread_lifecycle
		{
			void chef_olivia()
			{
				std::cout << "Olivia started & waiting for sausage to thaw..." << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(3));
				std::cout << "Olivia is done cutting sausage." << std::endl;
			}
			void demo1()
			{
				std::cout << "Barron requests Olivia's help." << std::endl;
				//thread takes 3 seconds
				std::thread olivia(chef_olivia);
				//joinable checks if thread is active
				std::cout << "Olivia is joinable? "<< (olivia.joinable() ? "true" : "false") << std::endl;

				std::cout << "Barron continues cooking soup." << std::endl;
				//only 1 second waste, olivia still has 2 second to finish 
				std::this_thread::sleep_for(std::chrono::seconds(1));
				//olivia thread is still active
				std::cout << "Olivia is joinable? " << (olivia.joinable() ? "true" : "false") << std::endl;
				//Baron task finishes but waiting for child thread to finish
				//Baron need to wait seconds
				std::cout << "Barron patiently waits for Olivia to finish and join..." << std::endl;
				//because of join main thread block here waiting for the child
				olivia.join();
				//here Baron calls olivia to join, when olivia is done Baron can finish
				//at this point olivia finished her task
				std::cout << "Olivia is joinable? " << (olivia.joinable() ? "true" : "false") << std::endl;


				std::cout << "Barron and Olivia are both done!" << std::endl;
			}
		}
		namespace detached_thread
		{

			void kitchen_cleaner()
			{
				while (true) 
				{
					//Garbage collector 
					std::cout << "Olivia cleaned the kitchen." << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			};
			void demo2()
			{
				std::thread olivia(kitchen_cleaner);
				olivia.detach();
				for (int i = 0; i < 3; i++)
				{
					std::cout << "Barron is cooking..." << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(600));
				}
				//false because detached from the main thread 
				std::cout << "Olivia is joinable? " << (olivia.joinable() ? "true" : "false") << std::endl;

				std::cout << "Barron is done!" << std::endl;
			}
		}
		namespace data_race
		{
			//each shopper cut the garlic 10.000.000 times 
			//two shoppers cut the garlic 20.000.000 times 
			//bus the os schedule the  thread  simultaenously 

			void shopper()
			{
				//for (int i = 0; i < 10000000; i++) 
				for (int i = 0; i < 5; i++) 
				{
					
					std::cout << "Shopper [" << std::this_thread::get_id() << "] is thinking..." << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					garlic_count_dr++;
				}
			}
			void demo3()
			{
				std::cout << "data race" << std::endl;
				std::thread barron(shopper);
				std::thread olivia(shopper);
				//main thread need to wait for children threads before exiting 
				barron.join();
				olivia.join();
				std::cout << "We should buy " << garlic_count_dr << " garlic." << std::endl;
			}

		}
		namespace mutual_exclusion
		{	//solution to data race
			//give access to shared data one thread at a time 
			//using a mutex 
			//lock and unlock
			void shopper()
			{
				//wrong position, just lock the critical segment
				//pencil_me.lock();
				//for (int i = 0; i < 5; i++) 
				for (int i = 0; i < 5; i++)
				{
					std::cout << "Shopper [" << std::this_thread::get_id() << "] is thinking..." << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					pencil_me.lock();
					garlic_count_me++;
					pencil_me.unlock();
				}
				//pencil_me.unlock();
			}
			void demo4()
			{
				std::cout << "mutual exclusion" << std::endl;
				std::thread barron(shopper);
				std::thread olivia(shopper);
				barron.join();
				olivia.join();

				//children threads done executing 
				std::cout << "We should buy " << garlic_count_me << " garlic." << std::endl;
			}

		}
		namespace atomic_object
		{
			//solution to data race
			//give access to shared data one thread at a time 
			//using an atomic data type

			void shopper()
			{
				for (int i = 0; i < 5; i++) 
				{
					std::cout << "Shopper [" << std::this_thread::get_id() << "] is thinking..." << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					garlic_count_ao++;
				}
			}
			void demo5()
			{
				std::cout << "atomic object" << std::endl;
				std::thread barron(shopper);
				std::thread olivia(shopper);
				barron.join();
				olivia.join();
				std::cout << "We should buy " << garlic_count_ao.load() << " garlic." << std::endl;
			}

		}
		namespace recursive_mutexx
		{
			//add_garlic first time locked for garlic, one time  
			//add_potato can't lock it this time, it needs to be 
			//unlock one time  before being available to the others 

			void add_garlic() 
			{
				pencil_rm.lock();
				garlic_count_rm++;
				pencil_rm.unlock();
			}

			void add_potato() 
			{
				pencil_rm.lock();
				potato_count_rm++;
				add_garlic();
				pencil_rm.unlock();
			}

			void shopper() 
			{
				for (int i = 0; i < 5; i++) 
				{
					add_garlic();
					add_potato();
				}
			}
			void demo6()
			{
				std::thread barron(shopper);
				std::thread olivia(shopper);
				barron.join();
				olivia.join();
				std::cout << "We should buy " << garlic_count_rm <<" garlic." << std::endl;
				std::cout << "We should buy " << potato_count_rm <<" potatoes." << std::endl;
			}
		}
		namespace try_lockk
		{
			void shopper(const char* name) 
			{
				int items_to_add = 0;
				while (items_on_notepad_tl <= 20) 
				{
					if (items_to_add && pencil_tl.try_lock()) 
					{ // add item(s) to shared items_on_notepad
						items_on_notepad_tl += items_to_add;
						std::cout << name <<" added " << items_to_add << "item(s) to notepad. " << std::endl;
						items_to_add = 0;
						std::this_thread::sleep_for(std::chrono::milliseconds(300)); // time spent writing
						pencil_tl.unlock();
					}
					else 
					{ // look for other things to buy
						std::this_thread::sleep_for(std::chrono::milliseconds(100)); // time spent searching
						items_to_add++;
						std::cout << name << " found something else to buy." << std:: endl;
					}
				}
			}
			void demo7()
			{
				std::thread barron(shopper, "Barron");
				std::thread olivia(shopper, "Olivia");
				auto start_time = std::chrono::steady_clock::now();
				barron.join();
				olivia.join();
				auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
				std::cout << "Elapsed Time: " << elapsed_time/1000.0 <<" seconds" << std::endl;
			}
		}
		namespace shared_mutexx
		{
			void calendar_reader(const int id)
			{
				for (int i = 0; i < 7; i++) 
				{
					marker_sm.lock_shared();
					std::stringstream coutUninterruptMsg;
					coutUninterruptMsg << "Reader[" << id << "] sees today is [" << WEEKDAYS_sm[today_sm] << "]\n";
					std::cout << coutUninterruptMsg.str();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					marker_sm.unlock_shared();
				}
			}

			void calendar_writer(const int id) 
			{
				for (int i = 0; i < 7; i++) 
				{
					marker_sm.lock();
					today_sm = (today_sm + 1) % 7;
					std::stringstream coutUninterruptMsg;
					coutUninterruptMsg << "Writer[" << id << "] updated date to ["<< WEEKDAYS_sm[today_sm] << "]\n";
					std::cout << coutUninterruptMsg.str();
					//std::cout << "Writer[" << id << "] updated date to ["<< WEEKDAYS_sm[today_sm] << "]" << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					marker_sm.unlock();
				}
			}                  

			void demo8()
			{
				// create ten reader threads ...but only two writer threads
				std::array<std::thread, 10> readers;
				for (unsigned int i = 0; i < readers.size(); i++) 
				{
					readers[i] = std::thread(calendar_reader, i);
				}
				std::array<std::thread, 2> writers;
				for (unsigned int i = 0; i < writers.size(); i++) 
				{
					writers[i] = std::thread(calendar_writer, i);
				}

				// wait for readers and writers to finish
				for (unsigned int i = 0; i < readers.size(); i++) 
				{
					readers[i].join();
				}
				for (unsigned int i = 0; i < writers.size(); i++) 
				{
					writers[i].join();
				}
			}
		}
		namespace deadlockk
		{

			void philosopher(std::mutex& first_chopstick, std::mutex& second_chopstick) 
			{
				while (sushi_count_d > 0) 
				{
					//Debug for visual studio
					std::scoped_lock  lock(first_chopstick, second_chopstick);
					if (sushi_count_d)
					{
						sushi_count_d--;
					}	
					// lock is detroyed when it goes out of the scope 
					//first_chopstick.lock();
					//second_chopstick.lock();
					//std::scoped_lock  lock(first_chopstick, second_chopstick);
					//if (sushi_count_d)
					//{
					//	sushi_count_d--;
					//}
					//second_chopstick.unlock();
					//first_chopstick.unlock();
				}
			}
			void demo9()
			{
				std::mutex chopstick_a, chopstick_b;
				std::thread barron(philosopher, std::ref(chopstick_a), std::ref(chopstick_b));
				std::thread olivia(philosopher, std::ref(chopstick_b), std::ref(chopstick_a));
				barron.join();
				olivia.join();
				std::cout << "The philosophers are done eating." << std::endl;
			}
		}
		namespace abandoned_lock
		{

			void philosopher_al(std::mutex& chopsticks)
			{
				while (sushi_count_al > 0)
				{
					//chopsticks.lock();
					// to be fixed
					std::scoped_lock lock(chopsticks);
					if (sushi_count_al)
					{
						sushi_count_al--;
					}
					if (sushi_count_al == 10)
					{
						std::cout << "This philosopher has had enough!" << std::endl;
						break;
					}
					//won't be reached if break 
					//chopsticks.unlock();
					//with scoped_lock the lock get destroyed immediately 
					//when out of scope based on raii concept 
				}
			}

			void demo10()
			{
				std::mutex chopsticks;
				std::thread barron(philosopher_al, std::ref(chopsticks));
				std::thread olivia(philosopher_al, std::ref(chopsticks));
				barron.join();
				olivia.join();
				std::cout << "The philosophers are done eating." << std::endl;
			}
		}
		namespace starvation
		{
			void philosopher_ss(std::mutex& chopsticks)
			{
				int sushi_eaten = 0;
				while (sushi_count_ss > 0) 
				{
					//to be fixed
					std::scoped_lock lock(chopsticks);
					//only one thread get executed frequently
					//chopsticks.lock();
					if (sushi_count_ss) 
					{
						sushi_count_ss--;
						sushi_eaten++;
					}
					//chopsticks.unlock();
				}
				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Philosopher " << std::this_thread::get_id() << " ate [" << sushi_eaten << "]\n";
				std::cout << coutUninterruptMsg.str();
			}
			void foo()
			{
				// do stuff...
			};
			void bar(int x)
			{
				// do stuff...
			}
			void demo11()
			{
				std::mutex chopsticks;
				std::array<std::thread, 200> philosophers;
				for (size_t i = 0; i < philosophers.size(); i++) 
				{
					philosophers[i] = std::thread(philosopher_ss, std::ref(chopsticks));
				}
				for (size_t i = 0; i < philosophers.size(); i++) 
				{
					philosophers[i].join();
				}
				std::cout << "The philosophers are done eating." << std::endl;


				//std::thread first(foo);     // spawn new thread that calls foo()
				//std::thread second(bar, 0);  // spawn new thread that calls bar(0)

				//std::cout << "main, foo and bar now execute concurrently..." << std::endl;;

				//// synchronize threads:
				//first.join();                // pauses until first finishes
				//second.join();               // pauses until second finishes

				//std::cout << "foo and bar completed." << std::endl;
	
			}
		}
		namespace live_lock
		{

			void philosopher_ll(std::mutex& first_chopstick, std::mutex& second_chopstick)
			{
				while (sushi_count_ll > 0) 
				{
					first_chopstick.lock();
					if (!second_chopstick.try_lock()) 
					{
						first_chopstick.unlock();
						std::this_thread::yield();
					}
					else 
					{
						if (sushi_count_ll)
						{
							sushi_count_ll--;
						}
						second_chopstick.unlock();
						first_chopstick.unlock();
					}
				}
			}
			void demo12()
			{
				std::mutex chopstick_a, chopstick_b;
				std::thread barron(philosopher_ll, std::ref(chopstick_a), std::ref(chopstick_b));
				std::thread olivia(philosopher_ll, std::ref(chopstick_b), std::ref(chopstick_a));
				std::thread steve(philosopher_ll, std::ref(chopstick_a), std::ref(chopstick_b));
				std::thread nikki(philosopher_ll, std::ref(chopstick_b), std::ref(chopstick_a));
				barron.join();
				olivia.join();
				steve.join();
				nikki.join();
				std::cout << "The philosophers are done eating." << std::endl;
			}
		}	
		namespace condition_variablee
		{
			/**
			* Two hungry threads, anxiously waiting for their turn to take soup
			*/
			void hungry_person(int id) 
			{
				int put_lid_back = 0;
				while (soup_servings_cv > 0) 
				{
					// pick up the slow cooker lid
					// with unique_lock no need to lock and unlock
					// mutex gets unlock or destroyed when out of scope using raii
					// contrary to scoped_lock unique_lock has lock, unlock and try_lock
					// member functions
					std::unique_lock<std::mutex> lid_lock(slow_cooker_lid_cv); 
					// is it your turn to take soup?
					while ((id != soup_servings_cv % 5) && (soup_servings_cv > 0)) 
					{ 
						// it's not your turn; put the lid back...
						put_lid_back++; 
						// ...and wait...
						// wait for the mutex to be available 
						// blocking call and exit
						soup_taken_cv.wait(lid_lock);
					}

					// it's your turn; take some soup!
					// mutex taken/locked 
					if (soup_servings_cv > 0) 
					{
						soup_servings_cv--; 
						// put back the lid
						lid_lock.unlock(); 
						// notify another thread to take their turn
						// inform the waiting/sleeping thread to continue execution 
						// starting from its wait call 
						// broacast to all active thread 
						soup_taken_cv.notify_all(); 
					}
				}
				
				// successfully escape the wait condition
				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Person [" << id  << "] put the lid back [" << put_lid_back << "] times.\n";
				std::cout << coutUninterruptMsg.str();

			}
			
			
			void demo13()
			{
				std::thread hungry_threads[5];
				for (int i = 0; i < 5; i++) 
				{
					hungry_threads[i] = std::thread(hungry_person, i);
				}
				for (auto& ht : hungry_threads) 
				{
					ht.join();
				}
			}
		}
		namespace producer_consumer
		{
			/**
			 * Threads serving and eating soup
			 */
			// producer 
			void ServingLine::serve_soup(int i)
			{
				std::unique_lock<std::mutex> ladle_lock(ladle);
				soup_queue.push(i);
				ladle_lock.unlock();
				soup_served.notify_one();
			}			
			// consummer 
			int ServingLine::take_soup()
			{
				std::unique_lock<std::mutex> ladle_lock(ladle);
				// wait till there is a soup 
				// served on the table 
				while (soup_queue.empty()) 
				{
					soup_served.wait(ladle_lock);
				}
				// eat the soup 
				int bowl = soup_queue.front();
				soup_queue.pop();
				return bowl;
			}
			

			void soup_producer(std::string name)
			{
				for (int i = 0; i < 10000; i++)
				{ 
					// serve a 10,000 bowls of soup
					// paralelly the consummer thread is running to consume the data 
					// from the shared queue
					serving_line.serve_soup(1);
				}
				serving_line.serve_soup(-1); // indicate no more soup
				std::cout << "Producer is done serving soup!" << std::endl;
			}

			void soup_consumer(std::string name)
			{
				int soup_eaten = 0;
				while (true) 
				{
					//consumer wait untill there is an item on the queue to take 
					int bowl = serving_line.take_soup();

					//at this point consumer is notified for the end of the 
					//production operation
					if (bowl == -1) 
					{ // check for last bowl of soup
						std::stringstream coutUninterruptMsg;
						coutUninterruptMsg << "Consumer[" << name <<"] ate [" << soup_eaten << "] bowls of soup.\n";
						std::cout << coutUninterruptMsg.str();
						// indicate the absence of item on the serving line
						serving_line.serve_soup(-1); 
						// put back last bowl for other consumers to take
						return;
					}
					else 
					{
						soup_eaten += bowl; // eat the soup
					}
				}
			}

			void demo14()
			{
				// usually we have more consummer than producer
				// consummer must wait producer to produce 
				std::thread olivia(soup_producer, "olivia");
				std::thread barron(soup_consumer, "barron");
				std::thread steve(soup_consumer, "steve");
				olivia.join();
				barron.join();
				steve.join();
			}
		}
		namespace semaphore
		{
			Semaphore::Semaphore(unsigned long init_count)
			{
				count_ = init_count;
			}
			void Semaphore::acquire()
			{
				// decrement the internal counter
				std::unique_lock<std::mutex> lck(m_);
				// if zero no slot is available wait here until someone finish
				// using the notfy one keyword 
				while (!count_) 
				{
					cv_.wait(lck);
				}
				count_--;
			}			
			void Semaphore::release()
			{
				// increment the internal counter
				// finish using the slot
				// make it available for other cellphone to use
				std::unique_lock<std::mutex> lck(m_);
				count_++;
				lck.unlock();
				cv_.notify_one();
			}
		
			
			void cell_phone(int id)
			{
				charger.acquire();

				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Phone [" << id << "] is charging...\n";
				std::cout << coutUninterruptMsg.str();

				srand(id); // charge for "random" amount between 1-3 seconds
				std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000 + 1000));
				coutUninterruptMsg.clear();
				coutUninterruptMsg << "Phone [" << id << "] is DONE charging...\n";
				std::cout << coutUninterruptMsg.str();
				charger.release();
			}

			void demo15()
			{
				std::thread phones[10];
				for (int i = 0; i < 10; i++) 
				{
					phones[i] = std::thread(cell_phone, i);
				}
				for (auto& p : phones) 
				{
					p.join();
				}
			}
		}
		namespace race_condition
		{


			void cpu_work(unsigned long workUnits)
			{
				unsigned long x = 0;
				for (unsigned long i = 0; i < workUnits * 1000000; i++) 
				{
					x++;
				}
			}
			void barron_shopper()
			{
				// no priority is set here 
				// both shopper can double or 3 to the item
				cpu_work(1); // do a bit of work first
				std::scoped_lock<std::mutex> lock(pencil);
				bags_of_chips *= 2;
				std::cout << "Barron DOUBLED the bags of chips." << std::endl;
			}
			void olivia_shopper()
			{
				cpu_work(1); // do a bit of work first
				{
					std::scoped_lock<std::mutex> lock(pencil);
					bags_of_chips += 3;
				}
				std::cout << "Olivia ADDED 3 bags of chips." << std::endl;
			}

			void demo16()
			{
				std::thread shoppers[10];
				for (int i = 0; i < 10; i += 2) 
				{
					shoppers[i] = std::thread(barron_shopper);
					shoppers[i + 1] = std::thread(olivia_shopper);
				}
				for (auto& s : shoppers) 
				{
					s.join();
				}

				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "We need to buy [" << bags_of_chips << "] bags_of_chips.\n";
				std::cout << coutUninterruptMsg.str();
			}
		}				
		namespace barrierr
		{


			void cpu_work(unsigned long workUnits)
			{
				unsigned long x = 0;
				for (unsigned long i = 0; i < workUnits * 1000000; i++) 
				{
					x++;
				}
			}
			void barron_shopper()
			{
				cpu_work(1); // do a bit of work first
				//fist_bump.arrive_and_wait();
				
				std::barrier<>::arrival_token&& arrival = fist_bump.arrive();
				fist_bump.wait(std::move(arrival));
				std::scoped_lock<std::mutex> lock(pencil);
				bags_of_chips *= 2;
				std::cout << "Barron DOUBLED the bags of chips." << std::endl;
			}
			void olivia_shopper()
			{
				cpu_work(1); // do a bit of work first
				{
					std::scoped_lock<std::mutex> lock(pencil);
					bags_of_chips += 3;
				}
				std::cout << "Olivia ADDED 3 bags of chips." << std::endl;
				std::barrier<>::arrival_token&& arrival = fist_bump.arrive();
				fist_bump.wait(std::move(arrival));
			}

			void demo17()
			{
				std::thread shoppers[10];
				for (int i = 0; i < 10; i += 2) 
				{
					shoppers[i] = std::thread(barron_shopper);
					shoppers[i + 1] = std::thread(olivia_shopper);
				}
				for (auto& s : shoppers) 
				{
					s.join();
				}

				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "We need to buy [" << bags_of_chips << "] bags_of_chips.\n";
				std::cout << coutUninterruptMsg.str();
			}
		}			
		namespace latchh
		{


			void cpu_work(unsigned long workUnits)
			{
				unsigned long x = 0;
				for (unsigned long i = 0; i < workUnits * 1000000; i++) 
				{
					x++;
				}
			}
			void barron_shopper()
			{
				cpu_work(1); // do a bit of work first
				//fist_bump.arrive_and_wait();
				fist_bump.arrive_and_wait();
				std::scoped_lock<std::mutex> lock(pencil);
				bags_of_chips *= 2;
				std::cout << "Barron DOUBLED the bags of chips.\n";
			}
			void olivia_shopper()
			{
				cpu_work(1); // do a bit of work first
				{
					std::scoped_lock<std::mutex> lock(pencil);
					bags_of_chips += 3;
				}
				std::cout << "Olivia ADDED 3 bags of chips.\n";
				fist_bump.count_down();
			}

			void demo18()
			{
				std::thread shoppers[10];
				for (int i = 0; i < 10; i += 2) 
				{
					shoppers[i] = std::thread(barron_shopper);
					shoppers[i + 1] = std::thread(olivia_shopper);
				}
				for (auto& s : shoppers) 
				{
					s.join();
				}

				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "We need to buy [" << bags_of_chips << "] bags_of_chips.\n";
				std::cout << coutUninterruptMsg.str();
			}
		}
		namespace thread_pool
		{
			/**
			 * Chopping vegetables with a thread pool
			 */
			void vegetable_chopper(int vegetable_id)
			{

				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Thread [" << std::this_thread::get_id() <<"] chopped vegetable[" << vegetable_id << "]\n";
				std::cout << coutUninterruptMsg.str();
			};
			

			void demo19()
			{
				// 4 threads
				// provide equivalent time of execution 
				// to each thread 
				// only 4 execution session is created 
				boost::asio::thread_pool pool(4); 
				for (int i = 0; i < 100; i++) 
				{
					boost::asio::post(pool, [i]() {vegetable_chopper(i); });
				}
				pool.join();
			}
		}
		namespace futuree
		{
			/**
			 * Check how many vegetables are in the pantry
			 */
			int how_many_vegetables()
			{
				std::cout<< "Olivia is counting vegetables...\n";
				std::this_thread::sleep_for(std::chrono::seconds(3));
				return 42;
			};

			void demo20()
			{
				std::cout << "Barron asks Olivia how many vegetables are in the pantry.\n";

				//non blocking call 
				std::future<int> result = std::async(std::launch::async, how_many_vegetables);
				std::cout << "Barron can do others things while he waits for the result...\n";

				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Olivia responded with  [" << result.get() << "]\n";
				std::cout << coutUninterruptMsg.str(); 
			}
		}
		namespace divide_and_conquer
		{
			/**
			 * Recursively sum range of numbers
			 */
			unsigned long long recursive_sum(unsigned int lo, unsigned int hi, unsigned int depth)
			{
				if (depth > 3) 
				{ // base case threshold
					unsigned long long sum = 0;
					for (auto i = lo; i < hi; i++) 
					{
						sum += i;
					}
					return sum;
				}
				else 
				{  // divide and conquer
					auto mid = (hi + lo) / 2; // middle index for splitting
					// left sum is independant of the right hand side sum 
					// calculate left and right asynchronously or parallely for speeding up 
					// the algorithm 
					auto left = std::async(std::launch::async, recursive_sum, lo, mid, depth + 1);
					auto right = recursive_sum(mid, hi, depth + 1);

					// get is a blocking function
					// wait till the result left is computed
					return left.get() + right;
				}
			}
			void demo21()
			{
				unsigned long long total = recursive_sum(0, 1000000000);

				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Total: [" << total << "]\n";
				std::cout << coutUninterruptMsg.str();
			}
		}
		namespace measure_speedup
		{

			unsigned long long sequential_sum(unsigned int lo, unsigned int hi)
			{
				unsigned long long sum = 0;
				for (auto i = lo; i < hi; i++) 
				{
					sum += i;
				}
				return sum;
			}

			unsigned long long parallel_sum(unsigned int lo, unsigned int hi, unsigned int depth)
			{
				if (depth > 3) 
				{ // base case threshold
					unsigned long long sum = 0;
					for (auto i = lo; i < hi; i++) 
					{
						sum += i;
					}
					return sum;
				}
				else 
				{  // divide and conquer
					auto mid = (hi + lo) / 2; // middle index for splitting
					auto left = std::async(std::launch::async, parallel_sum, lo, mid, depth + 1);
					auto right = parallel_sum(mid, hi, depth + 1);
					return left.get() + right;
				}
			}

			void demo22()
			{
				const int NUM_EVAL_RUNS = 10;
				const int SUM_VALUE = 1000000;

				std::cout << "Evaluating Sequential Implementation...\n"; 
				std::chrono::duration<double> sequential_time(0);
				auto sequential_result = sequential_sum(0, SUM_VALUE); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					auto start_time = std::chrono::high_resolution_clock::now();
					sequential_sum(0, SUM_VALUE);
					sequential_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				sequential_time /= NUM_EVAL_RUNS;

				std::cout << "Evaluating Parallel Implementation...\n";
				std::chrono::duration<double> parallel_time(0);
				auto parallel_result = parallel_sum(0, SUM_VALUE); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					auto start_time = std::chrono::high_resolution_clock::now();
					parallel_sum(0, SUM_VALUE);
					parallel_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				parallel_time /= NUM_EVAL_RUNS;
				std::stringstream coutUninterruptMsg;
				// display sequential and parallel results for comparison
				if (sequential_result != parallel_result) 
				{
					coutUninterruptMsg << "ERROR: Result mismatch!\n"
						<< "Sequential Result: [" << sequential_result << "]\n"
						<< "Parallel Result: [" << parallel_result << "]\n";
					std::cout << coutUninterruptMsg.str();
				}

				coutUninterruptMsg.clear();
				coutUninterruptMsg << "Average Sequential Time: [" << sequential_time.count() * 1000 << "] ms\n";

				coutUninterruptMsg << "Average Parallel Time: [" << parallel_time.count() * 1000 << "] ms\n";

				coutUninterruptMsg << "Speedup: [" << sequential_time / parallel_time << "]\n";

				coutUninterruptMsg << "Efficiency: [" << 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency() << "]\n";
				coutUninterruptMsg << "Number of threads supported: [" <<  std::thread::hardware_concurrency() << "]\n";
				std::cout << coutUninterruptMsg.str();
			}
		}
		namespace matrix_multiplyy
		{
			/**
			 *  Multiply two matrices
			 */
			 /* sequential implementation of matrix multiply */
			void sequential_matrix_multiply(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C)
			{
				for (size_t i = 0; i < num_rows_a; i++) 
				{
					for (size_t j = 0; j < num_cols_b; j++) 
					{
						C[i][j] = 0; // initialize result cell to zero
						// dot product between row vector i  of A and column vector j of B
						// row i of A has num_cols_a
						// column j of B has num_rows_b
						// for multiplication validity num_cols_a ==  num_rows_b
						// row elements of row i are A[i][0], A[i][1], ... A[i][k], ...A[i][num_cols_a - 1]
						// column elements of colum j are A[0][j], A[1][j], ... A[k][j], ...A[num_rows_b - 1][j]
						for (size_t k = 0; k < num_cols_a; k++) 
						{
							C[i][j] += A[i][k] * B[k][j];
						}
					}
				}
			}


			/* parallel implementation of matrix multiply */
			void parallel_matrix_multiply(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C)
			{
				size_t num_workers = std::thread::hardware_concurrency();
				size_t chunk_size = ceil((float)num_rows_a / num_workers);

				//std::thread workers[num_workers];
				std::vector< std::thread> workers(num_workers);
				for (size_t i = 0; i < num_workers; i++) 
				{
					size_t start_row_c = std::min(i * chunk_size, num_rows_a);

					size_t end_row_c = std::min((i + 1) * chunk_size, num_rows_a);

					workers[i] = std::thread(parallel_worker, A, num_rows_a, num_cols_a,
						B, num_rows_b, num_cols_b,
						C, start_row_c, end_row_c);
				}
				for (auto& w : workers) 
				{
					w.join();
				}
			}

			/* helper function for parallel_matrix_multiply */
			void parallel_worker(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C, size_t start_row_c, size_t end_row_c)
			{
				for (size_t i = start_row_c; i < end_row_c; i++) 
				{ // subset of rows in A
					for (size_t j = 0; j < num_cols_b; j++) 
					{
						C[i][j] = 0; // initialize result cell to zero
						for (size_t k = 0; k < num_cols_a; k++) 
						{
							C[i][j] += A[i][k] * B[k][j];
						}
					}
				}
			
			}

			void demo23()
			{
				const int NUM_EVAL_RUNS = 3;
				const size_t NUM_ROWS_A = 10;
				const size_t NUM_COLS_A = 10;
				const size_t NUM_ROWS_B = NUM_COLS_A;
				const size_t NUM_COLS_B = 10;

				// intialize A with values in range 1 to 100   
				// create rows  
				long** A = (long**)malloc(NUM_ROWS_A * sizeof(long*));
				if (A == NULL) 
				{
					exit(2);
				}
				for (size_t i = 0; i < NUM_ROWS_A; i++) 
				{
					// create columns for each row 
					A[i] = (long*)malloc(NUM_COLS_A * sizeof(long));

					if (A[i] == NULL) 
					{
						exit(2);
					}
					// fill up the columns of each column on the row 
					// with random number between 1 and 100 
					for (size_t j = 0; j < NUM_COLS_A; j++) 
					{
						A[i][j] = rand() % 100 + 1;
					}
				}

				// intialize B with values in range 1 to 100 
				// create rows of B
				// each rows has the same number of columns 
				long** B = (long**)malloc(NUM_ROWS_B * sizeof(long*));
				if (B == NULL) 
				{
					exit(2);
				}
				for (size_t i = 0; i < NUM_ROWS_B; i++) 
				{
					// for each row create its set of columns 
					B[i] = (long*)malloc(NUM_COLS_B * sizeof(long));
					if (B[i] == NULL) 
					{
						exit(2);
					}
					// fill up the columns of each row 
					for (size_t j = 0; j < NUM_COLS_B; j++) 
					{
						B[i][j] = rand() % 100 + 1;
					}
				}

				// allocate arrays for sequential and parallel results
				long** sequential_result = (long**)malloc(NUM_ROWS_A * sizeof(long*));
				long** parallel_result = (long**)malloc(NUM_ROWS_A * sizeof(long*));
				if ((sequential_result == NULL) || (parallel_result == NULL)) 
				{
					exit(2);
				}
				for (size_t i = 0; i < NUM_ROWS_A; i++) 
				{
					sequential_result[i] = (long*)malloc(NUM_COLS_B * sizeof(long));
					parallel_result[i] = (long*)malloc(NUM_COLS_B * sizeof(long));
					if ((sequential_result[i] == NULL) || (parallel_result[i] == NULL)) 
					{
						exit(2);
					}
				}

				std::cout << "Evaluating Sequential Implementation...\n";
				std::chrono::duration<double> sequential_time(0);
				sequential_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, sequential_result); // "warm up"
				
				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					auto startTime = std::chrono::high_resolution_clock::now();
					sequential_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, sequential_result);
					sequential_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				sequential_time /= NUM_EVAL_RUNS;

				std::cout << "Evaluating Parallel Implementation...\n";
				std::chrono::duration<double> parallel_time(0);
				parallel_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, parallel_result); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					auto startTime = std::chrono::high_resolution_clock::now();
					parallel_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, parallel_result);
					parallel_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				parallel_time /= NUM_EVAL_RUNS;

				// verify sequential and parallel results
				for (size_t i = 0; i < NUM_ROWS_A; i++) 
				{
					for (size_t j = 0; j < NUM_COLS_B; j++) 
					{
						if (sequential_result[i][j] != parallel_result[i][j]) 
						{
							std::stringstream coutUninterruptMsgLoc;
							coutUninterruptMsgLoc << "ERROR: Result mismatch at row[" << i << "], col[" << j << "]\n";
							std::cout << coutUninterruptMsgLoc.str();
						}
					}
				}


				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Average Sequential Time: [" << sequential_time.count() * 1000 << "]ms\n";
				coutUninterruptMsg << "Average Parallel Time: [" << parallel_time.count() * 1000 << "]ms\n";
				coutUninterruptMsg << "Speedup: [" << sequential_time / parallel_time << "]\n";
				coutUninterruptMsg << "Efficiency: [" << 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency() << "]\n";
				std::cout << coutUninterruptMsg.str();
			}
		}
		namespace merge_sortt
		{
			/**
			 *  Sort an array of random integers with merge sort
			 */
			 /* helper function to merge two sorted subarrays
				array[l..m] and array[m+1..r] into array */
			void merge(int* array, unsigned int left, unsigned int mid, unsigned int right)
			{
				unsigned int num_left = mid - left + 1; // number of elements in left subarray
				unsigned int num_right = right - mid; // number of elements in right subarray

				// copy data into temporary left and right subarrays to be merged
				//int array_left[num_left], array_right[num_right];
				std::vector<int> array_left, array_right;
				//std::vector<int> array_left(num_left), array_right(num_right);
				std::copy(&array[left], &array[mid + 1], std::back_inserter(array_left));
				std::copy(&array[mid + 1], &array[right + 1], std::back_inserter(array_right));

				// initialize indices for array_left, array_right, and input subarrays
				unsigned int index_left = 0;    // index to get elements from array_left
				unsigned int index_right = 0;    // index to get elements from array_right
				unsigned int index_insert = left; // index to insert elements into input array

				// merge temporary subarrays into original input array
				while ((index_left < num_left) || (index_right < num_right)) 
				{
					if ((index_left < num_left) && (index_right < num_right)) 
					{
						if (array_left[index_left] <= array_right[index_right]) 
						{
							array[index_insert] = array_left[index_left];
							index_left++;
						}
						else 
						{
							array[index_insert] = array_right[index_right];
							index_right++;
						}
					}
					// copy any remain elements of array_left into array
					else if (index_left < num_left) 
					{
						array[index_insert] = array_left[index_left];
						index_left += 1;
					}
					// copy any remain elements of array_right into array
					else if (index_right < num_right) 
					{
						array[index_insert] = array_right[index_right];
						index_right += 1;
					}
					index_insert++;
				}
			}

			/* sequential implementation of merge sort */
			void sequential_merge_sort(int* array, unsigned int left, unsigned int right)
			{
				if (left < right) 
				{
					unsigned int mid = (left + right) / 2; // find the middle point
					sequential_merge_sort(array, left, mid); // sort the left half
					sequential_merge_sort(array, mid + 1, right); // sort the right half
					merge(array, left, mid, right); // merge the two sorted halves
				}
			}

			/* parallel implementation of merge sort */
			void parallel_merge_sort(int* array, unsigned int left, unsigned int right, unsigned int depth)
			{
				if (depth >= std::log(std::thread::hardware_concurrency())) 
				{
					sequential_merge_sort(array, left, right);
				}
				else 
				{
					unsigned int mid = (left + right) / 2;
					std::thread left_thread = std::thread(parallel_merge_sort, array, left, mid, depth + 1);
					parallel_merge_sort(array, mid + 1, right, depth + 1);
					left_thread.join();
					merge(array, left, mid, right);
				}
			}

			void demo24()
			{
				const int NUM_EVAL_RUNS = 100;
				const int N = 100; // number of elements to sort

				int original_array[N], sequential_result[N], parallel_result[N];
				for (int i = 0; i < N; i++) 
				{
					original_array[i] = rand();
				}

				std::cout << "Evaluating Sequential Implementation...\n";
				std::chrono::duration<double> sequential_time(0);
				std::copy(&original_array[0], &original_array[N - 1], sequential_result);
				sequential_merge_sort(sequential_result, 0, N - 1); // "warm up"	

				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					std::copy(&original_array[0], &original_array[N - 1], sequential_result); // reset result array
					auto start_time = std::chrono::high_resolution_clock::now();
					sequential_merge_sort(sequential_result, 0, N - 1);
					sequential_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				sequential_time /= NUM_EVAL_RUNS;

				std::cout << "Evaluating Parallel Implementation...\n";
				std::chrono::duration<double> parallel_time(0);
				std::copy(&original_array[0], &original_array[N - 1], parallel_result);
				parallel_merge_sort(parallel_result, 0, N - 1); // "warm up"

				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					std::copy(&original_array[0], &original_array[N - 1], parallel_result); // reset result array
					auto start_time = std::chrono::high_resolution_clock::now();
					parallel_merge_sort(parallel_result, 0, N - 1);
					parallel_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				parallel_time /= NUM_EVAL_RUNS;

				// verify sequential and parallel results are same
				for (int i = 0; i < N; i++) 
				{
					if (sequential_result[i] != parallel_result[i]) 
					{

						std::stringstream coutUninterruptMsgLoc;
						coutUninterruptMsgLoc << "ERROR: Result mismatch at index [" << i << "]\n";
						std::cout << coutUninterruptMsgLoc.str();
					}
				}


				std::stringstream coutUninterruptMsg;
				coutUninterruptMsg << "Average Sequential Time: [" << sequential_time.count() * 1000 << "]ms\n";
				coutUninterruptMsg << "Average Parallel Time: [" << parallel_time.count() * 1000 << "]ms\n";
				coutUninterruptMsg << "Speedup: [" << sequential_time / parallel_time << "]\n";
				coutUninterruptMsg << "Efficiency: [" << 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency() << "]\n";
				std::cout << coutUninterruptMsg.str();
			}
		}
		namespace download_images
		{
			/* helper function to download a single image and return size in bytes */
			size_t download_image(int image_num)
			{
				curl_off_t num_bytes = 0;
				char url[] = "http://699340.youcanlearnit.net/imagexxx.jpg";
				sprintf_s(url, "http://699340.youcanlearnit.net/image%03d.jpg", ((image_num % 50) + 1));


				CURLcode res;
	
				CURL* curl = curl_easy_init();
				if (curl) 
				{
					curl_easy_setopt(curl, CURLOPT_URL, url);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
					res = curl_easy_perform(curl);
					if (res != CURLE_OK) 
					{
						fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
					}
					res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &num_bytes);
					if (res != CURLE_OK) 
					{
						fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
					}
					curl_easy_cleanup(curl);
				}
				return num_bytes;
			}
			/* support function for the download_image helper function */
			size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
			{
				return size * nmemb;
			}

			/* sequential implementation of image downloader */
			size_t sequential_image_downloader(int num_images)
			{
				size_t total_bytes = 0;
				for (int i = 1; i <= num_images; i++) 
				{
					total_bytes += download_image(i);
				}
				return total_bytes;
			}

			/* parallel implementation of image downloader */
			size_t parallel_image_downloader(int num_images)
			{
				size_t total_bytes = 0;
				std::list<std::future<size_t>> download_futures;
				for (int i = 1; i <= num_images; i++) 
				{
					download_futures.push_back(std::async(std::launch::async, download_image, i));
				}
				for (int i = 1; i <= num_images; i++) 
				{
					total_bytes += download_futures.front().get();
					download_futures.pop_front();
				}
				return total_bytes;
			}

			void demo25()
			{
				const int NUM_EVAL_RUNS = 3;
				const int NUM_IMAGES = 50;

				printf("Evaluating Sequential Implementation...\n");
				std::chrono::duration<double> sequential_time(0);
				size_t sequential_result = sequential_image_downloader(NUM_IMAGES); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					auto startTime = std::chrono::high_resolution_clock::now();
					sequential_image_downloader(NUM_IMAGES);
					sequential_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				sequential_time /= NUM_EVAL_RUNS;

				printf("Evaluating Parallel Implementation...\n");
				std::chrono::duration<double> parallel_time(0);
				size_t parallel_result = parallel_image_downloader(NUM_IMAGES); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) 
				{
					auto startTime = std::chrono::high_resolution_clock::now();
					parallel_image_downloader(NUM_IMAGES);
					parallel_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				parallel_time /= NUM_EVAL_RUNS;

				// display sequential and parallel results for comparison
				if (sequential_result != parallel_result) 
				{
					printf("ERROR: Result mismatch!\n  Sequential Result = %zd\n  Parallel Result = %zd\n", sequential_result, parallel_result);
				}
				printf("Average Sequential Time: %.2f ms\n", sequential_time.count() * 1000);
				printf("  Average Parallel Time: %.2f ms\n", parallel_time.count() * 1000);
				printf("Speedup: %.2f\n", sequential_time / parallel_time);
				printf("Efficiency %.2f%%\n", 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency());
			}
		}
	}
}