#pragma once

#include"distribution_func.h"

template<typename T>
inline DistributionFunction<T>::DistributionFunction() : rows_(0), colls_(0) {
#pragma omp parallel for
	for (int q = 0; q < kQ; ++q)
		dfunc_body_.at(q).resize(rows_, colls_);
}

template<typename T>
DistributionFunction<T>::DistributionFunction(unsigned rows, unsigned colls): rows_(rows), colls_(colls) {

#pragma omp parallel for
	for (int q = 0; q < kQ; ++q)
		dfunc_body_.at(q).resize(rows_, colls_);
}

template<typename T>
DistributionFunction<T>::~DistributionFunction() {}

template<typename T>
inline DistributionFunction<T>::DistributionFunction(DistributionFunction const & other) :
	rows_(other.rows_), colls_(other.colls_)
{
#pragma omp parallel for
	for (int q = 0; q < kQ; ++q) {
		dfunc_body_.at(q).resize(rows_, colls_);
		dfunc_body_.at(q) = other.dfunc_body_.at(q);
	}
}

template<typename T>
inline void DistributionFunction<T>::swap(DistributionFunction & dist_func)
{
	std::swap(rows_, dist_func.rows_);
	std::swap(colls_, dist_func.colls_);

#pragma omp parallel for
	for (int q = 0; q < kQ; ++q)
		dfunc_body_.at(q).swap(dist_func.dfunc_body_.at(q));
}

template<typename T>
inline Matrix<T>& DistributionFunction<T>::operator[](unsigned q)
{
	assert(q < kQ);
	return dfunc_body_.at(q);
}

template<typename T>
inline void DistributionFunction<T>::fillWithoutBoundaries(T const value)
{
#pragma omp parallel for
	for (int q = 0; q < kQ; ++q)
		dfunc_body_.at(q).fillWith(value);
}

template<typename T>
inline void DistributionFunction<T>::fillBoundaries(T const value)
{
//#pragma omp parallel for
	for (int q = 1; q < kQ; ++q) {	// �������� � 1 ��� ��� f[0] ������ �� ���������
		switch (q)
		{
		case 1:
			dfunc_body_.at(1).fillColumnWith(colls_ - 1, value);
			break;
		case 2:
			dfunc_body_.at(2).fillRowWith(0, value);
			break;
		case 3:
			dfunc_body_.at(3).fillColumnWith(0, value);
			break;
		case 4:
			dfunc_body_.at(4).fillRowWith(rows_ - 1, value);
			break;
		case 5:
			dfunc_body_.at(5).fillColumnWith(colls_ - 1, value);
			dfunc_body_.at(5).fillRowWith(0, value);
			break;
		case 6:
			dfunc_body_.at(6).fillColumnWith(0, value);
			dfunc_body_.at(6).fillRowWith(0, value);
			break;
		case 7:
			dfunc_body_.at(7).fillColumnWith(0, value);
			dfunc_body_.at(7).fillRowWith(rows_ - 1, value);
			break;
		case 8:
			dfunc_body_.at(8).fillColumnWith(colls_ - 1, value);
			dfunc_body_.at(8).fillRowWith(rows_ - 1, value);
			break;
		default:
			break;
		}
	}
}

template<typename T>
inline void DistributionFunction<T>::resize(unsigned rows, unsigned colls)
{
	rows_ = rows;
	colls_ = colls;

#pragma omp parallel for
	for (int q = 0; q < kQ; ++q)
		dfunc_body_.at(q).resize(rows_, colls_);
}

template<typename T>
inline std::pair<unsigned int, unsigned int> DistributionFunction<T>::size() const
{
	return std::make_pair(rows_, colls_);
}

template<typename T>
inline std::vector<T> DistributionFunction<T>::getTopBoundaryValues(int const q) const
{
	return dfunc_body_.at(q).getRow(1);
}

template<typename T>
inline std::vector<T> DistributionFunction<T>::getBottomBoundaryValue(int const q) const
{
	return dfunc_body_.at(q).getRow(rows_ - 2);
}

template<typename T>
inline std::vector<T> DistributionFunction<T>::getLeftBoundaryValue(int const q) const
{
	return dfunc_body_.at(q).getColumn(1);
}

template<typename T>
inline std::vector<T> DistributionFunction<T>::getRightBoundaryValue(int const q) const
{
	return dfunc_body_.at(q).getColumn(colls_ - 2);
}

template<typename T>
inline void DistributionFunction<T>::setTopBoundaryValue(int const q, std::vector<T> const & row)
{
	dfunc_body_.at(q).setRow(1, row);
}

template<typename T>
inline void DistributionFunction<T>::setBottomBoundaryValue(int const q, std::vector<T> const & row)
{
	dfunc_body_.at(q).setRow(rows_ - 2, row);
}

template<typename T>
inline void DistributionFunction<T>::setLeftBoundaryValue(int const q, std::vector<T> const & coll)
{
	dfunc_body_.at(q).setColumn(1, coll);
}

template<typename T>
inline void DistributionFunction<T>::setRightBoundaryValue(int const q, std::vector<T> const & coll)
{
	dfunc_body_.at(q).setColumn(colls_ - 2, coll);
}

template<typename T>
inline MacroscopicParam<T> DistributionFunction<T>::calculateDensity() const
{
	MacroscopicParam<T> result(rows_, colls_);
	result.fillWith(0.0);

#pragma omp parallel for
	for (int q = 0; q < kQ; ++q)
		result += dfunc_body_.at(q);

	return result;
}

template<typename T>
inline MacroscopicParam<T> DistributionFunction<T>::calculateVelocity(const double mas[kQ], MacroscopicParam<T> const & density) const
{
	MacroscopicParam<T> result(rows_, colls_);
	result.fillWith(0.0);

#pragma omp parallel for
	for (int q = 0; q < kQ; ++q)
		result += dfunc_body_.at(q) * mas[q];

	// ���������� ����� � return, �� ������ ����������� �� �������� error
	result.times_divide(density);
	return result;
	
}

template<typename T1>
std::ostream & operator<<(std::ostream & os, DistributionFunction<T1> const & dist_func) {
	unsigned i{ 0 };
	for (auto matrix : dist_func.dfunc_body_) {
		os << "Distribution func f[" << i++ << "] ------------------- \n";
		os << matrix;
	}

	return os;
}