#include "stdafx.h"
using namespace std;


size_t cutoff = 2000;

PARAMFUNC(ffunc) {
	using namespace IncExceptions;
	auto s = vec.size();
	switch (n) {
	case 0:
		if (s != 1)
			throw ParamException("Wrong number of argumets!");
		cutoff = static_cast<size_t>(stoi(vec[0]));
		break;
	}
}

int main(int argn, char * argv[])
{
	{
		using namespace IncExceptions;
		constexpr BaseParam bp[]{ {"c","cutoff","<N>","Don\'t reed first N steps"} };
		constexpr Param<sizeof(bp) / sizeof(BaseParam)> param(bp);
		try {
			param.TakeAgrs(argn, argv, ffunc);
		}
		catch (ParamException & ex) {
			cerr << ex.what() << endl;
			exit(1);
		}
		catch (...) {
			cerr << "Unknown error during initialisation!" << endl;
		}
	}
	nsShelxFile::ShelxData shelx;
	try {
		shelx.LoadXDATCAR(cutoff, 0);
	}
	catch (IncExceptions::OpenXDATCAR_Exception & ex) {
		cerr << ex.what() << endl;
		exit(1);
	}
	catch (IncExceptions::ReadXDATCAR_Exception & ex) {
		cerr << ex.what() << endl;
		exit(1);
	}
	catch (IncExceptions::ShelxDataException & ex) {
		cerr << ex.what() << endl;
		exit(1);
	}
	catch (...) {
		cerr << "Unknown error during reading XDATCAR!" << endl;
		exit(1);
	}

	return 0;
}