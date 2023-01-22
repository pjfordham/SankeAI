#include "matrix.h"
#include <random>
#include <fmt/core.h>

void Matrix::output() const {
   for(int i = 0; i < m.rows(); i++) {
      for(int j = 0; j < m.cols(); j++) {
         fmt::print("{:<+06.3f} ", m(i,j));
      }
      fmt::print("\n");
   }
   fmt::print("\n");
}

Matrix Matrix::dot(Matrix n) const {
   return {m * n.m};
}

void Matrix::randomize() {
   static std::uniform_real_distribution<float> randomLocationRange(-1.0, 1.0);
   static std::random_device rd;
   static std::mt19937 randomNumbers(rd());
   for(int i = 0; i < m.rows(); i++) {
      for(int j = 0; j < m.cols(); j++) {
         m(i,j) = randomLocationRange( randomNumbers );
      }
   }
}

Matrix Matrix::singleColumnMatrixFromArray(const std::vector<float> &arr) {
   Matrix n{arr.size(), 1};
   for(auto i = 0; i < arr.size(); i++) {
      n.m(i,0) = arr[i];
   }
   return n;
}

std::vector<float> Matrix::toArray() {
   std::vector<float> arr;
   arr.reserve(m.rows()*m.cols());
   for(int i = 0; i < m.rows(); i++) {
      for(int j = 0; j < m.cols(); j++) {
         arr.push_back( m(i,j) );
      }
   }
   return arr;
}

Matrix Matrix::addBias() {
   std::size_t rows = m.rows();
   Matrix n{rows+1, 1};
   for(int i = 0; i < rows; i++) {
      n.m(i,0) = m(i,0);
   }
   n.m(rows,0) = 1;
   return n;
}

Matrix Matrix::activate() {
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

float Matrix::relu(float x) {
   return std::max(0.0f,x);
}

void Matrix::mutate(float mutationRate) {
   static std::uniform_real_distribution<float> randomLocationRange(0.0, 1.0);
   static std::normal_distribution<float> randomNormal(0, 0.2);
   static std::random_device rd;
   static std::mt19937 randomNumbers(rd());
   std::size_t rows = m.rows();
   std::size_t cols = m.cols();
   for(int i = 0; i < rows; i++) {
      for(int j = 0; j < cols; j++) {
         float rand = randomLocationRange( randomNumbers );
         if(rand<mutationRate) {
            m(i,j) += randomNormal( randomNumbers );;

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

Matrix Matrix::crossover(Matrix partner) {
   std::size_t rows = m.rows();
   std::size_t cols = m.cols();

   std::uniform_int_distribution<int> randomCols(0, cols);
   std::uniform_int_distribution<int> randomRows(0, rows);
   static std::random_device rd;
   static std::mt19937 randomNumbers(rd());

   Matrix child{rows, cols};

   int randC = randomCols(randomNumbers);
   int randR = randomRows(randomNumbers);

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

