#pragma once
#include"Vector.h"
struct ARGB
{
	ARGB() { a = r = g = b = 0; }
	ARGB(float pa, float pr, float pg, float pb) :a(pa), r(pr), g(pg), b(pb) {}
	ARGB operator -(ARGB& rhs) { return ARGB(this->a - rhs.a, this->r - rhs.r, this->g - rhs.g, this->b - rhs.b); }
	ARGB operator +(ARGB& rhs) { return ARGB(this->a + rhs.a, this->r + rhs.r, this->g + rhs.g, this->b + rhs.b); }
	ARGB operator /(float rhs) { return ARGB(this->a / rhs, this->r / rhs, this->g / rhs, this->b / rhs); }
	ARGB& operator +=(ARGB& rhs) { return ARGB(this->a += rhs.a, this->r += rhs.r, this->g += rhs.g, this->b += rhs.b); return *this; }
	ARGB operator *(float rhs) { return ARGB(this->a * rhs, this->r * rhs, this->g * rhs, this->b * rhs); }
	~ARGB() {}
	float a;
	float r;
	float g;
	float b;
};
struct Vertex
{
	Vertex(float posx, float posy, ARGB color, FLOAT3 normal = FLOAT3(0, 0, 0)) :m_Position(posx, posy), Color(color), m_Normal(normal) {}
	Vertex& operator +=(Vertex rhs) { this->m_Position.x += rhs.m_Position.x; this->m_Position.y += rhs.m_Position.y; this->Color += rhs.Color; return *this; }
	FLOAT2 m_Position;
	FLOAT2 m_UV;
	FLOAT3 m_Normal;
	ARGB Color;
};