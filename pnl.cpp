#include "pnl.hpp"

CSV::CSV(const char* fn){
	file.open(fn);
	assert(file.good());
	while (hasNext()){
		getNext();
	}
	getPNL();
}

bool CSV::hasNext(){
	if (file.good()==0) return false;
	else return true;
}

void CSV::getNext(){
	std::string line;
	std::string value;
	std::getline(file,line);
	if (line.size()==0) return;
	std::stringstream line_ss(line);
	ROW row;
	std::getline(line_ss,value,',');
	row.id = value;
	std::getline(line_ss,value,',');
	row.sym = value;
	std::getline(line_ss,value,',');
	row.date = value;
	std::getline(line_ss,value,',');
	row.price = std::stod(value);
	std::getline(line_ss,value,',');
	row.qty = std::stod(value);
	std::getline(line_ss,value,',');
	row.total = std::stod(value);
	
	data.push_back(row);
}

void CSV::layout(int n=5, int col_w=20){
	
	std::cout << std::left << std::setw(col_w) << "account_id";
	std::cout << std::left << std::setw(col_w) << "symbol";
	std::cout << std::left << std::setw(col_w) << "trade_date";
	std::cout << std::left << std::setw(col_w) << "price";
	std::cout << std::left << std::setw(col_w) << "quantity";
	std::cout << std::left << std::setw(col_w) << "notional";
	std::cout << std::left << std::setw(col_w) << "remain_qty";
	std::cout << std::left << std::setw(col_w) << "pnl";
	std::cout << std::endl;
	
	for (int i=0; i<n; ++i){
		std::cout << std::left << std::setw(col_w) << data[i].id;
		std::cout << std::left << std::setw(col_w) << data[i].date;
		std::cout << std::left << std::setw(col_w) << data[i].sym;
		std::cout << std::left << std::setw(col_w) << data[i].price;
		std::cout << std::left << std::setw(col_w) << data[i].qty;
		std::cout << std::left << std::setw(col_w) << data[i].total;
		std::cout << std::left << std::setw(col_w) << data[i].remain_qty;
		std::cout << std::left << std::setw(col_w) << data[i].pnl;
		std::cout << std::endl;
	}
}

void CSV::getPNL(){
	auto& f = data[0];
	
	f.remain_qty = f.qty;
	f.cost = f.price;
	f.cost_total = f.total;
	f.pnl = 0;
	
	for (int i=1;i<data.size();++i){
		auto& p=data[i-1];
		auto& r=data[i];
		if (r.sym != p.sym){
			r.remain_qty = r.qty;
			r.cost = r.price;
			r.cost_total = r.total;
			r.pnl = 0;
			continue;
		}
		if (abs(p.remain_qty+r.qty) == abs(p.remain_qty) + abs(r.qty)){
			r.pnl = 0;
		}
		else {
			if ( abs(r.qty) <= abs(p.remain_qty) ){
				r.pnl = r.qty*(p.cost-r.price);
			}
			else {
				r.pnl = p.remain_qty*(r.price-p.cost);
			}
		}
		r.remain_qty = p.remain_qty + r.qty;
		r.cost_total = p.cost_total + r.total + r.pnl;
		r.cost = r.cost_total / r.remain_qty;
		
	}
	return;
}

void CSV::to_csv(const char* fn){
	std::ofstream ofs(fn);
	assert(ofs.good());
	ofs << "account_id,symbol,trade_date,price,quantity,notional,remain_qty,pnl\n";
	for (auto& r:data){
		ofs<<r.id<<',';
		ofs<<r.sym<<',';
		ofs<<r.date<<',';
		ofs<<r.price<<',';
		ofs<<r.qty<<',';
		ofs<<r.total<<',';
		ofs<<r.remain_qty<<',';
		ofs<<r.pnl;
		ofs<<std::endl;
	}
	ofs.close();
}


using namespace std;
int main(int argc,char* argv[]){
	CSV csv(argv[1]);
	csv.to_csv("pnl.csv");
	return 0;
}