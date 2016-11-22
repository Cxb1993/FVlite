// Decorator.hpp
//
// Templated over type T, which it both inherits and contains.
// Used to wrap classes and add additional behaviours at run-time.
//
// Is capable of accepting a Visitor. Passes visitor to inner
// class.

#ifndef DECORATOR_HPP
#define DECORATOR_HPP

#include "Visitor.hpp"

template<class T>
class Decorator : public T , public Visitable<Decorator<T>>{

protected:

    T* mp_inner;

public:

    Decorator( T* p_wrappee);
    virtual ~Decorator();

    template<class VisitorDerived>
    virtual void accept( VisitorDerived& visitor){
        mp_inner->accept<VisitorDerived>(visitor);
        Visitable<Decorator<T>>.accept<VisitorDerived>(visitor);
    }

};

template<class T>
Decorator<T>::Decorator( T* p_wrappee) : mp_inner( p_wrappee) {}

template<class T>
Decorator<T>::~Decorator(){
    delete mp_inner;
}


#endif /* DECORATOR_HPP */
