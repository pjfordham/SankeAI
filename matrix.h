#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <vector>
#include <fmt/core.h>
#include <cstddef>
#include <algorithm>
#include <Eigen/Dense>

class Matrix {

 public:
   Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m;

   std::size_t rows() { return  m.rows(); }
   std::size_t cols() { return  m.cols(); }

   Matrix() {}

   Matrix( Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> n) : m(std::move(n)) {}

   Matrix(std::size_t rows, std::size_t cols) : m(rows,cols) {
   }

   void output() const {
      for(int i = 0; i < m.rows(); i++) {
         for(int j = 0; j < m.cols(); j++) {
            fmt::print("{} ", m(i,j));
         }
         fmt::print("\n");
      }
      fmt::print("\n");
   }

   Matrix dot(Matrix n) const {
      return {m * n.m};
   }

   void randomize() {
      for(int i = 0; i < m.rows(); i++) {
         for(int j = 0; j < m.cols(); j++) {
            m(i,j) = 0;//random(-1,1);
         }
      }
   }

   Matrix singleColumnMatrixFromArray(const std::vector<float> &arr) {
      Matrix n{arr.size(), 1};
      for(auto i = 0; i < arr.size(); i++) {
         n.m(i,0) = arr[i];
      }
      return n;
   }

   std::vector<float> toArray() {
      std::vector<float> arr;
      arr.reserve(m.rows()*m.cols());
      for(int i = 0; i < m.rows(); i++) {
         for(int j = 0; j < m.cols(); j++) {
            arr.push_back( m(i,j) );
         }
      }
      return arr;
   }

   Matrix addBias() {
      std::size_t rows = m.rows();
      std::size_t cols = m.cols();
      Matrix n{rows+1, 1};
      for(int i = 0; i < rows; i++) {
         n.m(i,0) = m(i,0);
      }
      n.m(rows,0) = 1;
      return n;
   }

   Matrix activate() {
      std::size_t rows = m.rows();
      std::size_t cols = m.cols();
      Matrix n{rows, cols};
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            n.m(i,j) = relu(m(i,j));
         }
      }
      return n;
   }

   float relu(float x) {
      return std::max(0.0f,x);
   }

   void mutate(float mutationRate) {
      std::size_t rows = m.rows();
      std::size_t cols = m.cols();
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            float rand = 0;//random(1);
            if(rand<mutationRate) {
               m(i,j) += 0;//randomGaussian()/5;

               if(m(i,j) > 1) {
                  m(i,j) = 1;
               }
               if(m(i,j) <-1) {
                  m(i,j) = -1;
               }
            }
         }
      }
   }

   Matrix crossover(Matrix partner) {
      std::size_t rows = m.rows();
      std::size_t cols = m.cols();
      Matrix child{rows, cols};

      int randC = 0;//floor(random(cols));
      int randR = 0;//floor(random(rows));

      for(int i = 0; i < rows; i++) {
         for(int j = 0;  j < cols; j++) {
            if((i  < randR) || (i == randR && j <= randC)) {
               child.m(i,j) = m(i,j);
            } else {
               child.m(i,j) = partner.m(i,j);
            }
         }
      }
      return child;
   }


};

#endif
