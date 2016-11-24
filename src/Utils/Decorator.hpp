// Decorator.hpp
//
// Templated over type T, which it both inherits and contains.
// Used to wrap classes and add additional behaviours at run-time.

#ifndef DECORATOR_HPP
#define DECORATOR_HPP

template<class T>
class Decorator : public T {
private:
    Decorator() {}
protected:
    T* mp_inner;
public:
    Decorator( T* p_wrappee);
    virtual ~Decorator();
};

template<class T>
Decorator<T>::Decorator( T* p_wrappee) : mp_inner( p_wrappee) {}

template<class T>
Decorator<T>::~Decorator(){
    delete mp_inner;
}


#endif /* DECORATOR_HPP */
