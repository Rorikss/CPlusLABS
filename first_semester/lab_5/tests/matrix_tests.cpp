#include <lib/Matrix3D.h>
#include <gtest/gtest.h>

TEST(Matrix3DTestSuite, DefaultIndexTest) {
    Matrix3D matrix = Matrix3D::make_array(3, 4, 5);
    matrix[1][1][1] = 5;
    ASSERT_EQ(matrix[1][1][1], 5);
}

TEST(Matrix3DTestSuite, MultiIndexTest) {
    Matrix3D matrix = Matrix3D::make_array(100, 100, 100);
    matrix[1][1][1] = 100;
    matrix[30][30][30] = 300;
    ASSERT_EQ(matrix[1][1][1], 100);
    ASSERT_EQ(matrix[30][30][30], 300);
}

TEST(Matrix3DTestSuite, RewriteValueTest) {
    Matrix3D matrix = Matrix3D::make_array(100, 100, 100);
    matrix[1][1][1] = 100;
    ASSERT_EQ(matrix[1][1][1], 100);
    matrix[1][1][1] = 5;
    ASSERT_EQ(matrix[1][1][1], 5);
    matrix[1][1][1] = 2;
    ASSERT_EQ(matrix[1][1][1], 2);
}

TEST(Matrix3DTestSuite, InvalidIndexTest) {
    Matrix3D matrix = Matrix3D::make_array(5, 5, 5);
    ASSERT_THROW(matrix[30][30][30], std::runtime_error);
}

TEST(Matrix3DTestSuite, NegativeValueTest) {
    Matrix3D matrix = Matrix3D::make_array(5, 5, 5);
    ASSERT_THROW(matrix[1][1][1] = -5, std::runtime_error);
}

TEST(Matrix3DTestSuite, LargeValueTest) {
    Matrix3D matrix = Matrix3D::make_array(5, 5, 5);
    ASSERT_THROW(matrix[1][1][1] = 1000000000, std::runtime_error);
}

TEST(Matrix3DTestSuite, OstreamTest) {
    Matrix3D matrix = Matrix3D::make_array(3, 4, 5);
    matrix[1][1][1] = 5;
    std::stringstream output;
    output << matrix[1][1][1];
    std::string expected_output = std::to_string(matrix[1][1][1]);
    ASSERT_EQ(output.str(), expected_output);
}

TEST(Matrix3DTestSuite, IstreamTest) {
    Matrix3D matrix = Matrix3D::make_array(3, 4, 5);
    std::istringstream input("42");
    auto cur_in = matrix[1][1][1];
    input >> cur_in;
    int expected_value = 42;
    ASSERT_EQ(matrix[1][1][1], expected_value);
}

TEST(Matrix3DTestSuite, SubTest) {
    Matrix3D matrix_a = Matrix3D::make_array(10, 10, 10);
    Matrix3D matrix_b = Matrix3D::make_array(10, 10, 10);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                matrix_a[i][j][k] = i + j + k + 2;
                matrix_b[i][j][k] = i + j + k;
            }
        }
    }
    Matrix3D sub = matrix_a - matrix_b;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                ASSERT_EQ(sub[i][j][k], 2);
            }
        }
    }
}

TEST(Matrix3DTestSuite, SumTest) {
    Matrix3D matrix_a = Matrix3D::make_array(10, 10, 10);
    Matrix3D matrix_b = Matrix3D::make_array(10, 10, 10);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                matrix_a[i][j][k] = i + j + k + 2;
                matrix_b[i][j][k] = i + j + k;
            }
        }
    }
    Matrix3D summa = matrix_a + matrix_b;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                ASSERT_EQ(summa[i][j][k], (i + j + k) * 2 + 2);
            }
        }
    }
}

TEST(Matrix3DTestSuite, ProductTest) {
    Matrix3D matrix_a = Matrix3D::make_array(10, 10, 10);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                matrix_a[i][j][k] = 5;
            }
        }
    }
    Matrix3D product = matrix_a * 100;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                ASSERT_EQ(product[i][j][k], 500);
            }
        }
    }
}

TEST(Matrix3DTestSuite, InvalidSizeSumTest) {
    Matrix3D matrix_a = Matrix3D::make_array(5, 5, 5);
    Matrix3D matrix_b = Matrix3D::make_array(7, 7, 7);
    ASSERT_THROW(matrix_b + matrix_a, std::runtime_error);
}

TEST(Matrix3DTestSuite, InvalidSizeSubTest) {
    Matrix3D matrix_a = Matrix3D::make_array(5, 5, 5);
    Matrix3D matrix_b = Matrix3D::make_array(7, 7, 7);
    ASSERT_THROW(matrix_b - matrix_a, std::runtime_error);
}

TEST(Matrix3DTestSuite, LargeValSumTest) {
    Matrix3D matrix_a = Matrix3D::make_array(5, 5, 5);
    Matrix3D matrix_b = Matrix3D::make_array(5, 5, 5);
    matrix_a[1][1][1] = 131000;
    matrix_b[1][1][1] = 131000;
    ASSERT_THROW(matrix_a + matrix_b, std::runtime_error);
}

TEST(Matrix3DTestSuite, NegativeSubTest) {
    Matrix3D matrix_a = Matrix3D::make_array(5, 5, 5);
    Matrix3D matrix_b = Matrix3D::make_array(5, 5, 5);
    matrix_a[1][1][1] = 5;
    matrix_b[1][1][1] = 1000;
    ASSERT_THROW(matrix_a - matrix_b, std::runtime_error);
}

TEST(Matrix3DTestSuite, NegativeMultTest) {
    Matrix3D matrix_a = Matrix3D::make_array(5, 5, 5);
    ASSERT_THROW(matrix_a * -10, std::runtime_error);
}

TEST(Matrix3DTestSuite, MultiFuncTest) {
    Matrix3D matrix_a = Matrix3D::make_array(2, 2, 2);
    Matrix3D matrix_b = Matrix3D::make_array(2, 2, 2);
    std::istringstream input("1 2 3 4 5 6 7 8");
    std::stringstream output;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                auto in = matrix_a[i][j][k];
                input >> in;
            }
        }
    }
    int answer[8]{1, 2, 3, 4, 5, 6, 7, 8};
    int cnt = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                ASSERT_EQ(matrix_a[i][j][k], answer[cnt]);
                cnt++;
            }
        }
    }
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                matrix_b[i][j][k] = 1;
            }
        }
    }
    Matrix3D summa = matrix_a + matrix_b;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                output << summa[i][j][k];
            }
        }
    }
    Matrix3D sub = matrix_a - matrix_b;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                output << sub[i][j][k];
            }
        }
    }
    ASSERT_EQ(output.str(), "2345678901234567");
}

TEST(Matrix3DTestSuite, CopyTest) {
    Matrix3D original = Matrix3D::make_array(2, 2, 2);
    Matrix3D copy = Matrix3D::make_array(2, 2, 2);
    original[1][1][1] = 28;
    copy = original;
    ASSERT_EQ(copy[1][1][1], 28);
    ASSERT_EQ(original[1][1][1], 28);
    copy[1][1][1] = 5;
    ASSERT_EQ(original[1][1][1], 28);
    ASSERT_EQ(copy[1][1][1], 5);
}

TEST(Matrix3DTestSuite, CoutCinAllTest) {
    Matrix3D matrix = Matrix3D::make_array(2, 2, 2);
    std::istringstream input("1 2 3 4 5 6 7 8");
    std::stringstream output;
    input >> matrix;
    output << matrix;
    ASSERT_EQ(output.str(), "1 2 \n3 4 \n\n5 6 \n7 8 \n\n");
}
