#include"MyGameEngine.h"

PointCollection::PointCollection()
{
	m_pVB_Mem = new std::vector<Vertex>;
	m_pIB_Mem = new std::vector<UINT>;
}

PointCollection::~PointCollection()
{
	delete m_pVB_Mem;
	delete m_pIB_Mem;
}

void PointCollection::AddPoint(const Vertex & vec)
{
	m_pVB_Mem->push_back(vec);
	m_pIB_Mem->push_back(m_pVB_Mem->size() - 1);
}

void PointCollection::AddPoint(const FLOAT3 & pos, const FLOAT3 & color)
{
	Vertex v;
	v.m_Position = { pos.x,pos.y,pos.z,1.0f };
	v.Color = { color.x,color.y,color.z,1.0f };
	v.m_Normal = { 0,0,0 };
	v.m_UV = { 0,0 };
	m_pVB_Mem->push_back(v);
	m_pIB_Mem->push_back(m_pVB_Mem->size() - 1);
}

void PointCollection::SetPoint(UINT index, const Vertex & vec)
{
	if (index < m_pVB_Mem->size())
	{
		m_pVB_Mem->at(index)=vec;
	}
}

void PointCollection::SetPointPos(UINT index, const FLOAT3 & pos)
{
	if (index < m_pVB_Mem->size())
	{
		m_pVB_Mem->at(index).m_Position = { pos.x,pos.y,pos.z,1.0f };
	}
}

void PointCollection::DeletePoint(UINT index)
{
	if (index < m_pVB_Mem->size())
	{
		std::swap(m_pVB_Mem->at(index), m_pVB_Mem->back());
		m_pVB_Mem->pop_back();
		m_pIB_Mem->pop_back();
	}
}

UINT PointCollection::GetVertexCount()
{
	return m_pVB_Mem->size();
}

void PointCollection::PopBack()
{
	if (m_pVB_Mem->size() > 0)m_pVB_Mem->pop_back();
}

void PointCollection::ClearAll()
{
	m_pVB_Mem->clear();
	m_pIB_Mem->clear();
}
