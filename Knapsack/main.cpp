#include <iostream>
#include <vector>
#include <fstream>

struct Item {
	int value, weight;
};

int main() {
   std::ifstream in("tmp.data");
   int capacity, item_count;
   in >> item_count >> capacity;
   std::vector<Item> items(item_count);
   for (auto& item: items) {
   		in >> item.value >> item.weight;	
   }
   in.close();
   std::vector<std::vector<int>> lines(2, std::vector<int>(capacity + 1, 0));
   std::vector<std::vector<bool>> taken(capacity+1, std::vector<bool>(item_count, false));
   for (int i = 1; i <= item_count; i++) {
   		int line2 = i % 2;
   		int line1 = 1 - line2;	
   		for (int j = 0; j <= capacity; j++) {
   				if (j - items[i - 1].weight < 0) {
   					lines[line2][j] = lines[line1][j];
   				} else {
   					if (lines[line1][j] > lines[line1][j - items[i - 1].weight] + items[i - 1].value) {
   						// не берем
   						lines[line2][j] = lines[line1][j];
   					} else {
   						// берем
   						lines[line2][j] = lines[line1][j - items[i - 1].weight] + items[i - 1].value;
   						taken[j][i - 1] = true;
   					}
   				}
   		}
   }
   int value = std::max(lines[0][capacity], lines[1][capacity]);
   std::cout << value << " " << 1 << "\n";
   std::vector<bool> take(item_count, false);
   int k = capacity;
   for (int i = item_count - 1; i >= 0; i--) {
   		take[i] = taken[k][i];
   		if (taken[k][i])
   			k -= items[i].weight;
   }
   for (auto t: take) std::cout << t << " ";
   std::cout << "\n";
}
