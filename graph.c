// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#include "graph.h"

graph_t *graph_create(u_int nodes)
{
	// Allocate memory for the structure of the graph.
	graph_t *g = (graph_t *)malloc(sizeof(graph_t));
	DIE(!g, "malloc() failed\n");

	// Initialize the fields from the structure.
	g->nodes = nodes;
	g->neighbors = (ll_t **)malloc(nodes * sizeof(ll_t *));
	DIE(!g->neighbors, "malloc() failed\n");
	for (u_int i = 0; i < nodes; ++i)
		g->neighbors[i] = ll_create(sizeof(u_int));

	// Return the created graph.
	return g;
}

void directed_graph_add_edge(graph_t *graph, u_int src, u_int dst)
{
	// Verify the parameters.
	if (src >= graph->nodes || dst >= graph->nodes)
		DIE(1, "Invalid index of node\n");
	if (!graph || !graph->neighbors)
		DIE(1, "The graph isn't total allocated.\n");

	// Add the edge.
	ll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, &dst);
}

void undirected_graph_add_edge(graph_t *graph, u_int node_1, u_int node_2)
{
	directed_graph_add_edge(graph, node_1, node_2);
	directed_graph_add_edge(graph, node_2, node_1);
}

u_int directed_graph_has_edge(graph_t *graph, u_int src, u_int dst)
{
	// Verify the parameters.
	if (src >= graph->nodes || dst >= graph->nodes)
		DIE(1, "Invalid index of node\n");
	if (!graph || !graph->neighbors)
		DIE(1, "The graph isn't total allocated.\n");

	// Verify if exists an edge from src ro dst.
	ll_node_t *curr = graph->neighbors[src]->head;
	while (curr) {
		if (dst == *(u_int *)curr->data)
			return 1;
		curr = curr->next;
	}
	return 0;
}

u_int undirected_graph_has_edge(graph_t *graph, u_int node_1, u_int node_2)
{
	// Verify if exists edge from node_1 to node_2.
	if (!directed_graph_has_edge(graph, node_1, node_2))
		return 0;
	// Verify if exists edge from node_2 to node_1.
	if (!directed_graph_has_edge(graph, node_2, node_1))
		return 0;

	return 1;
}

void directed_graph_remove_edge(graph_t *graph, u_int src, u_int dst)
{
	// Verify the parameters.
	if (src >= graph->nodes || dst >= graph->nodes)
		DIE(1, "Invalid index of node\n");
	if (!graph || !graph->neighbors)
		DIE(1, "The graph isn't total allocated.\n");

	// Find the position of dst in the neighbour's list of src.
	ll_node_t *curr = graph->neighbors[src]->head;
	u_int pos = 0;
	while (curr) {
		if (dst == *(u_int *)curr->data)
			break;
		curr = curr->next;
		++pos;
	}

	// Verify if dst is neigbour with src.
	if (pos == graph->neighbors[src]->size)
		DIE(1, "Doesn't exist an edge from src to dst.\n");

	// Removes dst from the list of src.
	curr = ll_remove_nth_node(graph->neighbors[src], pos);
	free(curr->data);
	free(curr);
}

void undirected_graph_remove_edge(graph_t *graph, u_int src, u_int dst)
{
	directed_graph_remove_edge(graph, src, dst);
	directed_graph_remove_edge(graph, dst, src);
}

void graph_free(graph_t *graph)
{
	// Verify the parameter.
	if (!graph || !graph->neighbors)
		DIE(1, "The graph isn't total allocated.\n");

	// Free the memory.
	for (u_int i = 0; i < graph->nodes; ++i)
		ll_free(&graph->neighbors[i]);
	free(graph->neighbors);
	free(graph);
}

int find_length_min_path(graph_t *graph, u_int src, u_int dst)
{
	// Verify the parameters.
	if (src >= graph->nodes || dst >= graph->nodes)
		DIE(1, "Invalid index of node\n");
	if (!graph || !graph->neighbors)
		DIE(1, "The graph isn't total allocated.\n");

	// Start a BFS.

	// Create a vector of distances from src to the others nodes.
	int *distance = (int *)malloc(graph->nodes * sizeof(int));
	DIE(!distance, "malloc() failed\n");
	// distance = -1 ==> can't get to the node
	for (u_int i = 0; i < graph->nodes; ++i)
		distance[i] = -1;
	distance[src] = 0;

	// Initialize a queue.
	queue_t *q = q_create(sizeof(u_int), graph->nodes, free);
	q_enqueue(q, &src);

	while (!q_is_empty(q)) {
		// Take first node from q.
		u_int curr_node = *(u_int *)q_front(q);
		q_dequeue(q);

		// Verify every neighbor of the node.
		ll_node_t *curr_neighbor = graph->neighbors[curr_node]->head;
		while (curr_neighbor) {
			u_int neighbor = *(u_int *)curr_neighbor->data;
			// Did we find the minimum distance from src to this neigbor?
			if (distance[neighbor] == -1) {
				distance[neighbor] = distance[curr_node] + 1;
				q_enqueue(q, &neighbor);
			}
			// This neigbor is our destination?
			if (neighbor == dst) {
				q_free(q);
				int length = distance[dst];
				free(distance);
				return length;
			}
			curr_neighbor = curr_neighbor->next;
		}
	}

	// Free the memory allocated in this function.
	q_free(q);
	free(distance);

	// Doesn't exist a path from src to dst.
	return -1;
}

u_int *bfs_graph(graph_t *graph, u_int node)
{
	// Verify the parameters.
	if (!graph || node >= graph->nodes)
		DIE(1, "bfs_graph() - a parameter isn't valid\n");

	// Initialize a queue.
	queue_t *q = q_create(sizeof(u_int), graph->nodes, free);
	// Initialize a vector in which to mark the visited nodes.
	int *visited = (int *)calloc(graph->nodes, sizeof(int));
	DIE(!visited, "caloc() failed\n");
	// Initialize a vector in which to save the visited nodes.
	u_int *bfs = (u_int *)malloc((graph->nodes + 1) * sizeof(u_int));
	DIE(!bfs, "malloc() failed\n");
	// First element from bfs will be a counter of visited nodes.
	bfs[0] = 0;

	// Do the BFS.
	q_enqueue(q, &node);
	visited[node] = 1;
	while (!q_is_empty(q)) {
		u_int node = *(int *)q_front(q);
		q_dequeue(q);
		bfs[++bfs[0]] = node;

		ll_node_t *neighbor = graph->neighbors[node]->head;
		while (neighbor) {
			u_int neighbor_id = *(u_int *)neighbor->data;
			if (!visited[neighbor_id]) {
				q_enqueue(q, &neighbor_id);
				visited[neighbor_id] = 1;
			}
			neighbor = neighbor->next;
		}
	}

	// Free the memory allocated in thid function.
	q_free(q);
	free(visited);

	// Return the array which contains the BFS.
	return bfs;
}

void graph_double_nodes(graph_t *graph)
{
	int list_size = sizeof(ll_t *);
	graph->neighbors = realloc(graph->neighbors, graph->nodes * 2 * list_size);
	DIE(!graph->neighbors, "malloc() failed\n");
	for (u_int i = graph->nodes; i < graph->nodes * 2; ++i)
		graph->neighbors[i] = ll_create(sizeof(u_int));

	graph->nodes *= 2;
}

void
get_clique_helper(graph_t *graph, u_int *clique, u_int wanted_size, u_int start)
{
	ll_node_t *neighbor = graph->neighbors[clique[1]]->head;
	u_int pos = 0;

	// Get over the verified neighbors.
	while (pos != start) {
		neighbor = neighbor->next;
		pos++;
	}

	int remove_from_clique = 0;
	while (neighbor) {
		// Verify if the clique has the wanted size.
		if (clique[0] == wanted_size)
			return;

		// Verify if we must remove last node from clique.
		if (remove_from_clique)
			clique[0]--;

		// Get the id of current neighbor.
		u_int neighbor_id = *(u_int *)neighbor->data;

		// Verify if the current node can be added in clique.
		u_int can_be_in_clique = 1;
		for (u_int i = 1; i <= clique[0]; ++i) {
			if (!undirected_graph_has_edge(graph, clique[i], neighbor_id)) {
				can_be_in_clique = 0;
				break;
			}
		}
		if (can_be_in_clique) {
			clique[++clique[0]] = neighbor_id;
			remove_from_clique = 1;
			get_clique_helper(graph, clique, wanted_size, pos);
		} else {
			remove_from_clique = 0;
		}

		// Go at next neighbor.
		neighbor = neighbor->next;
		pos++;
	}

	// Verify if the clique has the wanted size.
	if (clique[0] == wanted_size)
		return;

	// Verify if we must remove last node from clique.
	if (remove_from_clique)
		clique[0]--;
}

u_int *get_clique(graph_t *graph, u_int node, u_int size)
{
	u_int *clique = malloc((size + 1) * sizeof(u_int));
	DIE(!clique, "malloc() failed\n");

	// Add the node of start in clique.
	clique[0] = 1;
	clique[1] = node;

	// Find the clique which start from the given node and has
	// the received size.
	get_clique_helper(graph, clique, size, 0);

	// Verify if exist such a clique.
	if (clique[0] != size)
		clique[0] = 0;

	// Return the found clique.
	return clique;
}

euler_tour_t *create_euler_tour(u_int nodes)
{
	// Allocate memory for the structure.
	euler_tour_t *euler = (euler_tour_t *)malloc(sizeof(euler_tour_t));
	DIE(!euler, "malloc() failed\n");

	// Allocate memory for every complex field from the structure.
	euler->tour = (u_int *)malloc(sizeof(u_int) * (2 * nodes - 1));
	DIE(!euler->tour, "malloc() failed\n");

	euler->first_appearance = (u_int *)calloc(nodes + 1, sizeof(u_int));
	DIE(!euler->first_appearance, "calloc() failed\n");
	euler->curr_step = 0;

	euler->depth = (u_int *)calloc(nodes + 1, sizeof(u_int));
	DIE(!euler->depth, "calloc() failed\n");
	euler->curr_depth = 0;

	// Return the created structure.
	return euler;
}

void euler_tour(euler_tour_t *euler, graph_t *tree, u_int node)
{
	// Verify the parameter.
	if (node >= tree->nodes)
		DIE(1, "euler_tour() - Invalid index of node()");

	// Update the step and the depth.
	euler->curr_step++;
	euler->curr_depth++;
	// Add the node in the Euler tour.
	euler->tour[euler->curr_step - 1] = node;

	// Verify if it's first appearnece in the tour.
	if (!euler->first_appearance[node]) {
		euler->first_appearance[node] = euler->curr_step;
		euler->depth[node] = euler->curr_depth;
	}

	// Continue the Euler Tour with his neighbors.
	ll_node_t *neighbor = tree->neighbors[node]->head;
	while (neighbor) {
		// Euler tour goes at a neighbor.
		u_int neighbor_id = *(u_int *)neighbor->data;
		euler_tour(euler, tree, neighbor_id);

		// Update the step and put the node again in
		// the Euler tour.
		euler->curr_step++;
		euler->tour[euler->curr_step - 1] = node;

		// Go at the next neighbor.
		neighbor = neighbor->next;
	}

	// We go up in tree with a level.
	euler->curr_depth--;
}

euler_tour_t *init_euler_tour(graph_t *tree, u_int root)
{
	euler_tour_t *euler = create_euler_tour(tree->nodes);
	euler_tour(euler, tree, root);
	return euler;
}

void free_euler_tour(euler_tour_t *euler)
{
	// Verify the parameter.
	if (!euler)
		return;

	// Deallocate the memory.
	if (euler->tour)
		free(euler->tour);
	if (euler->depth)
		free(euler->depth);
	if (euler->first_appearance)
		free(euler->first_appearance);
	free(euler);
}

u_int tree_lca(graph_t *tree, u_int root, u_int n1, u_int n2)
{
	// Do an Euler tour.
	euler_tour_t *euler = init_euler_tour(tree, root);

	// Determinate the zone from the Euler tour
	// in which is the LCA of n1 and n2.
	u_int start = euler->first_appearance[n1] - 1;
	u_int end = euler->first_appearance[n2] - 1;
	if (start > end) {
		u_int tmp = start;
		start = end;
		end = tmp;
	}

	// Search in that zone, the node which has the least
	// depth. That node is the LCA of n1 and n2.
	u_int lca = n1;
	for (u_int i = start; i <= end; ++i) {
		u_int curr_node = euler->tour[i];
		if (euler->depth[curr_node] < euler->depth[lca])
			lca = curr_node;
	}

	// Free the memory allocated in this function.
	free_euler_tour(euler);

	// Return the index of the LCA.
	return lca;
}

int tree_find_parent(graph_t *tree, u_int root, u_int node)
{
	// Verify the parameters.
	if (!tree || root >= tree->nodes || node >= tree->nodes)
		DIE(1, "tree_find_parent() - At least one parameter isn't valid\n");

	// The root doesn't have a parent.
	if (root == node)
		return -1;

	// We'll find the parent of the node using BFS.

	// Initialize a vector in which we mark with 1 the visited nodes.
	int *visited = (int *)calloc(tree->nodes, sizeof(int));
	DIE(!visited, "malloc() fai;ed\n");

	// Initialize a queue.
	queue_t *q = q_create(sizeof(u_int), tree->nodes, free);
	q_enqueue(q, &root);

	// Start BFS.
	while (!q_is_empty(q)) {
		u_int curr_node = *(u_int *)q_front(q);
		q_dequeue(q);
		visited[curr_node] = 1;

		ll_node_t *nbor = tree->neighbors[curr_node]->head;
		while (nbor) {
			u_int nbor_idx = *(u_int *)nbor->data;
			// Verify if the current neighbor is the node
			// whose parent we want to find it.
			if (nbor_idx == node) {
				free(visited);
				q_free(q);
				return curr_node;
			}
			if (!visited[nbor_idx])
				q_enqueue(q, &nbor_idx);
			nbor = nbor->next;
		}
	}

	// Free the memory allocated in this function.
	free(visited);
	q_free(q);

	return -1;
}

