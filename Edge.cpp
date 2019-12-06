#include "Edge.h"

Edge::Edge()
{

}

Edge::Edge(int key, int weight)
{
	m_key = key;
	m_weight = weight;
	m_pNext = NULL;
}

Edge::~Edge()
{
}

void Edge::SetNext(Edge* pNext)
{
	m_pNext = pNext;
}

int Edge::GetKey() const
{
	return m_key;
}

int Edge::GetWeight() const
{
	return m_weight;
}

Edge* Edge::GetNext() const
{
	return m_pNext;
}