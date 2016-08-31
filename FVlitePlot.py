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
    plottype = argv[1]
    filename = argv[2]
    variable = argv[3]

    if plottype == "Std":

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

        xi = np.linspace(1,3, num=800)
        yi = np.linspace(0,2, num=800)
        zi = griddata( X, Y, Z, xi, yi)

        rc('text', usetex=True)
        rc('font', **{
                      'family':'serif' ,
                      'serif':['Computer Modern Roman'],
                      #'serif':['Times'],
                      'size':14,
                      #'weight':'bold'
                     })
        colormap = cm.get_cmap('RdBu')

        plt.pcolormesh( xi, yi, zi, cmap=colormap, vmin=-0.006, vmax=0.006)

        plt.xlabel("x, m")
        plt.ylabel("y, m")
        plt.axis('scaled')
        plt.axis([1,3,0,2])

        ax = plt.gca()
        divider = make_axes_locatable(ax)
        cax = divider.append_axes( "right", size="3%", pad=0.1)
        cbar = plt.colorbar(cax=cax)
        cbar.set_label("$H_z$, A/m")

        circle = plt.Circle((2,1), 0.5, fill=False)
        ax.add_artist(circle)

        plt.savefig("FVlitePlot", transparent=True, bbox_inches='tight')
        plt.show()

    if plottype == "CFL":
        # Hard coded plot, for high CFL failure testing (and second order errors)
        #CFL
        #Data1 = get_data("data/Maxwell46")
        #Data2 = get_data("data/Maxwell51")
        #Data3 = get_data("data/Maxwell54")
        #Data4 = get_data("data/Maxwell58")
        #2nd
        Data1 = get_data("data/Maxwell103")
        Data2 = get_data("data/Maxwell108")
        Data3 = get_data("data/Maxwell112")
        Data4 = get_data("data/Maxwell116")
        MetaData = process_metadata(Data1)
        StartOfData = 1 + max(list(MetaData.values()))
        Data1 = Data1[StartOfData:]
        Data2 = Data2[StartOfData:]
        Data3 = Data3[StartOfData:]
        Data4 = Data4[StartOfData:]


        # Build array storing all data as floats
        Array1 = get_data_array(Data1,MetaData)
        Array2 = get_data_array(Data2,MetaData)
        Array3 = get_data_array(Data3,MetaData)
        Array4 = get_data_array(Data4,MetaData)

        rc('text', usetex=True)
        rc('font', **{
                      'family':'serif' ,
                      #'serif':['Computer Modern Roman'],
                      'serif':['Times'],
                      'size':14,
                      #'weight':'bold'
                     })

        colormap = cm.get_cmap('RdBu')

        f, ((ax1,ax2),(ax3,ax4)) = plt.subplots(2,2,sharex='col',sharey='row')

        # CFL, dt = 1.33426e-11
        #titles = ["$t=0.614$ns","$t=0.680$ns","$t=0.721$ns","$t=0.774$ns"]
        #vmaxs = [0.006,0.006,0.025,2.0]
        # 2nd, dt = 7.50519e-12
        titles = ["$t=0.773$ns","$t=0.811$ns","$t=0.841$ns","$t=0.871$ns"]
        vmaxs = [0.006,0.008,0.14,0.8]

        vmins = [-vmaxs[i] for i in range(len(vmaxs))]

        for n in range(4):

            if n==0:
                Array=Array1
                ax=ax1
            elif n==1:
                Array=Array2
                ax=ax2
            elif n==2:
                Array=Array3
                ax=ax3
            elif n==3:
                Array=Array4
                ax=ax4

            X = Array[:,MetaData["x"]]
            Y = Array[:,MetaData["y"]]
            Z = Array[:,MetaData[variable]]

            xi = np.linspace(1,3, num=400)
            yi = np.linspace(0,2, num=400)
            zi = griddata( X, Y, Z, xi, yi)

            im = ax.imshow(zi,cmap=colormap,vmin=vmins[n],vmax=vmaxs[n])

            divider = make_axes_locatable(ax)
            cax = divider.append_axes( "right", size="3%", pad=0.1)
            cbar = plt.colorbar(im,cax=cax)
            cbar.set_label("$H_z$, A/m")

            ax.pcolormesh( xi, yi, zi, cmap=colormap)
            ax.set_title(titles[n])

#            ax.axes.get_xaxis().set_ticklabels(['1.0','1.5','2.0','2.5','3.0'])
#            ax.axes.get_yaxis().set_ticklabels(['0.0','0.5','1.0','1.5','2.0'])
            ax.axes.get_xaxis().set_ticklabels([])
            ax.axes.get_yaxis().set_ticklabels([])

            circle = plt.Circle((2,1), 0.5, fill=False)
            ax.add_artist(circle)


        plt.savefig("SecondOrderError", transparent=True, bbox_inches='tight')
        plt.show()


main()
