#include "pch.h"
#include "CCoeff.h"

CCoeff::CCoeff()
{
}

CCoeff::~CCoeff()
{
}

std::vector<double> CCoeff::getCoeff()
{
	return coeff;
}

void CCoeff::setCoeff(std::vector<double> coeff)
{
	this->coeff = coeff;
}