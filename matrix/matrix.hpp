#pragma once
#include <iomanip>
#include <sstream>
#include <iostream>
namespace linalg {

#define index(i, j) ((m_cols * (i)) + (j))
#define double_cycle(x, y) for (size_t i = 0; i < (x); i++) for (size_t j = 0; j < (y); j++)

template <typename T>
Matrix<T>::Matrix()
{
	m_rows = 0;
	m_cols = 0;
	m_ptr = nullptr;
	m_capacity = 0;
}
// 
template <typename T>
Matrix<T>::Matrix(size_t rows, size_t columns)
{
	m_rows = rows;
	m_cols = columns;
	m_capacity = rows * columns;
	m_ptr = new T[m_rows * m_cols];
}
// Заполняет все ячейки матрицы заданным значением defaultvalue. привет numpy.zeroes((row, col))
template <typename T>
Matrix<T>::Matrix(size_t rows, size_t columns, T defaultvalue)
{
	m_rows = rows;
	m_cols = columns;
	m_ptr = new T[m_rows * m_cols];
	m_capacity = rows * columns;

	for (size_t i = 0; i < m_rows * m_cols; i++)
	{
		m_ptr[i] = defaultvalue;
	}
}
// Конструктор копирования
template <typename T>
Matrix<T>::Matrix(const Matrix& other)
{
	// copy_constructor(other);
	m_rows = other.rows();
	m_cols = other.columns();
	m_capacity = m_rows * m_cols;
	m_ptr = new T[m_rows * m_cols];
	for (size_t i = 0; i < m_rows * m_cols; i++)
	{
		m_ptr[i] = other.m_ptr[i];
	}
}
template <typename T>
template <typename T1>
Matrix<T>::Matrix(const Matrix<T1>& other)
{
	// copy_constructor(other);
	m_rows = other.rows();
	m_cols = other.columns();
	m_capacity = m_rows * m_cols;
	m_ptr = new T[m_rows * m_cols];
	for (size_t i = 0; i < m_rows * m_cols; i++)
	{
		m_ptr[i] = other.m_ptr[i];
	}
}
template <typename T>
template <typename T1>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T1>> list) {
	// попробуем отказаться от size_t, .size() возвращает именно его
	size_t max_cols = 0;
	for (auto sub_list : list) {
		if (max_cols < sub_list.size())
			max_cols = sub_list.size();
	}
	// Пробежались по вложенным спискам, найдя среди них максимальный размер. 
	m_rows = list.size();
	m_cols = max_cols;
	m_capacity = m_rows * m_cols;
	m_ptr = new T[m_rows * m_cols];
	
	int i = 0;
	for (auto sub_list : list) {
		size_t j = 0;
		for (auto elem : sub_list) {
			m_ptr[i * m_cols + j++] = elem;
		}
		// заполняем пропуски
		for (; j < m_cols; ++j) {
			m_ptr[i * m_cols + j] = T(); // по умолчанию double(), поэтому ругается.  
		}
		++i;
	}
}
template <typename T>
template <typename T1>
Matrix<T>::Matrix(std::initializer_list<T1> list) noexcept
{
	m_rows = list.size();
	m_cols = 1;
	m_ptr = new T[m_rows * m_cols];
	std::copy(list.begin(), list.end(), m_ptr);
}

template <typename T>
// linalg::Matrix<int> m2(std::move(C))
Matrix<T>::Matrix(Matrix<T>&& m) noexcept {
	this->swap(m);
}
/*Деструктор
template<typename T>
Matrix<T>::~Matrix() noexcept {
	for (T* ptr = m_ptr; ptr != m_ptr + m_rows * m_cols; ++ptr)
		ptr->~T();
	delete[] m_ptr;
}
*/
// Начало функций работы с памятью
template<typename T>
void Matrix<T>::shrink_to_fit() {
	if (m_capacity == m_rows * m_cols) return;
	*this = Matrix(*this);
}
template<typename T>
void Matrix<T>::clear() noexcept {
	// delete[] m_ptr?
	for (T* ptr = m_ptr; ptr != m_ptr + m_rows * m_cols; ++ptr)
		ptr->~T();
	m_rows = 0;
	m_cols = 0;
}
template <typename T>
size_t Matrix<T>::capacity() const
{
	return m_capacity;
}
template<typename T>
void Matrix<T>::reserve(size_t capacity) {
	if (m_capacity >= capacity) return;
	T* tmp_ptr = new T[capacity];
	for (size_t i = 0; i < m_rows * m_cols; i++)
	{
		tmp_ptr[i] = m_ptr[i];
	}
	delete[] m_ptr;
	m_ptr = tmp_ptr;
	m_capacity = capacity;
}
// Начало блока функций
template <typename T>
size_t Matrix<T>::rows() const
{
	return m_rows;
}
template <typename T>
size_t Matrix<T>::columns() const
{
	return m_cols;
}
template <typename T>
T Matrix<T>::trace() const {
	if (!square()) throw NotSquareMatrix();
	T trace = 0;
	for (size_t i = 0; i < m_rows; i++)
		trace += (*this)(i, i);
	return trace;
}
template <typename T>
T Matrix<T>::det() const {
	if (!square()) throw NotSquareMatrix();
	if (m_rows < 2) throw BadMultiSizesException();
	if (m_rows == 2) {
		return ((*this)(0, 0) * (*this)(1, 1)) - ((*this)(0, 1) * (*this)(1, 0));
	}
	T res = 0;
	for (size_t x = 0; x < m_cols; x++) {
		res += pow(-1, x) * (*this)(0, x) * this->submatrix(0, x).det();
	}
	return res;
}
template <typename T> 
bool Matrix<T>::empty() const
{
	return (m_cols == 0 || m_rows == 0);
}
template <typename T>
bool Matrix<T>::invertible() const
{
	return this->det() != 0;
}
template <typename T>
bool Matrix<T>::square() const {
	return m_rows == m_cols;
}
template <typename T>
Matrix<T> Matrix<T>::transpose() {
	Matrix<T> m(m_cols, m_rows, 0);
	for (size_t i = 0; i < (m_rows); i++) for (size_t j = 0; j < (m_cols); j++)
		m(j, i) = (*this)(i, j);
	return m;
}
template <typename T>
// Возвращение подматрицы, которая не содержит строку и столбец, в которой есть элемент (i, j). 
Matrix<T> Matrix<T>::submatrix(size_t i, size_t j) const {
	Matrix<T> R(m_rows - 1, m_cols - 1, 0);
	size_t i_sub = 0, j_sub;
	for (size_t n = 0; n < m_rows; n++) {
		if (n == i) // переходим к следующему элементу, если натыкаемся на старого друга. 
			continue;
		j_sub = 0;
		for (size_t m = 0; m < m_cols; m++) {
			if (m == j)
				continue;
			R(i_sub, j_sub) = (*this)(n, m);
			++j_sub;
		}
		++i_sub;
	}
	return R;
}
template <typename T>
T Matrix<T>::algebraic_adjucnt(size_t i, size_t j) {
	return pow(-1, (i + j)) * this->submatrix(i, j).det();
}
template <typename T>
Matrix<double> Matrix<T>::invert() {
	if (!invertible()) {
		throw NotInvertibleMatrix();
	}
	Matrix<T> R(this->m_rows, this->m_rows, 0);
	Matrix<T> A(this->m_rows, this->m_rows, 0);
	for (size_t i = 0; i < (m_rows); i++) for (size_t j = 0; j < (m_cols); j++) R.m_ptr[index(i, j)] = algebraic_adjucnt(i, j);
	double det = this->det();
	double x = 1 / det; 
	A = R.transpose();
	return A * x;
}
template <typename T>
Matrix<T> Matrix<T>::reshape(size_t new_rows, size_t new_cols) {
	if (this->m_cols*this->m_rows != new_cols*new_rows) throw InvalidSizesException();
	m_rows = new_rows;
	m_cols = new_cols;
	return *this;
}
template <typename T>
void Matrix<T>::swap(Matrix<T>& other) {
	std::swap(m_rows, other.m_rows);
	std::swap(m_cols, other.m_cols);
	std::swap(m_ptr, other.m_ptr);
}

// Операторы

constexpr int nmax{ 100 };
template <typename T>
int number_of_digits(T n) {
	std::ostringstream strs;
	strs << n;
	return strs.str().size();
}
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& M)
{
	int max_len_per_column[nmax] = {};
	// Создаем массив, размеры которого орграничены константой. Он будет хранить максимальную длину для каждой колонки.
	for (size_t j = 0; j < M.columns(); ++j) {
		size_t max_len = 0; // переменная max_len, которая будет хранить максимальный размер, который нужно выделить под элемент.
		for (size_t i = 0; i < M.rows(); ++i) {
			size_t num_length = number_of_digits(M.m_ptr[i * M.columns() + j]);
			if (num_length > max_len) {
				max_len = num_length;
				max_len_per_column[j] = max_len;
			}
		}
	}
	for (size_t i = 0; i < M.rows(); ++i)
		for (size_t j = 0; j < M.columns(); ++j)
			os << (j == 0 ? "\n|" : "") << std::setw(max_len_per_column[j]) << M.m_ptr[i * M.columns() + j] << (j == M.columns() - 1 ? "|" : " ");
	return os;
}
template <typename T>
T& Matrix<T>::operator()(size_t i, size_t j) {
	if (i >= m_rows || j >= m_cols) throw OutofBoundsException();
	return m_ptr[index(i, j)];
}
template <typename T>
T Matrix<T>::operator()(size_t i, size_t j) const {
	if (i >= m_rows || j >= m_cols) throw OutofBoundsException();
	return m_ptr[index(i, j)];
}
/*
1) присваивание перемещением
2) присваивание копированием
В чём суть: && позволяет проинициализировать r-value значением, позволяя обойти ограничение
на инициализацию только const lvalue значением (пункт 2). В первом случае мы избегаем ненужной копии temp,
которую уже можно передать в swap.
*/
template <typename T>
template <typename T1>
auto& Matrix<T>::operator=(Matrix<T1>&& m) noexcept { // IDE посоветовала мне объявить её noexcept. Кто я такой, чтобы спорить с такими авторитетами
	this->swap(m);
	return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m) {
	Matrix temp(m);
	this->swap(temp);
	return *this;
}
template <typename T>
template <typename T1>
Matrix<T>& Matrix<T>::operator=(const Matrix<T1>& m) { 
	Matrix temp(m);
	this->swap(temp);
	return *this;
}
template <typename T>
template <typename T1>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T1>& other)  {
	if (this->columns() != other.columns() || this->rows() != other.rows()) throw InvalidSizesException();
	for (size_t i = 0; i < (m_rows); i++) for (size_t j = 0; j < (m_cols); j++) m_ptr[index(i, j)] += other.m_ptr[index(i, j)];
	return *this;
}
template <typename T>
Matrix<T> Matrix<T>::operator-() {
	Matrix<T> A(*this);
	for (size_t i = 0; i < (m_rows); i++) for (size_t j = 0; j < (m_cols); j++) A.m_ptr[index(i, j)] *= -1;
	return A;
}
template <typename T>
template <typename T1>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T1>& other){
	if (this->columns() != other.columns() || this->rows() != other.rows()) throw InvalidSizesException();
	for (size_t i = 0; i < (m_rows); i++) for (size_t j = 0; j < (m_cols); j++) m_ptr[index(i, j)] -= other.m_ptr[index(i, j)];
	return *this;
}
template <typename T>
template <typename T1>
Matrix<T>& Matrix<T>::operator*=(T1 a) {
	for (size_t i = 0; i < (m_rows); i++) for (size_t j = 0; j < (m_cols); j++) m_ptr[index(i, j)] *= a;
	return *this;
}
template<typename T>
bool eq(T one, T another) {
	return fabs(one - another) < 0.000001;
}
template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) {
	if ((other.m_rows != this->m_rows) || (other.m_cols != this->m_cols))
		return false;
	for (size_t k = 0; k < (this->m_rows * this->m_cols); k++)
		if (!eq(other.m_ptr[k], m_ptr[k]))
			return false;
	return true;
}
template <typename T, typename T1>
auto operator*(const Matrix<T>& A, T1 b) {
	Matrix<decltype(T() * T1())> M(A);
	M *= b;
	return M;
}
template <typename T, typename T1>
auto operator*(T1 b, const Matrix<T>& A) {
	return A * b;
}
template <typename T, typename T1> // t1 t2 - ОБРАЗЕЦ!
auto operator+(const Matrix<T>& A, const Matrix<T1>& B) {
	if (A.columns() != B.columns() || A.rows() != B.rows()) throw InvalidSizesException();
	Matrix<decltype(T() + T1())> M = A; // M(A)
	M += B;
	return M;
}

template <typename T, typename T1>
auto operator-(const Matrix<T>& A, const Matrix<T1>& B) {
	if (A.columns() != B.columns() || A.rows() != B.rows()) throw InvalidSizesException();
	Matrix<decltype(T() + T1())> M(A);
	M -= B;
	return M;
}

template <typename T, typename T1>
auto operator*(const Matrix<T>& A, const Matrix<T1>& B) {
	if (A.columns() != B.rows()) throw BadMultiSizesException();
	Matrix<decltype(T()* T1())> R(A.rows(), B.columns(), 0);
	for (size_t i = 0; i < A.rows(); i++) {
		for (size_t j = 0; j < B.columns(); j++) {
			for (size_t k = 0; k < A.columns(); k++) {
				R(i, j) += A(i, k) * B(k, j);
			}
		}
	}
	return R;
}
template <typename T>
template <typename T1>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T1>& A) {
	if (this->columns() != A.rows()) throw BadMultiSizesException();
	Matrix<decltype(T() * T1())> M = *this * A;
	*this = M;
	return *this;
}
template <typename T>
void Matrix<T>::copy_constructor(const Matrix<T>& other) {
	m_rows = other.rows();
	m_cols = other.columns();
	m_capacity = m_rows * m_cols;
	m_ptr = new T[m_rows * m_cols];
	for (size_t i = 0; i < m_rows * m_cols; i++)
	{
		m_ptr[i] = other.m_ptr[i];
	}
}

template <typename T>
Matrix<T> Matrix<T>::power(size_t pow) {
	if (pow == 0) return eye<T>(this->rows());
	if (pow == 1) return *this;
	Matrix<T> initialMatrix = *this;
	Matrix<T> result(*this);
	for (size_t k = 1; k < pow; k++) {
		result *= initialMatrix;
	}
	return result;
}

template <typename T>
Matrix<T> eye(size_t size) {
	linalg::Matrix<T> A(size, size, 0);
	for (size_t i = 0; i < size; i++) { A.m_ptr[i * A.columns() + i] = 1; }
	return A;
}

}


