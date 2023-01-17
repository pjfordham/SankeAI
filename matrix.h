#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <vector>
#include <fmt/core.h>
#include <cstddef>
#include <algorithm>

class Matrix {

 public:
   std::size_t rows, cols;
   std::vector<std::vector<float> > matrix;

   Matrix(std::size_t r, std::size_t c) {
      rows = r;
      cols = c;
   }

   void output() {
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            fmt::print("{} ", matrix[i][j]);
         }
         fmt::print("\n");
      }
      fmt::print("\n");
   }

   Matrix dot(Matrix n) {
      Matrix result{rows, n.cols};

      if(cols == n.rows) {
         for(int i = 0; i < rows; i++) {
            for(int j = 0; j < n.cols; j++) {
               float sum = 0;
               for(int k = 0; k < cols; k++) {
                  sum += matrix[i][k]*n.matrix[k][j];
               }
               result.matrix[i][j] = sum;
            }
         }
      }
      return result;
   }

   void randomize() {
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            matrix[i][j] = 0;//random(-1,1);
         }
      }
   }

   Matrix singleColumnMatrixFromArray(std::vector<float> arr) {
      Matrix n{arr.size(), 1};
      for(auto i = 0; i < arr.size(); i++) {
         n.matrix[i][0] = arr[i];
      }
      return n;
   }

   std::vector<float> toArray() {
      std::vector<float> arr;
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            arr[j+i*cols] = matrix[i][j];
         }
      }
      return arr;
   }

   Matrix addBias() {
      Matrix n{rows+1, 1};
      for(int i = 0; i < rows; i++) {
         n.matrix[i][0] = matrix[i][0];
      }
      n.matrix[rows][0] = 1;
      return n;
   }

   Matrix activate() {
      Matrix n{rows, cols};
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            n.matrix[i][j] = relu(matrix[i][j]);
         }
      }
      return n;
   }

   float relu(float x) {
      return std::max(0.0f,x);
   }

   void mutate(float mutationRate) {
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            float rand = 0;//random(1);
            if(rand<mutationRate) {
               matrix[i][j] += 0;//randomGaussian()/5;

               if(matrix[i][j] > 1) {
                  matrix[i][j] = 1;
               }
               if(matrix[i][j] <-1) {
                  matrix[i][j] = -1;
               }
            }
         }
      }
   }

   Matrix crossover(Matrix partner) {
      Matrix child{rows, cols};

      int randC = 0;//floor(random(cols));
      int randR = 0;//floor(random(rows));

      for(int i = 0; i < rows; i++) {
         for(int j = 0;  j < cols; j++) {
            if((i  < randR) || (i == randR && j <= randC)) {
               child.matrix[i][j] = matrix[i][j];
            } else {
               child.matrix[i][j] = partner.matrix[i][j];
            }
         }
      }
      return child;
   }

   Matrix clone() {
      Matrix clone{rows, cols};
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            clone.matrix[i][j] = matrix[i][j];
         }
      }
      return clone;
   }
};

#endif
