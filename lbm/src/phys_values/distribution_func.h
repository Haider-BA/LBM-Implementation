#pragma once

#include<array>

#include"..\math\my_matrix.h"
#include"..\phys_values\macroscopic_param.h"

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

#pragma region opeartors

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

#pragma endregion

#pragma region function

	//! ���������� q-�� ���������� ������� �������������
	Matrix<T> & operator[](unsigned q);

	//! ���������� ��� �������� �������� ������� ������������� ����� ��������� �� ������� ��������� value
	void fill(T const value);

	//! ��������� ������� ��� ������ �� kQ ������� ������������� ���������� value
	void fill_boundaries(T const value);

	//! �������� ������ ������ �� ��������� ������� ������������� �� rows � colls ��������������
	void resize(unsigned rows, unsigned colls);

	//! ���������� ���� � ������� first = rows_, second = colls_
	std::pair<unsigned int, unsigned int> size() const;

	//! ���������� �������� ������� ������������� �� ������� ������
	std::vector<T> get_top_boundary_val(int const q) const;	
	//! ���������� �������� ������� ������������� �� ������ ������
	std::vector<T> get_bottom_boundary_val(int const q) const;
	//! ���������� �������� ������� ������������� �� ����� ������
	std::vector<T> get_left_boundary_val(int const q) const;
	//! ���������� �������� ������� ������������� �� ������ ������
	std::vector<T> get_right_boundary_val(int const q) const;
	
	//! �������� �������� ������� ������������� �� ������� ������ ������� �������� ������ colls
	void set_top_boundary_value(int const q, std::vector<T> const & row);
	//! �������� �������� ������� ������������� �� ������ ������ ������� �������� ������ colls
	void set_bottom_boundary_value(int const q, std::vector<T> const & row);
	//! �������� �������� ������� ������������� �� ����� ������ ������� �������� ������ colls
	void set_left_boundary_value(int const q, std::vector<T> const & coll);
	//! �������� �������� ������� ������������� �� ������ ������ ������� �������� ������ colls
	void set_right_boundary_value(int const q, std::vector<T> const & coll);

	//! ������� ��������� � ����� �� ����� �������
	MacroscopicParam<T> get_density() const;
	//! ������� �������� � ����� �� ����� �������
	MacroscopicParam<T> get_velocity(const double mas[kQ], MacroscopicParam<T> const & density) const;

#pragma endregion

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