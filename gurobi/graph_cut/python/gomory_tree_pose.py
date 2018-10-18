import sys
import time

import matplotlib.pyplot as plt
import networkx as nx
from networkx.algorithms import flow

# init
pathLength = 1174
G = nx.Graph();


for i in range(0, 1173):
	G.add_node(i)

#file = open("/home/yh/mapModel/2018/10.16/pose_graph.txt")
file = open("/home/yinhuan/pose_graph_yq.txt")

print('file open, adding edges')

for line in file.readlines():
	line.strip('\n')	
	nums = line.split(" ")
	nums = [int(x) for x in nums ]
	G.add_weighted_edges_from([tuple(nums)])

file.close()

print('start gomory')

# gomory and count
start = time.clock()
T = nx.gomory_hu_tree(G, capacity='weight')
elapsed = (time.clock() - start)
print("Time used:",elapsed)

# save
nx.write_weighted_edgelist(G, '/home/yinhuan/gomory_yq.edgelist')

# draw try
weight = nx.get_edge_attributes(T,'weight')
nx.draw_circular(T, with_labels=True, font_weight='bold')
layout = nx.circular_layout(T)
nx.draw_networkx_edge_labels(T,pos=layout,edge_labels=weight)
plt.show()
#
