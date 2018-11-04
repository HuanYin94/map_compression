import sys
import time

import networkx as nx


# some params
size_th = 100
set_cnt = 0
off_set = 500

def loop_min_cut(input_graph):

	print('-----------------------------------------')
	print('All graph size:	' + str(len(input_graph)))

	end_node = max(input_graph.nodes())
	start_node = min(input_graph.nodes())

	cut_value, partition = nx.minimum_cut(input_graph, start_node+off_set, end_node-off_set)
	set1, set2 = partition

	list1 = list(set1)
	list2 = list(set2)

	g1 = G.subgraph(list1)
	g2 = G.subgraph(list2)


	print('Cut value:	    ' + str(cut_value))
	print('graph_1 size:	' + str(len(g1)) + ' from ' + str(min(g1.nodes())) + ' to ' + str(max(g1.nodes())))
	print('graph_2 size:	' + str(len(g2)) + ' from ' + str(min(g2.nodes())) + ' to ' + str(max(g2.nodes())))

	global set_cnt

	if len(g1) > size_th:
		loop_min_cut(g1)
	else:
		file = open("/home/yinhuan/graph/" + str(set_cnt) + ".txt", 'w')
		file.writelines(["%s\n" % node  for node in g1.nodes()])
		file.close()
		print('set ' + str(set_cnt) + ' finished')
		set_cnt += 1

	if len(g2) > size_th:
		loop_min_cut(g2)
	else:
		file = open("/home/yinhuan/graph/" + str(set_cnt) + ".txt", 'w')
		file.writelines(["%s\n" % node  for node in g2.nodes()])
		file.close()
		print('set ' + str(set_cnt) + ' finished')
		set_cnt += 1

	return

# init
G = nx.Graph();

for i in range(0, 4923):
	G.add_node(i)

file = open("/home/yinhuan/pose_graph_yq_all.txt")

print('file open, adding edges')

for line in file.readlines():
	line.strip('\n')	
	nums = line.split(" ")
	nums = [int(x) for x in nums ]
	G.add_edge(nums[0], nums[1], capacity = nums[2])

print('init graph built')

start = time.clock()
loop_min_cut(G)
elapsed = (time.clock() - start)
print("Time used:",elapsed)

print('loop finished')








# test 

# G.add_nodes_from([0,1,2,3,4,5])

# G.add_edge(0, 1, capacity = 1) 
# G.add_edge(1, 2, capacity = 2) 
# G.add_edge(2, 3, capacity = 2) 
# G.add_edge(3, 4, capacity = 2) 
# G.add_edge(4, 5, capacity = 1) 

# cut_value, partition = nx.minimum_cut(G, 1, 4)

# set1, set2 = partition

# print set1

# print set2