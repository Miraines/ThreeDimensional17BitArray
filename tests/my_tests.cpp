#include "MyClass.h"
#include <gtest/gtest.h>

TEST(Array3DTest, CanCreateArray) {
  EXPECT_NO_THROW(Array3D::MakeArray(10, 2, 30));
}

TEST(Array3DTest, CanAccessElements) {
  Array3D arr1 = Array3D::MakeArray(90, 89, 88);
  EXPECT_NO_THROW(arr1[89][30][26] = 123);
  EXPECT_EQ(static_cast<uint32_t>(arr1[89][30][26]), 123);
}

TEST(Array3DTest, CanAddArraysWithSameDimensions) {
  Array3D arr1 = Array3D::MakeArray(10, 10, 10);
  Array3D arr2 = Array3D::MakeArray(10, 10, 10);

  arr1[8][8][8] = 26;
  arr2[8][8][8] = 40;

  EXPECT_NO_THROW({
                    Array3D result = arr1 + arr2;
                    EXPECT_EQ(static_cast<uint32_t>(result[8][8][8]), 66);
                  });
}

TEST(Array3DTest, ThrowsWhenAddingArraysWithDifferentDimensions) {
  Array3D arr1 = Array3D::MakeArray(5, 5, 5);
  Array3D arr2 = Array3D::MakeArray(3, 3, 3);

  EXPECT_THROW({
                 Array3D result = arr1 + arr2;
               }, std::runtime_error);
}

TEST(Array3DTest, CheckDimensions) {
  Array3D arr1 = Array3D::MakeArray(90, 89, 88);
  EXPECT_EQ(arr1.GetFirstDimension(), 90);
  EXPECT_EQ(arr1.GetSecondDimension(), 89);
  EXPECT_EQ(arr1.GetThirdDimension(), 88);
}

TEST(Array3DTest, CheckDifference) {
  Array3D arr1 = Array3D::MakeArray(5, 5, 5);
  Array3D arr2 = Array3D::MakeArray(5, 5, 5);

  arr1[4][4][4] = 10;
  arr2[4][4][4] = 5;

  EXPECT_NO_THROW({
                    Array3D result = arr1 - arr2;
                    EXPECT_EQ(static_cast<uint32_t>(result[4][4][4]), 5);
                  });

}

TEST(Array3DTest, CanMultiplyArrayByNumber) {
  Array3D arr = Array3D::MakeArray(10, 10, 10);
  int32_t multiplier = 3;

  arr[8][8][8] = 20;

  EXPECT_NO_THROW({
                    Array3D result = arr * multiplier;
                    EXPECT_EQ(static_cast<uint32_t>(result[8][8][8]), 20 * multiplier);
                  });
}

TEST(Array3DTest, CanMultiplyArrayByZero) {
  Array3D arr = Array3D::MakeArray(10, 10, 10);
  int32_t multiplier = 0;

  arr[8][8][8] = 20;

  EXPECT_NO_THROW({
                    Array3D result = arr * multiplier;
                    EXPECT_EQ(static_cast<uint32_t>(result[8][8][8]), 0 * multiplier);
                  });
}

TEST(Array3DTest, CanSubtractArraysWithSameDimensions) {
  Array3D arr1 = Array3D::MakeArray(10, 10, 10);
  Array3D arr2 = Array3D::MakeArray(10, 10, 10);

  arr1[8][8][8] = 50;
  arr2[8][8][8] = 20;

  EXPECT_NO_THROW({
                    Array3D result = arr1 - arr2;
                    EXPECT_EQ(static_cast<uint32_t>(result[8][8][8]), 30);
                  });
}

TEST(Array3DTest, OutputStream) {
  const Array3D arr = Array3D::MakeArray(2, 2, 2);
  std::stringstream ss;

  arr[1][1][1] = 42;

  ss << arr;

  std::string expected_output = "0 0 \n0 0 \n\n0 0 \n0 42 \n\n";
  EXPECT_EQ(ss.str(), expected_output);
}

TEST(Array3DTest, InputStream) {
  Array3D arr = Array3D::MakeArray(10, 10, 10);
  std::stringstream ss;

  // Подготовка строки для ввода в массив
  std::string input = "0 0 \n0 0 \n\n0 0 \n0 42 \n\n";
  ss.str(input);

  ss >> arr;

  EXPECT_EQ(static_cast<uint32_t>(arr[1][1][1]), 42);
}
