#include "Vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(int key)
{
	m_key = key;
	m_size = 0;
	m_pEHead = NULL;
	m_pNext = NULL;
}

Vertex::~Vertex()
{
}

void Vertex::SetNext(Vertex* pNext)
{
	m_pNext = pNext;
}

int Vertex::GetKey() const
{
	return m_key;
}

Vertex* Vertex::GetNext() const
{
	return m_pNext;
}

Edge* Vertex::GetHeadOfEdge() const
{
	return m_pEHead;
}

int Vertex::Size() const
{
	return m_size;
}

void Vertex::AddEdge(int edgeKey, int weight)
{
	m_size++;
	if (m_pEHead == NULL)
	{
		m_pEHead = new Edge(edgeKey, weight);
		return;
	}

	Edge* current = m_pEHead;
	while (current->GetNext() != NULL)
	{
		current = current->GetNext();
	}
	current->SetNext(new Edge(edgeKey, weight));
}

void Vertex::Clear()
{

}