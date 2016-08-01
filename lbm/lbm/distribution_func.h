#pragma once

#include<array>

#include"my_matrix.h"


/*!
	��������� ����� �����������, � ������� ����� ������������
	�������������. ������� �� ����������� ������ D2Q3 D2Q9.
*/
unsigned const kQ{ 9 };

/*!
	������� ������������� ��������� ����������� f.
		- ������ ������� kQ � ������ ������ �������� �������� �������� ������� ������������� ��� q = 0...kQ
		  �����������.
*/
template<typename T>
class DistributionFunction
{
public:
	DistributionFunction();
	DistributionFunction(unsigned rows, unsigned colls);
	~DistributionFunction();
	
	DistributionFunction(DistributionFunction<T> const & other);
	
	void swap(DistributionFunction & dist_func);

	DistributionFunction<T> & operator=(DistributionFunction<T> const & other) {
		assert(rows_ == other.rows_ && colls_ == other.colls_);
		if (this != &other) {
			DistributionFunction<T> temp(other);
			temp.swap(*this);
		}
		return *this;
	}

	DistributionFunction<T> & operator+=(DistributionFunction<T> const & other) {
	#pragma omp parallel for
		for (int q = 0; q < kQ; ++q)
			dfunc_body_.at(q) += other.dfunc_body_.at(q);

		return *this;
	}

	DistributionFunction<T> operator+(DistributionFunction<T> const & other) {
		assert(rows_ == other.rows_ && colls_ == other.colls_);

		DistributionFunction<T> result(*this);
	#pragma omp parallel for
		for (int q = 0; q < kQ; ++q)
			result.dfunc_body_.at(q) += other.dfunc_body_.at(q);

		return result;
	}


	DistributionFunction<T> & operator-=(DistributionFunction<T> const & other) {
	#pragma omp parallel for
		for (int q = 0; q < kQ; ++q)
			dfunc_body_.at(q) -= other.dfunc_body_.at(q);

		return *this;
	}

	DistributionFunction<T> operator-(DistributionFunction<T> const & other) {
		assert(rows_ == other.rows_ && colls_ == other.colls_);

		DistributionFunction<T> result(*this);
	#pragma omp parallel for
		for (int q = 0; q < kQ; ++q)
			result.dfunc_body_.at(q) -= other.dfunc_body_.at(q);

		return result;
	}


	DistributionFunction<T> & operator/=(T const & other) {
	#pragma omp parallel for
		for (int q = 0; q < kQ; ++q)
			dfunc_body_.at(q) /= other;

		return *this;
	}

	DistributionFunction<T> operator/(T const & other) {
		
		DistributionFunction<T> result(*this);
	#pragma omp parallel for
		for (int q = 0; q < kQ; ++q)
			result.dfunc_body_.at(q) /= other;

		return result;
	}

	//! ���������� q-�� ���������� ������� �������������
	Matrix<T> & operator[](unsigned q);

	//! ���������� ��� �������� �������� ������� ������������� ����� ��������� �� ������� ��������� value
	void fill(T const value);

	//! �������� ������ ������ �� ��������� ������� ������������� �� rows � colls ��������������
	void resize(unsigned rows, unsigned colls);


	/*! 
		� q-y� ������ ������������ ������� ���������� �������� q-�� ���������� ������� ������������� 
		����������� �� ������� �������.
	*/
	std::array<Matrix<T>, kQ> get_values_on_upper_boundary() const;

	/*!
	� q-y� ������ ������������ ������� ���������� �������� q-�� ���������� ������� �������������
	����������� �� ������ �������.
	*/
	std::array<Matrix<T>, kQ> get_values_on_bottom_boundary() const;

	/*!
	� q-y� ������ ������������ ������� ���������� �������� q-�� ���������� ������� �������������
	����������� �� ����� �������.
	*/
	std::array<Matrix<T>, kQ> get_values_on_left_boundary() const;

	/*!
	� q-y� ������ ������������ ������� ���������� �������� q-�� ���������� ������� �������������
	����������� �� ������ �������.
	*/
	std::array<Matrix<T>, kQ> get_values_on_right_boundary() const;


	template<typename T1>
	friend std::ostream & operator<<(std::ostream & os, DistributionFunction<T1> const & distr_func);

private:
	unsigned rows_;
	unsigned colls_;
	
	/*!
		������ �������� ������� ������������� ��������� ����������� ����� q = 1...kQ ����������� ��������
		�������������.

		���������� ��� ������ ������ �������������� ������ kQ.
	*/
	std::array<Matrix<T>, kQ> dfunc_body_;
};


#include"distribution_func_impl.h"