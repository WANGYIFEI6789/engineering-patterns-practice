#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace duan{
    namespace locks_demo {
        class Counter{
        private:
            int count_;
            std::mutex mtx_; // 互斥锁
        public:
            void increment(){
                std
            }

            void decrement() {
                std::lock_guard<std::mutex> lock(mutex_);
                --count_;
            }
    
            int get() const {
                std::lock_guard<std::mutex> lock(mutex_);
                return count_;
            }
        };

        void mutex_demo() {
            // 创建多个线程进行增减操作
            for (int i = 0; i < 5; ++i) {
                threads.emplace_back([&counter]() {
                    for (int j = 0; j < 1000; ++j) {
                        counter.increment();
                    }
                });
                
                threads.emplace_back([&counter]() {
                    for (int j = 0; j < 1000; ++j) {
                        counter.decrement();
                    }
                });
            }
            
            // 等待所有线程完成
            for (auto& t : threads) {
                t.join();
            }

            std::cout << "最终计数器值：" << counter.get() << std::endl;
        }

        void unique_lock_demo(){
            std::cout << "\n=== Unique Lock 示例 ===" << std::endl;
    
            std::mutex mtx;
            int shared_data = 0;
    
            auto worker = [&](int id) {
                // 先声明lock对象，在需要的时候加锁
                std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
                
                // 手动加锁
                lock.lock();
                std::cout << "线程 " << id << " 开始工作" << std::endl;
                shared_data += id;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                // 可以临时解锁
                lock.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                
                // 重新加锁
                lock.lock();
                std::cout << "线程 " << id << " 完成工作，当前数据: " << shared_data << std::endl;
                // lock 在析构时自动解锁
            };
    
            std::vector<std::thread> threads;
            for (int i = 1; i <= 3; ++i) {
                threads.emplace_back(worker, i);
            }
            
            for (auto& t : threads) {
                t.join();
            }
        }
        /*
        std::lock_guard与std::unique_lock的区别
        std::lock_guard： 一旦声明即自动加锁，作用域结束自动解锁，无解锁和再加锁等灵活操作 不可 move，不可 unlock，只能短作用域临界区保护
        std::unique_lock： 可以先声明 lock 对象，再在需要时加锁  支持 lock.unlock() 优雅释放临界区，之后可 lock.lock() 重新上锁  支持 move 语义（可以通过 std::move 转移所有权）
        */

        // shared_mutex 示例 (读写锁)
        class ReadWriteCounter {
        private:
            int count_ = 0;
            mutable std::shared_mutex mutex_;

        public:
            void write(int value) {
                std::unique_lock<std::shared_mutex> lock(mutex_);
                count_ = value;
                std::cout << "写入: " << count_ << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            int read() const {
                std::shared_lock<std::shared_mutex> lock(mutex_);
                std::cout << "读取: " << count_ << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                return count_;
            }
        };

        void shared_mutex_demo() {
            std::cout << "\n=== Shared Mutex 示例 ===" << std::endl;
            
            ReadWriteCounter rwc;
            std::vector<std::thread> threads;
            
            // 创建读线程
            for (int i = 0; i < 3; ++i) {
                threads.emplace_back([&rwc, i]() {
                    for (int j = 0; j < 3; ++j) {
                        rwc.read();
                    }
                });
            }
            
            // 创建写线程
            threads.emplace_back([&rwc]() {
                for (int i = 1; i <= 5; ++i) {
                    rwc.write(i * 10);
                }
            });
            
            for (auto& t : threads) {
                t.join();
            }
        }

        using namespace duan::demo;
        void locks_demo(){
            std::cout << "锁机制示例" << std::endl;
            mutex_demo();
            unique_lock_demo();
            shared_mutex_demo();
        }
        /*
        std::shared_mutex 是 C++17 标准引入的一种 共享互斥量
        它允许多个线程同时进行读操作（共享锁/读锁），但写操作（独占锁/写锁）时只允许一个线程访问，并阻塞其余所有线程（无论是读还是写）
        适用于读多写少的并发场景，提高多线程程序的性能

        unique_lock  独占锁（写锁）：只允许一个线程写，其它线程不能读也不能写
        shared_lock  共享锁（读锁）：允许多个线程同时读，但不能写
        */
    } // namespace locks_demo
}