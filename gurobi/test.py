#!/usr/bin/python

# used for test by yh for 3D point cloud reduction

from gurobipy import *

try:

    # Create a new model
    m = Model("compression")

    # Create variables
    x = m.addVars(1, 2, vtype=GRB.BINARY)

    # Set objective
    m.setObjective(x[0,0]+x[0,1], GRB.MAXIMIZE)

    # Add constraint
    m.addConstr(x[0,0]+x[0,1]>=1)

    m.optimze()

    print('Finished!')

except GurobiError as e:
    print('Error code ' + str(e.errno) + ": " + str(e))

except AttributeError:
    print('Encountered an attribute error')

