// OperatorVisitor.hpp
//
// Handles core Operators. Special versions exist which also
// handle, for example, finite volume Operators.
//
// Must be updated to account for any new operators.
// The visitor will hold references to a grid and a timer. These
// will be made available to any Operators visited.

#ifndef OPERATORVISITOR_HPP
#define OPERATORVISITOR_HPP

class OperatorVisitor : 
    public Visitor<OperatorTimerIncrement>,
    public Visitor<OperatorTimerCalibrate>,
    public Visitor<OperatorInitialisation>,
    public Visitor<OperatorInitialisationManager>,
    public Visitor<OperatorBoundary>,
    public Visitor<OperatorBoundaryManager>
{
protected:
    Grid& m_grid;
    Timer& m_timer;

public:
    OperatorVisitor( Grid& grid, Timer& timer) : m_grid(grid), m_timer(timer){}

    void visit( OperatorTimerIncrement& op){
        op.exec( m_timer);
    }

    void visit( OperatorTimerCalibrate& op){
        op.exec( m_grid, m_timer);
    }

    void visit( OperatorInitialisation& op){
        op.exec( m_grid);
    }

    void visit( OperatorInitialisationManager& op){
        op.exec( m_grid);
    }

    void visit( OperatorBoundary& op){
        op.exec( m_grid, m_timer);
    }

    void visit( OperatorBoundaryManager& op){
        op.exec( m_grid, m_timer);
    }

};

#endif /* OPERATORVISITOR_HPP */
