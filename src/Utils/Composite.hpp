// Composite.hpp
//
// Templated over type T.
// A collection of T which behaves as if it were a single
// instance of T.
//
// Is capable of accepting a Visitor, allowing a diverse
// range of actions to be called on their constituent parts.

#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <vector>

#include "Visitor.hpp"

template<class T>
class Composite : public T , public Visitable<Composite<T>>{

protected:

    std::vector<T*> m_elements;

public:

    Composite(){}
    virtual ~Composite();
    void add_element( T* p_element);

    template<class VisitorDerived>
    virtual void accept(VisitorDerived& visitor){
        typename std::vector<T*>::iterator iter;
        for( iter = m_elements.begin(); iter != m_elements.end(); iter++){
            (*iter)->accept<VisitorDerived>(visitor);
        }
    }
};

template<class T>
Composite::~Composite(){
    typename std::vector<T*>::iterator iter;
    for( iter = m_elements.begin(); iter != m_elements.end(); iter++){
        delete *iter;
    }
}

template<class T>
void Composite::add_element( T* p_element){
    m_elements.push_back(p_element);
}

#endif /* COMPOSITE_HPP */
