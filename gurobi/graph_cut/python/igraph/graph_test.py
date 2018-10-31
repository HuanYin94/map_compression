from igraph import *

g = Graph();

g.add_vertices(6);

g.vs["name"] = ["0", "1", "2", "3", "4", "5"]

g.add_edge("0", "1", weight=1)
g.add_edge("0", "2", weight=7)
g.add_edge("1", "2", weight=1)
g.add_edge("1", "3", weight=3)
g.add_edge("1", "4", weight=2)
g.add_edge("2", "4", weight=4)
g.add_edge("3", "4", weight=1)
g.add_edge("3", "5", weight=6)
g.add_edge("4", "5", weight=2)

t = g.gomory_hu_tree(capacity="weight")

print t

mc = g.mincut()

print mc

# layout = t.layout("kk")
# plot(t, layout = layout)

saveFile = open("/home/yh/saved.txt", "w");

for e in g.es:
	print e.tuple
	saveFile.write(str(e.tuple[0])+' ')
	saveFile.write(str(e.tuple[1])+' ')
	print e["weight"]
	saveFile.write(str(e["weight"])+'\n')

saveFile.close();