#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include "demos.h"

namespace duan{
    namespace smart_pointers_demo {
        class DuanResource {
        public:
            DuanResource(int id) : evt_id(id) {
                std::cout << "DuanResource " <<  evt_id << " create" << std::endl;
            }

            ~DuanResource() {
                std::cout << "DuanResource evt_id destroy" << std::endl;
            }

            void use(){
                std::cout << "DuanResource " << evt_id << " use" << std::endl;
            }

            int get_id() const { return evt_id; }

        private:
            int evt_id;
        };

        // 独占指针 shared_ptr的使用
        void unique_ptr_demo(){
            std::cout << "unique_ptr_demo" << std::endl;

            // 创建unique_ptr
            // std::unique_ptr<DuanResource> ptr1 = std::make_unique<DuanResource>(1);
            auto ptr1 = std::make_unique<DuanResource>(1);

            // 使用unique_ptr
            ptr1->use();

            // 移动语义 转移资源
            auto ptr2 = std::move(ptr1);
            if(!ptr1){
                std::cout << "ptr1 is now empty after move" << std::endl;
            }
            ptr2->use();

            // 自定义删除器
            // 使用std::function作为自定义删除器
            auto ptr3 = std::unique_ptr<DuanResource, std::function<void(DuanResource*)>>( 
                new DuanResource(3), 
                [](DuanResource* res) {
                    std::cout << "Custom deleter for DuanResource " << res->get_id() << std::endl;
                    delete res;
                }
            );
        }

        void shared_ptr_demo(){
            std::cout << "shared_ptr_demo" << std::endl;

            // 创建shared_ptr
            auto ptr1 = std::make_shared<DuanResource>(2);
            // 给一个作用域
            {
                auto ptr2 = ptr1; // 引用计数增加
                std::cout << "引用计数: " << ptr1.use_count() << std::endl;
                std::cout << "引用计数: " << ptr2.use_count() << std::endl;
                // 再把ptr2赋给ptr1
                ptr1 = ptr2; // 这不会增加引用计数，因为ptr1已经指向了ptr2的资源
                std::cout << "引用计数: " << ptr1.use_count() << std::endl;

                auto ptr3 = ptr2; // 再创建一个shared_ptr，引用计数增加
                std::cout << "引用计数: " << ptr1.use_count() << std::endl;
                std::cout << "引用计数: " << ptr2.use_count() << std::endl;
                std::cout << "引用计数: " << ptr3.use_count() << std::endl;
            }
            // 退出作用域之后的引用计数
            std::cout << "引用计数: " << ptr1.use_count() << std::endl;
        }


        // weak_ptr示例
        /*
        首先看一下循环引用场景
        class B;
        class A{
        public:
            A(){};
            ~A(){};
            std::shared_ptr<B> b_ptr;
        }
        class B{
        public:
            B(){};
            ~B(){};
            std::shared_ptr<A> a_ptr;
        };
        auto a = std::make_shared<A>();
        auto b = std::make_shared<B>();
        a->b_ptr = b;
        b->a_ptr = a;
        // 这种情况下，A和B互相持有对方的shared_ptr，导致引用计数永远不会归零，从而造成内存泄漏。
        */
        class DuanNode{
        public:
            DuanNode(int value) : value_(value) {
                std::cout << "DuanNode " << value_ << " created" << std::endl;
            }
            ~DuanNode() {
                std::cout << "DuanNode " << value_ << " destroyed" << std::endl;
            }

            void SetNext(std::shared_ptr<DuanNode> next) { next_ = next; }
            void SetParent(std::shared_ptr<DuanNode> parent) { parent_ = parent; }
        
        private:
            int value_;
            std::shared_ptr<DuanNode> next_;
            std::weak_ptr<DuanNode> parent_; // 使用weak_ptr避免循环引用
        };

        void weak_ptr_demo(){
            std::cout << "weak_ptr_demo" << std::endl;

            auto parent = std::make_shared<DuanNode>(1);
            auto child = std::make_shared<DuanNode>(2);

            // 设置next和parent关系
            parent->SetNext(child);
            child->SetParent(parent);   // weak_ptr不会增加引用计数

            std::cout << "Parent 的引用计数： " << parent.use_count() << std::endl;
            std::cout << "Child 的引用计数： " << child.use_count() << std::endl;
        }

        using namespace duan::demo;
        void smart_pointers_demo(){
            std::cout << "============智能指针示例=============" << std::endl;
            std::cout << "smart_pointers_demo" << std::endl;
            unique_ptr_demo();
            shared_ptr_demo();
            weak_ptr_demo();
        }
    }
}