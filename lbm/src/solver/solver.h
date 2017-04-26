#pragma once

#include"../phys_values/2d/distribution_func_2d.h"
#include"../phys_values/3d/distribution_func_3d.h"
#pragma region 2d



//! Weigth for probability distribution function calculation
const double kW[kQ]{ 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
		1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 };


/*
Directions in D2Q9 model
6   2   5
\  |  /
3 --  0  -- 1
/  |  \
7   4   8
*/

//! X-components witch determ particle movement
const double kEx[kQ]{ 0.0, 1.0, 0.0, -1.0, 0.0, 1.0, -1.0, -1.0, 1.0 };

//! Y-components witch determ particle movement
const double kEy[kQ]{ 0.0, 0.0, 1.0, 0.0, -1.0, 1.0, 1.0, -1.0, -1.0 };

//! Y-���������� ������������� ����������� ��������������� ������������ (�������� �� -1 ����� up = 0, boottom = rows)
//const double kEy[kQ]{ 0.0, 0.0, -1.0, 0.0, 1.0, -1.0, -1.0, 1.0, 1.0 };

/*!
Interface for different simulation aproaches:
- SRT
- MRT
- Immersed Boundary SRT
*/

#pragma endregion

#pragma region 3d

#include<vector>

// This is the vector of weights : initilize in feqCalculation() because it is too big
std::vector<double> w;

void FillWeightsFor3D(std::vector<double> & w)
{
	w.resize(kQ3d, 1.0 / 36.0);

	w.at(0) = 12.0 / 36.0;
	for (int i = 1; i <= 6; ++i)
		w.at(i) = 2.0 / 36.0;

}


// Directions in assordace with Dmitry Biculov article
const int ex[kQ3d] { 0, 1,  -1, 0,   0, 0,   0, 1,  -1, -1,   1, 1,   1, -1,  -1, 0,   0,  0,  0 };
const int ey[kQ3d] { 0, 0,   0, 1,  -1, 0,   0, 1,   1, -1,  -1, 0,   0, 0,    0, 1,  -1, -1,  1 };
const int ez[kQ3d] { 0, 0,   0, 0,   0, 1,  -1, 0,   0, 0,    0, 1,  -1, -1,   1, 1,   1, -1, -1 };

#pragma endregion



class iSolver
{
public:

	virtual ~iSolver() {}

#pragma region Methods

	//! Equilibrium probability distribution function calculation
	virtual void feqCalculate() = 0;

	//! Streaming of particles to neighbour nodes
	virtual void streaming() = 0;
	//! Collision of particles in nodes
	virtual void collision() = 0;

	//! Solver for modeling procedure
	virtual void solve(int iteration_number) = 0;

	//! Recalculation procedure (recalculate density, velocity)
	virtual void recalculate() = 0;

#pragma endregion

private:

};
