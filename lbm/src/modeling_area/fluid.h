#pragma once

#include"..\phys_values\macroscopic_param.h"
#include"..\phys_values\distribution_func.h"
#include"medium.h"

class SRTsolver;

/*!
	Stores all parameters for fluid describing, i.e:
		- Fluid density field
		- Fluid velocity field (Two components)
		- Probability distribution function field
		- Equilibrium probability distribution function field
*/
class Fluid
{
	friend class SRTsolver;

public:

#pragma region Constructor

	Fluid(int rows, int colls);
	~Fluid();

#pragma endregion

#pragma region Properties(Get/Set)

	std::pair<int, int> size() const;

#pragma endregion

#pragma region Methods

	void Poiseuille_IC(double const dvx);

#pragma endregion

private:

#pragma region Fields

	//! Rows count for fluid modeling area
	int rows_;
	//! Columns count for fluid modeling area
	int colls_;

// ������������������������ - ��� ���� ������ ��� ����������� ����� �������� f_ �� �������� ��� BCs
public:
	//! Fluid density field
	MacroscopicParam<double> rho_;
	//! Fluid velocity X-component field
	MacroscopicParam<double> vx_;
	//! Fluid velocity Y-component field
	MacroscopicParam<double> vy_;

	//! Probability distribution function field
	DistributionFunction<double> f_;
	//! Equilibrium probability distribution function field
	DistributionFunction<double> feq_;

#pragma endregion

};
