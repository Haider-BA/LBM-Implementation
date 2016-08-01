#pragma once

#include"macroscopic_param.h"
#include"distribution_func.h"
#include"medium.h"

class SRTsolver;

/*!
	����� ������� ������ � ���� ��� ��������������� ��������:
	 - ���������
	 - 2-� ���������� ��������
	 - ������� ������������� kQ ���������
	 - ����������� ������� ������������� kQ ���������
*/
class Fluid
{
	friend class SRTsolver;
public:
	Fluid(unsigned rows, unsigned colls);
	~Fluid();

	void Poiseuille_IC(double const dvx);

	std::pair<unsigned, unsigned> size() const;

private:
	unsigned rows_;
	unsigned colls_;

// ������������������������ - ��� ���� ������ ��� ����������� ����� �������� f_ �� �������� ��� BCs
public:
	MacroscopicParam<double> rho_;
	MacroscopicParam<double> vx_;
	MacroscopicParam<double> vy_;


	DistributionFunction<double> f_;
	DistributionFunction<double> feq_;
};
