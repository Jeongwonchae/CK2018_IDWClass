#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	PutPixel(0, 0);

	static float angle = 0.0f;
	angle += 0.1f;

	int length = 100;
	Matrix2 RMat;
	RMat.SetRotation(angle);

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			if (i == j)
			{
				Vector2 CurrentPos((float)i, (float)j);
				Vector2 NewPos = CurrentPos * RMat;
				PutPixel(NewPos.X, NewPos.Y);
			}
		}
	}

	//Vector2 Center(0.0f, 0.0f);
	//float radius = 100.0f;
	//int nradius = (int)radius;

	//Matrix2 Scale;
	//Scale.SetScale(2.0f, 1.0f);

	//for (int i = -nradius; i < nradius; i++)
	//{
	//	for (int j = -nradius; j < nradius; j++)
	//	{
	//		Vector2 CurrentPos(i, j);
	//		if (Vector2::DistSquared(Center, CurrentPos) < radius * radius)
	//		{
	//			Vector2 newPos = CurrentPos * Scale;
	//			PutPixel(newPos.X, newPos.Y);
	//		}
	//	}
	//}

	// Buffer Swap 
	BufferSwap();
}

