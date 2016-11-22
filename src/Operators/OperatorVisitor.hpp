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
        double max_speed = m_grid.max_speed();
        double min_ds = m_grid.min_ds();
        op.exec( m_timer, max_speed, min_ds);
    }

    void visit( OperatorInitialisation& op){
        op.accept( m_grid);
    }

    void visit( OperatorInitialisationManager& op){
        op.accept( *this);
    }

    void visit( OperatorBoundary& op){
        double t = m_timer.t();
        op.exec( m_grid, t);
    }

    void visit( OperatorBoundaryManager& op){
        op.accept( *this);
    }

};

#endif /* OPERATORVISITOR_HPP */
