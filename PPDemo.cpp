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

		namespace condition_variablee
		{
			void hungry_person(int id) {
				int put_lid_back = 0;
				while (soup_servings > 0) {
					std::unique_lock<std::mutex> lid_lock(slow_cooker_lid); // pick up the slow cooker lid
					while ((id != soup_servings % 5) && (soup_servings > 0)) { // is it your turn to take soup?
						put_lid_back++; // it's not your turn; put the lid back...
						soup_taken.wait(lid_lock); // ...and wait...
					}
					if (soup_servings > 0) {
						soup_servings--; // it's your turn; take some soup!
						lid_lock.unlock(); // put back the lid
						soup_taken.notify_all(); // notify another thread to take their turn
					}
				}
				printf("Person %d put the lid back %u times.\n", id, put_lid_back);
			}

			void demo13()
			{
				std::thread hungry_threads[5];
				for (int i = 0; i < 5; i++) {
					hungry_threads[i] = std::thread(hungry_person, i);
				}
				for (auto& ht : hungry_threads) {
					ht.join();
				}
			}
		}
		namespace producer_consumer
		{
			class ServingLine {
			public:
				void serve_soup(int i) {
					std::unique_lock<std::mutex> ladle_lock(ladle);
					soup_queue.push(i);
					ladle_lock.unlock();
					soup_served.notify_one();
				}

				int take_soup() {
					std::unique_lock<std::mutex> ladle_lock(ladle);
					while (soup_queue.empty()) {
						soup_served.wait(ladle_lock);
					}
					int bowl = soup_queue.front();
					soup_queue.pop();
					return bowl;
				}

			private:
				std::queue<int> soup_queue;
				std::mutex ladle;
				std::condition_variable soup_served;
			};

			ServingLine serving_line = ServingLine();

			void soup_producer() {
				for (int i = 0; i < 10000; i++) { // serve a 10,000 bowls of soup
					serving_line.serve_soup(1);
				}
				serving_line.serve_soup(-1); // indicate no more soup
				printf("Producer is done serving soup!\n");
			}

			void soup_consumer() {
				int soup_eaten = 0;
				while (true) {
					int bowl = serving_line.take_soup();
					if (bowl == -1) { // check for last bowl of soup
						printf("Consumer ate %d bowls of soup.\n", soup_eaten);
						serving_line.serve_soup(-1); // put back last bowl for other consumers to take
						return;
					}
					else {
						soup_eaten += bowl; // eat the soup
					}
				}
			}

			void demo14()
			{
				std::thread olivia(soup_producer);
				std::thread barron(soup_consumer);
				std::thread steve(soup_consumer);
				olivia.join();
				barron.join();
				steve.join();
			}
		}
		namespace semaphoree
		{
			class Semaphore {
			public:
				Semaphore(unsigned long init_count) {
					count_ = init_count;
				}

				void acquire() { // decrement the internal counter
					std::unique_lock<std::mutex> lck(m_);
					while (!count_) {
						cv_.wait(lck);
					}
					count_--;
				}

				void release() { // increment the internal counter
					std::unique_lock<std::mutex> lck(m_);
					count_++;
					lck.unlock();
					cv_.notify_one();
				}

			private:
				std::mutex m_;
				std::condition_variable cv_;
				unsigned long count_;
			};

			Semaphore charger(1);

			void cell_phone(int id) {
				charger.acquire();
				printf("Phone %d is charging...\n", id);
				srand(id); // charge for "random" amount between 1-3 seconds
				std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000 + 1000));
				printf("Phone %d is DONE charging!\n", id);
				charger.release();
			}

			void demo15()
			{
				std::thread phones[10];
				for (int i = 0; i < 10; i++) {
					phones[i] = std::thread(cell_phone, i);
				}
				for (auto& p : phones) {
					p.join();
				}
			}
		}
		namespace race_condition
		{
			void cpu_work(unsigned long workUnits) {
				unsigned long x = 0;
				for (unsigned long i; i < workUnits * 1000000; i++) {
					x++;
				}
			}

			void barron_shopper() {
				cpu_work(1); // do a bit of work first
				std::scoped_lock<std::mutex> lock(pencil);
				bags_of_chips *= 2;
				printf("Barron DOUBLED the bags of chips.\n");
			}

			void olivia_shopper() {
				cpu_work(1); // do a bit of work first
				std::scoped_lock<std::mutex> lock(pencil);
				bags_of_chips += 3;
				printf("Olivia ADDED 3 bags of chips.\n");
			}

			void demo16()
			{
				std::thread shoppers[10];
				for (int i = 0; i < 10; i += 2) {
					shoppers[i] = std::thread(barron_shopper);
					shoppers[i + 1] = std::thread(olivia_shopper);
				}
				for (auto& s : shoppers) {
					s.join();
				}
				printf("We need to buy %u bags_of_chips.\n", bags_of_chips);
			}
		}
		namespace barrierr
		{
			void cpu_work(unsigned long workUnits) {
				unsigned long x = 0;
				for (unsigned long i; i < workUnits * 1000000; i++) {
					x++;
				}
			}

			void barron_shopper() {
				cpu_work(1); // do a bit of work first
				fist_bump.wait();
				std::scoped_lock<std::mutex> lock(pencil);
				bags_of_chips *= 2;
				printf("Barron DOUBLED the bags of chips.\n");
			}

			void olivia_shopper() {
				cpu_work(1); // do a bit of work first
				{
					std::scoped_lock<std::mutex> lock(pencil);
					bags_of_chips += 3;
				}
				printf("Olivia ADDED 3 bags of chips.\n");
				fist_bump.wait();
			}

			void demo17()
			{
				std::thread shoppers[10];
				for (int i = 0; i < 10; i += 2) {
					shoppers[i] = std::thread(barron_shopper);
					shoppers[i + 1] = std::thread(olivia_shopper);
				}
				for (auto& s : shoppers) {
					s.join();
				}
				printf("We need to buy %u bags_of_chips.\n", bags_of_chips);
			}
		}
		namespace latchh
		{
			void cpu_work(unsigned long workUnits) {
				unsigned long x;
				for (unsigned long i; i < workUnits * 1000000; i++) {
					x++;
				}
			}

			void barron_shopper() {
				cpu_work(1); // do a bit of work first
				fist_bump.wait();
				std::scoped_lock<std::mutex> lock(pencil);
				bags_of_chips *= 2;
				printf("Barron DOUBLED the bags of chips.\n");
			}

			void olivia_shopper() {
				cpu_work(1); // do a bit of work first
				{
					std::scoped_lock<std::mutex> lock(pencil);
					bags_of_chips += 3;
				}
				printf("Olivia ADDED 3 bags of chips.\n");
				fist_bump.count_down();
			}


			void cpu_work_ll(unsigned long workUnits);
			void barron_shopper_ll();

			void olivia_shopper_ll();
			void demo18()
			{
				std::thread shoppers[10];
				for (int i = 0; i < 10; i += 2) {
					shoppers[i] = std::thread(barron_shopper);
					shoppers[i + 1] = std::thread(olivia_shopper);
				}
				for (auto& s : shoppers) {
					s.join();
				}
				printf("We need to buy %u bags_of_chips.\n", bags_of_chips);
			}
		}
		namespace thread_pool
		{


			void vegetable_chopper_tp(int vegetable_id) {
				printf("Thread %d chopped vegetable %d.\n", std::this_thread::get_id(), vegetable_id);
			}
			void demo19()
			{
				boost::asio::thread_pool pool(4); // 4 threads
				for (int i = 0; i < 100; i++) {
					boost::asio::post(pool, [i]() {vegetable_chopper(i); });
				}
				pool.join();
			}
		}

		namespace futuree
		{
			int how_many_vegetables() {
				printf("Olivia is counting vegetables...\n");
				std::this_thread::sleep_for(std::chrono::seconds(3));
				return 42;
			}

			void demo20()
			{
				printf("Barron asks Olivia how many vegetables are in the pantry.\n");
				std::future<int> result = std::async(std::launch::async, how_many_vegetables);
				printf("Barron can do others things while he waits for the result...\n");
				printf("Olivia responded with %d.\n", result.get());
			}
		}
		namespace divide_conquer
		{
			unsigned long long recursive_sum(unsigned int lo, unsigned int hi, unsigned int depth = 0) {
				if (depth > 3) { // base case threshold
					unsigned long long sum = 0;
					for (auto i = lo; i < hi; i++) {
						sum += i;
					}
					return sum;
				}
				else {  // divide and conquer
					auto mid = (hi + lo) / 2; // middle index for splitting
					auto left = std::async(std::launch::async, recursive_sum, lo, mid, depth + 1);
					auto right = recursive_sum(mid, hi, depth + 1);
					return left.get() + right;
				}
			}

			void demo21()
			{
				unsigned long long total = recursive_sum(0, 1000000000);
				printf("Total: %lld\n", total);
			}
		}
		namespace measure_speedup
		{

			unsigned long long sequential_sum_ms(unsigned int lo, unsigned int hi) {
				unsigned long long sum = 0;
				for (auto i = lo; i < hi; i++) {
					sum += i;
				}
				return sum;
			}

			unsigned long long sequential_sum_ms(unsigned int lo, unsigned int hi, unsigned int depth = 0) {
				if (depth > 3) { // base case threshold
					unsigned long long sum = 0;
					for (auto i = lo; i < hi; i++) {
						sum += i;
					}
					return sum;
				}
				else {  // divide and conquer
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

				printf("Evaluating Sequential Implementation...\n"); \
					std::chrono::duration<double> sequential_time(0);
				auto sequential_result = sequential_sum(0, SUM_VALUE); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					auto start_time = std::chrono::high_resolution_clock::now();
					sequential_sum(0, SUM_VALUE);
					sequential_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				sequential_time /= NUM_EVAL_RUNS;

				printf("Evaluating Parallel Implementation...\n");
				std::chrono::duration<double> parallel_time(0);
				auto parallel_result = parallel_sum(0, SUM_VALUE); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					auto start_time = std::chrono::high_resolution_clock::now();
					parallel_sum(0, SUM_VALUE);
					parallel_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				parallel_time /= NUM_EVAL_RUNS;

				// display sequential and parallel results for comparison
				if (sequential_result != parallel_result) {
					printf("ERROR: Result mismatch!\n  Sequential Result = %lld\n  Parallel Result = %lld\n", sequential_result, parallel_result);
				}
				printf("Average Sequential Time: %.1f ms\n", sequential_time.count() * 1000);
				printf("  Average Parallel Time: %.1f ms\n", parallel_time.count() * 1000);
				printf("Speedup: %.2f\n", sequential_time / parallel_time);
				printf("Efficiency %.2f%%\n", 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency());
			}
		}

		namespace design_parallel
		{

		}
		namespace matrix_multiply
		{
			/* sequential implementation of matrix multiply */
			void sequential_matrix_multiply(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C) {
				for (size_t i = 0; i < num_rows_a; i++) {
					for (size_t j = 0; j < num_cols_b; j++) {
						C[i][j] = 0; // initialize result cell to zero
						for (size_t k = 0; k < num_cols_a; k++) {
							C[i][j] += A[i][k] * B[k][j];
						}
					}
				}
			}

			/* parallel implementation of matrix multiply */
			void parallel_matrix_multiply(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C) {
				size_t num_workers = std::thread::hardware_concurrency();
				size_t chunk_size = ceil((float)num_rows_a / num_workers);

				std::thread workers[num_workers];
				for (size_t i = 0; i < num_workers; i++) {
					size_t start_row_c = std::min(i * chunk_size, num_rows_a);
					size_t end_row_c = std::min((i + 1) * chunk_size, num_rows_a);
					workers[i] = std::thread(parallel_worker, A, num_rows_a, num_cols_a,
						B, num_rows_b, num_cols_b,
						C, start_row_c, end_row_c);
				}
				for (auto& w : workers) {
					w.join();
				}
			}

			/* helper function for parallel_matrix_multiply */
			void parallel_worker(long** A, size_t num_rows_a, size_t num_cols_a,
				long** B, size_t num_rows_b, size_t num_cols_b,
				long** C, size_t start_row_c, size_t end_row_c) {
				for (size_t i = start_row_c; i < end_row_c; i++) { // subset of rows in A
					for (size_t j = 0; j < num_cols_b; j++) {
						C[i][j] = 0; // initialize result cell to zero
						for (size_t k = 0; k < num_cols_a; k++) {
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
				long** A = (long**)malloc(NUM_ROWS_A * sizeof(long*));
				if (A == NULL) {
					exit(2);
				}
				for (size_t i = 0; i < NUM_ROWS_A; i++) {
					A[i] = (long*)malloc(NUM_COLS_A * sizeof(long));
					if (A[i] == NULL) {
						exit(2);
					}
					for (size_t j = 0; j < NUM_COLS_A; j++) {
						A[i][j] = rand() % 100 + 1;
					}
				}

				// intialize B with values in range 1 to 100   
				long** B = (long**)malloc(NUM_ROWS_B * sizeof(long*));
				if (B == NULL) {
					exit(2);
				}
				for (size_t i = 0; i < NUM_ROWS_B; i++) {
					B[i] = (long*)malloc(NUM_COLS_B * sizeof(long));
					if (B[i] == NULL) {
						exit(2);
					}
					for (size_t j = 0; j < NUM_COLS_B; j++) {
						B[i][j] = rand() % 100 + 1;
					}
				}

				// allocate arrays for sequential and parallel results
				long** sequential_result = (long**)malloc(NUM_ROWS_A * sizeof(long*));
				long** parallel_result = (long**)malloc(NUM_ROWS_A * sizeof(long*));
				if ((sequential_result == NULL) || (parallel_result == NULL)) {
					exit(2);
				}
				for (size_t i = 0; i < NUM_ROWS_A; i++) {
					sequential_result[i] = (long*)malloc(NUM_COLS_B * sizeof(long));
					parallel_result[i] = (long*)malloc(NUM_COLS_B * sizeof(long));
					if ((sequential_result[i] == NULL) || (parallel_result[i] == NULL)) {
						exit(2);
					}
				}

				printf("Evaluating Sequential Implementation...\n");
				std::chrono::duration<double> sequential_time(0);
				sequential_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, sequential_result); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					auto startTime = std::chrono::high_resolution_clock::now();
					sequential_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, sequential_result);
					sequential_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				sequential_time /= NUM_EVAL_RUNS;

				printf("Evaluating Parallel Implementation...\n");
				std::chrono::duration<double> parallel_time(0);
				parallel_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, parallel_result); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					auto startTime = std::chrono::high_resolution_clock::now();
					parallel_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, parallel_result);
					parallel_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				parallel_time /= NUM_EVAL_RUNS;

				// verify sequential and parallel results
				for (size_t i = 0; i < NUM_ROWS_A; i++) {
					for (size_t j = 0; j < NUM_COLS_B; j++) {
						if (sequential_result[i][j] != parallel_result[i][j]) {
							printf("ERROR: Result mismatch at row %ld, col %ld!\n", i, j);
						}
					}
				}
				printf("Average Sequential Time: %.2f ms\n", sequential_time.count() * 1000);
				printf("  Average Parallel Time: %.2f ms\n", parallel_time.count() * 1000);
				printf("Speedup: %.2f\n", sequential_time / parallel_time);
				printf("Efficiency %.2f%%\n", 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency());
			}
		}
		namespace merge_sortt
		{
			/* sequential implementation of merge sort */
			void sequential_merge_sort(int* array, unsigned int left, unsigned int right) {
				if (left < right) {
					unsigned int mid = (left + right) / 2; // find the middle point
					sequential_merge_sort(array, left, mid); // sort the left half
					sequential_merge_sort(array, mid + 1, right); // sort the right half
					merge(array, left, mid, right); // merge the two sorted halves
				}
			}

			/* parallel implementation of merge sort */
			void parallel_merge_sort(int* array, unsigned int left, unsigned int right, unsigned int depth = 0) {
				if (depth >= std::log(std::thread::hardware_concurrency())) {
					sequential_merge_sort(array, left, right);
				}
				else {
					unsigned int mid = (left + right) / 2;
					std::thread left_thread = std::thread(parallel_merge_sort, array, left, mid, depth + 1);
					parallel_merge_sort(array, mid + 1, right, depth + 1);
					left_thread.join();
					merge(array, left, mid, right);
				}
			}

			/* helper function to merge two sorted subarrays
			   array[l..m] and array[m+1..r] into array */
			void merge(int* array, unsigned int left, unsigned int mid, unsigned int right) {
				unsigned int num_left = mid - left + 1; // number of elements in left subarray
				unsigned int num_right = right - mid; // number of elements in right subarray

				// copy data into temporary left and right subarrays to be merged
				int array_left[num_left], array_right[num_right];
				std::copy(&array[left], &array[mid + 1], array_left);
				std::copy(&array[mid + 1], &array[right + 1], array_right);

				// initialize indices for array_left, array_right, and input subarrays
				unsigned int index_left = 0;    // index to get elements from array_left
				unsigned int index_right = 0;    // index to get elements from array_right
				unsigned int index_insert = left; // index to insert elements into input array

				// merge temporary subarrays into original input array
				while ((index_left < num_left) || (index_right < num_right)) {
					if ((index_left < num_left) && (index_right < num_right)) {
						if (array_left[index_left] <= array_right[index_right]) {
							array[index_insert] = array_left[index_left];
							index_left++;
						}
						else {
							array[index_insert] = array_right[index_right];
							index_right++;
						}
					}
					// copy any remain elements of array_left into array
					else if (index_left < num_left) {
						array[index_insert] = array_left[index_left];
						index_left += 1;
					}
					// copy any remain elements of array_right into array
					else if (index_right < num_right) {
						array[index_insert] = array_right[index_right];
						index_right += 1;
					}
					index_insert++;
				}
			}

			
			
			void demo24()
			{
				const int NUM_EVAL_RUNS = 100;
				const int N = 100; // number of elements to sort

				int original_array[N], sequential_result[N], parallel_result[N];
				for (int i = 0; i < N; i++) {
					original_array[i] = rand();
				}

				printf("Evaluating Sequential Implementation...\n");
				std::chrono::duration<double> sequential_time(0);
				std::copy(&original_array[0], &original_array[N - 1], sequential_result);
				sequential_merge_sort(sequential_result, 0, N - 1); // "warm up"	
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					std::copy(&original_array[0], &original_array[N - 1], sequential_result); // reset result array
					auto start_time = std::chrono::high_resolution_clock::now();
					sequential_merge_sort(sequential_result, 0, N - 1);
					sequential_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				sequential_time /= NUM_EVAL_RUNS;

				printf("Evaluating Parallel Implementation...\n");
				std::chrono::duration<double> parallel_time(0);
				std::copy(&original_array[0], &original_array[N - 1], parallel_result);
				parallel_merge_sort(parallel_result, 0, N - 1); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					std::copy(&original_array[0], &original_array[N - 1], parallel_result); // reset result array
					auto start_time = std::chrono::high_resolution_clock::now();
					parallel_merge_sort(parallel_result, 0, N - 1);
					parallel_time += std::chrono::high_resolution_clock::now() - start_time;
				}
				parallel_time /= NUM_EVAL_RUNS;

				// verify sequential and parallel results are same
				for (int i = 0; i < N; i++) {
					if (sequential_result[i] != parallel_result[i]) {
						printf("ERROR: Result mismatch at index %d!\n", i);
					}
				}
				printf("Average Sequential Time: %.2f ms\n", sequential_time.count() * 1000);
				printf("  Average Parallel Time: %.2f ms\n", parallel_time.count() * 1000);
				printf("Speedup: %.2f\n", sequential_time / parallel_time);
				printf("Efficiency %.2f%%\n", 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency());
			}
		}
		namespace download_image
		{
			/* sequential implementation of image downloader */
			size_t sequential_image_downloader(int num_images) {
				size_t total_bytes = 0;
				for (int i = 1; i <= num_images; i++) {
					total_bytes += download_image(i);
				}
				return total_bytes;
			}

			/* parallel implementation of image downloader */
			size_t parallel_image_downloader(int num_images) {
				size_t total_bytes = 0;
				std::list<std::future<size_t>> download_futures;
				for (int i = 1; i <= num_images; i++) 
				{
					download_futures.push_back(std::async(std::launch::async, download_image, i));
				}
				for (int i = 1; i <= num_images; i++) {
					total_bytes += download_futures.front().get();
					download_futures.pop_front();
				}
				return total_bytes;
			}

			/* helper function to download a single image and return size in bytes */
			size_t download_image(int image_num) {
				char url[] = "http://699340.youcanlearnit.net/imageXXX.jpg";
				sprintf(url, "http://699340.youcanlearnit.net/image%03d.jpg", ((image_num % 50) + 1));

				CURLcode res;
				curl_off_t num_bytes = 0;
				CURL* curl = curl_easy_init();
				if (curl) {
					curl_easy_setopt(curl, CURLOPT_URL, url);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
					res = curl_easy_perform(curl);
					if (res != CURLE_OK) {
						fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
					}
					res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &num_bytes);
					if (res != CURLE_OK) {
						fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
					}
					curl_easy_cleanup(curl);
				}
				return num_bytes;
			}

			/* support function for the download_image helper function */
			size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
				return size * nmemb;
			}

			void demo25()
			{
				const int NUM_EVAL_RUNS = 3;
				const int NUM_IMAGES = 50;

				printf("Evaluating Sequential Implementation...\n");
				std::chrono::duration<double> sequential_time(0);
				size_t sequential_result = sequential_image_downloader(NUM_IMAGES); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					auto startTime = std::chrono::high_resolution_clock::now();
					sequential_image_downloader(NUM_IMAGES);
					sequential_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				sequential_time /= NUM_EVAL_RUNS;

				printf("Evaluating Parallel Implementation...\n");
				std::chrono::duration<double> parallel_time(0);
				size_t parallel_result = parallel_image_downloader(NUM_IMAGES); // "warm up"
				for (int i = 0; i < NUM_EVAL_RUNS; i++) {
					auto startTime = std::chrono::high_resolution_clock::now();
					parallel_image_downloader(NUM_IMAGES);
					parallel_time += std::chrono::high_resolution_clock::now() - startTime;
				}
				parallel_time /= NUM_EVAL_RUNS;

				// display sequential and parallel results for comparison
				if (sequential_result != parallel_result) {
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