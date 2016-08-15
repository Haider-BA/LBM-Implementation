#pragma once

#include <type_traits>

#include"..\math\my_matrix.h"

/*!
	��� ������: ��������, ��� ������ �� ������� ������� �������������.
	� ���������� ���������� ��� ���� ����������� �������.

	���������� ��� enum class ����� �������� ��������� ��������� ����.
*/
enum class NodeType : int 
{
	FLUID				= 0,
	UPPER_BOUNDARY		= 1,
	BOTTOM_BOUNDARY		= 2,
	LEFT_BOUNDARY		= 3,
	RIGHT_BOUNDARY		= 4,
};

/*!
	����� ����������� ������� �������������.

	������������ ����� ������� NodeType-�� � ������� �������� ���������� � ���� ������.
*/
class Medium
{
public:
	Medium();
	Medium(unsigned rows, unsigned colls);
	~Medium();
	
	bool is_fluid(unsigned y, unsigned x) const;
	// �������� ����������� ����������
	/*bool is_upper_boundary(unsigned y, unsigned x) const;
	bool is_bottom_boundary(unsigned y, unsigned x) const;
	bool is_left_boundary(unsigned y, unsigned x) const;
	bool is_right_boundary(unsigned y, unsigned x) const;*/

	void resize(unsigned rows, unsigned colls);

	friend std::ostream & operator<<(std::ostream & os, Medium const & medium);

	// ���������� ����� ����� size() ������������� � ������ Matrix<>
	std::pair<unsigned int, unsigned int> size() const;

private:
	unsigned rows_;
	unsigned colls_;

	Matrix<NodeType> medium_;
};
