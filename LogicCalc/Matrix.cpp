#include "stdafx.h"
#include "Matrix.h"


Matrix::Matrix(int m, int n) :m(m), n(n)
{
	data = new int[m*n];
}


Matrix::~Matrix()
{
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}
}

//Matrix::Matrix(const Matrix& mat) :m(mat.m), n(mat.n)
//{
//	data = new int[m*n];
//	memcpy(data, mat.data, sizeof(int)*m*n);
//}

int *Matrix::operator[] (int i)
{
	return data + i*n;
}