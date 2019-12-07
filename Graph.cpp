#include "Graph.h"
#include "Stack.h"
#include "MinHeap.h"
#include <set>
#include <map>
#include <stack>
#include <algorithm>
#include <string>

#define DFS_FIRST_PATH

struct compare
{
	bool operator()	(const pair<int, int>& a, const pair<int, int>& b)
	{
		if (a.second == b.second)
		{
			return a.first < b.first;
		}

		return a.second < b.second;
	}
};

Graph::Graph()
{
    // TODO: implement
	m_pVHead = NULL;
	m_vSize = 0;
	m_isNegative = false;
}
Graph::~Graph()
{
    // TODO: implement
	Clear();
}

void Graph::AddVertex(int vertexKey)
{
	// Increase the number of vertices.
	m_vSize++;

	// If you don't have a head, you make a head.
	if (m_pVHead == NULL)
	{
		m_pVHead = new Vertex(vertexKey);
		return;
	}

	// Find the last vertex and attach it to the back..
	Vertex* current = m_pVHead;
	while (current->GetNext() != NULL)
	{
		current = current->GetNext();
	}
	current->SetNext(new Vertex(vertexKey));
}

void Graph::AddEdge(int startVertexKey, int endVertexKey, int weight)
{
	// Change the flag to true to indicate that there is a negative number if wait is negative.
	if (weight < 0) m_isNegative = true;

	// Locate the starting point and enter the main line after that.
	FindVertex(startVertexKey)->AddEdge(endVertexKey, weight);
}

Vertex* Graph::FindVertex(int key)
{
	Vertex* current = m_pVHead;
	while (current->GetKey() != key)
	{
		current = current->GetNext();
	}

	return current;
}

int Graph::Size() const
{
	return m_vSize;
}

void Graph::Clear()
{
	// TODO
}

void Graph::Print(std::ofstream& fout)
{
	for (int start = 0; start < m_vSize; start++)
	{
		Edge* current = FindVertex(start)->GetHeadOfEdge();
		int end = 0;

		while (current != NULL)
		{
			for (; end < current->GetKey(); end++)
			{
				fout << "0 ";
			}

			fout << current->GetWeight() << " ";
			current = current->GetNext();
			end++;
		}

		for (; end < m_vSize; end++)
		{
			fout << "0 ";
		}
		fout << std::endl;
	}
}

bool Graph::IsNegativeEdge()
{
	return m_isNegative;
}

std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
	// pair<vertexKey, distance>
	// pair<정점, 정점까지의 거리>
	std::set<std::pair<int, int>, compare> qSet;

	// 최단거리를 나타내는 배열
	std::vector<int> dist(m_vSize, IN_FINITY);

	// Maps for storing shortest path
	// path_m[end] = start;
	std::map<int, int> pathMap;


	// Initialize Start Point

	dist[startVertexKey] = 0;

	// If there is an isolated node, it should be handled as below!!!
	// there may be isolated nodes
	/*for (int i = 0; i < m_vSize; i++)
	{
		if (i == startVertexKey)
		{
			q_set.insert(make_pair(i, 0));
		}
		else
		{
			q_set.insert(make_pair(i, IN_FINITY));
		}
	}*/

	qSet.insert(make_pair(startVertexKey, 0));
	pathMap[startVertexKey] = -1;


	// Search
	while (!qSet.empty())
	{
		// Visit the lightest node available today

		int index = (qSet.begin())->first;
		int cost = (qSet.begin())->second;
		qSet.erase(qSet.begin());

		// Skip if new path is heavier than existing path
		if (dist[index] < cost) continue;

		// Insert a new path to navigate through the queue to go in the current nodes.
		Edge* current_edge = FindVertex(index)->GetHeadOfEdge();
		while (current_edge != NULL)
		{
			int key = current_edge->GetKey();
			int weight = current_edge->GetWeight();

			// If the new route is lighter,
			if (dist[key] > dist[index] + weight)
			{
				// Replace with a new route and insert the path into the queue.
				dist[key] = dist[index] + weight;
				qSet.insert(make_pair(key, dist[key]));

				//Enter a path in the map for output
				pathMap[key] = index;
			}

			// the next edge
			current_edge = current_edge->GetNext();
		}
	}

	// Derivation of result value
	std::vector<int> result;

	if (dist[endVertexKey] != IN_FINITY)
	{
		std::stack<int> s;
		int next = endVertexKey;
		
		while (next != -1)
		{
			result.push_back(next);
			next = pathMap[next];
		}
	}
	
	// Flipping an array (to make it a starting point ~ an endpoint)
	std::reverse(result.begin(), result.end());

	// Insert the shortest path value at the end of the array for return of the shortest path value
	result.push_back(dist[endVertexKey]);

	return result;
}

std::vector<int> Graph::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)
{
	// pair<vertexKey, distance>
	MinHeap<int, int> qHeap;

	// an arrangement representing the shortest distance
	std::vector<int> dist(m_vSize, IN_FINITY);

	// Maps for storing shortest path
	// path_m[end] = start;
	std::map<int, int> pathMap;


	// Initialize Start Point
	dist[startVertexKey] = 0;
	qHeap.Push(startVertexKey, 0);
	pathMap[startVertexKey] = -1;


	// Explore
	while (!qHeap.IsEmpty())
	{
		// Visit the lightest node available today
		int index = qHeap.Top().first;
		int cost = qHeap.Top().second;
		qHeap.Pop();

		// Skip if new path is heavier than existing path
		if (dist[index] < cost) continue;

		// Insert a new path to navigate through the queue to go in the current nodes.
		Edge* current_edge = FindVertex(index)->GetHeadOfEdge();
		while (current_edge != NULL)
		{
			int key = current_edge->GetKey();
			int weight = current_edge->GetWeight();

			// If the new route is lighter,
			if (dist[key] > dist[index] + weight)
			{
				// Replace with a new route and insert the path into the queue.
				dist[key] = dist[index] + weight;
				qHeap.Push(key, dist[key]);

				// Enter a path in the map for output
				pathMap[key] = index;
			}

			current_edge = current_edge->GetNext();
		}
	}

	//Derivation of result value
	std::vector<int> result;

	if (dist[endVertexKey] != IN_FINITY)
	{
		std::stack<int> s;
		int next = endVertexKey;

		while (next != -1)
		{
			result.push_back(next);
			next = pathMap[next];
		}
	}

	// Flipping an array (to make it a starting point ~ an endpoint)
	std::reverse(result.begin(), result.end());

	//Insert the shortest path value at the end of the array for return of the shortest path value
	result.push_back(dist[endVertexKey]);

	return result;
}

std::vector<int> Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
	// an arrangement representing the shortest distance
	std::vector<int> dist(m_vSize, IN_FINITY);

	// Maps for storing shortest path
	// path_m[end] = start;
	std::map<int, int> pathMap;

	// 시작점 초기화
	dist[startVertexKey] = 0;
	pathMap[startVertexKey] = -1;
	// Explore
	// Belmanford Algorithm
	// Number of total vertices - repeat by 1
	for (int i = 0; i < m_vSize - 1; i++)
	{
		for (int start = 0; start < m_vSize; start++)
		{
			Edge* current_edge = FindVertex(start)->GetHeadOfEdge();

			while (current_edge != NULL)
			{
				int key = current_edge->GetKey();
				int weight = current_edge->GetWeight();

				// Start vertices have already been discovered (=not unlimited, = have visited vertices)
				// If the new path is lighter
				if (dist[start] != IN_FINITY
					&& dist[start] + weight < dist[key])
				{
					dist[key] = dist[start] + weight;
					pathMap[key] = start;
				}

				current_edge = current_edge->GetNext();
			}
		}
	}
	
	// derive results (for negative cycles)
	std::vector<int> result;

	// check if negative cycles exist after finishing algorithm
	// If negative cycles exist
	// perform edge recovery as much as (vertex count - 1)
	// change in shortest path value once again.

	// edge reduction (as above)
	for (int start = 0; start < m_vSize; start++)
	{
		Edge* current_edge = FindVertex(start)->GetHeadOfEdge();

		while (current_edge != NULL)
		{
			int key = current_edge->GetKey();
			int weight = current_edge->GetWeight();

			// Start vertices have already been discovered (=not unlimited, = have visited vertices)
			// If the new path is lighter
			if (dist[start] != IN_FINITY
				&& dist[start] + weight < dist[key])
			{
				// A negative cycle exists because the shortest path update occurred even after the algorithm was finished
				result.push_back(-1);

				return result;
			}

			current_edge = current_edge->GetNext();
		}
	}

	if (dist[endVertexKey] != IN_FINITY)
	{
		std::stack<int> s;
		int next = endVertexKey;

		while (next != -1)
		{
			result.push_back(next);
			next = pathMap[next];
		}
	}

	// Flip the array (to make it a start ~ end point)
	std::reverse(result.begin(), result.end());

	// insert the shortest path value at the end of the array for return of the shortest path value
	result.push_back(dist[endVertexKey]);

	return result;
}

std::vector<vector<int> > Graph::FindShortestPathFloyd()
{
	// Initialize Results Table
	std::vector<vector<int> > result(m_vSize, vector<int>(m_vSize, IN_FINITY));

	for (int start = 0; start < m_vSize; start++)
	{
		Edge* current = FindVertex(start)->GetHeadOfEdge();

		while (current != NULL)
		{
			result[start][current->GetKey()] = current->GetWeight();

			current = current->GetNext();
		}
	}
	
	for (int i = 0; i < m_vSize; i++)
	{
		result[i][i] = 0;
	}

	// Calculate Table
	// intermediate path
	for (int i = 0; i < m_vSize; i++)
	{
		// endpoint
		for (int j = 0; j < m_vSize; j++)
		{
			// startpoint
			for (int k = 0; k < m_vSize; k++)
			{
				// Start point-end point > start point-middle point-end point
				if (result[k][j] > result[k][i] + result[i][j])
				{
					// Renew with new path
					result[k][j] = result[k][i] + result[i][j];
				}
			}
		}
	}

	return result;
}
