import sys
import time

import networkx as nx


# init
G = nx.Graph();

# pathLength = 4924
# for i in range(0, 4923):
# 	G.add_node(i)

# file = open("/home/yinhuan/pose_graph_yq_all.txt")

# print('file open, adding edges')

# for line in file.readlines():
# 	line.strip('\n')	
# 	nums = line.split(" ")
# 	nums = [int(x) for x in nums ]
# 	# G.add_weighted_edges_from([tuple(nums)])
# 	G.add_edge(nums[0], nums[1], capacity = nums[2])

# file.close()

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

start = time.clock()
cut_value, partition = nx.minimum_cut(G, 0, 5)
elapsed = (time.clock() - start)
print("Time used:",elapsed)

set1, set2 = partition

print set1
