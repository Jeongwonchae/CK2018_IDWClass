
#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"
#include <vector>
#include <algorithm>

struct sprite {
	Vector3 posion;
	Vector3 scale;
	Vector3 rotation;
	int lay;
	Mesh	*mesh;
	sprite(Vector3 _pos, Vector3 _sca, Vector3 _rot, Mesh* _mesh, int _lay) : posion(_pos),
		scale(_sca), rotation(_rot), mesh(_mesh), lay(_lay) {}

};

Mesh* m = new Mesh();
//sprite* sp;

float offsetX = 0.0f;
float angle = 0.0f;
float scale = 1.0f;

std::vector<sprite> sp;

Matrix3 TMat, RMat, SMat;

Matrix3 TRSMat = TMat * RMat * SMat;

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;

	int halfWidth = RoundToInt(g_nClientWidth * 0.5f);
	int halfHeight = RoundToInt(g_nClientHeight * 0.5f);

	DWORD offset = (halfHeight * g_nClientWidth - g_nClientWidth * pt.Y) + (halfWidth + pt.X);
	*(dest + offset) = g_CurrentColor;
}

void DrawCall (Mesh * MeshToDraw)
{
	V2F_CUSTOM* v2f = new V2F_CUSTOM [ MeshToDraw->VSize ];

	for ( int i = 0; i < MeshToDraw->VSize; i++ )
	{
		Vertex v = MeshToDraw->Vertices [ i ];
		APPDATA_CUSTOM vdata;
		vdata.position = v.position;
		vdata.color = v.color;
		vdata.uv = v.uv;
		v2f[i] = VertexShader ( vdata );
	}

	for ( int i = 0; i < MeshToDraw->ISize; i+=3 )
	{
		/*Triangle t(v2f[MeshToDraw->Indices[i]],
			v2f[MeshToDraw->Indices[i + 1]],
			v2f[MeshToDraw->Indices[i + 2]]);*/
		Triangle t ( v2f [ MeshToDraw->Indices [ i ]],
			v2f [ MeshToDraw->Indices [ i+1 ]],
			v2f [ MeshToDraw->Indices [ i+2 ]] );
		// Rasterization
		for ( int y = t.Min.Y; y <= t.Max.Y; y++ )
		{
			for ( int x = t.Min.X; x <= t.Max.X; x++ )
			{
				Vector3 target ( ( float ) x + 0.5f, ( float ) y + 0.5f, 0.0f );
				float outS, outT;
				t.CalcBaryCentricCoord ( target, &outS, &outT );
				if ( t.IsInTrianble ( outS, outT ) )
				{
					V2F_CUSTOM v2f_;
					v2f_.position = t.GetFragmentPos ( target, outS, outT );
					v2f_.color = t.GetFragmentColor ( target, outS, outT );
					v2f_.uv = t.GetFragmentUV ( target, outS, outT );
					ULONG finalColor = FragmentShader (v2f_, outS, outT );
					g_CurrentColor = finalColor;
					PutPixel ( IntPoint(x, y) );
				}
			}
		}
	}

	if ( NULL != v2f )
	{
		delete [ ] v2f;
	}
}

V2F_CUSTOM VertexShader ( APPDATA_CUSTOM in )
{
	V2F_CUSTOM output;

	output.position = in.position * TRSMat;
	output.color = in.color;
	output.uv = in.uv;

	return output;
}

ULONG FragmentShader ( V2F_CUSTOM in, float s, float t )
{
	// Texture에서 색상 빼오기..
	if ( g_Texture->IsLoaded ( ) )
	{
		return g_Texture->TextureSample ( in.uv, s, t );
	}

	return in.color;
}



bool comp(sprite a, sprite b)
{
	return (a.lay < b.lay);
}

//void DrawLine(const Vector3& start, const Vector3& end)
//{
//	float length = (end - start).Dist();
//	float inc = 1.0f / length;
//
//	int maxLength = RoundToInt(length);
//	for (int i = 0; i <= maxLength; i++)
//	{
//		float t = inc * i;
//		if (t >= length) t = 1.0f;
//		Vector3 Pt = start * (1.0f - t) + end * t;
//		PutPixel(Pt.ToIntPoint());
//	}
//}

//void Draw2DTriangle(Triangle t)
//{
//	for (int y = t.Min.Y; y <= t.Max.Y; y++)
//	{
//		for (int x = t.Min.X; x <= t.Max.X; x++)
//		{
//			Vector3 target((float)x + 0.5f, (float)y + 0.5f, 0.0f);
//			float outS, outT;
//			t.CalcBaryCentricCoord(target, &outS, &outT);
//			if (t.IsInTrianble(outS, outT))
//			{
//				if (g_Texture->IsLoaded())
//				{
//					g_CurrentColor = g_Texture->GetTexturePixel(outS, outT, t);
//				}
//				else
//				{
//					g_CurrentColor = t.GetPixelColor(target, outS, outT);
//				}
//
//				PutPixel(IntPoint(x, y));
//			}			
//		}
//	}
//}

void InitFrame ( void )
{
	// Draw
	Vector3 Pt1, Pt2, Pt3, Pt4;

	Pt1.SetPoint(-150, 150.0f);
	Pt2.SetPoint(150.0f, 150.0f);
	Pt3.SetPoint(150.0f, -150.0f);
	Pt4.SetPoint(-150.0f, -150.0f);

	Vertex *v = new Vertex[4];
	v[0].color  = RGB32(255, 0, 0);
	v[0].uv = Vector2(0.125f, 0.125f);
	v[0].position = Pt1;

	v[1].color = RGB32(0, 255, 0);
	v[1].uv = Vector2(0.25f, 0.125f);
	v[1].position = Pt2;

	v[2].color = RGB32(0, 0, 255);
	v[2].uv = Vector2(0.25f, 0.25f);
	v[2].position = Pt3;

	v[3].color = RGB32(255, 255, 0);
	v[3].uv = Vector2(0.125f, 0.25f);
	v[3].position = Pt4;

	int *i = new int[6]{ 0, 1, 2, 0, 2, 3 };

	m->SetVertetices(v, 4);
	m->SetIndices(i, 6);
	//sprite* sp;

	sprite _sp(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0, 0, 0), m, 1);
	sp.push_back(_sp);
	sprite _sp1(Vector3(10, 100, 0), Vector3(1, 1, 1), Vector3(0, 0, 0), m, 0);
	sp.push_back(_sp1);

	std::sort(sp.begin(), sp.end(), comp);
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();
	if (GetAsyncKeyState(VK_LEFT)) offsetX -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) offsetX += 1.0f;
	if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
	if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;

	// Mesh 생성하기.. 
	for (auto i = sp.begin(); i != sp.end(); i++)
	{
		TMat.SetTranslation(i->posion.X, i->posion.Y);
		RMat.SetRotation(i->rotation.X);
		SMat.SetScale(i->scale.X);

		TRSMat = TMat * RMat * SMat;

		DrawCall(i->mesh);
	}

	BufferSwap();
}
