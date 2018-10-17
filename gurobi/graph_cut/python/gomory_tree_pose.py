import sys

import matplotlib.pyplot as plt
import networkx as nx
from networkx.algorithms import flow

# init
pathLength = 4924
G = nx.Graph();


for i in range(0, 4923):
	G.add_node(i)

file = open("/home/yh/mapModel/2018/10.16/pose_graph.txt")

print('file open, adding edges')

for line in file.readlines():
	line.strip('\n')	
	nums = line.split(" ")
	nums = [int(x) for x in nums ]
	G.add_weighted_edges_from([tuple(nums)])

file.close()

print('start gomory')

T = nx.gomory_hu_tree(G, capacity='weight')

weight = nx.get_edge_attributes(T,'weight')
nx.draw_circular(T, with_labels=True, font_weight='bold')
layout = nx.circular_layout(T)
nx.draw_networkx_edge_labels(T,pos=layout,edge_labels=weight)
plt.show()
