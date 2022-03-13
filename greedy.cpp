#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

std::set<int> set_init(int N) {
	std::set<int> sets;
	for (int i = 0; i < N; i++) {
		sets.insert(i);
	}
	return sets;
}

int argmin(const std::vector<double>& qs) {
	double ans = qs[0];
	int	 min_q = 0;
	for (int i = 0; i < qs.size(); i++) {
		if (qs[i] < 0) continue;
		if (qs[i] < ans or ans < 0) {
			min_q = i;
			ans = qs[i];
		}
	}
	return min_q;
}

struct cover {
	std::set<int> mems;
	int cost;
	bool is_used;
};

int main() {
	std::ifstream in("tmp.data");
	int N, M; // N - количество множеств, M - число покрытий
	std::string line;
	in >> N >> M;	
	getline(in, line);
	std::vector<cover> covers(M);
	for (int i = 0; i < M; i++) {
		getline(in, line);
		std::stringstream line_read(line);
		line_read >> covers[i].cost;
		covers[i].is_used = false;
		int s;
		while (line_read >> s) {
			covers[i].mems.insert(s);
		}
	}
	auto sets = set_init(N);
	while (!sets.empty()) {
		std::vector<double> qs(M);
		/*std::cout << "set: ";
		for (const auto c: sets) {
			std::cout << c << " ";
		}
		std::cout << "\n";*/
		for (int i = 0; i < M; i++) {
			if (covers[i].is_used) {
				qs[i] = -1.;
				continue;
			}
			std::vector<int> sets_inter;
			std::set_intersection(sets.begin(), sets.end(),
					covers[i].mems.begin(), covers[i].mems.end(),
					std::back_inserter(sets_inter));
			if (sets_inter.size() == 0) {
				qs[i] = -1.;
			} else {
				qs[i] = static_cast<double>(covers[i].cost) / sets_inter.size();
			}
		}
		int to_get = argmin(qs);
		/*std::cout << "qs: ";
		for (const auto q: qs) {
			std::cout << q << " ";
		}
		std::cout << "\n";*/
		covers[to_get].is_used = true;
		//std::cout << "to_get: " << to_get << "\n";
		for (auto a: covers[to_get].mems) {
			sets.erase(a);
		}
	}
	int ans = 0;
	for (const auto& cov: covers) {
		if (cov.is_used) {
			ans += cov.cost;
		}
	}
	std::cout << ans << " 0\n";
	for (const auto& cov: covers) {
		std::cout << cov.is_used << " ";
	}
	std::cout << "\n";
}
