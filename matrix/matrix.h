#pragma once
namespace linalg {

	template <typename T = double>
	class Matrix {
	private: 
		size_t m_rows;
		size_t m_cols;
	public:
		T* m_ptr;
		size_t m_capacity;
		Matrix();
		Matrix(size_t rows, size_t columns);
		Matrix(size_t rows, size_t columns, T defaultvalue);
		Matrix(const Matrix<T>& other);
		template <typename T1>
		Matrix(const Matrix<T1>& other);
		template <typename T1>
		Matrix(std::initializer_list<std::initializer_list<T1>> list);
		template <typename T1>
		Matrix(std::initializer_list<T1> list) noexcept;
		Matrix(Matrix&& m) noexcept;
		size_t rows() const;
		size_t columns() const;
		T trace() const;
		T det() const;
		T algebraic_adjucnt(size_t i, size_t j);
		Matrix<double> invert();
		bool empty() const;
		bool invertible() const;
		bool square() const;
		Matrix submatrix(size_t i, size_t j) const;
		void swap(Matrix<T>& other);
		Matrix reshape(size_t new_rows, size_t new_cols);
		Matrix transpose();
		Matrix power(size_t pow);
		size_t capacity() const;
		void shrink_to_fit();
		void clear() noexcept;
		void reserve(size_t capacity);
		T& operator()(size_t i, size_t j);
		T operator()(size_t i, size_t j) const; // спасет от C3848, константным методам - константные значения! таков путь.
		template <typename T1>
		Matrix& operator=(const Matrix<T1>& m); 
		Matrix& operator=(const Matrix<T>& m);
		template <typename T1>
		auto& operator=(Matrix<T1>&& m) noexcept;
		template <typename T1>
		Matrix& operator+=(const Matrix<T1>& other);
		template <typename T1>
		Matrix& operator*=(T1 a);
		template <typename T1>
		Matrix& operator*=(const Matrix<T1>& A);
		Matrix operator-();
		template <typename T1>
		Matrix& operator-=(const Matrix<T1>& other);
		class Proxy_Row {
			friend class Matrix;
		public:
			T& operator [](size_t col) {
				return _master(_row, col);
			}
		private:
			Proxy_Row(Matrix& master, size_t row) : _master(master), _row(row) {}
			Matrix& _master;
			size_t _row;
		};
		Proxy_Row operator[](size_t row) {
			return Proxy_Row(*this, row);
		}
		bool operator==(const Matrix<T>& other);;
		void copy_constructor(const Matrix<T>& other);
	};
	template <typename T = double>
	std::ostream& operator<<(std::ostream&, const Matrix<T>&);
	template <typename T, typename T1>
	auto operator*(const Matrix<T>& A, const Matrix<T1>& B);
	template <typename T, typename T1>
	auto operator*(const Matrix<T>& A, T1 b);
	template <typename T, typename T1>
	auto operator*(T1 b, const Matrix<T>& A);
	template <typename T, typename T1>
	auto operator+(const Matrix<T>& A, const Matrix<T1>& B);
	template <typename T, typename T1>
	auto operator-(const Matrix<T>& A, const Matrix<T1>& B);
	template <typename T>
	Matrix<T> eye(size_t size);
	// структуры, а не классы, потому что замучаешься вытаскивать сообщение из private.
	struct MatrixException : public std::runtime_error {
		MatrixException(const char* message): runtime_error(message) {}
	};
	// делегирующие конструкторы, зовем родителя.
	struct OutofBoundsException : public MatrixException {
		OutofBoundsException() : MatrixException("It is impossible to get a matrix cell with the specified indexes!") {}
	};
	struct EmptyMatrixException : public MatrixException {
		EmptyMatrixException() : MatrixException("It is impossible to perform this operation on an empty matrix") {}
	};
	struct InvalidSizesException : public MatrixException {
		InvalidSizesException() : MatrixException("Bad sizes of the matrix") {}
	};
	struct BadMultiSizesException : public MatrixException {
		BadMultiSizesException() : MatrixException("The sizes of the multiplied matrices do not correlate with each other") {}
	};
	struct NotSquareMatrix : public MatrixException {
		NotSquareMatrix() : MatrixException("Matrix must be square for this operation!") {}
	};
	struct NotInvertibleMatrix : public MatrixException {
		NotInvertibleMatrix() : MatrixException("Couldn't invert this matrix...") {}
	};
}

#include "matrix.hpp"





