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
				std::cout << "  Olivia is joinable? "<< olivia.joinable() ? "true\n" : "false\n";

				std::cout << "Barron continues cooking soup." << std::endl;
				//only 1 second waste, olivia still has 2 second to finish 
				std::this_thread::sleep_for(std::chrono::seconds(1));
				//olivia thread is still active
				std::cout << "  Olivia is joinable? " << olivia.joinable() ? "true\n" : "false\n";
				//Baron task finishes but waiting for child thread to finish
				//Baron need to wait seconds
				std::cout << "Barron patiently waits for Olivia to finish and join..." << std::endl;
				//because of join main thread block here waiting for the child
				olivia.join();
				//here Baron calls olivia to join, when olivia is done Baron can finish
				//at this point olivia finished her task
				std::cout << "  Olivia is joinable? " << olivia.joinable() ? "true\n" : "false\n";


				std::cout << "Barron and Olivia are both done!" << std::endl;
			}
		}
		namespace detached_thread
		{

			void kitchen_cleaner()
			{
				while (true) 
				{
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
				std::cout << "Barron is done!" << std::endl;
			}
		}
		namespace data_race
		{
			void shopper()
			{
				for (int i = 0; i < 10000000; i++) 
				{
					garlic_count_dr++;
				}
			}
			void demo3()
			{
				std::cout << "data race" << std::endl;
				std::thread barron(shopper);
				std::thread olivia(shopper);
				barron.join();
				olivia.join();
				std::cout << "We should buy " << garlic_count_dr << " garlic." << std::endl;
			}

		}
		namespace mutual_exclusion
		{
			void shopper()
			{
				//wrong position, just lock the critical segment
				//pencil_me.lock();
				for (int i = 0; i < 5; i++) 
				{
					std::cout << "Shopper " << std::this_thread::get_id() << " is thinking..." << std::endl;
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
				std::cout << "We should buy " << garlic_count_me << " garlic." << std::endl;
			}

		}
		namespace atomic_object
		{

			void shopper()
			{
				for (int i = 0; i < 10000000; i++) 
				{
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
				for (int i = 0; i < 10000; i++) 
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
					std::cout << "Reader-" << id << " sees today is " << WEEKDAYS_sm[today_sm] << std::endl;
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
					std::cout << "Writer-" << id << " updated date to "<< WEEKDAYS_sm[today_sm] << " s" << std::endl;
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
					//std::scoped_lock  lock(first_chopstick, second_chopstick);
					//if (sushi_count) 
					//{
						//sushi_count--;
					//}					
					first_chopstick.lock();
					second_chopstick.lock();
					//std::scoped_lock  lock(first_chopstick, second_chopstick);
					if (sushi_count_d)
					{
						sushi_count_d--;
					}
					second_chopstick.unlock();
					first_chopstick.unlock();
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
					//std::scoped_lock lock(chopsticks);
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
					//std::scoped_lock lock(chopsticks);
					// only one thread get executed frequenty
					//chopsticks.lock();
					if (sushi_count_ss) 
					{
						sushi_count_ss--;
						sushi_eaten++;
					}
					//chopsticks.unlock();
				}
				std::cout << "Philosopher " << std::this_thread::get_id() << " ate " << sushi_eaten << " ." << std::endl;
			}
			void demo11()
			{
				std::mutex chopsticks;
				std::array<std::thread, 200> philosophers;
				for (size_t i = 0; i < philosophers.size(); i++) 
				{
					philosophers[i] = std::thread(philosopher, std::ref(chopsticks));
				}
				for (size_t i = 0; i < philosophers.size(); i++) 
				{
					philosophers[i].join();
				}
				std::cout << "The philosophers are done eating." << std::endl;
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
		
	}
}