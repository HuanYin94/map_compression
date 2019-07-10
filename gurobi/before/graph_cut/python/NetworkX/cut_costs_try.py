import sys
import time

import networkx as nx
from networkx.algorithms import cuts

# init
G = nx.Graph();

G.add_nodes_from([0,1,2,3,4,5])

G.add_edge(0, 1, capacity = 1) 
G.add_edge(0, 2, capacity = 7) 
G.add_edge(1, 2, capacity = 1) 
G.add_edge(1, 3, capacity = 3) 
G.add_edge(1, 4, capacity = 2) 
G.add_edge(2, 4, capacity = 4) 
G.add_edge(3, 4, capacity = 1) 
G.add_edge(3, 5, capacity = 6) 
G.add_edge(4, 5, capacity = 2) 

print('start cut')

A = [0, 2, 4, 1]

cut_value = nx.cut_size(G, A, weight='capacity')

print cut_value