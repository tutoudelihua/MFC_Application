#pragma once
#ifndef _CURVESFITTING_H
#define _CURVESFITTING_H

#include <iostream>
#include <Eigen/eigen>
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace CurveFitting {

	struct Point {
		int x;
		double y;
	};

	static class LeastSquaresFunction {
	public:
		void setDegree(int degree) {
			this->degree = degree;
		}

		void CalculateData(
			std::vector<std::pair<int, double>> tmpData, std::vector<double>& coeff);

		Eigen::Array<double, Eigen::Dynamic, 1> leastSquares(
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A,
			Eigen::Matrix<double, Eigen::Dynamic, 1> B);

		Eigen::Array<double, Eigen::Dynamic, 1> reweightedLeastSquares(
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A,
			Eigen::Matrix<double, Eigen::Dynamic, 1> B,
			Eigen::Array<double, Eigen::Dynamic, 1> vectorW);

		Eigen::Array<double, Eigen::Dynamic, 1> iterativeReweightedLeastSquares(
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A,
			Eigen::Matrix<double, Eigen::Dynamic, 1> B, double p, int kk);

	private:
		int degree;
	};

	static class PolynomialFunction {
	public:
		void setDegree(int degree) {
			this->degree = degree;
		}

		void curveFitting(std::vector<std::pair<int, double>> tmpDataHC,
			std::vector<std::pair<int, double>> tmpDataNO);

		double dotProduct(const std::vector<double>& v1, const std::vector<double>& v2);

		std::vector<double> solveLinearEquations(
			const std::vector<std::vector<double>>& A, const std::vector<double>& B);

		std::vector<double> polynomialFit(
			const std::vector<double>& x, const std::vector<double>& y, int degree);

	private:
		int degree;
	};
};

#endif // _CURVESFITTING_H