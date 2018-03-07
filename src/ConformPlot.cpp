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
	ios::sync_with_stdio(false);
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
	vector<Point> fPos;
	std::vector<std::vector<Point> > data;
	{
		try {
			data = shelx.LoadXDATCAR(cutoff, &fPos);
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
	}
	while (true) {
		char ch;
		cout << "Choose option:\n"
			<< "(1)  Add distance\n"
			<< "(2)  Add angle\n"
			<< "(3)  Add dihedral angle\n"
			<< "(4)  Add mean plane deviation\n"
			<< "(0)  Calculate and create result.csv\n\n"
			<< "Enter your choise: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear();
		cin >> ch;
		switch (ch) {
		case '1':
		case '2':
		case '3':
		case '4':
			//+

			continue;
		case '0':
			break;
		default:
			cerr << "Wrong choise! Try again." << endl;
			continue;
		}
		break;
	}
	return 0;
}
std::vector<flo> Distance(std::vector<std::vector<Point> > & data, size_t a, size_t b) {
	vector<flo> out;
	size_t size = data[0].size();
	out.reserve(size);
	for (size_t i = 0; i < size; i++)
	{
		out.push_back((data[a][i] - data[b][i]).r());
	}
	return std::move(out);
}
std::vector<flo> Angle(std::vector<std::vector<Point> > & data, size_t a, size_t b, size_t c) {

}
std::vector<flo> DihedralAngle(std::vector<std::vector<Point> > & data, size_t a, size_t b, size_t c, size_t d) {

}
std::vector<flo> MplnDev(std::vector<std::vector<Point> > & data, std::vector<size_t> & in) {

}