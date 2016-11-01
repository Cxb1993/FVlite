// Composite.hpp
//
// Templated over type T.
// A collection of T which behaves as if it were a single
// instance of T.

#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <vector>

template<class T>
class Composite : public T {

protected:

    std::vector<T*> m_elements;

public:

    Composite(){}
    virtual ~Composite();
    void add_element( T* element);
    virtual void exec();

};

template<class T>
Composite<T>::~Composite(){
    typename std::vector<T*>::iterator iter;
    for( iter = m_elements.begin(); iter != m_elements.end(); iter++){
        delete *iter;
    }
}

template<class T>
void Composite<T>::add_element( T* p_element){
    m_elements.push_back(p_element);
}

template<class T>
void Composite<T>::exec(){
    typename std::vector<T*>::iterator iter;
    for( iter = m_elements.begin(); iter != m_elements.end(); iter++){
        (*iter)->exec();
    }
}

#endif /* COMPOSITE_HPP */
