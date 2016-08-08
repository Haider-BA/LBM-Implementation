#pragma once

#include"my_matrix.h"
/*!
	����� ��� ���������� ���������������� ����������, ����� ��� ���������, ��������.

	���������� �� ������ ������ �������, �� ���� ������������ �� ���� ���� ���������,
	� �������� ��������������.
*/
template<typename T>
class MacroscopicParam : public Matrix<T>
{
public:
	MacroscopicParam();
	MacroscopicParam(unsigned rows, unsigned colls);
	virtual ~MacroscopicParam() {}

private:

};

#include"macroscopic_param_impl.h"