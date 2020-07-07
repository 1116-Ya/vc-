#pragma once

void othello();
void board(int data[8][8]);
void field();
int Choice(int data[8][8], int x, int y, int put);
void Check(int data[8][8], int x, int y, int i, int j);
void Enemy_Choice(int data[8][8]);
void Enemy_Check(int data[8][8], int x, int y, int i, int j);
int valuation(int data[8][8]);
void putCheck(int data[8][8]);
void Finish(int data[8][8]);
int putValue(int data[8][8], int Next); 