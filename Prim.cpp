/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Institute of CG & CAD, Tsinghua University.
 * All Rights Reserved.
 * 
 * @file    main.cpp
 * @brief   Solver01 class declaration.
 * 
 * This file declares ...
 * 
 * @version 1.0
 * @author  Jackie Pang
 * @e-mail: 15pengyi@gmail.com
 * @date    2012/10/11
 */

#include <iostream>

#include "USSolver.h"



#include<iostream>
#include<stack>
#include<iomanip>
using namespace std;

#define maxint 999999

// 判断结束条件，即所有结点进入MST时程序结束
bool ifEnd(bool* ifvisit, int v)
{
	for (int i=0; i<v; i++)
	{
		if(ifvisit[i] == false) // ifvisit中有未被访问过得结点则返回false
			return false;
	}
	return true;
}

// 用Prim算法求得最小生成树，Weightmatrix为权值矩阵，v为结点个数
void Prim(std::ostream &outputstream, int** Weightmatrix, int v)
{
	/*int* minlength = new int[v]; // minlength[i]存储到结点i的最小长度
	int* startnode = new int[v]; // startnode[i]存储权值为minlength[i]的起始结点
	bool* ifvisit = new bool[v]; // ifvisit[i]表示结点i是否加入了最小生成树*/
	int minlength[100], startnode[100];
	bool ifvisit[100];
	int weightsum = 0, minl = 0, minnode = 0; // weightsum记录最小生成树的权值，minl为临时存储的最小权值，minnode为临时最小权值所在边的尾结点
	for (int I=0; I<v; I++) // 数据初始化，以结点0为起始结点
	{
		ifvisit[I] = false;
		startnode[I] = 0;
		minlength[I] = Weightmatrix[0][I];
	}
	while (ifEnd(ifvisit, v) == false)
	{
		minl = maxint;
		for (int i=0; i<v; i++)
		{
			if (ifvisit[i] != true || ifvisit[startnode[i]] != true) // 比较的两结点至少有一个为被访问过
			{
				if (minlength[i] < minl)
				{
					minl = minlength[i];
					minnode = i;
				}
			}
		}
		minlength[minnode] = maxint;
		weightsum += minl;
		ifvisit[startnode[minnode]] = true;
		ifvisit[minnode] = true;
		Weightmatrix[startnode[minnode]][minnode] = maxint;
		Weightmatrix[minnode][startnode[minnode]] = maxint;
		outputstream << "[" << startnode[minnode] << " -> " << minnode << "]"; // 找到权值最小的结点则输出
		for (int j=0; j<v; j++) // 更新权值数组minlength,起始结点数组startnode
		{
			if (Weightmatrix[minnode][j] < minlength[j])
			{
				minlength[j] = Weightmatrix[minnode][j];
				startnode[j] = minnode;
			}
		}
	}
	outputstream << " " << weightsum;
}

void USSolver::Solve(std::istream &inputStream, std::ostream &outputStream)
{
	//请在此填充代码
	int v, e; // v代表定点个数，e代表边个数
	inputStream >> v;
	inputStream >> e;
	int *line1 = new int[v+1]; // 正向表第一行
	for (int i=0; i<(v+1); i++)
		inputStream >> line1[i];
	int *line2 = new int[e]; // 正向表第二行
	for (int i=0; i<e; i++)
		inputStream >> line2[i];
	int *r = new int[e];
	for (int i=0; i<e; i++) // 输入权重r[i]
		inputStream >> r[i];
	int **Weightmatrix = new int *[v]; // v*v的权值矩阵Weightmatrix
	for (int i=0; i<v; i++)
		Weightmatrix[i] = new int[v];
	for	(int i=0; i<v; i++) // 矩阵初始化
		for (int j=0; j<v; j++)
			Weightmatrix[i][j] = maxint;
	int temp = 0, next = 0;
	for (int k=0, g=0; k<v; k++)
	{
		temp = line1[k+1] - line1[k];
		for (int l=0; l<temp; l++)
		{
			Weightmatrix[k][line2[next]] = r[g];
			next++;
			g++;
		}
	}
	/*for (int i=0; i<v; i++) // 测试权值矩阵
	{
		for (int j=0; j<v; j++)
		{
			cout << Weightmatrix[i][j] << " ";
		}
		cout << endl;
	}*/
	Prim(outputStream, Weightmatrix, v);
}



int main(int argc, char *argv[])
{
	//可在此更改测试文件
    std::string fileName = "test101.txt"; 
    
    USSolver unofficialSolver(fileName);
    const std::string unofficialAnswer = unofficialSolver.GetAnswer();

    std::cout << "Your Answer:" << std::endl << unofficialAnswer << std::endl;
}