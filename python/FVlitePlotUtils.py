#!/usr/bin/env python

import sys
import numpy as np

def get_data( filename ):
    """
    Reads data files formatted by FVlite code
    Returns data as a list of strings, one entry per line
    """
    with open(filename+".dat") as File:
        Data = File.read()
    File.close()
    Data = Data.split('\n')
    return Data

def process_metadata( Data):
    """
    Reads Data
    Analyses metadata section (at top, denoted by '# ' at start of line)
    Returns dictionary mapping data contents to column index.
    Removes metadata from data
    """
    Map = {}
    for ii in range(len(Data)):
        Line = Data[ii]
        if Line[0] != '#':
            break
        Map[Line[2:]] = ii
    return Map

def get_data_array( Data, MetaData):
    """
    From processed data and metadata, produce NumPy array containing data
    as floats.
    """
    Entries = len(Data)
    Columns = len(list(MetaData.values()))
    Array = np.zeros( (Entries,Columns) )
    for ii in range(Entries):
        Line = Data[ii].split()
        for jj in range(len(Line)):
            Array[ii,jj] = float(Line[jj])
    return Array

