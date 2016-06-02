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

class Child1 : public Parent{
public:
    Child1(){ std::cout<<"CHILD1 CREATED"<<std::endl;}
    virtual int exec() const{ return 1;}
};

class Child2 : public Parent{
public:
    Child2(){ std::cout<<"CHILD2 CREATED"<<std::endl;}
    virtual int exec() const{ return 2;}
};


// Startup code

ObjectFactory<Parent,Parent::creator> factory;

Parent* CreateChild1(){ return new Child1;}
Parent* CreateChild2(){ return new Child2;}

const bool Child1Registered = factory.addBlueprint("Child1",CreateChild1);
const bool Child2Registered = factory.addBlueprint("Child2",CreateChild2);

int main(void){

    std::cout << "Child1: " << Child1Registered << std::endl;
    std::cout << "Child2: " << Child2Registered << std::endl;
    
    Parent* test1;
    Parent* test2;

    std::cout << "Test uninit pointers:" << std::endl;
    std::cout << "test1 ptr:" << test1 << std::endl;
    std::cout << "test2 ptr:" << test2 << std::endl;

    std::cout << "Starting factories:" << std::endl;
    test1 = factory.create("Child1");
    test2 = factory.create("Child2");

    std::cout << "RESULTS:" << std::endl;
    std::cout << "test1 ptr: " << test1 << std::endl;
    std::cout << "test2 ptr: " << test2 << std::endl;
    std::cout << "Test1: " << test1->exec() << std::endl;
    std::cout << "Test2: " << test2->exec() << std::endl;

    factory.eraseBlueprint("Child1");
    Parent* test3 = factory.create("Child1");

    return EXIT_SUCCESS;
}
