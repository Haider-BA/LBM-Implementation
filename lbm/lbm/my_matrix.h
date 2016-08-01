#pragma once

#include<vector>
#include <functional>
#include<ostream>

#include <cassert> // setw()


#include<omp.h>

//#define TEST_INCLUDE 

#ifndef TEST_INCLUDE 
 	#include<cstdlib>
 	#include<ctime>
 	#include<iostream>
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
	// ��������� ������� ��� �����������
	virtual ~Matrix();

	Matrix(Matrix<T> const & other);	// ��������� ��� �� ������ ��� ������ ������� ������

	void swap(Matrix<T> & other);

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

	/*! 
		��������� ������������ ���� ������ �� ���� �������������� ������� ������ �������
		���������� �� ��������������� ������� ������ �������.
	*/
	Matrix<T> scalar_multiplication(Matrix<T> const & other);


	T & operator()(unsigned y, unsigned x) {
		return body_.at(y * colls_ + x);
	}

	T const operator()(unsigned y, unsigned x) const {
		return body_.at(y * colls_ + x);
	}

	//! ���������� ���� � ������� first = rows_, second = colls_
	std::pair<unsigned int, unsigned int> size() const;

	//! ���������� ����� ��������� �������
	long double get_sum() const;

	//! ���������� �������, � ������� �������� ��� �������� ������ � ������� y
	Matrix<T> get_row(unsigned const y) const;

	//! ������ �������� ������� ������ y ������� ��������� ������� row
	void set_row(unsigned const y, Matrix<T> const & row);

	//! ���������� �������, � ������� �������� ��� �������� ��������� [1 : rows_ - 2] ������� � ������� x
	Matrix<T> get_coll(unsigned const x) const;

	//! ������ �������� ��������� [1 : colls - 1] ������� ������� x ������� ��������� ������� coll
	void set_coll(unsigned const x, Matrix<T> const & coll);

	//! ��������� ������� ������� ��������� value
	void fill_with(T const value);

	//! ��������� ������� ��� ��������� ��������� ���������� value
	void fill_withought_boundary(T const value);

	//! �������� ������� ������� �� rows � colls ��������������, �������� ��� ���� ������
	void resize(unsigned rows, unsigned colls);

	template<typename T1>
	friend std::ostream & operator<<(std::ostream & os, Matrix<T1> const & matrix);

protected:
	unsigned rows_;
	unsigned colls_;

	std::vector<T> body_;
};

#include"my_matrix_impl.h"
