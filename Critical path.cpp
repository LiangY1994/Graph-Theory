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

struct HNode // �洢���
{
	int verdata; // �����
	int in; // ���
	struct Node *link;
};

struct Node // �洢��Ȩ
{
	int adjdata; // �����
	int weight; // Ȩֵ
	struct Node *next;
};

struct Info // ����Ŀ�е�������ʽת��Ϊ��㡢�յ㡢Ȩֵ��������ʽ
{
	int num; // �����
	int dut; // ��㵽�յ��Ȩֵ
	int pre[500]; // �˽���ǰ�����
	int prenum; // ǰ���������
};

struct Input // ����㡢�յ㡢Ȩֵ����ʽ�洢
{
	int head; // ���
	int tail; // �յ�
	int len; // Ȩֵ
};

void CreateGraph(std::istream &inputStream, HNode *&Adjlist, int n) // ����ͼ
{
	int temp = 0, first = 0, time = 0, flag = 0;
	int *judge = new int[n]; // �жϽ���Ƿ�Ϊβ���
	for (int i=0; i<n; i++) // ��ʼ��
		judge[i] = i;
	char ch;
	struct Info *q = new struct Info[n];
	for (int i=0; i<n; i++)
	{
		inputStream >> q[i].num >> q[i].dut;
		inputStream.get(ch);
		if(ch == '\n')
			first = i;
		else
		{
			q[i].prenum = 0;
			inputStream >> q[i].pre[q[i].prenum];
			for (int k=0; k<n; k++) // ����β�������ֵ��Ϊ-1
			{
				if(k == q[i].pre[q[i].prenum])
					judge[k] = -1;
			}
			q[i].prenum++;
			inputStream.get(ch);
			while (ch == ',')
			{
				inputStream >> q[i].pre[q[i].prenum];
				for (int k=0; k<n; k++)
				{
					if(k == q[i].pre[q[i].prenum])
						judge[k] = -1;
				}
				q[i].prenum++;
				inputStream.get(ch);
			}
		}
	}
	// �ٶ���i������Ϊ����i-1
	struct Input k[1000];
	struct Input kk;
	for (int i=0; i<n; i++)
	{
		if (i != first)
		{
			for (int j=0; j<q[i].prenum; j++)
			{
				k[time].head = q[i].pre[j];
				k[time].tail = q[i].num;
				k[time].len = q[q[i].pre[j]].dut;
				time++;
			}
		}
	}
	for (int l=0; l<n; l++)
	{
		if (judge[l] != -1)
		{
			k[time].head = l;
			k[time].tail = n;
			k[time].len = q[l].dut;
			time++;
		}
	}
	for (int i=0; i<time; i++)
	{
		for (int j=i; j<time; j++)
		{
			if (k[i].head > k[j].head)
			{
				kk.head = k[i].head;
				kk.tail = k[i].tail;
				kk.len = k[i].len;
				k[i].head = k[j].head;
				k[i].tail = k[j].tail;
				k[i].len = k[j].len;
				k[j].head = kk.head;
				k[j].tail = kk.tail;
				k[j].len = kk.len;
			}
		}
	}
	/*���Խṹ��Input�Ƿ�ת���ɹ�
	for (int m=0; m<time; m++)
		cout << k[m].head << " " << k[m].tail << " " << k[m].len << endl;
	//*/
	struct Node *p;
	for (int i=0; i<=n; i++)
	{
		Adjlist[i].verdata = i;
		Adjlist[i].in = 0;
		Adjlist[i].link = NULL;
	}
	for (int j=0; j<time; j++)
	{
		p = new struct Node; // �����µı߱���
		p->adjdata = k[j].tail;
		p->weight = k[j].len;
		p->next = Adjlist[k[j].head].link;
		Adjlist[k[j].head].link = p;
	}
	for (int l=0; l<=n; l++) // �������
	{
		p = Adjlist[l].link;
		while (p != NULL)
		{
			temp = p->adjdata;
			Adjlist[temp].in++;
			p = p->next;
		}
	}
	/*�����Ȳ���
	for (int i=1; i<=n; i++)
		cout << Adjlist[i].in << " ";
	*/
}

void PrintPath(std::ostream &outputStream, HNode *Adjlist, int *ve, int *vl, int *&path, int head, int tail)
{
	int i = 0, temp = 0;
	Node *p = NULL;
	temp = path[head-1];
	if (temp == tail) // ��Ҫ�����
	{
		outputStream << "[" << path[0];
		while (path[i] != tail)
		{
			outputStream << " " << "->" << path[i] << " ";
			i++;
		}
		outputStream << "-> "<< tail << "-> end]" << endl;
	}
	for (p=Adjlist[temp].link; p!=NULL; p=p->next)
	{
		if (ve[p->adjdata] == vl[p->adjdata])
		{
			path[head] = p->adjdata; // p->adjdata��ջ
			head++;
			PrintPath(outputStream, Adjlist, ve, vl, path, head, tail); // ����·��
			head--; // ��ջ
		}
	}
}

void CriticalPath(std::ostream &outputStream, HNode *Adjlist, int n) // �ҵ��ؼ�·��
{
	n++;
	int front = 0, rear = 0, flag = 0; // front��rear����ջ������flag�ж��Ƿ��л�·
	int j, k, l;
	int *ve = new int[n];
	int *vl = new int[n];
	int *stack = new int[n];
	struct Node *p;
	for (int i=0; i<n; i++)
	{
		ve[i] = 0;
		vl[i] = 0;
		if (Adjlist[i].in == 0) // Ѱ�����Ϊ0�Ľ����ջ
		{
			stack[rear] = i;
			rear++;
		}
	}
	while (front != rear) // ջ������ʼ
	{
		j = stack[front];
		front++;
		flag++;
		p = Adjlist[j].link; // pָ���һ���ڽӽ��
		while (p != NULL)
		{
			k = p->adjdata;
			Adjlist[k].in--; // ��ȼ�1
			if(ve[j]+p->weight > ve[k])
				ve[k] = ve[j] + p->weight; // �·������
			if (Adjlist[k].in == 0) // ���Ϊ0����ջ
			{
				stack[rear] = k;
				rear++;
			}
			p = p->next;
		}
	}
	if(flag < n)
	{
		outputStream << "Error���л�·,��������������" << endl;
		return;
	}
	for (int i=0; i<n; i++)
		vl[i] = ve[n-1];
	for (int i=n-2; i>=0; i--)
	{
		j = stack[i];
		p = Adjlist[j].link; // pָ����һ�ڽӽڵ�
		while (p != NULL)
		{
			k = p->adjdata;
			if(vl[k]-p->weight < vl[j]) // ���·���ȸ���
				vl[j] = vl[k] - p->weight;
			p = p->next;
		}
	}
	int *record = new int[n]; // record�洢�ؼ�·�����
	int *delay = new int[n]; // delay�洢������ʱ��
	int *path = new int[n]; // path���ڹؼ�·�������
	int m = 0, t = 0;
	outputStream << ve[n-1] << endl;
	for (int i=0; i<n; i++)
	{
		delay[i] = vl[i] - ve[i];
		if (delay[i] == 0)
		{
			record[m] = i;
			m++;
		}
	}
	m = m - 2; // ȥ�����һ����ʱ���
	path[0] = record[0];
	PrintPath(outputStream, Adjlist, ve, vl, path, 1, record[m]);
	for (int i=0; i<n-1; i++)
		outputStream << delay[i] << endl;
}

void USSolver::Solve(std::istream &inputStream, std::ostream &outputStream)
{
	outputStream << "Your Answer:" << endl;
	int n; // nΪ�������
	inputStream >> n;
	struct HNode *Adjlist = new struct HNode[n+1];
	CreateGraph(inputStream, Adjlist, n);
	CriticalPath(outputStream, Adjlist, n);
}

int main(int argc, char *argv[])
{
	//���ڴ˸��Ĳ����ļ�
    std::string fileName = "test101.txt"; 
    USSolver unofficialSolver(fileName);
    const std::string unofficialAnswer = unofficialSolver.GetAnswer();
    std::cout << unofficialAnswer;
	return 0;
}