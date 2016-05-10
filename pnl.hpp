#pragma once

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

class ROW{
public:
	std::string id;
	std::string date;
	std::string sym;
	double price;
	double qty;
	double total;
	double remain_qty;
	double cost;
	double cost_total;
	double pnl;
};

class CSV{
public:
	CSV(const char*);
	void to_csv(const char*);
	bool hasNext();
	void getNext();
	void layout(int,int);
	std::vector<ROW> data;
	
private:
	std::ifstream file;
	
	void getPNL();
};