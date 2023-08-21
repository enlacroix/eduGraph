import sys
import numpy as np
import matplotlib.pyplot as plt
from netgraph import Graph


def findDigitAndConvertIt(sym):
    # float(''.join(c for c in sym if c.isdigit() or c == ',').replace(',', '.'))
    return float(''.join(c for c in sym if c.isdigit() or c == '.'))


def drawGraph(filePath: str, paths=None):
    """
    :param filePath:
    :param route: 3 5 4 2 - пример! (2, 4), (4, 3), (3, 1) - настоящие edge, которые записаны в графе.
    :return:
    """
    with open(filePath, 'r') as f:
        info = [[findDigitAndConvertIt(num) for num in line.split('\t')] for line in f]

    adj_matrix = np.array(info)
    numberOfNodes = adj_matrix.shape[0]
    node_labels = dict(zip(list(range(0, numberOfNodes)), list(range(1, numberOfNodes + 1))))
    sources, targets = np.where(adj_matrix)
    weights = adj_matrix[sources, targets]
    edges = list(zip(sources, targets))
    colors = ['b'] * len(edges)
    if paths is not None:
        for i in range(len(edges)):
            if edges[i] in paths:
                colors[i] = 'r'
    colors_dict = dict(zip(edges, colors))
    edge_labels = dict(zip(edges, weights))
    fig, ax = plt.subplots()
    Graph(edges, node_labels=node_labels, edge_labels=edge_labels, edge_label_position=0.66, arrows=True, ax=ax, edge_color=colors_dict)
    plt.show()



if __name__ == "__main__":
    numberOfTest = int(sys.argv[1])
    # n = 1
    # test_list = ['some', '1', '3', '5', '4', '2']
    paths = []
    for k in range(2, len(sys.argv) - 1):
        paths.append((int(sys.argv[k]) - 1, int(sys.argv[k + 1]) - 1))

    drawGraph(fr"C:\Users\User\source\repos\DijkstraImpl\Release\graph{numberOfTest}.txt", paths)



# import networkx as nx
# A = np.array(info)
# G = nx.from_numpy_matrix(np.matrix(A), create_using=nx.DiGraph)
# layout = nx.spring_layout(G)
# nx.draw(G, layout, node_size=1000, with_labels=True, font_weight='bold', font_size=15)
# labels = nx.get_edge_attributes(G,'weight')
# nx.draw_networkx_edge_labels(G,pos=layout,edge_labels=labels)
# plt.show()
