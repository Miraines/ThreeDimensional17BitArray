#pragma once
#include <iostream>
#include <cstdint>

inline void RaiseError(const char* message) {
  std::cerr << message << std::endl;
  std::exit(EXIT_FAILURE);
}

// Класс для представления одного 17-битного числа
class Number17Bit {
 public:
  Number17Bit(uint8_t* base, size_t bit_shift);
  Number17Bit& operator=(uint32_t value);

  static uint32_t GetValue(const Number17Bit& number);

  static void WriteValue(uint8_t* base, size_t bit_shift, uint32_t value);

  // Оператор преобразования Number17Bit к uint32_t
  operator uint32_t() const {
    return GetValue(*this);
  }

 private:
  uint8_t* base_pointer;
  size_t bit_shift;
};

class Array1D {
 public:

  Array1D(uint8_t* const arr_2d, size_t index_x, size_t y_index, size_t y, size_t z);
  Number17Bit operator[](size_t index_z) const;

 private:
  uint8_t* arr_1d;
  size_t x_index;
  size_t y_index;
  size_t y_size;
  size_t z_size;
};

class Array2D {
 public:
  Array2D(uint8_t* const arr_3d, size_t index_x, size_t y, size_t z);
  Array1D operator[](size_t y_index) const;

 private:
  uint8_t* arr_2d;
  size_t x_index;
  size_t y_size;
  size_t z_size;
};

class Array3D {
 public:
  Array3D(size_t x, size_t y, size_t z);

  ~Array3D() {
    if (owns_memory) {
      delete[] arr_3d;
    }
  }

  Array2D operator[](size_t index_x) const;

  static Array3D MakeArray(size_t x, size_t y, size_t z);
  friend Array3D operator+(const Array3D& lhs, const Array3D& rhs);
  friend Array3D operator-(const Array3D& lhs, const Array3D& rhs);
  friend Array3D operator*(const Array3D& array, int32_t multiple);
  friend std::ostream& operator<<(std::ostream& out, const Array3D& array);
  friend std::istream& operator>>(std::istream& in, Array3D& array);

  size_t GetFirstDimension() const { return x_size; }
  size_t GetSecondDimension() const { return y_size; }
  size_t GetThirdDimension() const { return z_size; }
  uint8_t* arr_3d;

 private:
  size_t x_size;
  size_t y_size;
  size_t z_size;
  bool owns_memory;
};
