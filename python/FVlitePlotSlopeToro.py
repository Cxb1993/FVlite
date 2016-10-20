#!/usr/bin/env python

import sys
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.mlab import griddata
from matplotlib import cm
from matplotlib import rc
from mpl_toolkits.axes_grid1 import make_axes_locatable

from FVlitePlotUtils import *

def main():

    # Get command line args
    argv = sys.argv
    filename = argv[1]
    variable = argv[2]

    # Read file, process metadata
    Data = get_data( filename )
    MetaData = process_metadata(Data)
    StartOfData = 1 + max(list(MetaData.values()))
    Data = Data[StartOfData:]

    # Build array storing all data as floats
    Array = get_data_array(Data,MetaData)

    X = Array[:,MetaData["x"]]
    Y = Array[:,MetaData["y"]]
    Z = Array[:,MetaData[variable]]

    xi = np.linspace(min(X),max(X), num=1200)
    yi = np.linspace(min(Y),max(Y), num=720)
    zi = griddata( X, Y, Z, xi, yi)

    rc('text', usetex=True)
    rc('font', **{
                  'family':'serif' ,
                  'serif':['Computer Modern Roman'],
                  #'serif':['Times'],
                  'size':14,
                  #'weight':'bold'
                 })
    colormap = cm.get_cmap('Greys')

    plt.pcolormesh( xi, yi, zi, cmap=colormap,vmax=4)

    plt.xlabel("x, m")
    plt.ylabel("y, m")
    plt.axis('scaled')
    plt.axis([0,25,0,15])

    ax = plt.gca()
    divider = make_axes_locatable(ax)
    cax = divider.append_axes( "right", size="3%", pad=0.1)
    cbar = plt.colorbar(cax=cax)
    cbar.set_label("Density, kg m \\textsuperscript{-3}")

    plt.savefig("FVlitePlot", transparent=True, bbox_inches='tight')
    plt.show()

main()
