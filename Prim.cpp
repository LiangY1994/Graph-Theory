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

// �жϽ��������������н�����MSTʱ�������
bool ifEnd(bool* ifvisit, int v)
{
	for (int i=0; i<v; i++)
	{
		if(ifvisit[i] == false) // ifvisit����δ�����ʹ��ý���򷵻�false
			return false;
	}
	return true;
}

// ��Prim�㷨�����С��������WeightmatrixΪȨֵ����vΪ������
void Prim(std::ostream &outputstream, int** Weightmatrix, int v)
{
	/*int* minlength = new int[v]; // minlength[i]�洢�����i����С����
	int* startnode = new int[v]; // startnode[i]�洢ȨֵΪminlength[i]����ʼ���
	bool* ifvisit = new bool[v]; // ifvisit[i]��ʾ���i�Ƿ��������С������*/
	int minlength[100], startnode[100];
	bool ifvisit[100];
	int weightsum = 0, minl = 0, minnode = 0; // weightsum��¼��С��������Ȩֵ��minlΪ��ʱ�洢����СȨֵ��minnodeΪ��ʱ��СȨֵ���ڱߵ�β���
	for (int I=0; I<v; I++) // ���ݳ�ʼ�����Խ��0Ϊ��ʼ���
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
			if (ifvisit[i] != true || ifvisit[startnode[i]] != true) // �Ƚϵ������������һ��Ϊ�����ʹ�
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
		outputstream << "[" << startnode[minnode] << " -> " << minnode << "]"; // �ҵ�Ȩֵ��С�Ľ�������
		for (int j=0; j<v; j++) // ����Ȩֵ����minlength,��ʼ�������startnode
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
	//���ڴ�������
	int v, e; // v�����������e����߸���
	inputStream >> v;
	inputStream >> e;
	int *line1 = new int[v+1]; // ������һ��
	for (int i=0; i<(v+1); i++)
		inputStream >> line1[i];
	int *line2 = new int[e]; // �����ڶ���
	for (int i=0; i<e; i++)
		inputStream >> line2[i];
	int *r = new int[e];
	for (int i=0; i<e; i++) // ����Ȩ��r[i]
		inputStream >> r[i];
	int **Weightmatrix = new int *[v]; // v*v��Ȩֵ����Weightmatrix
	for (int i=0; i<v; i++)
		Weightmatrix[i] = new int[v];
	for	(int i=0; i<v; i++) // �����ʼ��
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
	/*for (int i=0; i<v; i++) // ����Ȩֵ����
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
	//���ڴ˸��Ĳ����ļ�
    std::string fileName = "test101.txt"; 
    
    USSolver unofficialSolver(fileName);
    const std::string unofficialAnswer = unofficialSolver.GetAnswer();

    std::cout << "Your Answer:" << std::endl << unofficialAnswer << std::endl;
}