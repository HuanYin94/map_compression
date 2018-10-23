import sys

import matplotlib.pyplot as plt
import networkx as nx
from networkx.algorithms import flow
from networkx.algorithms.flow import shortest_augmenting_path

G = nx.Graph();
G.add_nodes_from([0,1,2,3,4,5])
G.add_weighted_edges_from([(0,1,1), (0,2,7), (1,2,1), (1,3,3), (1,4,2), (2,4,4), (3,4,1), (3,5,6), (4,5,2)])
#nx.set_edge_attributes(G, 99, 'capacity')
T = nx.gomory_hu_tree(G, 99, flow_func=shortest_augmenting_path)
                                                                                                                                                                                

#weight = nx.get_edge_attributes(G,'weight')
#nx.draw_circular(G, with_labels=True, font_weight='bold')
#layout = nx.circular_layout(G)
#nx.draw_networkx_edge_labels(G,pos=layout,edge_labels=weight)

weight = nx.get_edge_attributes(T,'weight')
nx.draw_circular(T, with_labels=True, font_weight='bold')
layout = nx.circular_layout(T)
nx.draw_networkx_edge_labels(T,pos=layout,edge_labels=weight)

plt.show()
