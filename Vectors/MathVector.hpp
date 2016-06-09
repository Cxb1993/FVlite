// MathVector.hpp
//
// Defines a mathematical vector.
// Stores arbitrary number of doubles and defines operators.
// Size set using templates
//
// Author: Liam Pattinson
// Date: Feb 16 2016

#ifndef MATHVECTOR_HPP
#define MATHVECTOR_HPP

#include <cmath>
#include <ctgmath>

template <unsigned int N>
class MathVector{

protected:

    double mX[N];

public:

    MathVector();
    MathVector(const MathVector& other);
    ~MathVector();

    double operator[](const unsigned int& ii) const;
    double& operator[](const unsigned int& ii);

    MathVector<N>& operator=(const MathVector<N>& other);
    MathVector<N>& operator+=(const MathVector<N>& rhs);
    MathVector<N>& operator-=(const MathVector<N>& rhs);
    MathVector<N>& operator*=(const MathVector<N>& rhs);
    MathVector<N>& operator*=(const double& rhs);
    MathVector<N>& operator/=(const MathVector<N>& rhs);
    MathVector<N>& operator/=(const double& rhs);
    MathVector<N> operator-() const;

    template<unsigned int M> friend MathVector<M> operator+(const MathVector<M>& lhs, const MathVector<M>& rhs);
    template<unsigned int M> friend MathVector<M> operator+(const MathVector<M>& lhs, const double& rhs);
    template<unsigned int M> friend MathVector<M> operator+(const double& lhs, const MathVector<M>& rhs);
    template<unsigned int M> friend MathVector<M> operator-(const MathVector<M>& lhs, const MathVector<M>& rhs);
    template<unsigned int M> friend MathVector<M> operator-(const MathVector<M>& lhs, const double& rhs);
    template<unsigned int M> friend MathVector<M> operator-(const double& lhs, const MathVector<M>& rhs);
    template<unsigned int M> friend MathVector<M> operator*(const MathVector<M>& lhs, const MathVector<M>& rhs);
    template<unsigned int M> friend MathVector<M> operator*(const MathVector<M>& lhs, const double& rhs);
    template<unsigned int M> friend MathVector<M> operator*(const double& lhs, const MathVector<M>& rhs);
    template<unsigned int M> friend MathVector<M> operator/(const MathVector<M>& lhs, const MathVector<M>& rhs);
    template<unsigned int M> friend MathVector<M> operator/(const MathVector<M>& lhs, const double& rhs);
    template<unsigned int M> friend MathVector<M> operator/(const double& lhs, const MathVector<M>& rhs);

    bool isnan();
};

template<unsigned int M>
MathVector<M>::MathVector(){
}

template<unsigned int M>
MathVector<M>::MathVector(const MathVector<M>& other){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] = other[ii];
    }
}

template<unsigned int M>
MathVector<M>::~MathVector(){
}

template<unsigned int M>
double MathVector<M>::operator[](const unsigned int& ii) const{
    return mX[ii];
}

template<unsigned int M>
double& MathVector<M>::operator[](const unsigned int& ii){
    return mX[ii];
}

template<unsigned int M>
MathVector<M>& MathVector<M>::operator=( const MathVector<M>& other){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] = other[ii];
    }
    return *this;
}


template<unsigned int M>
MathVector<M>& MathVector<M>::operator+=( const MathVector<M>& rhs){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] += rhs[ii];
    }
    return *this;
}

template<unsigned int M>
MathVector<M>& MathVector<M>::operator-=( const MathVector<M>& rhs){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] -= rhs[ii];
    }
    return *this;
}

template<unsigned int M>
MathVector<M>& MathVector<M>::operator*=( const MathVector<M>& rhs){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] *= rhs[ii];
    }
    return *this;
}

template<unsigned int M>
MathVector<M>& MathVector<M>::operator*=( const double& rhs){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] *= rhs;
    }
    return *this;
}

template<unsigned int M>
MathVector<M>& MathVector<M>::operator/=( const MathVector<M>& rhs){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] /= rhs[ii];
    }
    return *this;
}

template<unsigned int M>
MathVector<M>& MathVector<M>::operator/=( const double& rhs){
    for( unsigned int ii=0; ii<M; ii++){
        mX[ii] /= rhs;
    }
    return *this;
}

template<unsigned int M>
MathVector<M> MathVector<M>::operator-() const{
    MathVector<M> result;
    for( unsigned int ii=0; ii<M; ii++){
        result[ii] = -mX[ii];
    }
    return result;
}

template<unsigned int M>
MathVector<M> operator+( const MathVector<M>& lhs, const MathVector<M>& rhs){
    MathVector<M> result(lhs);
    result += rhs;
    return result;
}

template<unsigned int M>
MathVector<M> operator+( const MathVector<M>& lhs, const double& rhs){
    MathVector<M> result(lhs);
    for( unsigned int ii=0; ii<M; ii++){
        result[ii] += rhs;
    }
    return result;
}

template<unsigned int M>
MathVector<M> operator+( const double& lhs, const MathVector<M>& rhs){
    return rhs+lhs;
}

template<unsigned int M>
MathVector<M> operator-( const MathVector<M>& lhs, const MathVector<M>& rhs){
    MathVector<M> result(lhs);
    result -= rhs;
    return result;
}

template<unsigned int M>
MathVector<M> operator-( const MathVector<M>& lhs, const double& rhs){
    MathVector<M> result(lhs);
    for( unsigned int ii=0; ii<M; ii++){
        result[ii] -= rhs;
    }
    return result;
}

template<unsigned int M>
MathVector<M> operator-( const double& lhs, const MathVector<M>& rhs){
    return lhs+(-rhs);
}

template<unsigned int M>
MathVector<M> operator*( const MathVector<M>& lhs, const MathVector<M>& rhs){
    MathVector<M> result(lhs);
    result *= rhs;
    return result;
}

template<unsigned int M>
MathVector<M> operator*( const MathVector<M>& lhs, const double& rhs){
    MathVector<M> result(lhs);
    result *= rhs;
    return result;
}

template<unsigned int M>
MathVector<M> operator*( const double& lhs, const MathVector<M>& rhs){
    return rhs*lhs;
}

template<unsigned int M>
MathVector<M> operator/( const MathVector<M>& lhs, const MathVector<M>& rhs){
    MathVector<M> result(lhs);
    result /= rhs;
    return result;
}

template<unsigned int M>
MathVector<M> operator/( const MathVector<M>& lhs, const double& rhs){
    MathVector<M> result(lhs);
    result /= rhs;
    return result;
}

template<unsigned int M>
MathVector<M> operator/( const double& lhs, const MathVector<M>& rhs){
    MathVector<M> result;
    for( unsigned int ii=0; ii<M; ii++){
        result[ii] = lhs/rhs[ii];
    }
    return result;
}

template<unsigned int M>
bool MathVector<M>::isnan(){
    bool result = false;
    for( unsigned int ii=0; ii<M; ii++){
        if( isnan(mX[ii])) result = true;
    }
    return result;
}


#endif /* MATHVECTOR_HPP */
