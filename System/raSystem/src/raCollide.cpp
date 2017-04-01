#include "..\include\raMain.h"

namespace System
{
	RAPI bool raColPhisicsObject(raPhysik* p1, raPhysik* p2)
	{
		raVector3 dist;
		dist = p1->GetPosition() - p2->GetPosition();

		if(raVector3Lenght(dist) <= p1->GetBoundingSphereRadius() + p2->GetBoundingSphereRadius())
		{
			raVector3 v1 = p1->GetVelocity();
			raVector3 v2 = p2->GetVelocity();

			float m1 = p1->GetMass();
			float m2 = p2->GetMass();

			raVector3 v1New, v2New, a;

			a = raVector3Normalize(dist);

			raVector3 n;
			n = raVector3Cross(p1->GetVelocity(), p2->GetVelocity());

			if((float)n.x == (float)n.y && (float)n.x == (float)n.z && (float)n.x == 0.0f) // Eindimensional
			{
				v1New = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
				v2New = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);
			}
			else
			{
				n = raVector3Normalize(n);

				raVector3 b;
				b = raVector3Cross(n, a);

				float ab = raVector3Dot(a, b);
				float v11 = (raVector3Dot(v1, a) - raVector3Dot(v1, b) * ab) /
					( 1 - ab * ab);
				float v12 = (raVector3Dot(v1, b) - raVector3Dot(v1, a) * ab) /
					( 1 - ab * ab);

				float v21 = (raVector3Dot(v2, a) - raVector3Dot(v2, b) * ab) /
					( 1 - ab * ab);
				float v22 = (raVector3Dot(v2, b) - raVector3Dot(v2, a) * ab) /
					( 1 - ab * ab);

				v1New = a * ((m1 - m2) * v11 + 2 * m2 * v21) / (m1 + m2) + b * v12;
				v2New = a * ((m2 - m1) * v21 + 2 * m1 * v11) / (m1 + m2) + b * v22;
			}

			p1->SetVelocity(v1New.x, v1New.y, v1New.z);
			p2->SetVelocity(v2New.x, v2New.y, v2New.z);

			return true;
		}
		return false;
	}
};