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
	UP,
	BOTTOM,
	RIGHT,
	LEFT,
};

class BCs
{
	friend class Fluid;
public:
	BCs(unsigned rows, unsigned colls, DistributionFunction<double> & dfunc);
	~BCs();

	//! �����, ����������� ���� ������ BCs ���������������� ���������� ������� �������������
	bool get_boundady_values();

	/*!
		������������� ��������� �������.

		���������� ��� ��, ��� ������� ������ �� kQ �������� �������� ��� ���������� ������� �������
	������������� ������������ �� ��������������� �������.

	������: ��� ����������� ������ ������ ���������� f[1], f[5], f[8] ��������� � ��-�� ����������
	�� ����� ������� � f[3], f[6], f[7] ����� ������� ��������� � ��-�� ���������� ������
	�������.

	*/
	void set_periodic_bc(Boundary first, Boundary second);

	/*!
		��������� ������� ���� �������.

		�� ��� ������� ������ �� kQ ��������� ������� ������������� ��� ���������� ������� �������
		������������ ���������� �� ���������������.

		������: �� ������ ������� f[1] ��������� � f[3], f[5]->f[6], f[8]->f[7] �� ���-�� �������.
	*/
	void set_bounce_back_bc(Boundary first);

	/*
		��������� ������� ���-�������.

		�� ��� ������� ����������� ���������� �������� ������ �� ��������� �����������. ��������
		������ v ���������� � ������� ��������� �������. 

		������� �������� Fluid �� ������, ��� ��� ��� ���� ���� �� ���������� ��������� � ��������.
		�� ������ ������ �� ������������ ������ ��� ����� ������.
	*/
	void set_von_neumann_bc(Boundary first, Fluid & fluid, double const v);

	friend std::ostream & operator<<(std::ostream & os, BCs const & BC);
private:
	//! �����, ����������� ���� �� ����� ������ BCs ���������������� ���������� ������� ������������� 
	bool get_values(Boundary BC);

private:
	//! ����� ����� � �������, �������� �������� �� ������� � ������ ������ [= rows_ �������]
	unsigned length_;
	//! ����� ����� � �������, �������� �������� �� ������ � ����� ������ [= colls_ - 2 �������]
	unsigned height_;

	//! ��������� �� ����������� � Fluid ������� �������������, ����� �������� � ���������
	distr_func_ptr f_ptr_;

	/*!
		������ ������ � ������� � q-�� ������ ����� q-�� ������� ������������� �� ������� ������

		f[0]( �� ������� ������� )
		f[1]( �� ������� ������� )
		...
		f[8]( �� ������� ������� )

	*/
	std::array<Matrix<double>, kQ> up_boundary_;

	std::map<unsigned, Matrix<double> > top_;
	

	//!	������ ������ � ������� � q-�� ������ ����� q-�� ������� ������������� �� ������ ������	
	std::array<Matrix<double>, kQ> bottom_boundary_;
	
	//!	������ ������ � ������� � q-�� ������ ����� q-�� ������� ������������� �� ������ ������
	std::array<Matrix<double>, kQ> right_boundary_;

	//!	������ ������ � ������� � q-�� ������ ����� q-�� ������� ������������� �� ����� ������	
	std::array<Matrix<double>, kQ> left_boundary_;

};
