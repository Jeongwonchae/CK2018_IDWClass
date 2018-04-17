#pragma once

#include "stdafx.h"
#include "Vector.h"
#include "GDIHelper.h"
#include "Renderer.h"

struct Vertex
{
public:
	Vertex() {}
	Vertex(const Vector3& v, const ULONG& c) : position(v), color(c) {}
	Vector3 position;
	ULONG color;
};

class myTriangle
{
private:
	Vertex vertices[3];
	float xMin, xMax, yMin, yMax;
	Vector2 u, v, w;
	float invDenom;
	float dotUU, dotUV, dotVV;
public:
	myTriangle() {}
	myTriangle(const Vertex& v1, const Vertex& v2, const Vertex v3) {
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
	}

	void drawTriangle();
private:

	void calculate() {
		xMin = yMin = INFINITY;
		xMax = yMax = -INFINITY;

		// xMin operating
		if (vertices[0].position.X < xMin) xMin = vertices[0].position.X;
		if (vertices[1].position.X < xMin) xMin = vertices[1].position.X;
		if (vertices[2].position.X < xMin) xMin = vertices[2].position.X;
		// xMax operating
		if (vertices[0].position.X > xMax) xMax = vertices[0].position.X;
		if (vertices[1].position.X > xMax) xMax = vertices[1].position.X;
		if (vertices[2].position.X > xMax) xMax = vertices[2].position.X;
		// yMin operating
		if (vertices[0].position.Y < yMin) yMin = vertices[0].position.Y;
		if (vertices[1].position.Y < yMin) yMin = vertices[1].position.Y;
		if (vertices[2].position.Y < yMin) yMin = vertices[2].position.Y;
		// yMax operating
		if (vertices[0].position.Y > yMax) yMax = vertices[0].position.Y;
		if (vertices[1].position.Y > yMax) yMax = vertices[1].position.Y;
		if (vertices[2].position.Y > yMax) yMax = vertices[2].position.Y;

		// Vector operating
		u = (vertices[1].position - vertices[0].position).ToVector2();
		v = (vertices[2].position - vertices[0].position).ToVector2();

		// dot operating
		dotUU = u.Dot(u);
		dotUV = u.Dot(v);
		dotVV = v.Dot(v);

		// invDenom[ºÐ¸ð] operating
		invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);
	}

	bool IsInTriangle(int x, int y) {
		w = (Vector3((float)x, (float)y, 0.0f) - vertices[0].position).ToVector2();
		float dotUW = u.Dot(w);
		float dotVW = v.Dot(w);
		float outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
		float outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;
		if (outS < 0.0f || outT < 0.0f) return false;
		if (outS + outT > 1.0f) return false;

		return true;
	}

	Vector2 getMin(float d1, float d2, float d3) {

		float first, second;
		if (d1 < d2) first = d1;
		else first = d2;

		if (d2 < d3) {
			second = d2;
			first = d1;
		}
		else {
			second = first;
			first = d3;
		}
		return Vector2(first, second);
	}
};
