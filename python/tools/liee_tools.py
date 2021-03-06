'''
Created on Mar 1, 2014

usage: 
    python2.7 transpose.py input-file output-file

Simply transposes matrix data of a given text file and writes the result to an output-file.
(For some reason the used numpy routines work (here) only for python2.7) 

Problem     commenting lines disappear
Solution    grep '#' input-file >> output-file.dat

Problem     File-size blows up by too high precision of printed numbers
Solution    don't care (or look for savetxt() parameters)

@author: quark
'''

import sys
import numpy
#import math
import ctypes

CONV_au_m = 5.2917720859e-11
CONV_au_nm = 5.2917720859e-2
CONV_au_eV = 27.211
CONV_au_s = 2.418884326505e-17
CONV_au_fs = 2.418884326505e-2
CONV_au_V_over_m = 5.1421e11
CONV_au_V = CONV_au_V_over_m * CONV_au_m

def transpose( infile, outfile ):
    data = numpy.loadtxt( infile, numpy.float, '#', )
    numpy.savetxt( outfile, data.transpose() )
    return

def stats( infile, outfile):
    glob_min = 6e66
    glob_max = -6e66
    data = numpy.loadtxt( infile, numpy.float, '#', )
    for y in data.flat:
        if y < glob_min: glob_min = y
        if y > glob_max: glob_max = y
    fobj = open( outfile, "a" )
    fobj.write( "glob_min=" + str( glob_min ) + ";\nglob_max=" + str( glob_max ) + ";")
    fobj.close()


def potential( r0, r1, Nr, t0, t1, Nt, outfile ):
        r0 = r0 / CONV_au_nm
        r1 = r1 / CONV_au_nm
        t0 = t0 / CONV_au_fs
        t1 = t1 / CONV_au_fs
        if Nr > 1:  dr = (r1 - r0) / (Nr - 1)
        else:       dr = 0
        if Nt > 1:  dt = (t1 - t0) / (Nt - 1)
        else:       dt = 0
        
        lib = ctypes.cdll.LoadLibrary('/usr/local/lib/libliee.so')
        lib.init_potential()
        pot = numpy.zeros( (Nr, Nt+1) )
        c_Vr = ctypes.c_double( 0.0 );
        c_Vi = ctypes.c_double( 0.0 );
        for ti in range( Nt ):
            for ri in range( Nr ):
                c_r = ctypes.c_double( r0 + ri * dr );
                c_t = ctypes.c_double( t0 + ti * dt );
                lib.calc_potential( c_r, c_t, ctypes.byref(c_Vr), ctypes.byref(c_Vi) )
                if ti==0:   pot[ri][0] = c_r.value * CONV_au_nm
                pot[ri][ti+1] = c_Vr.value * CONV_au_eV
        numpy.savetxt( outfile, pot )

if __name__ == '__main__':
    if sys.argv[1] == "transpose":      transpose( sys.argv[2], sys.argv[3] )
    elif sys.argv[1] == "stats":        stats( sys.argv[2], sys.argv[3] )
    elif sys.argv[1] == "potential":    potential( float(sys.argv[2]), float(sys.argv[3]), int(sys.argv[4]), float(sys.argv[5]), float(sys.argv[6]), int(sys.argv[7]), sys.argv[8] )


