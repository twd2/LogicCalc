#pragma once
class Matrix
{
public:
	Matrix(int, int);
	~Matrix();
	

	int m, n;
	int *operator[] (int i);

private:
	int *data = NULL;

	Matrix(const Matrix& mat);
};

