#pragma once
#include <cmath> 

namespace MSS {
	using namespace System::Data::SQLite;
	using namespace System::Diagnostics;

	/**
	 * ������ ��� ���������� ����� �� 4-�� �����
	 * ���������:
	 * +double a;
	 * �����: �������� ���������
	 */
	double Round(double a) {
		return ceil(10000 * a) / 10000;
	}

	/**
	 * ������ ����������� ������� �������������� ������� ���������
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int n - ���-�� ���������
	 * �����: �������� ���������
	 */
	double Average(double *measures, int n) {
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum += measures[i];
		}
		return sum / n;
	}

	/**
	 * ������ ����������� ������������������ ���������� ������� ���������
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int n - ���-�� ���������
	 * �����: �������� ���������
	 */
	double StdDeviation(double *measures, int n) {
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum += pow(measures[i] - Average(measures, n), 2);
		}
		return sqrt(sum / (n - 1));
	}

	/**
	 * ������ ����������� ������������������ ����������� ������� ���������
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int n - ���-�� ���������
	 * �����: �������� ���������
	 */
	double MeanSquareError(double *measures, int n) {
		return StdDeviation(measures, n) / sqrt(n);
	}

	/**
	 * ������� ����������� �� ����������� ������ ����������� ���������
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int n - ���-�� ���������
	 * �����: ������������ �����
	 */
	void Sort(double *measures, int n) {
		double temp;
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++)
				if (measures[j] > measures[j + 1]) {
					temp = measures[j];
					measures[j] = measures[j + 1];
					measures[j + 1] = temp;
				}
		}
	}

	/**
	 * ������� ������������ �������� �� i-�� ��������� ��������� �������� ��������� ������������
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int i - ����������
	 * +int n - ���-�� ���������
	 * +System::String^ q - ������� ����������
	 * �����: ������������ �����, ������� ��������(������ � ��)
	 */
	bool IsFailByRomanovsky(double *measures, int i, int n, System::String^ q) {
		double beta = abs(Average(measures, n) - measures[i]) / StdDeviation(measures, n);
		SQLiteConnection^ connect = gcnew SQLiteConnection("Data Source=tables.db3; Version=3;");
		connect->Open();
		SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT value FROM Romanovsky WHERE n=" + n + " AND q=" + q + ";", connect);
		SQLiteDataReader^ reader = cmd->ExecuteReader();
		reader->Read();
		double betaq = reader->GetDouble(0); //��������� �������� �������� ������������
		connect->Close();
		return beta > betaq;
	}

	/**
	 * ������� ��� ����������� ������� ��������������� ����������� ��������� ����
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int n - ���-�� ���������
	 * +System::String^ q - ������� ����������
	 * �����: ������������ �����, ������� ��������(������ � ��)
	 */
	bool IsSystematicError(double *measures, int n, System::String^ q) {
		double sum = 0;
		for (int i = 0; i < n - 1; i++) {
			sum += pow(measures[i + 1] - measures[i], 2);
		}
		double V = (sum / (2 * (n - 1))) / pow(StdDeviation(measures, n), 2);
		SQLiteConnection^ connect = gcnew SQLiteConnection("Data Source=tables.db3; Version=3;");
		connect->Open();
		SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT value FROM Abbe WHERE n="+n+" AND q="+q+";", connect); 
		SQLiteDataReader^ reader = cmd->ExecuteReader();
		reader->Read();
		double Vq = reader->GetDouble(0); //��������� �������� �������� ����
		connect->Close();
		return V < Vq;
	}

	/**
	 * ������ ��� ���������� ������������ �������� ������� ���������.
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int n - ���-�� ���������
	 * �����: �������� ���������
	 */
	double Min(double *measures, int n) {
		double min = measures[0];
		for (int i = 1; i < n; i++) {
			if (min > measures[i]) {
				min = measures[i];
			}
		}
		return min;
	}

	/**
	 * ������ ��� ���������� ������������� �������� ������� ���������.
	 * ���������:
	 * +double *measures - ������ ���������
	 * +int n - ���-�� ���������
	 * �����: ������������ �����
	 */
	double Max(double *measures, int n) {
		double max = measures[0];
		for (int i = 1; i < n; i++) {
			if (max < measures[i]) {
				max = measures[i];
			}
		}
		return max;
	}
}
/**
 * ������ ��� ���������� �������� ����
 * ���������:
 * +String q - ������� �����������
 * +int n - ���-�� ���������
 * �����: �������� ���������
*/
double GetAbbe(System::String^ q, int n) {
	if (q == "0.01") {
		return (0.786460084 - (7.06909515 / n) + (41.85569806 / pow(n, 2)) - (146.757431 / pow(n, 3)) + (247.8015024 / pow(n, 4)));
	}
	if (q == "0.05") {
		return (0.867561498 - (5.94495279 / n) + (37.40106356 / pow(n, 2)) - (137.710475 / pow(n, 3)) + (210.6458956 / pow(n, 4)));
	}
}