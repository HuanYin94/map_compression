import sys

import matplotlib.pyplot as plt
import networkx as nx
from networkx.algorithms import flow

# init
pathLength = 4924
G = nx.Graph();


for i in range(0, 4923):
	G.add_node(i)

file = open("/home/yh/mapModel/2018/10.16/pose_graph.txt")\

for line in file.readlines():
    line.strip('\n')	
	nums = line.split(" ")
	nums = [int(x) for x in nums ]
	
file.close()

# G.add_weighted_edges_from([(0,1,1)])