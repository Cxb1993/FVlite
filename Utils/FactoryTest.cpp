// Test file for objectfactory.hpp

#include "ObjectFactory.hpp"

// Making a basic inheritance structure

class Parent{
protected:
    int filler;
public:
    typedef Parent* (*creator)();
    virtual int exec() const = 0;
};

class ParentChild1 : public Parent{
public:
    ParentChild1(){ std::cout<<"CHILD1 CREATED"<<std::endl;}
    virtual int exec() const{ return 1;}
};

class ParentChild2 : public Parent{
public:
    ParentChild2(){ std::cout<<"CHILD2 CREATED"<<std::endl;}
    virtual int exec() const{ return 2;}
};


// Startup code

//ObjectFactory<Parent,Parent::creator> factory;
ObjectFactory<Parent> ParentFactory;
REGISTER(Parent,Child1)
REGISTER(Parent,Child2)

int main(void){
    
    Parent* test1;
    Parent* test2;

    std::cout << "Test uninit pointers:" << std::endl;
    std::cout << "test1 ptr:" << test1 << std::endl;
    std::cout << "test2 ptr:" << test2 << std::endl;

    std::cout << "Starting factories:" << std::endl;
    test1 = ParentFactory.create("Child1");
    test2 = ParentFactory.create("Child2");

    std::cout << "RESULTS:" << std::endl;
    std::cout << "test1 ptr: " << test1 << std::endl;
    std::cout << "test2 ptr: " << test2 << std::endl;
    std::cout << "Test1: " << test1->exec() << std::endl;
    std::cout << "Test2: " << test2->exec() << std::endl;

    ParentFactory.eraseBlueprint("Child1");
    Parent* test3 = ParentFactory.create("Child1");

    return EXIT_SUCCESS;
}
