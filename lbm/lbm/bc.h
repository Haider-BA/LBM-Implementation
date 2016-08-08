#pragma once

#include"fluid.h"
#include<memory>

#include<map>

//! ��������� �� DistriputionFunction
typedef std::unique_ptr<DistributionFunction<double>> distr_func_ptr;

enum class BCType {
	PERIODIC,
	BOUNCE_BACK,
	VON_NEUMAN,
};

enum class Boundary {
	TOP,
	BOTTOM,
	RIGHT,
	LEFT,
};

typedef std::pair<Boundary, BCType> Wall_info;

class BCs
{
	friend class Fluid;
public:
	BCs(unsigned rows, unsigned colls, DistributionFunction<double> & dfunc);
	~BCs();
	
	/*!
		���������� ����������� ��� ���������� boundary_condition_type �� ������� �������������
		� ��������������� BC ���� ������.

		������: get_values(TOP, PERIODIC) - �������� top_boundary_ 2, 5, 6 ����������� � �������
		�������.
	*/ 
	bool get_values(Boundary const BC, BCType const boundary_condition_type);
	
	//! �������������� ��� �������� ��� ����������� �������� ��
	//! ������� ���������� ���� ����������� (TOR, BOTTOM, LEFT, RIGHT)
	void prepair_bc_values(BCType const top_bc, BCType const bottm_bc, BCType const left_bc, BCType const right_bc);

	//! ��������� ��������������� ���������� ������� ������������� ������������ � BC ����������
	void set_values(Boundary const BC, BCType const boundary_condition_type);
	
	//! 
	void recording_bc_values(BCType const top_bc, BCType const bottm_bc, BCType const left_bc, BCType const right_bc);

#pragma region different_BC_implementation
	//! ������������� ��
	void periodic_bc(Boundary const first, Boundary const second);
	//! �� ���� �������
	void bounce_back_bc(Boundary const first);
	//! �� ���� ���-������� (���������� ����� ����� �����������)
	void von_neuman_bc(Boundary const first, Fluid & fluid, double const v);

	void apply_bc(BCType const top_bc, BCType const bottm_bc, BCType const left_bc, BCType const right_bc);

#pragma endregion

	friend std::ostream & operator<<(std::ostream & os, BCs const & BC);

private:
	//! ������ � std::map<> first �������� � from �� to �� ������� second
	void swap_id(std::map<int, std::vector<double> > & map, int const from, int const to);

private:
	//! ����� ����� � �������, �������� �������� �� ������� � ������ ������ [= rows_ �������]
	unsigned length_;
	//! ����� ����� � �������, �������� �������� �� ������ � ����� ������ [= colls_ - 2 �������]
	unsigned height_;

	//! ��������� �� ����������� � Fluid ������� �������������, ����� �������� � ���������
	DistributionFunction<double>* f_ptr_;

	//! ������ ������ ���������� ������� ������������� � �� �������� �� ������� ������
	//!	������: top_boundary_[1] = { �������� f[1] �� ������� ������� }
	std::map<int, std::vector<double> > top_boundary_;
	//! ������ ������ ���������� ������� ������������� � �� �������� �� ������ ������
	std::map<int, std::vector<double> > bottom_boundary_;
	//! ������ ������ ���������� ������� ������������� � �� �������� �� ����� ������
	std::map<int, std::vector<double> > left_boundary_;
	//! ������ ������ ���������� ������� ������������� � �� �������� �� ������ ������
	std::map<int, std::vector<double> > right_boundary_;
};
