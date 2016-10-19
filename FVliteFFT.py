#!/usr/bin/env python

# FVliteFFT.py
# A script that reads FVlite data files and performs Fourier transforms over them.
# To be used for cylindrical scattering test case for Maxwell's equations.
# Not a general purpose solver.

import sys
import glob
import math
import cmath
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.special import *

def get_data( filename ):
    """
    Reads data files
    Returns data as a list of strings, one entry per line
    """
    with open(filename) as File:
        Data = File.read()
    File.close()
    Data = Data.split('\n')
    return Data

def cartesian_to_cylindrical( x, y, x0=0., y0=0.):
    """
    Simple map from cartesian to cylindrical coordinates
    x0,y0 are the center of the cylinder
    Does not account for 3D case
    """
    X = x-x0
    Y = y-y0
    r = math.sqrt( X*X + Y*Y )
    phi = math.atan2(Y,X)
    return r, phi

def process_data( Data, centreCoord, cutcells=True ):
    """
    Reads data file, returns cylindrical coordinates and Hz
    """
    # List of coordinates and H fields
    Coord = []
    Hz = []
    # Extract circle centre
    x0 = centreCoord[0]
    y0 = centreCoord[1]
    # Process file
    for ii in range(len(Data)):
        Line = Data[ii].split()
        if (len(Line)==0) or (Line[0]=='#') or (Line[3]=="inf") :
            continue
        if not cutcells and float(Line[8])>0:
            continue
        x,y = float(Line[0]), float(Line[1])
        r,phi = cartesian_to_cylindrical(x,y,x0,y0)
        Coord = Coord + [(r,phi)]
        Hz = Hz + [float(Line[7])]
    return Coord, Hz

def exact(phis=[]):
    """
    Calculates exact solutions
    Can pass in phis, or if not provided will automatically generate solutions in full range
    """
    c  = 299792458.0
    Z0 = 376.73031346177
    E0 = 1.0
    # FIXME 5e9
    omega = 5e9
    k = omega / c
    R = 0.5
    kR = k*R

    N_max = 100 # Number of bessel funcs to evaluate

    if phis==[]: # Test phi empty list
        n_points = 2000 # points in grid
        phis = np.linspace(-math.pi+0.01, math.pi-0.01, num=n_points)
    else:
        n_points = len(phis)

    bessel = [ jn(n,kR) for n in range(-N_max,N_max+1)]
    hankel = [ hankel1(n,kR) for n in range(-N_max,N_max+1)]
    bessel_der = [ jvp(n,kR,1) for n in range(-N_max,N_max+1)]
    hankel_der = [ h1vp(n,kR,1) for n in range(-N_max,N_max+1)]

    Hz = np.zeros( n_points)
    for ii in range(n_points):
        phi = phis[ii]
        H = 0.0+0.0j
        for N in range(-N_max,N_max+1):
            n = N+N_max
            prefix = (E0/Z0) * (1.0j**N) * cmath.exp(1.0j*N*phi)
            bessel_part = bessel[n] - (bessel_der[n]/hankel_der[n])*hankel[n]
            H += prefix*bessel_part
        Hz[ii] = abs(H)

    return phis, Hz

def get_numerical_results( rootname, cutcells=True):

    centrecoord = (2.0,1.0)

    files = sorted(glob.glob(rootname+ "*dat"))
    n_times = len(files)

    # read first file to find dimensionality and other useful data
    Data = get_data(files[0])
    coords, Hz = process_data( Data, centrecoord, cutcells )
    n_points = len(Hz)
    phis = np.zeros(n_points)
    for i in range(n_points):
        coord = coords[i]
        phi  = coord[1]
        phis[i] = phi

    # set up numpy array accordingly, input first values
    HzTime = np.zeros( (n_times, n_points) )
    HzTime[0,:] = Hz[:]

    # read in remaining files
    for ii in range(1,n_times):
        filename = files[ii]
        Data = get_data( filename )
        CoordMe, Hz = process_data( Data, centrecoord, cutcells )
        HzTime[ii-1,:] = Hz[:]

    # set up frequency domain
    HzFreq = np.zeros( n_points )

    # Fourier transform over time for each point
    for ii in range(n_points):
         HzFreq[ii] = abs(2*(np.fft.fft(HzTime[:,ii]))[1]/n_times)
         # GAUSSIAN TEST
         #result =abs(2*(np.fft.fft(HzTime[:,ii]))[1]/n_times)
         #HzFreq[ii] = result * math.exp(-(5e8*5e8*1e-9*1e-9/2.0))

    return phis, HzFreq

def RMS_error( results1, results2):
    if len(results1) != len(results2):
        print("ERROR: tried to find RMS error between lists of different lengths")

    N = len(results1)
    total = 0.0
    for i in range(N):
        total += (results1[i]-results2[i])**2
    return math.sqrt(total/N)

def main():

    # Get cmd line args
    cutcells = bool(sys.argv[1])
    plottype = sys.argv[2]
    rootnames = sys.argv[3:]

    # Set up plotting
    rc('text', usetex=True)
    rc('font', **{
                  'family':'serif' ,
                  #'serif':['Computer Modern Roman'],
                  'serif':['Times'],
                  'size':16,
                  #'weight':'bold'
                 })

    # Define colors
    Colors = "krbgm"

    if plottype=="Std":

        # Get exact solutions
        PhisExact, HzExact = exact()

        # Plot exact solution
        plt.figure(figsize=(2.5,2.5))
        plt.plot( PhisExact, HzExact, color=Colors[0])

        # Obtain and plot numerical solution(s)
        markevery=9
        for ii in range(len(rootnames)):
            phis, HzFreq = get_numerical_results( rootnames[ii], cutcells)
            plt.scatter( phis[0:len(phis)-1:markevery], HzFreq[0:len(HzFreq)-1:markevery], color=Colors[ii+1])

        # Add labels
        xtiks = [-math.pi,-math.pi/2,0,math.pi/2, math.pi]
        xlabels =["$-\\pi$","-$\\pi/2$","$0$","$\\pi/2$","$\\pi$"]
        plt.xlim([-math.pi,math.pi])
        plt.xticks(xtiks,xlabels)
        plt.xlabel("$\\phi$, rad")
        plt.ylabel("$H_z$, A/m")

        # Show and save plot
        plt.savefig("FVliteFFT", transparent=True, bbox_inches='tight')
        plt.show()

    if plottype=="RMS":
        # Currently hardcoded results
        resolutions = [150,210,300,480,600,720]
        cutcell_rms = [0.,0.,0.,0.,0.,0.]
        staircase_rms = [0.,0.,0.,0.,0.,0.]
        rootname = "saved_data/Maxwell"
        end = "/Maxwell"
        res_list = ["_150_100","_210_140","_300_200","_420_280","_600_400","_720_480"]
        for i in range(6):
            # cut cell
            filestring = rootname + "CutCell" + res_list[i] + end
            print(filestring)
            phis, Hz = get_numerical_results( filestring, True)
            phisExact, HzExact = exact(phis)
            cutcell_rms[i] = RMS_error( Hz, HzExact)
            # staircase
            filestring = rootname + "Staircase" + res_list[i] + end
            print(filestring)
            phis, Hz = get_numerical_results( filestring, False)
            phisExact, HzExact = exact(phis)
            staircase_rms[i] = RMS_error( Hz, HzExact)

        plt.figure(figsize=(2.5,2.5))
        plt.plot( resolutions, cutcell_rms, marker='o', color='k')
        plt.plot( resolutions, staircase_rms, marker='o', color='r')
        plt.xlim([0,800])
        plt.xticks([0,200,400,600,800])
        plt.xlabel("Number of cells, x direction")
        plt.ylabel("RMS error")

        plt.savefig("FVliteRMS", transparent=True, bbox_inches='tight')
        plt.show()


    if plottype!="Std" and plottype!="RMS":
        print("Error, invalid plot type")

main()
