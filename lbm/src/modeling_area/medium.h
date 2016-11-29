#pragma once

#include <type_traits>

#include"..\math\my_matrix_2d.h"

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

/// <summary>
/// ����� ����������� ������� �������������.

/// ������������ ����� ������� NodeType - �� � ������� �������� ���������� � ���� ������.
/// </summary>
class Medium
{
public:
	Medium();
	Medium(int rows, int colls);
	~Medium();
	
	bool is_fluid(int y, int x) const;
	// �������� ����������� ����������
	/*bool is_upper_boundary(int y, int x) const;
	bool is_bottom_boundary(int y, int x) const;
	bool is_left_boundary(int y, int x) const;
	bool is_right_boundary(int y, int x) const;*/

	/// <summary>
	/// Resize current Medium with values !!!!!
	/// </summary>
	/// <param name="rows"> lol </param>
	/// <param name="colls"> hah </param>
	void resize(int rows, int colls);

	friend std::ostream & operator<<(std::ostream & os, Medium const & medium);

	// ���������� ����� ����� size() ������������� � ������ Matrix<>
	std::pair<int, int> size() const;

private:
	/// <summary>
	/// Rows in modaling area
	/// </summary>
	int rows_;
	/// <summary>
	/// Columns in modaling area
	/// </summary>
	int colls_;

	Matrix2D<NodeType> medium_;
};
