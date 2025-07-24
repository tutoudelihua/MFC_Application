#include "CurveFitting.h"

void CurveFitting::PolynomialFunction::curveFitting(std::vector<std::pair<int, double>> tmpDataHC,
	std::vector<std::pair<int, double>> tmpDataNO) {

	std::vector<double> x;
	std::vector<double> y;
	x.push_back(0), y.push_back(0);
	for (auto& pair : tmpDataHC) {
		x.push_back(pair.second);//吸光度
		y.push_back(pair.first);//浓度
	}
	int degree = this->degree;
	std::vector<double> coefficientsHC = polynomialFit(x, y, degree);
	double S = 0;
	std::cout << "\n多项式拟合HC曲线: " << std::endl;
	bool flag = false;
	int k = 0, t = 0;
	for (int i = 0; i < x.size(); ++i) {
		double tmp = 0;
		t = 0;
		for (auto& coeff : coefficientsHC) {
			if (!flag) std::cout << "(" << coeff << ")" << "x^" <<
				k++ << (k < degree ? " + " : "\n");
			tmp += coeff * pow(x[i], t++);
		}
		flag = true;
		S += (y[i] - tmp) * (y[i] - tmp);
	}
	std::cout << "HC拟合残差平方和: " << S / x.size() << std::endl;
	x.clear(), y.clear();
	x.push_back(0), y.push_back(0);
	for (auto& pair : tmpDataNO) {
		x.push_back(pair.second);
		y.push_back(pair.first);
	}
	degree = this->degree;
	std::vector<double> coefficientsNO = polynomialFit(x, y, degree);
	S = 0;
	std::cout << "\n多项式拟合NO曲线: " << std::endl;
	flag = false;
	k = 0;
	for (int i = 0; i < x.size(); ++i) {
		double tmp = 0;
		t = 0;
		for (auto& coeff : coefficientsNO) {
			if (!flag) std::cout << "(" << coeff << ")" << "x^" <<
				k++ << (k < degree ? " + " : "\n");
			tmp += coeff * pow(x[i], t++);
		}
		flag = true;
		S += (y[i] - tmp) * (y[i] - tmp);
	}
	std::cout << "NO拟合残差平方和: " << S / x.size() << std::endl;
}

double CurveFitting::PolynomialFunction::dotProduct(const std::vector<double>& v1, const std::vector<double>& v2) {
	double result = 0.0;
	for (size_t i = 0; i < v1.size(); ++i) {
		result += v1[i] * v2[i];
	}
	return result;
}

std::vector<double> CurveFitting::PolynomialFunction::solveLinearEquations(
	const std::vector<std::vector<double>>& A, const std::vector<double>& B) {

	int n = A.size();
	std::vector<std::vector<double>> AB(n, std::vector<double>(n + 1));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			AB[i][j] = A[i][j];
		}
		AB[i][n] = B[i];
	}

	for (int i = 0; i < n; ++i) {
		int maxRow = i;
		for (int k = i + 1; k < n; ++k) {
			if (fabs(AB[k][i]) > fabs(AB[maxRow][i])) {
				maxRow = k;
			}
		}
		std::swap(AB[i], AB[maxRow]);
		for (int k = i + 1; k < n; ++k) {
			double factor = AB[k][i] / AB[i][i];
			for (int j = i; j <= n; ++j) {
				AB[k][j] -= factor * AB[i][j];
			}
		}
	}

	std::vector<double> x(n);
	for (int i = n - 1; i >= 0; --i) {
		x[i] = AB[i][n];
		for (int k = i + 1; k < n; ++k) {
			x[i] -= AB[i][k] * x[k];
		}
		x[i] /= AB[i][i];
	}
	return x;
}

std::vector<double> CurveFitting::PolynomialFunction::polynomialFit(
	const std::vector<double>& x, const std::vector<double>& y, int degree) {
	int n = x.size();
	std::vector<std::vector<double>> A(degree + 1, std::vector<double>(degree + 1, 0.0));
	std::vector<double> B(degree + 1, 0.0);
	for (int i = 0; i <= degree; ++i) {
		for (int j = 0; j <= degree; ++j) {
			for (int k = 0; k < n; ++k) {
				A[i][j] += pow(x[k], i + j);
			}
		}
		for (int k = 0; k < n; ++k) {
			B[i] += pow(x[k], i) * y[k];
		}
	}
	return solveLinearEquations(A, B);
}