#pragma once

#include<memory>


#include"solver.h"
#include"..\modeling_area\fluid.h"
#include"..\modeling_area\medium.h"
#include"bc\bc.h"



/*!
	����� ����������� ��������� ��� ������� �������� � ������ SRT �������.

	����� ���������� \tau >= 0.5 ��� ����������. ����� �������� ����� 1.0
*/
class SRTsolver : iSolver
{
public:
	SRTsolver(double const tau, Medium & medium, Fluid & fluid);
	virtual ~SRTsolver() {}

	virtual void feq_calculate();

	virtual void streaming();
	virtual void collision();

	virtual void solve(int iteration_number);
	virtual void recalculate();

private:
	//! ����� ����������
	double const tau_;

	Medium* medium_;
	Fluid* fluid_;
};

