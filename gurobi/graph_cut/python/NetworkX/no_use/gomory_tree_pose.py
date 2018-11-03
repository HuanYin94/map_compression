import sys
import time

import matplotlib.pyplot as plt
import networkx as nx
from networkx.algorithms import flow
from networkx.algorithms.flow import shortest_augmenting_path

# init
pathLength = 4924
G = nx.Graph();


for i in range(0, 4923):
	G.add_node(i)

#file = open("/home/yh/mapModel/2018/10.16/pose_graph.txt")
file = open("/home/yinhuan/pose_graph_yq_vis50.txt")

print('file open, adding edges')

for line in file.readlines():
	line.strip('\n')	
	nums = line.split(" ")
	nums = [int(x) for x in nums ]
	G.add_weighted_edges_from([tuple(nums)])

file.close()

print('start gomory')

# gomory and count time
start = time.clock()
T = nx.gomory_hu_tree(G, capacity='weight', flow_func=shortest_augmenting_path)
elapsed = (time.clock() - start)
print("Time used:",elapsed)

# save
nx.write_weighted_edgelist(T, '/home/yinhuan/gomory_yq_vis50.txt')



# draw try, it's a mass of course
# weight = nx.get_edge_attributes(T,'weight')
# nx.draw_circular(T, with_labels=True, font_weight='bold')
# layout = nx.circular_layout(T)
# nx.draw_networkx_edge_labels(T,pos=layout,edge_labels=weight)
# plt.show()
# #