// Visitor.hpp
//
// A design pattern which introduces 'double dispatch' to C++.
// This class allows you to perform a diverse range of actions on
// a collection of basic elements, with the action performed depending
// on both the type of element and the type of visitor involved.
//
// Uses the Curiously Recurring Template Pattern (CRTP), which roughly
// translates to 'evil dark magic'. Requires that visitable objects
// be marked as such (which I personally think is a neat side effect).
//
// Also defined here is a visitable class that should be inherited by
// anything which a visitor should be able to use. 

#ifndef VISITOR_HPP
#define VISITOR_HPP

template<class VisitableDerived>
class Visitor {
public:
    virtual ~Visitor(){}
    virtual void visit(VisitableDerived&) = 0;
};

template<class VisitableDerived>
class Visitable {
public:
    template<class VisitorDerived>
    virtual void accept(VisitorDerived& visitor){
        visitor.visit( static_cast<VisitableDerived&>(*this));
    }
};

#endif /* VISITOR_HPP */
