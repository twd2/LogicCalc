#pragma once
class Matrix
{
public:
	Matrix(size_t, size_t);
	~Matrix();
	

	size_t m, n;
	int *operator[] (size_t i);

private:
	int *data = NULL;

	Matrix(const Matrix& mat);
};

