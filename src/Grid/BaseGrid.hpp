// BaseGrid
//
// Simple 'hook' for all grids. Doesn't implement anything, simply used for polymorphism.

#ifndef BASEGRID_HPP
#define BASEGRID_HPP
namespace FVlite{
    class BaseGrid {
        public:
        virtual ~BaseGrid(){}
    };
}
#endif
