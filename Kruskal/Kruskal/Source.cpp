#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<cstdio>
#include<iomanip>
#include<thread>
#include<omp.h>
#include<fstream>


using namespace std;

typedef struct //khai báo các biến cần thiết cho thuật toán Kruskal
{
	int dinhDau, dinhCuoi, trongSo;
}Canh;

typedef Canh MC[2000]; //tạo mảng cạnh

//void Nhap(int &n, int &m, MC &a) //sua &m
//{
//	printf("\nSo dinh n="); //Nhập số đỉnh
//	scanf("%d", &n);
//	printf("So canh m="); //Nhập số cạnh
//	scanf("%d", &m);
//	for (int i = 1; i <= m; i++) //sua i<=m
//	{
//		printf("Dinh dau:"); scanf("%d", &a[i].d);
//		printf("Dinh cuoi:"); scanf("%d", &a[i].c);
//		printf("Trong so:"); scanf("%d", &a[i].ts);
//	}
//}
void docFile(int &n, int &m, MC &a)
{
	char * fileName = "C:\\Users\\Dell\\Desktop\\ParallelMST--master\\test1.txt";
	int rong;
	ifstream in;
	in.open(fileName);
	if (in)
	{
		in >> n;
		in >> m;
		for (int i = 1; i <= m; i++)
		{
			in >> a[i].dinhDau;
			in >> a[i].dinhCuoi;
			in >> a[i].trongSo;
		}
	}
	in.close();
}

void SapXep(int m, MC a)
{
	for (int i = 1; i <= m; i++) //Sắp xếp các cạnh theo thứ tự tăng dần
	{
		for (int j = 1 + i; j <= m; j++) //duyệt tất cả các cạnh
		if (a[i].trongSo>a[j].trongSo)
		{
			Canh c = a[i];
			a[i] = a[j];
			a[j] = c;
		}
	}
}

//tuần tự
void Kruskal_Sequential(int n, int m, MC a)
{
	int d[100], i, k = 0, T = 0;
	for (i = 1; i <= n; i++)
		d[i] = -i;

	SapXep(m, a);

	for (int j = 1; j <= m; j++)
	{
		if (d[a[j].dinhDau] != d[a[j].dinhCuoi])
		{
			cout << "\nChon canh (" << a[j].dinhDau << "," << a[j].dinhCuoi << ") Trong so:" << a[j].trongSo;
			T = T + a[j].trongSo; // chọn trọng số tối ưu
			if (d[a[j].dinhDau]<0 && d[a[j].dinhCuoi]<0)
			{
				k++;
				d[a[j].dinhDau] = d[a[j].dinhCuoi] = k;
			}
			else
			{
				if (d[a[j].dinhDau]>0 && d[a[j].dinhCuoi]>0)
				{
					int x = d[a[j].dinhCuoi];
					for (i = 1; i <= n; i++)
					{
						if (d[i] == x)
						{
							d[i] = d[a[j].dinhDau]; //sua a[j].d
						}
					}
				}
				else
					d[a[j].dinhDau] = d[a[j].dinhCuoi] = d[a[j].dinhDau]>d[a[j].dinhCuoi] ? d[a[j].dinhDau] : d[a[j].dinhCuoi];
			}
		}
	}
	cout << "\nTong trong so nho nhat la:" << T << endl;
}

// song song
void Kruskal_Parallel(int n, int m, MC a)
{
	int d[100], i, k = 0, T = 0;

#pragma omp parallel
	for (i = 1; i <= n; i++) d[i] = -i;
	SapXep(m, a);

#pragma omp for
	for (int j = 1; j <= m; j++)
	{
		if (d[a[j].dinhDau] != d[a[j].dinhCuoi])
		{
			cout << "\nChon canh (" << a[j].dinhDau << "," << a[j].dinhCuoi << ") Trong so:" << a[j].trongSo;
			T = T + a[j].trongSo;
			if (d[a[j].dinhDau]<0 && d[a[j].dinhCuoi]<0)
			{
				k++;
				d[a[j].dinhDau] = d[a[j].dinhCuoi] = k;
			}
			else
			{
				if (d[a[j].dinhDau] > 0 && d[a[j].dinhCuoi] > 0)
				{
					int x = d[a[j].dinhCuoi];
					//#pragma omp for
					for (i = 1; i <= n; i++)
					{
						if (d[i] == x)
							d[i] = d[a[j].dinhDau];
					}
				}
				else
					d[a[j].dinhDau] = d[a[j].dinhCuoi] = d[a[j].dinhDau] > d[a[j].dinhCuoi] ? d[a[j].dinhDau] : d[a[j].dinhCuoi];
			}
		}
	}
	cout << "\nTong trong so nho nhat la: " << T;
}

int main()
{
	cout << " THUAT TOAN KRUSKAL \n";

	MC a;
	int m, n;
	//Nhap(n, m, a);
	docFile(n, m, a);
	cout << "So Dinh =" << n << "\n";
	cout << "So Dinh =" << m << "\n";

	//tuần tự
	clock_t begint = clock(); 
	Kruskal_Sequential(n, m, a);
	clock_t endt = clock(); 


	//songsong
	clock_t begins = clock();
	Kruskal_Parallel(n, m, a);
	clock_t ends = clock();

	cout << "\n\nThoi gian thuc hien tuan tu: " << (float)(endt - begint) / CLOCKS_PER_SEC << " s" << endl;

	cout << "Thoi gian thuc hien song song: " << (float)(ends - begins) / CLOCKS_PER_SEC << " s" << endl;

	cout << "So nhan CPU:" << omp_get_max_threads();

	_getch();
}