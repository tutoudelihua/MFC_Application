#pragma once
#include <vector>
class CCoeff
{
	public:
		CCoeff();
		~CCoeff();
		void setCoeff(std::vector<double> coeff);
		std::vector<double> getCoeff();

	private:
		std::vector<double> coeff;
};

