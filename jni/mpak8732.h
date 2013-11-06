#ifndef _Mpak8732__h
#define _Mpak8732__h

#include <PreInclude.h>

struct complx8 {   /* single precision */
    float r;
    float i;
};

struct complx16 {  /* double precision */
	double r;
	double i;
};

inline void LinFit(float *x, float *y, float *m, float *b, int n)
{
	float av_x, av_y;
	float L_xx, L_yy, L_xy;
	av_x = 0;
	av_y = 0;
	L_xx = 0;
	L_yy = 0;
	L_xy = 0;

	for (int i = 0; i < n; i++)
	{
		av_x += x[i] / n;
		av_y += y[i] / n;
	}

	for (int i = 0; i < n; i++)
	{
	   L_xx += (x[i] - av_x) * (x[i] - av_x);
	   L_yy += (y[i] - av_y) * (y[i] - av_y);
	   L_xy += (x[i] - av_x) * (y[i] - av_y);
	}

	*m = L_xy / L_xx;
	*b = av_y - L_xy * av_x / L_xx;
}

void rfft(float *x, int n, int m)
{
	int j, i, k, is, id;
	int i0, i1, i2, i3, i4, i5, i6, i7, i8;
	int n2, n4, n8;
	float xt, a0, e, a, a3;
	float t1, t2, t3, t4, t5, t6;
	float cc1, ss1, cc3, ss3;
	float *r0;

	j = 0;
	r0 = x;

	for( i = 0; i < n ; i ++)
		x[i] = x[i] / n;

	for (i = 0; i < n - 1; i++)
	{
		if (i < j)
		{
			xt = x[j];
			x[j] = *r0;
			*r0 = xt;
		}
		r0++;

		k = n >> 1;

		while (k <= j)
		{
			j = j - k;
			k >>= 1;
		}
		j += k;
	}

 /* Length two butterflies */
	is = 0;
	id = 4;

	while (is < n - 1)
	{
		for (i0 = is; i0 < n; i0 += id)
		{
			i1 = i0 + 1;
			a0 = x[i0];
			x[i0] += x[i1];
			x[i1] = a0 - x[i1];
		}
		is = (id << 1) - 2;
		id <<= 2;
	}

 /* L shaped butterflies */
	n2 = 2;
	for (k = 1; k < m; k++)
	{
		n2 <<= 1;
		n4 = n2 >> 2;
		n8 = n2 >> 3;
		e = (float) 6.2831853071719586f / n2;
		is = 0;
		id = n2 << 1;
		while (is < n)
		{
			for (i = is; i <= n - 1; i += id)
			{
				i1 = i;
				i2 = i1 + n4;
				i3 = i2 + n4;
				i4 = i3 + n4;
				t1 = x[i4] + x[i3];
				x[i4] -= x[i3];
				x[i3] = x[i1] - t1;
				x[i1] += t1;

				if (n4 != 1)
				{
					i1 += n8;
					i2 += n8;
					i3 += n8;
					i4 += n8;
					t1 = (x[i3] + x[i4]) * .7071067811865475244f;
					t2 = (x[i3] - x[i4]) * .7071067811865475244f;
					x[i4] = x[i2] - t1;
					x[i3] = -x[i2] - t1;
					x[i2] = x[i1] - t2;
					x[i1] = x[i1] + t2;
				}
			}
			is = (id << 1) - n2;
			id <<= 2;
		}

		for (j = 1; j < n8; j++)
		{
			a = j * e;
			a3 = 3 * a;
			cc1 = cos (a);
			ss1 = sin (a);
			cc3 = cos (a3);
			ss3 = sin (a3);

			is = 0;
			id = n2 << 1;

			while (is < n)
			{
				for (i = is; i <= n - 1; i += id)
				{
					i1 = i + j;
					i2 = i1 + n4;
					i3 = i2 + n4;
					i4 = i3 + n4;
					i5 = i + n4 - j;
					i6 = i5 + n4;
					i7 = i6 + n4;
					i8 = i7 + n4;
					t1 = x[i3] * cc1 + x[i7] * ss1;
					t2 = x[i7] * cc1 - x[i3] * ss1;
					t3 = x[i4] * cc3 + x[i8] * ss3;
					t4 = x[i8] * cc3 - x[i4] * ss3;
					t5 = t1 + t3;
					t6 = t2 + t4;
					t3 = t1 - t3;
					t4 = t2 - t4;
					t2 = x[i6] + t6;
					x[i3] = t6 - x[i6];
					x[i8] = t2;
					t2 = x[i2] - t3;
					x[i7] = -x[i2] - t3;
					x[i4] = t2;
					t1 = x[i1] + t5;
					x[i6] = x[i1] - t5;
					x[i1] = t1;
					t1 = x[i5] + t4;
					x[i5] = x[i5] - t4;
					x[i2] = t1;
				}
				is = (id << 1) - n2;
				id <<= 2;
			}
		}
	}
}


#endif
