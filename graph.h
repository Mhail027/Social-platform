// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "queue.h"

#define DIE(assertion, call_description)                                      \
	do {                                                                      \
		if (assertion) {                                                      \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                \
			perror(call_description);                                         \
			exit(errno);                                                      \
		}                                                                     \
	} while (0)                                                               \

/******************************
 Structure to save the information about a graph.
*******************************/
typedef struct graph_t {
	/* Lists of neighbors for every node. */
	ll_t **neighbors;
	/* Number of nodes from graph. */
	u_int nodes;
} graph_t;

/******************************
 Structure to save the information about an Euler tour.
*******************************/
typedef struct euler_tour_t {
	/* Euler tour. */
	u_int *tour;
	/* First appearance of every node in Euler tour. */
	u_int *first_appearance;
	/* Array with the depth of every node. */
	u_int *depth;
	/* Number of visited nodes. (A node can be visited more than
	   one time.)*/
	u_int curr_step;
	/* Depth of current node. */
	u_int curr_depth;
} euler_tour_t;

/******************************
 * create_graph() - Create and initialize a graph.
 *
 * @param node: The number of nodes from graph.
 *
 * @return - The created graph.
*******************************/
graph_t *graph_create(u_int nodes);

/******************************
 * directed_graph_add_edge() - Add an edge in a directed graph.
 *
 * @param graph: The directed graph.
 * @param src: The index of the source node.
 * @param dst: The index of the destination node.
*******************************/
void directed_graph_add_edge(graph_t *graph, u_int src, u_int dst);

/******************************
 * undirected_graph_add_edge() - Add an edge in an undirected graph.
 *
 * @param graph: The undirected graph.
 * @param node_1: The index of the first node.
 * @param node_2: The index of the second node.
*******************************/
void undirected_graph_add_edge(graph_t *graph, u_int node_1, u_int node_2);

/******************************
 * directed_graph_has_edge() - Verify the existence of an edge in a
 *      directed graph.
 *
 * @param graph: The directed graph.
 * @param src: The index of the source node.
 * @param dst: The index of the destination node.
 *
 * @return 1, if the edge exists
 *         0, if doesn't exist
*******************************/
u_int directed_graph_has_edge(graph_t *graph, u_int src, u_int dst);

/******************************
 * undirected_graph_has_edge() - Verify the existence of an edge in an
 *      undirected graph.
 *
 * @param graph: The undirected graph.
 * @param node_1: The index of the first node.
 * @param node_2: The index of the second node.
 *
 * @return 1, if the edge exists
 *         0, if doesn't exist
*******************************/
u_int undirected_graph_has_edge(graph_t *graph, u_int src, u_int dst);

/******************************
 * directed_graph_remove_edge() - Remove an edge from a directed graph.
 *
 * @param graph: The directed graph.
 * @param src: The index of the source node.
 * @param dst: The index of the destination node.
*******************************/
void directed_graph_remove_edge(graph_t *graph, u_int src, u_int dst);

/******************************
 * undirected_graph_remove_edge() - Remove an edge from an undirected graph.
 *
 * @param graph: The undirected graph.
 * @param node_1: The index of the first node.
 * @param node_2: The index of the second node.
*******************************/
void undirected_graph_remove_edge(graph_t *graph, u_int src, u_int dst);

/******************************
 * @brief: Deallocate the memory of a graph.
*******************************/
void graph_free(graph_t *graph);

/******************************
 * find_length_min_path() - Find the minimum number of edges of a path from
 *      src to dst, in a graph.
 *
 * @param graph: The graph with which we work.
 * @param src: The node from where the path must start.
 * @param dst: The node from where the past must end.
 *
 * @return: min number of edges of a path from src to dst, if exists at least
 *      a path
 *          -1, in contrary case
*******************************/
int find_length_min_path(graph_t *graph, u_int src, u_int dst);

/******************************
 * bfs_graph() - Do BFS from a node in a graph.
 *
 * @param graph: Graph with which we work.
 * @param node: ID of node from where BFS starts.
 *
 * @return: Array with visited nodes (from position 1 to end).
 *		On first position (0) is the number of visited nodes.
*******************************/
u_int *bfs_graph(graph_t *graph, u_int node);

/******************************
 * @brief: Double the number of nodes from a graph.
*******************************/
void graph_double_nodes(graph_t *graph);

/******************************
 * get_clique_helper() - Help the function get_clique().
 *
 * @param graph: Graph with which we work.
 * @param node: Array with the IDs of nodes from current clique
 *		(from position 1 to end). On first position (0) is the
 *		number of nodes from current clique.
 * @param wanted_size: Number of nodes which want to be in clique.
 * @param start: The index of node from neighbors's list of first
 *		node from clique from where we start to verify. All first
 *		$(start) neighbours were verified, if cn be added in clique.
*******************************/
void get_clique_helper(graph_t *graph, u_int *clique, u_int wanted_size,
					   u_int start);

/******************************
 * get_clique() - Find a clique of a specified size and which
 *		contains a specified node, in a graph.
 *
 * @param graph: Graph with which we work.
 * @param node: ID of a node that we want to be in clique.
 * @param size: Number of nodes which must be in clique.
 *
 * @return: Array with the IDs of nodes from clique (from position 1 to end).
 *		On first position (0) is the number of nodes from clique.
 *		If the number of nodes is 0, it's means that doesn't exist a clique
 *		which meets the given requirements.
*******************************/
u_int *get_clique(graph_t *graph, u_int node, u_int size);

/******************************
 * create_euler_tour() - Allocate memory for a structure of type
 *		euler_tour_t.
 *
 * @param max_nodes: Number of nodes of the graph on which will
 *		do the Euler tour.
 *
 * @return: The created structure
*******************************/
euler_tour_t *create_euler_tour(u_int nodes);

/******************************
 * euler_tour() - Recursive function which realize the Euler tor
 *		for a tree.
 *
 * @param euler: Structure with contains the information about
 *		the tour.
 * @param tree: Tree with which we work.
 * @param node: Index of the current node.
*******************************/
void euler_tour(euler_tour_t *euler, graph_t *tree, u_int node);

/******************************
 * init_euler_tour() - Initialize an Euler tour.
 *
 * @param tree: Tree with which we work.
 * @param root: Root of the tree.
 *
 * @return: Structure which conatins the information
 *		of the tour.
*******************************/
euler_tour_t *init_euler_tour(graph_t *tree, u_int root);

/******************************
 * tree_lca() - Return lowest common ancestor of n1 and n2, from a tree.
 *
 * @param tree: Tree.
 * @param root: Root of the tree.
 * @param n1: Index of first node.
 * @param n2: Index of second node.
 *
 * @return: LCA of n1 and n2.
*******************************/
u_int tree_lca(graph_t *tree, u_int root, u_int n1, u_int n2);

/******************************
 * tree_find_parent() - Find the parent of a node in a tree.
 *
 * @param tree: Tree with which we work.
 * @param root: Root of tree.
 * @param node: Index of node whose parent we want.
 *
 * @return: Index of parent, if has a parent
 *			-1, if the node is the root
*******************************/
int tree_find_parent(graph_t *tree, u_int root, u_int node);

#endif

