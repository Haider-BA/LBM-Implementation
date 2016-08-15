#pragma once

#include<iostream>
#include<ostream>
#include <cassert> // setw()
#include<vector>
#include <functional>
#include<string>
#include<omp.h>

#define TEST_INCLUDE 

#ifndef TEST_INCLUDE 
 	#include<cstdlib>
 	#include<ctime>
#endif // !TEST_INCLUDE


/*!
	����� ������� �� ���� ������� ����� ������������ ���������� ��������, ����� ��� ���������, ��������, 
	������� ������������� ��������� �����������.

	���������� ������������� ������ ��� ����������, ������ ����������� � ������.

*/
template<typename T>
class Matrix
{
public:
	Matrix();
	Matrix(unsigned rows, unsigned colls);
	virtual ~Matrix();

	Matrix(Matrix<T> const & other);	// ��������� ��� �� ������ ��� ������ ������� ������

	void swap(Matrix<T> & other);

#pragma region operator+

	Matrix<T> & operator=(Matrix<T> const & other) 
	{
		assert(rows_ == other.rows_ && colls_ == other.colls_);
		if (this != & other) {
			Matrix<T> temp(other);
			temp.swap(*this);
		}

		return *this;
	}

	Matrix<T> & operator+=(Matrix<T> const & other) {
		assert(rows_ == other.rows_ && colls_ == other.colls_);
	#pragma omp parallel for
			for (int i = 0; i < body_.size(); ++i)
				body_.at(i) += other.body_.at(i);

		return *this;
	}

	Matrix<T> & operator+=(T const other) {
		std::for_each(body_.begin(), body_.end(),
			[&](T & value) {value += other; });

		return *this;
	}

	Matrix<T> operator+(Matrix<T> const & other) const {
		assert(rows_ == other.rows_ && colls_ == other.colls_);

		Matrix<T> result(*this);
	#pragma omp parallel for
		for (int i = 0; i < result.body_.size(); ++i)
			result.body_.at(i) += other.body_.at(i);

		return result;
	}

	Matrix<T> operator+(T const other) const {
		Matrix<T> result(*this);

		std::for_each(result.body_.begin(), result.body_.end(),
			[&](T & value) {value += other; });

		return result;
	}

	template<typename T1>
	friend Matrix<T1> operator+(T1 const left, Matrix<T1> const & right);

#pragma endregion

#pragma region operator-

	Matrix<T> & operator-=(Matrix<T> const & other) {
		assert(rows_ == other.rows_ && colls_ == other.colls_);
	#pragma omp parallel for
			for (int i = 0; i < body_.size(); ++i)
				body_.at(i) -= other.body_.at(i);

		return *this;
	}

	Matrix<T> & operator-=(T const other) {
		std::for_each(body_.begin(), body_.end(),
			[&](T & value) {value -= other; });

		return *this;
	}

	Matrix<T> operator-(Matrix<T> const & other) const {
		assert(rows_ == other.rows_ && colls_ == other.colls_);

		Matrix<T> result(*this);
	#pragma omp parallel for
		for (int i = 0; i < result.body_.size(); ++i)
			result.body_.at(i) -= other.body_.at(i);

		return result;
	}

	Matrix<T> operator-(T const other) const {
		Matrix<T> result(*this);

		std::for_each(result.body_.begin(), result.body_.end(),
			[&](T & value) {value -= other; });

		return result;
	}

#pragma endregion

#pragma region operator*

	Matrix<T> & operator*=(T other) {
	#pragma omp parallel for
		for (int i = 0; i < body_.size(); ++i)
			body_.at(i) *= other;

		return *this;
	}

	Matrix<T> operator*(T other) const {
		Matrix<T> result(*this);

		std::for_each(result.body_.begin(), result.body_.end(),
			[&](T & value) {value *= other; });

		return result;
	}

	template<typename T1>
	friend Matrix<T1> operator*(T1 const left, Matrix<T1> const & right);

	/*!
	��������� ������������ ���� ������ �� ���� �������������� ������� ������ �������
	���������� �� ��������������� ������� ������ �������.
	*/
	Matrix<T> scalar_multiplication(Matrix<T> const & other);

#pragma endregion

#pragma region operator/

	Matrix<T> & operator/=(T other) {
	#pragma omp parallel for
		for (int i = 0; i < body_.size(); ++i)
			body_.at(i) /= other;

		return *this;
	}

	Matrix<T> operator/(T other) const {
		Matrix<T> result(*this);

		std::for_each(result.body_.begin(), result.body_.end(),
			[&](T & value) {value /= other; });

		return result;
	}

	//! ��������� ������� �� ������� � ���������
	Matrix<T> times_divide(Matrix<T> const & other);

#pragma endregion

	T & operator()(unsigned y, unsigned x) {
		return body_.at(y * colls_ + x);
	}

	T const operator()(unsigned y, unsigned x) const {
		return body_.at(y * colls_ + x);
	}

#pragma region functions

	//! ���������� ���� � ������� first = rows_, second = colls_
	std::pair<unsigned int, unsigned int> size() const;

	//! ���������� ����� ��������� �������
	long double get_sum() const;

	//! ���������� std::vector<T>, � ������� �������� ��� �������� ������ � ������� y
	std::vector<T> get_row(unsigned const y) const;

	//! ������ �������� ������� ������ y ������� ��������� std::vector<T> row
	void set_row(unsigned const y, std::vector<T> const & row);

	//! ���������� std::vector<T>, � ������� �������� ��� �������� ��������� [1 : rows_ - 2] ������� � ������� x
	std::vector<T> get_coll(unsigned const x) const;

	//! ������ �������� ��������� [1 : colls - 1] ������� ������� x ������� ��������� std::vector<T> coll
	void set_coll(unsigned const x, std::vector<T> const & coll);

	//! ��������� ������� ������� ��������� value
	void fill_with(T const value);

	//! ��������� ������� ��� ��������� ��������� ���������� value
	void fill_withought_boundary(T const value);

	//! ��������� ������� ����� coll_id ������� ���������� value
	void fill_coll_with(int const coll_id, T const value);

	//! ��������� ������ ����� row_id ������� ���������� value
	void fill_row_with(int const row_id, T const value);

	//! �������� ������� ������� �� rows � colls ��������������, �������� ��� ���� ������
	void resize(unsigned rows, unsigned colls);

#pragma endregion

#pragma ostream_functions

	//! ������� ������ ���� ������� � ����
	void to_file(std::string value_name, int const time);

	//! �������� ������� ����� coll_id � ����
	void coll_to_file(std::string value_name, int const coll_id, int const time);

	//! �������� ������� ����� coll_id � ����
	void row_to_file(std::string value_name, int const row_id, int const time);

#pragma endregion

	template<typename T1>
	friend std::ostream & operator<<(std::ostream & os, Matrix<T1> const & matrix);

protected:
	unsigned rows_;
	unsigned colls_;

	std::vector<T> body_;
};

#pragma region additional_functions
	
#pragma endregion


#include"my_matrix_impl.h"
