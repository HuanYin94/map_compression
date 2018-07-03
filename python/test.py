#!/usr/bin/python

# used for test by yh for 3D point cloud reduction

from gurobipy import *

try:

    # Create a new model
    m = Model("compression")

    # Create variables
    x = m.addVars(1, 500000, vtype=GRB.BINARY)

    print('Finished!')

except GurobiError as e:
    print('Error code ' + str(e.errno) + ": " + str(e))

except AttributeError:
    print('Encountered an attribute error')

