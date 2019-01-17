#pragma once

class PointCollection
{
public:
	friend class QRender;

	PointCollection();

	~PointCollection();

	PointCollection(const PointCollection& c) = delete;

	void		AddPoint(const Vertex& vec);

	void		AddPoint(const FLOAT3&pos, const FLOAT3& color);

	void		SetPoint(UINT index, const Vertex& vec);

	void		SetPointPos(UINT index, const FLOAT3& pos);

	void		DeletePoint(UINT index);

	UINT		GetVertexCount();

	void		PopBack();

	void		ClearAll();
	
private:

	std::vector<UINT>*		m_pIB_Mem;
	std::vector<Vertex>*		m_pVB_Mem;
};