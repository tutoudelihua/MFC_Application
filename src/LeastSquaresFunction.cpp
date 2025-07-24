#include "CurveFitting.h"

void CurveFitting::LeastSquaresFunction::CalculateData(
	std::vector<std::pair<int, double>> tmpData, std::vector<double>& coeff) {

	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A;
	Eigen::Matrix<double, Eigen::Dynamic, 1> B;
	CurveFitting::LeastSquaresFunction LSF;

	std::vector<double> X;
	std::vector<double> Y;
	X.push_back(0), Y.push_back(0);
	for (auto& pair : tmpData) {
		X.push_back(pair.second);//吸光度
		Y.push_back(pair.first);//浓度
	}
	int degree = this->degree;
	A.resize(tmpData.size() + 1, degree);
	B.resize(tmpData.size() + 1, 1);
	for (int i = 1; i <= tmpData.size() + 1; ++i) {
		for (int j = 0; j < degree; ++j) {
			A.coeffRef(i - 1, j) = pow(X[i - 1], j);
		}
	}
	for (int i = 0; i < tmpData.size() + 1; ++i) {
		B.coeffRef(i) = Y[i];
	}

	Eigen::Array<double, Eigen::Dynamic, 1> result = LSF.iterativeReweightedLeastSquares(
		A, B, 2, 100);

	for (int i = 0; i < result.size(); ++i) {
		coeff.push_back(result(i, 0));
	}
	return;
}

Eigen::Array<double, Eigen::Dynamic, 1> CurveFitting::LeastSquaresFunction::leastSquares(
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A,
	Eigen::Matrix<double, Eigen::Dynamic, 1> B) {

	int rows = A.rows();
	int col = A.cols();
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> AT;
	AT.resize(col, rows);

	Eigen::Array<double, Eigen::Dynamic, 1> x;
	x.resize(col, 1);

	AT = A.transpose();

	//x = (A^T * A)^-1 * A^T * B
	x = ((AT * A).inverse()) * (AT * B);
	return x;
}

Eigen::Array<double, Eigen::Dynamic, 1> CurveFitting::LeastSquaresFunction::reweightedLeastSquares(
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A,
	Eigen::Matrix<double, Eigen::Dynamic, 1> B,
	Eigen::Array<double, Eigen::Dynamic, 1> vectorW) {

	int rows = A.rows();
	int col = A.cols();
	if (vectorW.isZero()) {
		vectorW.resize(rows, 1);
		for (int i = 0; i < rows; ++i) {
			vectorW(i, 0) = 1;
		}
	}

	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> AT;
	AT.resize(col, rows);

	Eigen::Array<double, Eigen::Dynamic, 1> x;
	x.resize(col, 1);

	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> WT, W;
	W.resize(rows, rows);
	WT.resize(rows, rows);

	W = vectorW.matrix().asDiagonal();
	WT = W.transpose();
	AT = A.transpose();

	// x = (A^T * W^T * W * A)^-1 * A^T * W^T * W * B
	x = ((AT * WT * W * A).inverse()) * (AT * WT * W * B);
	return x;
}

/* 迭代重加权最小二乘（IRLS）  W为权重,p为范数
 * e = Ax - B
 * W = e^(p−2)/2
 * W²(Ax - B) = 0
 * W²Ax = W²B
 * (A^T * W^T * W * A) * x = A^T * W^T * W * B
 * x = (A^T * W^T * W * A)^-1 * A^T * W^T * W * B
 */
Eigen::Array<double, Eigen::Dynamic, 1> CurveFitting::LeastSquaresFunction::iterativeReweightedLeastSquares(
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A,
	Eigen::Matrix<double, Eigen::Dynamic, 1> B, double p, int kk) {
	CurveFitting::LeastSquaresFunction LSF;
	/* x(k) = q x1(k) + (1-q)x(k-1)
	 * q = 1 / (p-1)
	 */
	int rows = A.rows();
	int col = A.cols();

	double pk = 2;//初始同伦值
	double K = 1.5;

	double epsilong = 10e-9; // ε
	double delta = 10e-15; // δ
	Eigen::Array<double, Eigen::Dynamic, 1> x, _x, x1, e, w;
	x.resize(col, 1);
	_x.resize(col, 1);
	x1.resize(col, 1);
	e.resize(rows, 1);
	w.resize(rows, 1);

	for (int i = 0; i < rows; ++i) {
		w.coeffRef(i, 0) = 1;
	}
	x = LSF.reweightedLeastSquares(A, B, w);

	//迭代  最大迭代次数kk
	for (int i = 0; i < kk; ++i) {
		//保留前一个x值,用作最后比较确定收敛
		_x = x;
		if (p >= 2) {
			pk = std::min(p, K * pk);
		}
		else {
			pk = std::max(p, K * pk);
		}
		//偏差
		e = (A * x.matrix()) - B;
		//偏差的绝对值//  求矩阵绝对值 ：e = e.cwiseAbs(); 或 e.array().abs().matrix()
		e = e.abs();
		//对每个偏差值小于delta,用delta赋值给它
		for (int i = 0; i < e.rows(); ++i) {
			e(i, 0) = std::max(delta, e(i, 0));
		}
		//对每个偏差值进行幂操作
		w = e.pow(p / 2.0 - 1);
		w = w / w.sum();

		x1 = LSF.reweightedLeastSquares(A, B, w);

		double q = 1 / (pk - 1);
		if (p > 2) {
			x = x1 * q + x * (1 - q);
		}
		else {
			x = x1;
		}
		//达到精度,结束
		if ((x - _x).abs().sum() < epsilong) {
			return x;
		}
	}
	return x;
}