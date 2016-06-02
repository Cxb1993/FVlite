// FT_Module.hpp
//
// Defines Fourier transform module.
// Makes use of the FFTW3 library.

#ifndef FT_MODULE_HPP
#define FT_MODULE_HPP

#include<cstdlib>
#include<vector>

#include<fftw3.h>

typedef std::vector<double> vec;

class FT_Module{

    protected:

    size_t mN; // number of points

    fftw_complex *mTimeDomain, *mFreqDomain;
    fftw_plan mPlan;

    public:

    FT_Module( size_t N);
    ~FT_Module();

    void exec( vec& InputReal, vec& InputImag, vec& OutputReal, vec& OutputImag);
};

FT_Module::FT_Module( size_t N) : mN(N){
    mTimeDomain = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*N );
    mFreqDomain = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*N );
    mPlan = fftw_plan_dft_1d( mN, mTimeDomain, mFreqDomain, FFTW_FORWARD, FFTW_ESTIMATE);
}

FT_Module::~FT_Module(){
    fftw_destroy_plan(mPlan);
    fftw_free( mTimeDomain);
    fftw_free( mFreqDomain);
}

void FT_Module::exec( vec& InputReal, vec& InputImag, vec& OutputReal, vec& OutputImag){

    // Input must be copied into fftw_complex array
    // No vector length checks are performed. The user must ensure this.

    for( size_t ii=0; ii<mN; ii++){
        mTimeDomain[ii][0] = InputReal[ii];
        mTimeDomain[ii][1] = InputImag[ii];
    }

    // Execute fftw_plan
    // This performs a discrete Fourier transform on mTimeDomain.
    // Results are stored in mFreqDomain

    fftw_execute( mPlan);

    // Copy results to output vectors.
    
    for( size_t ii=0; ii<mN; ii++){
        OutputReal[ii] = mFreqDomain[ii][0];
        OutputImag[ii] = mFreqDomain[ii][1];
    }

    return;
}

#endif
