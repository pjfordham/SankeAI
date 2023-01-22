#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <vector>
#include <cstddef>
#include <algorithm>
#include <Eigen/Dense>

class Matrix {

 public:
   Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m;

   std::size_t rows() const { return  m.rows(); }
   std::size_t cols() const { return  m.cols(); }

   Matrix() {}

   Matrix( Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> n) : m(std::move(n)) {}

   Matrix(std::size_t rows, std::size_t cols) : m(rows,cols) {
   }

   void output() const;

   Matrix dot(Matrix n) const;

   void randomize();

   Matrix singleColumnMatrixFromArray(const std::vector<float> &arr);

   std::vector<float> toArray();

   Matrix addBias();

   Matrix activate();

   float relu(float x);

   void mutate(float mutationRate);

   Matrix crossover(Matrix partner);

};

#endif
