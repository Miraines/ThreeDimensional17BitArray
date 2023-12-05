#include"MyClass.h"
#include <cstring>

Number17Bit::Number17Bit(uint8_t* base, size_t bit_shift)
    : base_pointer(base), bit_shift(bit_shift) {
  if (bit_shift > 7) {
    RaiseError("Bit shift must be in the range 0-7");
  }
}

Array1D::Array1D(uint8_t* arr_2d, size_t index_x, size_t y_index, size_t y, size_t z)
    : arr_1d(arr_2d), x_index(index_x), y_index(y_index),
      y_size(y), z_size(z) {
}

Array2D::Array2D(uint8_t* arr_3d, size_t index_x, size_t y, size_t z)
    : arr_2d(arr_3d), x_index(index_x),
      y_size(y), z_size(z) {
}

Array3D::Array3D(size_t x, size_t y, size_t z) {
  x_size = x;
  y_size = y;
  z_size = z;
  owns_memory = true;

  // Выделение памяти с учетом, что каждый элемент занимает 17 бит
  size_t total_bits = x * y * z * 17;
  size_t total_bytes = total_bits / 8 + (total_bits % 8 ? 1 : 0);
  arr_3d = new uint8_t[total_bytes]();
  std::memset(arr_3d, 0, total_bytes);
}

void Number17Bit::WriteValue(uint8_t* base, size_t bit_shift, uint32_t value) {
  size_t byte_index = bit_shift / 8;
  size_t bit_index = bit_shift % 8;

  // Очистка области, где будет храниться 17-битное значение.
  base[byte_index] &= ~(0xFF << bit_index);
  base[byte_index + 1] &= ~(0xFF >> (8 - bit_index));
  base[byte_index + 2] &= ~(0x01 << (bit_index - 1));

  // Установка нового 17-битного значения.
  base[byte_index] |= static_cast<uint8_t>(value >> (9 + bit_index));
  base[byte_index + 1] = static_cast<uint8_t>((value >> (bit_index + 1)) & 0xFF);
  base[byte_index + 2] |= static_cast<uint8_t>((value << (7 - bit_index)) & 0xFF);
}

uint32_t Number17Bit::GetValue(const Number17Bit& number) {
  return ((((*number.base_pointer) & ((1 << (8 - number.bit_shift)) - 1))
      << (number.bit_shift + 9)) |
      (*(number.base_pointer + 1) << (number.bit_shift + 1)) |
      (*(number.base_pointer + 2) >> (7 - number.bit_shift)));
}

Number17Bit& Number17Bit::operator=(uint32_t value) {
  if (value >= (1 << 17)) {
    RaiseError("Value must be less than 2^17");
  }
  // Вызываем WriteValue с текущим базовым адресом и смещением
  Number17Bit::WriteValue(base_pointer, bit_shift, value);

  return *this;
}

Array3D Array3D::MakeArray(size_t x, size_t y, size_t z) {

  return Array3D(x, y, z);
}

std::ostream& operator<<(std::ostream& out, const Array3D& array) {
  for (size_t i = 0; i < array.GetFirstDimension(); ++i) {
    for (size_t j = 0; j < array.GetSecondDimension(); ++j) {
      for (size_t k = 0; k < array.GetThirdDimension(); ++k) {
        uint32_t value = static_cast<uint32_t>(array[i][j][k]);
        out << value << ' ';
      }
      out << '\n';
    }
    out << '\n';
  }

  return out;
}

std::istream& operator>>(std::istream& in, Array3D& array) {
  for (size_t i = 0; i < array.GetFirstDimension(); ++i) {
    for (size_t j = 0; j < array.GetSecondDimension(); ++j) {
      for (size_t k = 0; k < array.GetThirdDimension(); ++k) {
        uint32_t value;
        in >> value;
        // Рассчитываем базовый указатель и смещение для текущего элемента
        size_t byte_index =
            (i * array.GetSecondDimension() * array.GetThirdDimension() + j * array.GetThirdDimension() + k) * 17 / 8;
        size_t bit_shift = (k * 17) % 8;
        Number17Bit::WriteValue(&array.arr_3d[byte_index], bit_shift, value);
      }
    }
  }

  return in;
}

Number17Bit Array1D::operator[](size_t index_z) const {
  if (index_z >= z_size) {
    RaiseError("Index out of bounds in third dimension");
  }
  size_t byte_index =
      (x_index * y_size * z_size + y_index * z_size + index_z)
          * 17 / 8;
  size_t bit_shift = (index_z * 17) % 8;

  return Number17Bit(arr_1d + byte_index, bit_shift);
}

Array1D Array2D::operator[](size_t y_index) const {
  if (y_index >= y_size) {
    RaiseError("Index out of bounds in second dimension");
  }

  return Array1D(arr_2d, x_index, y_index, y_size, z_size);
}

Array2D Array3D::operator[](size_t index_x) const {
  if (index_x >= x_size) {
    RaiseError("Index out of bounds in first dimension");
  }

  return Array2D(arr_3d, index_x, y_size, z_size);
}

Array3D operator+(const Array3D& lhs, const Array3D& rhs) {
  if (lhs.GetFirstDimension() != rhs.GetFirstDimension() ||
      lhs.GetSecondDimension() != rhs.GetSecondDimension() ||
      lhs.GetThirdDimension() != rhs.GetThirdDimension()) {
    throw std::runtime_error("Dimensions of Array3D instances do not match for addition");
  }

  Array3D result(lhs.GetFirstDimension(), lhs.GetSecondDimension(), lhs.GetThirdDimension());
  for (size_t i = 0; i < lhs.GetFirstDimension(); ++i) {
    for (size_t j = 0; j < lhs.GetSecondDimension(); ++j) {
      for (size_t k = 0; k < lhs.GetThirdDimension(); ++k) {
        result[i][j][k] = Number17Bit::GetValue(lhs[i][j][k]) + Number17Bit::GetValue(rhs[i][j][k]);
      }
    }
  }

  return result;
}

Array3D operator-(const Array3D& lhs, const Array3D& rhs) {
  if (lhs.GetFirstDimension() != rhs.GetFirstDimension() ||
      lhs.GetSecondDimension() != rhs.GetSecondDimension() ||
      lhs.GetThirdDimension() != rhs.GetThirdDimension()) {
    throw std::runtime_error("Dimensions of Array3D instances do not match for subtraction");
  }

  Array3D result(lhs.GetFirstDimension(), lhs.GetSecondDimension(), lhs.GetThirdDimension());
  for (size_t i = 0; i < lhs.GetFirstDimension(); ++i) {
    for (size_t j = 0; j < lhs.GetSecondDimension(); ++j) {
      for (size_t k = 0; k < lhs.GetThirdDimension(); ++k) {
        int32_t diff = Number17Bit::GetValue(lhs[i][j][k]) - Number17Bit::GetValue(rhs[i][j][k]);
        result[i][j][k] = (diff >= 0) ? diff : 0; // Обеспечиваем, чтобы результат не был отрицательным
      }
    }
  }

  return result;
}

Array3D operator*(const Array3D& array, int32_t multiple) {
  Array3D result(array.GetFirstDimension(), array.GetSecondDimension(), array.GetThirdDimension());
  for (size_t i = 0; i < array.GetFirstDimension(); ++i) {
    for (size_t j = 0; j < array.GetSecondDimension(); ++j) {
      for (size_t k = 0; k < array.GetThirdDimension(); ++k) {
        result[i][j][k] = Number17Bit::GetValue(array[i][j][k]) * multiple;
      }
    }
  }

  return result;
}
