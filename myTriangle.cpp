#include "stdafx.h"
#include "myTriangle.h"
#include "Renderer.h"

void myTriangle::drawTriangle()
{
	calculate();
	int x, y;

	for (y = RoundToInt(yMin); y < RoundToInt(yMax); y++) {
		for (x = RoundToInt(xMin); x < RoundToInt(xMax); x++) {
			if (IsInTriangle(x, y)) {
				float distToVertex1 = Vector3(x, y, 1.0f).DistTo(vertices[0].position);
				float distToVertex2 = Vector3(x, y, 1.0f).DistTo(vertices[1].position);
				float distToVertex3 = Vector3(x, y, 1.0f).DistTo(vertices[2].position);

				ULONG cur, tar;
				Vector2 dVector = getMin(distToVertex1, distToVertex2, distToVertex3);

				if (dVector.X == distToVertex1) cur = vertices[0].color;
				if (dVector.X == distToVertex2) cur = vertices[1].color;
				if (dVector.X == distToVertex3) cur = vertices[2].color;

				if (dVector.Y == distToVertex1) tar = vertices[0].color;
				if (dVector.Y == distToVertex2) tar = vertices[1].color;
				if (dVector.Y == distToVertex3) tar = vertices[2].color;

				ULONG rgb = Interpolation(cur, tar, 0.5);
				int b, g, r;
				b = rgb >> 16 & 255;
				g = rgb >> 8 & 255;
				r = rgb >> 0 & 255;

				SetColor(r, g, b);
				PutPixel(IntPoint(x, y));
			}
		}
	}
}
