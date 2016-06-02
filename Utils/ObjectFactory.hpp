// ObjectFactory.hpp
// 
// A templated object factory. Provides an interface for run-time polymorphic object creation.
// Templated to take a return type (object ptr) and a product creator (function ptr).
// Assumes the use of std::string to identify types. This could also be templated out, but I'm not aware
// of a better identifier.
//
//  TODO Error handling that doesn't amount to throwing a tantrum
//
// See "Modern C++ Design", Andrei Alexandrescu

#ifndef OBJECTFACTORY_HPP
#define OBJECTFACTORY_HPP

#include <cstdlib>
#include <iostream>
#include <string>
#include <map>

template<class AbstractProduct, typename ProductCreator>
class ObjectFactory{

private:

    typedef std::map<std::string,ProductCreator> IdMap;
    IdMap mIdMap;

public:

    // No constructor/destructor -- default behaviour is acceptable

    AbstractProduct* create( const std::string& Id);
    AbstractProduct* create( const char* Id){ return create(std::string(Id));}

    bool addBlueprint( const std::string& Id, ProductCreator creator);
    bool addBlueprint( const char* Id, ProductCreator creator){ return addBlueprint(std::string(Id),creator);}

    bool eraseBlueprint( const std::string& Id);
    bool eraseBlueprint( const char* Id){ return eraseBlueprint(std::string(Id));}
};

// Function definitons

template<class AbstractProduct, typename ProductCreator>
AbstractProduct* ObjectFactory<AbstractProduct,ProductCreator>::create( const std::string& Id){
    typename IdMap::const_iterator iter = mIdMap.find(Id);
    if( iter != mIdMap.end()){
        return (iter->second)();
    } else { //error
        std::cerr << "ERROR: Failed to locate factory blueprint with the Id: " << Id << std::endl;
        exit(EXIT_FAILURE);
    }
    return NULL;
}

template<class AbstractProduct, typename ProductCreator>
bool ObjectFactory<AbstractProduct,ProductCreator>::addBlueprint( const std::string& Id, ProductCreator creator){
    return mIdMap.insert(typename IdMap::value_type(Id,creator)).second;
}

template<class AbstractProduct, typename ProductCreator>
bool ObjectFactory<AbstractProduct,ProductCreator>::eraseBlueprint( const std::string& Id){
    return mIdMap.erase(Id) == 1;
}

#endif /* OBJECTFACTORY_HPP */
