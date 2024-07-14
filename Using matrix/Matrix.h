/*
* Matrix class
* 
* Author: Farhan Syed
* Year: 2024
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

template <typename T>
class Matrix {
    static_assert(std::is_move_constructible<T>::value,"T must be move-constructible");
    static_assert(std::is_move_assignable<T>::value,"T must be move-assignable");
public:
    // constructors and assignment operators
    Matrix();
    explicit Matrix(size_t dim); 
    Matrix(size_t rows, size_t cols);
    Matrix(const std::initializer_list<T> & list);
    Matrix(const Matrix<T> & other);
    Matrix(Matrix<T> && other) noexcept;

    Matrix<T> & operator=(const Matrix<T> & other);
    Matrix<T> & operator=(Matrix<T> && other) noexcept;

    ~Matrix();

    // accessors
    size_t rows() const;
    size_t cols() const;

    T & operator()(size_t row, size_t col);
    const T & operator()(size_t row, size_t col) const;

    // operators
    Matrix<T> operator*(const Matrix<T> & other) const;
    Matrix<T> operator+(const Matrix<T> & other) const;
    Matrix<T> operator-(const Matrix<T> & other) const;

    void operator*=(const Matrix<T> & other);
    void operator+=(const Matrix<T> & other);
    void operator-=(const Matrix<T> & other);

    // methods
    void reset();

    void insert_row(size_t row);
    void append_row(size_t row);
    void remove_row(size_t row);
    void insert_column(size_t col);
    void append_column(size_t col);
    void remove_column(size_t col);

    // iterators
    typedef T* iterator;

    iterator begin();
    iterator end();

private:
    size_t m_rows;
    size_t m_cols;
    size_t m_capacity;
    T * m_vec;
};

// input/output operators
template<typename T>
std::istream & operator>>(std::istream & is, Matrix<T> & m);

template<typename T>
std::ostream & operator<<(std::ostream & os, const Matrix<T> & m);

// functions
template<typename T>
Matrix<T> identity(size_t dim);

//
// Implementations
//

// CONSTRUCTORS

// Default constructor with default elements
template<typename T>
Matrix<T>::Matrix() : m_rows(0), m_cols(0), m_capacity(0), m_vec(nullptr) {}

// Square matrix with default elements constructor
template<typename T>
Matrix<T>::Matrix(size_t dim) : m_rows(dim), m_cols(dim), m_capacity(dim*dim), m_vec(new T[dim*dim]) {  
   std::fill_n(m_vec, dim*dim, T()); // Elements are initialised to default element of type T
}  

// Defined row and column size with default elements constructor
template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols), m_capacity(rows*cols), m_vec(new T[rows*cols]) {
    std::fill_n(m_vec, rows*cols, T());
} 

// Create square matrix using list that decides the elements. List length must be perfect square. 
template<typename T>
Matrix<T>::Matrix(const std::initializer_list<T> & list) {
    int squareRoot = sqrt(list.size()); 
    if(squareRoot*squareRoot == list.size()){
        m_rows = m_cols = squareRoot;
        m_capacity = list.size();
        m_vec = new T[m_capacity];

        // Each element in the list becomes an element in the matrix
        size_t i = 0;
        for (const T& elem : list) {
            m_vec[i++] = elem;
        }
    } else{
        throw std::out_of_range("List size is not a perfect square!"); 
    }
    
}

// Copy constructor
template<typename T>
Matrix<T>::Matrix(const Matrix<T> & other) : m_rows(other.m_rows), m_cols(other.m_cols), m_capacity(other.m_capacity), m_vec(new T[other.m_capacity]) {
    for (size_t i = 0; i < m_capacity; i++) {
        m_vec[i] = other.m_vec[i];
    }
}

// Move constructor
template<typename T>
Matrix<T>::Matrix(Matrix<T> && other) noexcept : m_rows(other.m_rows), m_cols(other.m_cols), m_capacity(other.m_capacity), m_vec(other.m_vec) {
    other.m_rows = other.m_cols = other.m_capacity = 0;
    other.m_vec = nullptr;
}

// Copy assignment operator
template<typename T>
Matrix<T> & Matrix<T>::operator=(const Matrix<T> & other) {
    if(this != &other){
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_capacity = other.m_capacity;
        
        delete[] m_vec;
        m_vec = new T[m_capacity];

        for (size_t i = 0; i < m_capacity; i++) {
            m_vec[i] = other.m_vec[i];
        }
    }
    return *this;

}

// Move assignment operator
template<typename T>
Matrix<T> & Matrix<T>::operator=(Matrix<T> && other) noexcept {
    if(this != &other){
        delete[] m_vec;
        
        m_rows = other.m_rows; 
        m_cols = other.m_cols;
        m_capacity = other.m_capacity;
        m_vec = other.m_vec; 
        
        other.m_rows = other.m_cols = other.m_capacity = 0;
        other.m_vec = nullptr;
    }

    return *this;
}

// Destructor
template<typename T>
Matrix<T>::~Matrix() {
    delete[] m_vec;
}

// ACCESSORS

// Get number of rows
template<typename T>
size_t Matrix<T>::rows() const {
    return m_rows;
}

// Get number of columns
template<typename T>
size_t Matrix<T>::cols() const {
    return m_cols;
}

// OPERATORS 

// Access/modify an element 
template<typename T>
T & Matrix<T>::operator()(size_t row, size_t col) {
    if(row < m_rows && col < m_cols){
        return m_vec[row * m_cols + col];
    }
    throw std::out_of_range("Wrong dimensions!");
}

// Access an element - read only version
template<typename T>
const T & Matrix<T>::operator()(size_t row, size_t col) const {
    if(row < m_rows && col < m_cols){
        return m_vec[row * m_cols + col];
    }
    throw std::out_of_range("Wrong dimensions!");
}
// Multiplication of matrices
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> & other) const {
    if(m_cols == other.m_rows){
        Matrix<T> resultMatrix(m_rows, other.m_cols);
        
        // Go through each row of matrix 1 and multiply with each column of matrix 2 by calculating the dot product. 
        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < other.m_cols; j++) {
                // Calculate the dot product
                T sum = 0;
                for (size_t k = 0; k < m_cols; k++) {
                    sum += m_vec[i * m_cols + k] * other.m_vec[k * other.m_cols + j];
                }
                resultMatrix(i, j) = sum;
            }
        }
        return resultMatrix;
    }
    throw std::out_of_range("Wrong dimensions!");
}

// Addition of matrices
template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> & other) const {
    if(m_rows == other.m_rows && m_cols == other.m_cols){
        Matrix<T> resultMatrix(m_rows, m_cols);
        for (size_t i = 0; i < resultMatrix.m_capacity; i++) {
            resultMatrix.m_vec[i] = m_vec[i] + other.m_vec[i]; // Add each corresponding element
        }
        return resultMatrix;
    }
    throw std::out_of_range("Wrong dimensions!");
}

// Subtraction of matrices
template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> & other) const {
    if(m_rows == other.m_rows && m_cols == other.m_cols){
        Matrix<T> resultMatrix(m_rows, m_cols);
        for (size_t i = 0; i < resultMatrix.m_capacity; i++) {
            resultMatrix.m_vec[i] = m_vec[i] - other.m_vec[i]; // Subtract each corresponding element
        }
        return resultMatrix;
    }
    throw std::out_of_range("Wrong dimensions!");
}

// *= Operator
template<typename T>
void Matrix<T>::operator*=(const Matrix<T> & other) {
    *this = *this * other;
}

// += Operator
template<typename T>
void Matrix<T>::operator+=(const Matrix<T> & other) {
    *this = *this + other;
}

// -= Operator
template<typename T>
void Matrix<T>::operator-=(const Matrix<T> & other) {
    *this = *this - other;
}
 
// FUNCTIONS

// Reset a matrix with default value. 
template<typename T>
void Matrix<T>::reset() {
    std::fill(begin(), end(), T());
    m_rows = 0;
    m_cols = 0;
}

// Insert row of zeroes before selected row
template<typename T>
void Matrix<T>::insert_row(size_t row) {
    if (row < m_rows) {
        Matrix<T> newMatrix(m_rows + 1, m_cols); // New matrix with extra row

        for (size_t i = 0; i < row; i++) {      // Rows above new row are in same position
            for (size_t j = 0; j < m_cols; j++) {
                newMatrix(i, j) = std::move(m_vec[i * m_cols + j]);
            }
        }

        for (size_t j = 0; j < m_cols; j++) {   // Insert row of zeroes
            newMatrix(row, j) = T();
        }

        for (size_t i = row; i < m_rows; i++) { // Place remaining rows below row of zeroes
            for (size_t j = 0; j < m_cols; j++) {
                newMatrix(i+1, j) = std::move(m_vec[i * m_cols + j]);
            }
        }
        *this = std::move(newMatrix);
    } else{
        throw std::out_of_range("Wrong dimensions!");
    }
}

// Append row of zeroes after selected row
template<typename T>
void Matrix<T>::append_row(size_t row) { 
    if (row < m_rows) {
        Matrix<T> newMatrix(m_rows + 1, m_cols); // New matrix with extra row
        for (size_t i = 0; i < row+1; i++) {      // Rows above new row are in same position
            for (size_t j = 0; j < m_cols; j++) {
                newMatrix(i, j) = std::move(m_vec[i * m_cols + j]);
            }
        }

        for (size_t j = 0; j < m_cols; j++) {   // Insert row of zeroes
            newMatrix(row+1, j) = T();
        }

        for (size_t i = row+1; i < m_rows; i++) { // Place remaining rows below row of zeroes
            for (size_t j = 0; j < m_cols; j++) {
                newMatrix(i+1, j) = std::move(m_vec[i * m_cols + j]);
            }
        }
        *this = std::move(newMatrix);

    } else{
        throw std::out_of_range("Wrong dimensions!");
    }
}

// Remove selected row
template<typename T>
void Matrix<T>::remove_row(size_t row) {
    if (row < m_rows) {
        Matrix<T> newMatrix(m_rows - 1, m_cols);    // New matrix with one less row

        for (size_t i = 0; i < row; i++) {          // Rows above selected row are in the same position
            for (size_t j = 0; j < m_cols; j++) {
                newMatrix(i, j) = std::move(m_vec[i * m_cols + j]);
            }
        }
        // We skip the selected row
        
        for (size_t i = row + 1; i < m_rows; i++) {     // Rows below selected row are the same
            for (size_t j = 0; j < m_cols; j++) {
                newMatrix(i - 1, j) = std::move(m_vec[i * m_cols + j]);
            }
        }

        *this = std::move(newMatrix);
    } else{
        throw std::out_of_range("Wrong dimensions!");
    }
}

// Insert column of zeroes to the left of a selected column
template<typename T>
void Matrix<T>::insert_column(size_t col) {
    if (col < m_cols) {
        Matrix<T> newMatrix(m_rows, m_cols + 1); // New matrix with extra column

        for (size_t i = 0; i < m_rows; i++) { 
            for (size_t j = 0; j < col; j++) {      // Columns to left of selected column are the same
                newMatrix(i, j) = std::move(m_vec[i * m_cols + j]);
            }
        }

        for (size_t i = 0; i < m_rows; i++) { // Insert column of zeroes
            newMatrix(i, col) = T();
        }

        for (size_t i = 0; i < m_rows; i++) {  // Place remaining columns to right of selected column
            for (size_t j = col; j < m_cols; j++) {
                newMatrix(i, j + 1) = std::move(m_vec[i * m_cols + j]);
            }
        }
        *this = std::move(newMatrix);
    } else{
        throw std::out_of_range("Wrong dimensions!");
    }
}

// Append column of zeroes to the right of a selected column
template<typename T>
void Matrix<T>::append_column(size_t col) {
    if (col < m_cols) {
        Matrix<T> newMatrix(m_rows, m_cols + 1); // New matrix with extra column
        
        for (size_t i = 0; i < m_rows; i++) { 
            for (size_t j = 0; j < col+1; j++) {      // Columns to left of selected column are the same
                newMatrix(i, j) = std::move(m_vec[i * m_cols + j]);
            }
        }

        for (size_t i = 0; i < m_rows; i++) { // Insert column of zeroes
            newMatrix(i, col+1) = T();
        }

        for (size_t i = 0; i < m_rows; i++) {  // Place remaining columns to right of selected column
            for (size_t j = col+1; j < m_cols; j++) {
                newMatrix(i, j + 1) = std::move(m_vec[i * m_cols + j]);
            }
        }
        *this = std::move(newMatrix);
    } else{
        throw std::out_of_range("Wrong dimensions!");
    }
}

// Remove selected column
template<typename T>
void Matrix<T>::remove_column(size_t col) {
    if (col < m_cols) {
        Matrix<T> newMatrix(m_rows, m_cols - 1); // New matrix with one less row

        for (size_t i = 0; i < m_rows; i++) { 
            for (size_t j = 0; j < col; j++) {      // Columns to left of selected column are the same
                newMatrix(i, j) = std::move(m_vec[i * m_cols + j]);
            }
        }

        // We skip the selected column

        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = col + 1; j < m_cols; j++) { // Columns after removed row are the same
                newMatrix(i, j - 1) = std::move(m_vec[i * m_cols + j]);
            }
        }
        *this = std::move(newMatrix);
    } else {
        throw std::out_of_range("Wrong dimensions!");
    }
}

// ITERATORS

// begin()
template<typename T>
typename Matrix<T>::iterator Matrix<T>::begin() {
    return m_vec;
}

// end()
template<typename T>
typename Matrix<T>::iterator Matrix<T>::end() {
    return m_vec + m_rows * m_cols;
}

// INPUT / OUTPUT

// Input operator
template<typename T>
std::istream & operator>>(std::istream & is, Matrix<T> & m) {
    std::vector<std::vector<T>> parsedMatrix;
    std::string line;
    size_t columns = 0;
    size_t rows = 0;

    // Parse the matrix in the input stream
    while (std::getline(is, line)) {
        line.erase(std::remove(line.begin(), line.end(), '['), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(std::remove(line.begin(), line.end(), ']'), line.end());
        rows++;

        std::istringstream rowIS(line); // convert line string to input stream
        std::vector<T> parsedRow;
        T columnElement;

        while (rowIS >> columnElement) { 
            parsedRow.push_back(columnElement);
        }
        parsedMatrix.push_back(parsedRow); // Save the whole row in the temporary parsed matrix
    }
    if(rows > 0 ){
        columns = parsedMatrix[0].size();
    }
    m = Matrix<T>(rows, columns);  // Resize the matrix that is reading from input stream
    for (size_t i = 0; i < rows; i++) {  // Copy values from parsedMatrix to m
        for (size_t j = 0; j < columns; j++) {
            m(i, j) = std::move(parsedMatrix[i][j]);
        }
    } 
    return is;
}

// Output operator
template<typename T>
std::ostream & operator<<(std::ostream & os, const Matrix<T> & m) {
    for (size_t i = 0; i < m.rows(); i++) {
        if (i == 0) {
            os << "[ ";
        } else {
            os << "  "; 
        }
        for (size_t j = 0; j < m.cols(); j++) {
            os << m(i,j);      
            if (j < m.cols()-1) {
                os << " "; 
            }  
        }
        if (i == m.rows() - 1) {
            os << " ]";
        } else {
            os << std::endl; 
        }
    }
    return os;
}

// Identity matrix
template<typename T>
Matrix<T> identity(size_t dim) {
    Matrix<T> id (dim);
    for (int i = 0; i < dim; i++){
        id(i,i) = 1; // Elements in diagonal become 1
    }
    return id;
}

#endif //MATRIX_H