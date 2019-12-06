#include "Manager.h"
#include <cstring>

#define SWAP(x, y, tp) ( (tp)=(x), (x)=(y), (y)=(tp) )

Manager::~Manager()
{
    if (fout.is_open())
        fout.close();

    if (ferr.is_open())
        ferr.close();
}

void insertionSort(std::vector<int>& arr, int left, int right)
{
	for (int i = left + 1; i <= right; i++)
	{
		int target = arr[i];
		int j;

		for (j = i - 1; j >= left && arr[j] > target; j--)
		{
			arr[j + 1] = arr[j];
		}

		arr[j + 1] = target;
	}
}

int partition(std::vector<int>& arr, int left, int right)
{
	int pivot = left;
	int low = left + 1;
	int high = right;
	int temp;

	while (1)
	{
		while (arr[low] < arr[pivot] && low <= right) low++;
		while (arr[high] > arr[pivot] && low >= left) high--;

		if (low < high) SWAP(arr[low], arr[high], temp);
		else break;
	}

	SWAP(arr[pivot], arr[high], temp);

	return high;
}

void quickSort(std::vector<int>& arr, int left, int right)
{
	if (left < right)
	{
		if (right - left + 1 <= 6)
		{
			insertionSort(arr, left, right);
		}
		else
		{
			int pivot = partition(arr, left, right);
			quickSort(arr, left, pivot - 1);
			quickSort(arr, pivot + 1, right);
		}
	}
}

void Manager::Run(const char* filepath)
{
    fout.open(RESULT_LOG_PATH);

    // TODO: implement
	std::ifstream fin;
	fin.open(filepath);

	// command data file does not exist
	// Error code 100
	if (!fin)
	{
		fout << "====== SYSTEM ======" << std::endl;
		fout << "CommandFileNotExist" << std::endl;
		fout << "====================" << std::endl;

		PrintError(CommandFileNotExist);
		return;
	}

	char* cmd = new char[64];
	while (!fin.eof())
	{
		fin.getline(cmd, 64);
		char* tok = strtok(cmd, " ");

		// LOAD
		if (!strcmp(tok, "LOAD"))
		{
			PrintError(Load(strtok(NULL, " ")));
			//Load(strtok(NULL, " "));
		}
		// PRINT
		else if (!strcmp(tok, "PRINT"))
		{
			PrintError(Print());
		}
		// DIJKSTRA
		else if (!strcmp(tok, "DIJKSTRA"))
		{
			char* startVertex = strtok(NULL, " ");
			char* endVertex = strtok(NULL, " ");

			if (startVertex == NULL || endVertex == NULL)
			{
				PrintError(FindShortestPathDijkstraUsingSet(-1, -1));
			}
			else
			{
				PrintError(FindShortestPathDijkstraUsingSet(atoi(startVertex), atoi(endVertex)));
			}
		}
		// DIJKSTRA MIN
		else if (!strcmp(tok, "DIJKSTRAMIN"))
		{
			char* startVertex = strtok(NULL, " ");
			char* endVertex = strtok(NULL, " ");

			if (startVertex == NULL || endVertex == NULL)
			{
				PrintError(FindShortestPathDijkstraUsingMinHeap(-1, -1));
			}
			else
			{
				PrintError(FindShortestPathDijkstraUsingMinHeap(atoi(startVertex), atoi(endVertex)));
			}
		}
		// BELLMANFORD
		else if (!strcmp(tok, "BELLMANFORD"))
		{
			char* startVertex = strtok(NULL, " ");
			char* endVertex = strtok(NULL, " ");

			if (startVertex == NULL || endVertex == NULL)
			{
				PrintError(FindShortestPathBellmanFord(-1, -1));
			}
			else
			{
				PrintError(FindShortestPathBellmanFord(atoi(startVertex), atoi(endVertex)));
			}
		}
		// FLOYD
		else if (!strcmp(tok, "FLOYD"))
		{
			PrintError(FindShortestPathFloyd());
		}
		else
		{
			char* error = strtok(tok, " ");
			fout << "======" << error << "======" << std::endl;
			fout << "NonDefinedCommand" << std::endl;
			fout << "====================" << std::endl;
			PrintError(NonDefinedCommand);
		}

		fout << std::endl;
	}
}

void Manager::PrintError(Result result)
{
	// ferr << "Error code: " << result << std::endl;
	fout << std::endl;
	fout << "==================" << std::endl;
    fout << "Error code: " << result << std::endl;
	fout << "==================" << std::endl;
}

/// <summary>
/// make a graph
/// </summary>
///
/// <param name="filepath">
/// the filepath to read to make the graph
/// </param>
///
/// <returns>
/// Result::Success if load is successful.
/// Result::LoadFileNotExist if file is not exist.
/// </returns>
Result Manager::Load(const char* filepath)
{
    // TODO: implement
	fout << "====== LOAD ======" << std::endl;

	std::ifstream fin;
	fin.open(filepath);

	// ====================== ERROR AREA ========================
	// data file not found
	// Error code 101
	if (!fin)
	{
		fout << "LoadFileNotExist" << std::endl;
		fout << "=================" << std::endl;
	
		return LoadFileNotExist;
	}
	// ===========================================================

	// Data is read
	char* cmd = new char[64];
	fin.getline(cmd, 64);

	// enter graph data
	// the same form
	//
	// vertex ¡æ edge ¡æ edge ¡æ NULL
	// ¡é
	// vertex ¡æ edge ¡æ edge ¡æ edge ¡æ NULL
	// ¡é
	// NULL
	//
	m_graph = Graph();
	int size = atoi(cmd);
	int start = 0;
	while (!fin.eof())
	{
		// register vertex

		m_graph.AddVertex(start);

		fin.getline(cmd, 64);
		char* tok = strtok(cmd, " ");
		for (int end = 0; end < size; end++)
		{
			int weight = atoi(tok);
			if (weight != 0)
			{
				m_graph.AddEdge(start, end, weight);
			}
			tok = strtok(NULL, " ");
		}

		start++;
	}

	fout << "Success" << std::endl;
	fout << "=================" << std::endl;
	return Success;
}
/// <summary>
/// print out the graph as matrix form
/// </summary>
///
/// <returns>
/// Result::Success if the printing is successful
/// Result::GraphNotExist if there is no graph
/// </returns>
Result Manager::Print()
{
    // TODO: implement
	fout << "====== PRINT ======" << std::endl;

	// ====================== ERROR =========================
	// without graphs
	// Error code 202
	if (m_graph.FindVertex(0) == NULL)
	{
		fout << "GraphNotExist" << std::endl;
		fout << "=================" << std::endl;
		return GraphNotExist;
	}
	// ===========================================================

	// Ãâ·Â
	m_graph.Print(fout);

	fout << "=================" << std::endl;
	return Success;
}
/// <summary>
/// find the path from startVertexKey to endVertexKey with DFS (stack)
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>


/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    // TODO: implement
	fout << "===== DIJKSTRA =====" << std::endl;

	// ====================== ERROR =========================
	// Insufficient factors
	// Error code 200
	if (startVertexKey == -1 && endVertexKey == -1)
	{
		fout << "VertexKeyNotExist" << std::endl;
		fout << "=================" << std::endl;
		return VertexKeyNotExist;
	}

	// without graphs
	// Error code 202
	if (m_graph.FindVertex(0) == NULL)
	{
		fout << "GraphNotExist" << std::endl;
		fout << "=================" << std::endl;
		return GraphNotExist;
	}

	// if the entered vertex is not on the graph
	// Error code 201
	if (startVertexKey < 0 || endVertexKey < 0
		|| startVertexKey >= m_graph.Size() || endVertexKey >= m_graph.Size())
	{
		fout << "InvalidVertexKey" << std::endl;
		fout << "=================" << std::endl;
		return InvalidVertexKey;
	}

	// if there is a negative light in the graph
	// Error code 203
	if (m_graph.IsNegativeEdge())
	{
		fout << "InvalidAlgorithm" << std::endl;
		fout << "=================" << std::endl;
		return InvalidAlgorithm;
	}
	// ===========================================================

	// run the multi-extra algorithm and get results
	std::vector<int> result = m_graph.FindShortestPathDijkstraUsingSet(startVertexKey, endVertexKey);
	int pathLength = result.back(); result.pop_back();

	// output result value
	// shortest path
	fout << "shortest path: ";
	for (int i = 0; i < result.size(); i++)
	{
		fout << result[i] << " ";
	}
	fout << std::endl;

	// Sort
	quickSort(result, 0, result.size() - 1);

	// sorted nodes
	fout << "sorted nodes: ";
	for (int i = 0; i < result.size(); i++)
	{
		fout << result[i] << " ";
	}
	fout << std::endl;

	// path length
	fout << "path length: " << pathLength << std::endl;

	fout << "=================" << std::endl;
	return Success;
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using MinHeap
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)
{
    // TODO: implement
	fout << "====== DIJKSTRAMIN ======" << std::endl;

	// ====================== ERROR =========================
	// Insufficient factors
	// Error code 200
	if (startVertexKey == -1 && endVertexKey == -1)
	{
		fout << "VertexKeyNotExist" << std::endl;
		fout << "=================" << std::endl;
		return VertexKeyNotExist;
	}

	// without graphs
	// Error code 202
	if (m_graph.FindVertex(0) == NULL)
	{
		fout << "GraphNotExist" << std::endl;
		fout << "=================" << std::endl;
		return GraphNotExist;
	}

	// if the entered vertex is not on the graph
	// Error code 201
	if (startVertexKey < 0 || endVertexKey < 0
		|| startVertexKey >= m_graph.Size() || endVertexKey >= m_graph.Size())
	{
		fout << "InvalidVertexKey" << std::endl;
		fout << "=================" << std::endl;
		return InvalidVertexKey;
	}
	// if there is a negative light in the graph
	// Error code 203
	if (m_graph.IsNegativeEdge())
	{
		fout << "InvalidAlgorithm" << std::endl;
		fout << "=================" << std::endl;
		return InvalidAlgorithm;
	}
	// ===========================================================
	// run the multi-extra (minimum-heap) algorithm and get results
	std::vector<int> result = m_graph.FindShortestPathDijkstraUsingMinHeap(startVertexKey, endVertexKey);
	int pathLength = result.back(); result.pop_back();

	// shortest path
	fout << "shortest path: ";
	for (int i = 0; i < result.size(); i++)
	{
		fout << result[i] << " ";
	}
	fout << std::endl;

	// Sort
	quickSort(result, 0, result.size() - 1);

	// sorted nodes
	fout << "sorted nodes: ";
	for (int i = 0; i < result.size(); i++)
	{
		fout << result[i] << " ";
	}
	fout << std::endl;

	// path length
	fout << "path length: " << pathLength << std::endl;

	fout << "=================" << std::endl;
	return Success;
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::NegativeCycleDetected if exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    // TODO: implement
	fout << "===== BELLMANFORD =====" << std::endl;

	// ====================== ERROR =========================
	// Insufficient factors
	// Error code 200
	if (startVertexKey == -1 && endVertexKey == -1)
	{
		fout << "VertexKeyNotExist" << std::endl;
		fout << "=================" << std::endl;
		return VertexKeyNotExist;
	}
	// without graphs
	// Error code 202
	if (m_graph.FindVertex(0) == NULL)
	{
		fout << "GraphNotExist" << std::endl;
		fout << "=================" << std::endl;
		return GraphNotExist;
	}

	// if the entered vertex is not on the graph
	// Error code 201
	if (startVertexKey < 0 || endVertexKey < 0
		|| startVertexKey >= m_graph.Size() || endVertexKey >= m_graph.Size())
	{
		fout << "InvalidVertexKey" << std::endl;
		fout << "=================" << std::endl;
		return InvalidVertexKey;
	}
	// ===========================================================
	// run the Belmanford algorithm and get results
	std::vector<int> result = m_graph.FindShortestPathBellmanFord(startVertexKey, endVertexKey);
	int pathLength = result.back(); result.pop_back();

	// ===========================================================
	// when there is a negative cycle
	// Error code 204
	if (pathLength == -1)
	{
		fout << "NegativeCycleDetected" << std::endl;
		fout << "=================" << std::endl;
		return NegativeCycleDetected;
	}
	// ===========================================================

	// shortest path
	fout << "shortest path: ";
	for (int i = 0; i < result.size(); i++)
	{
		fout << result[i] << " ";
	}
	fout << std::endl;

	// sort
	quickSort(result, 0, result.size() - 1);

	// sorted nodes
	fout << "sorted nodes: ";
	for (int i = 0; i < result.size(); i++)
	{
		fout << result[i] << " ";
	}
	fout << std::endl;

	// path length
	fout << "path length: " << pathLength << std::endl;

	fout << "=================" << std::endl;
	return Success;
}

Result Manager::FindShortestPathFloyd()
{
	// TODO: implement
	fout << "===== FLOYD =====" << std::endl;

	// ====================== ERROR =========================
	// without graphs
	// Error code 202
	if (m_graph.FindVertex(0) == NULL)
	{
		fout << "GraphNotExist" << std::endl;
		fout << "=================" << std::endl;
		return GraphNotExist;
	}
	// ===========================================================

	// the Floyd algorithm and has been the results.
	std::vector<vector<int> > result = m_graph.FindShortestPathFloyd();

	// ===========================================================
	// if there is the negative cycle.
	// Error code 204
	for (int i = 0; i < m_graph.Size(); i++)
	{
		if (result[i][i] < 0)
		{
			fout << "NegativeCycleDetected" << std::endl;
			fout << "=================" << std::endl;
			return NegativeCycleDetected;
		}
	}
	// ===========================================================

	// print result
	for (int i = 0; i < m_graph.Size(); i++)
	{
		for (int j = 0; j < m_graph.Size(); j++)
		{
			if (i == j) fout << "0 ";
			else		fout << result[i][j] << " ";
		}

		fout << std::endl;
	}

	fout << "=================" << std::endl;
	return Success;
}
