#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <functional>

#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TGaxis.h>
#include <TH1D.h>
#include <TStyle.h>

using namespace std;

double min(vector<double> P) {
	if (P.size() == 0) throw 0;
	double res = P[0];
	for (int i=1; i<P.size(); i++) {
		if (P[i] < res) res = P[i];
	}
	return res;
}
double max(vector<double> P) {
	if (P.size() == 0) throw 0;
	double res = P[0];
	for (int i=1; i<P.size(); i++) {
		if (P[i] > res) res = P[i];
	}
	return res;
}
void printv(vector<double> vec) {
	cout << "v[";
	for (int i =0; i<vec.size(); i++) cout << vec[i] << ", ";
	cout << "]";
}

vector<double> get_var(string v, int N) {
	double v1, v2, Na = 0.6022;;
	string s;
	vector<double> res;
	int var = 2;
	if (v == "-t") {
		ifstream file("graph_vra.txt");

		for (int i=0; i<3; i++) {
			getline(file, s);
		}
		while (file >> v1 >> v2) {
			res.push_back(v1);
		}
	} else if (v == "Vm") {
		ifstream file("graph_vra.txt");

		for (int i=0; i<3; i++) {
			getline(file, s);
		}
		while (file >> v1 >> v2) {
			res.push_back(v2*Na / N);
		}
	} else {
		ifstream file("graph_"+v+".txt");

		for (int i=0; i<3; i++) {
			getline(file, s);
		}
		while (file >> v1 >> v2) {
			res.push_back(v2);
		}
	}
	return res;
}



Color_t cor_get(string cor_) {
	map<string, Color_t> cor;
	cor["r"] = kRed;
	cor["red"] = kRed;
	cor["Red"] = kRed;

	cor["blue"] = kBlue;
	cor["Blue"] = kBlue;

	cor["b"] = kBlack;
	cor["black"] = kBlack;
	cor["Black"] = kBlack;

	cor["g"] = kGreen;
	cor["green"] = kGreen;
	cor["Green"] = kGreen;
	return cor[cor_];
}

int main(int argc, char *argv[]) {
	TApplication App("A", nullptr, nullptr);
	TCanvas C("C", "V", 1500, 800);

	int N = stoi(argv[1]);
	string type = argv[2];
	if (type == "g") {
		vector<double> x, v1 = get_var(argv[3], N);
		vector<double> y, v2 = get_var(argv[4], N);
		//printv(v1);
		//printv(v2);
		Color_t color= cor_get(argv[5]);
		string titlece = argv[6];
		if (argc > 8) {
			double xi = stoi(argv[7]), xf = stoi(argv[8]);
			for (int i=xi; i<xf; i++) {
				x.push_back(v1[i]);
				y.push_back(v2[i]);
			}
		} else {
			x=v1;
			y=v2;
		}
		//printv(x);
		//printv(y);
		cout << x.size();
		cout << y.size();


		TGraph gg(x.size(), x.data(), y.data());
		//gg->GetYaxis()->SetMaxDigits(2);
		gg.SetLineWidth(3);
		gg.SetLineColor(color);
		//sy+" vs "+sx+";"+sx+";"+sy
		//Grafico V vs \"t\"; \"t\" / step; V / #AA ^3
		gg.SetTitle(titlece.data());
		gg.Draw("AL");
		gg.GetXaxis()->Set(1, min(x), max(x));
		//gg.GetXaxis()->ChangeLabel(0, -1, -1.,-1, kWhite);

		C.Update();
		C.SaveAs("graph.png");
		C.WaitPrimitive();

	} else if (type == "h") {
		vector<double> y, v2 = get_var(argv[3], N);
		Color_t color= cor_get(argv[4]);
		string titlece = argv[5];

		double yi = stoi(argv[6]), yf = stoi(argv[7]);

		if (argc > 9) {
			double xi = stoi(argv[8]), xf = stoi(argv[9]);
			cout << v2.size();
			for (int i=xi; i<xf; i++) {
				y.push_back(v2[i]);
			}
		} else y=v2;

		int nbins = 100;
		if (argc > 10) nbins = stoi(argv[10]);
		if (argc > 11) gStyle->SetOptStat(argv[11]);

		//Histograma do volume molar; Vm / cm^3/mol
		TH1D gg("Dados", titlece.data(), nbins, yi, yf);
		for (int i=0; i<y.size(); i++) {
			gg.Fill(y[i]);
		}
		gg.SetLineColor(color+1);
		gg.SetLineWidth(4);
		gg.SetFillColor(color);
		gg.SetFillStyle(1001);
		gg.Draw();
		gg.SetMinimum(0);
		//gg->GetXaxis()->ChangeLabel(0, -1, -1.,-1, kWhite);
		C.Update();
		C.SaveAs("TH1D_.png");
		C.WaitPrimitive();
	} else throw 0;
}
