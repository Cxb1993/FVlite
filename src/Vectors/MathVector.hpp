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

    // Access operators

    double operator[](const unsigned int& ii) const;
    double& operator[](const unsigned int& ii);

    // Unary Operators

    MathVector<N>& operator=(const MathVector<N>& other);
    MathVector<N>& operator+=(const MathVector<N>& rhs);
    MathVector<N>& operator-=(const MathVector<N>& rhs);
    MathVector<N>& operator*=(const MathVector<N>& rhs);
    MathVector<N>& operator*=(const double& rhs);
    MathVector<N>& operator/=(const MathVector<N>& rhs);
    MathVector<N>& operator/=(const double& rhs);
    MathVector<N> operator-() const;

    // Binary Operators

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

    // Utility functions

    // Check to see if it contains NaN
    bool isnan();
    // Norm function -- 0 and inf norm not included
    double norm( unsigned int order = 2) const;
    template<unsigned int M> friend double norm( const MathVector<M>& vec, unsigned int order=2);
    // Dot product
    template<unsigned int M> friend double dot(const MathVector<M>& lhs, const MathVector<M>& rhs);
    // Curl
    friend MathVector<3> curl( const MathVector<3>& lhs, const MathVector<3>& rhs);
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
        if( std::isnan(mX[ii])) result = true;
    }
    return result;
}

template<unsigned int M>
double dot(const MathVector<M>& lhs, const MathVector<M>& rhs){
    double total = 0.;
    for( unsigned int ii=0; ii<M; ii++){
        total += lhs[ii] * rhs[ii];
    }
    return total;
}

MathVector<3> curl(const MathVector<3>& lhs, const MathVector<3>& rhs){
    MathVector<3> result;
    result[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
    result[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
    result[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];
    return result;
}

template<unsigned int M>
double MathVector<M>::norm( unsigned int order) const {
    if(order==0){
        std::cout << "Error, 0 norm not implemented" << std::endl;
        exit(EXIT_FAILURE);
    }
    double inner_total = 0.;
    for( unsigned int ii=0; ii<M; ii++){
        inner_total += pow( mX[ii], order);
    }
    return pow( inner_total, 1.0/order);
}

template<unsigned int M> double norm( const MathVector<M>& vec, unsigned int order=2){
    return vec.norm(order);
}

#endif /* MATHVECTOR_HPP */
