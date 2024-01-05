#include <iostream>
#include <functional>
#include <lib/Matrix3D.h>

using namespace std;

int main() {
    cout << "Hello, world!\n";
    Matrix3D matrix = Matrix3D::make_array(3, 4, 5);
    Matrix3D reality = Matrix3D::make_array(3, 4, 5);
    matrix[1][1][1] = 5;
    cout << matrix[1][1][1] << endl;
    reality[1][1][1] = 3;
    cout << reality[1][1][1] << endl;
    Matrix3D summ = matrix + reality;
    cout << summ[1][1][1] << "\n";
    Matrix3D product = summ * 2;
    cout << product[1][1][1] << endl;
    Matrix3D sub = matrix - reality;
    cout << sub[1][1][1] << endl;
    return 0;
}