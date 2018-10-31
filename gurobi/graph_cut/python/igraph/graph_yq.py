from igraph import *

g = Graph();

g.add_vertices(1174);

file = open("/home/yinhuan/gomory_yq_s.edgelist")

print('file open, adding edges')

for line in file.readlines():
	line.strip('\n')	
	nums = line.split(" ")
	nums = [int(x) for x in nums ]
	print(nums(1), nums(2), nums(3))
