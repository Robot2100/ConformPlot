#include "stdafx.h"
using namespace std;

std::vector<flo> Distance(const std::vector<std::vector<Point> > & data, const size_t a, const size_t b, const Matrix & mat);



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
	std::vector<std::vector<Point> > vdata;
	{
		try {
			vdata = shelx.LoadXDATCAR(cutoff, &fPos);
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
			<< "(0)  Close program\n\n"
			<< "Enter your choise: ";
		cout.flush();
		cin.clear();
		cin >> ch;
		switch (ch) {
		case '1':
		{
			int n1, n2;
			cout << "First atom number (-1 to exit): ";
			cout.flush();
			while (!(cin >> n1)) {
				if (n1 = -1) break;
				cout << "Wrong input! Try again (-1 to exit): ";
			}
			cout << "Second atom number (-1 to exit): ";
			cout.flush();
			while (!(cin >> n2)) {
				if (n2 = -1) break;
				cout << "Wrong input! Try again (-1 to exit): ";
			}
			auto output = Distance(vdata, n1, n2, shelx.cell.FracToCart());
			string name("d_");
			name += to_string(n1) + '_' + to_string(n2) + ".csv";
			ofstream o(name, ios::trunc);
			auto size = output.size();
			for (size_t i = 0; i < size; i++)
			{
				o << i << ';' << output[i] << endl;
			}
		}
		case '2':
		case '3':
		case '4':
			//+

			continue;
		case '0':
			break;
		default:
			cout << "Wrong choise! Try again." << endl;
			continue;
		}
		break;
	}
	return 0;
}
std::vector<flo> Distance(const std::vector<std::vector<Point> > & data, const size_t a, const size_t b, const Matrix & mat) {
	vector<flo> out;
	Point shift(0, 0, 0);
	for (size_t i = 0; i < 3; i++)
	{
		auto di = data[a][0].a[i] - data[b][0].a[i];
		if (abs(di) > 0.5) {
			if (di < 0) {
				shift.a[i] = 1;
			}
			else {
				shift.a[i] = -1;
			}
		}
	}
	size_t size = data[0].size();
	out.reserve(size);
	if (shift == Point(0, 0, 0)) {
		for (size_t i = 0; i < size; i++)
		{
			out.push_back((mat*(data[a][i] - data[b][i])).r());
		}
	}
	else {
		for (size_t i = 0; i < size; i++)
		{
			out.push_back((mat*(data[a][i] + shift - data[b][i])).r());
		}
	}
	return std::move(out);
}
std::vector<flo> Angle(std::vector<std::vector<Point> > & data, size_t a, size_t b, size_t c, const Matrix & mat) {
	vector<flo> out;
	Point shift(0, 0, 0);
	for (size_t i = 0; i < 3; i++)
	{
		auto di = data[a][0].a[i] - data[b][0].a[i];
		if (abs(di) > 0.5) {
			if (di < 0) {
				shift.a[i] = 1;
			}
			else {
				shift.a[i] = -1;
			}
		}
	}
	size_t size = data[0].size();
	out.reserve(size);
	if (shift == Point(0, 0, 0)) {
		for (size_t i = 0; i < size; i++)
		{
			out.push_back((mat*(data[a][i] - data[b][i])).r());
		}
	}
	else {
		for (size_t i = 0; i < size; i++)
		{
			out.push_back((mat*(data[a][i] + shift - data[b][i])).r());
		}
	}
	for (size_t i = 0; i < 3; i++)
	{
		auto di = data[a][0].a[i] - data[b][0].a[i];
		if (abs(di) > 0.5) {
			if (di < 0) {
				shift.a[i] += 1;
			}
			else {
				shift.a[i] += -1;
			}
		}
	}
	out.reserve(size);
	if (shift == Point(0, 0, 0)) {
		for (size_t i = 0; i < size; i++)
		{
			out.push_back((mat*(data[a][i] - data[b][i])).r());
		}
	}
	else {
		for (size_t i = 0; i < size; i++)
		{
			out.push_back((mat*(data[a][i] + shift - data[b][i])).r());
		}
	}
	return std::move(out);
}
std::vector<flo> DihedralAngle(std::vector<std::vector<Point> > & data, size_t a, size_t b, size_t c, size_t d, const Matrix & mat) {
	return std::vector<flo>();
}
std::vector<flo> MplnDev(std::vector<std::vector<Point> > & data, std::vector<size_t> & in, const Matrix & mat) {
	return std::vector<flo>();
}