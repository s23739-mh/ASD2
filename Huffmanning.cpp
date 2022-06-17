
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
class BigArray{
private:
	std::vector<std::vector<std::pair<std::string, unsigned int>>> allarrays;
public:
	BigArray() {
}
	void insertTree(std::vector<std::pair<std::string, unsigned int>> wektor) {
		allarrays.push_back(wektor);
	}
	std::vector<std::pair<std::string, unsigned int>> getTree(int i){
		return allarrays[i];
	}
	std::pair<std::string, unsigned int> getPair(int i, int j) {
		return allarrays[i][j];
	}
	int getSize() {
		return allarrays.size();
	}
	void invertTable() {
		for (int i = 0; i < (allarrays.size() / 2); i++) {
			std::vector<std::pair<std::string, unsigned int>> help;
			help = allarrays[i];
			allarrays[i] = allarrays[(allarrays.size() - (i+1))];
			allarrays[allarrays.size() - (i + 1)] = help;
		}
	}
};

std::vector<int> toDecimal(std::string text) {
	std::vector<std::string> zeros;
	std::vector<int> numbers;
	std::string help = "";
	int sum;

	for (int i = 0; i < text.length(); i++) {
		help += text[i];
		if ((((i+1) % 9)==0) && (i != 0)) {
			zeros.push_back(help);
			help = "";
		}
	}
	
	for (int i = 0; i < zeros.size(); i++) {
		sum = 0;
		for (int j = zeros[i].length(); j >0 ; j--) {
			if (zeros[i][j] == '1') {
				sum += pow(2, zeros[i].length()-j-1);
				
			}
		}
		numbers.push_back(sum);
	}

	return numbers;
}

std::string retrieveText(std::string path) {
	std::ifstream file;
	file.open("input.txt", std::ios::in);
	std::string line="";
	std::string text="";
	file >> line;
	if (file.good()) {
		while (!file.eof()) {
			file >> line;
			text += line;
		}
		
		return text;
	}
	throw std::invalid_argument("Plik sie nie otwiera poprawnie");
	return "nothing";
}

void saveText(std::string path, std::string text) {
	std::ofstream stream;
	stream.open(path);
	stream << text;
	stream.close();
}


std::pair <std::string, unsigned int> getOccurences(std::string text, int pivot) {
	int occ = 0;
	std::string helptext = "";
	char character = text[pivot];
	for (int i = pivot; i < text.length();i++) {
		if (text[i] == character) { occ++; }
	}
	helptext += character;
	return std::make_pair(helptext,occ);
}

std::vector<std::pair<std::string, unsigned int>> makeVector(std::string text) {
	bool washere = false;
	std::vector<std::pair<std::string, unsigned int>> list;
	for (int i = 0; i < text.length(); i++) {
		washere = false;
		for (int j = 0; j < list.size(); j++) {
			if(list[j].first[0]==text[i]){
				washere = true;
			}
		}
		if (!washere) {list.push_back(getOccurences(text, i)); }
	}
	return list;
}


void heapify(std::vector<std::pair<std::string, unsigned int>>& numbs, int size, int i) {
	int biggest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if (l < size && numbs[l].second > numbs[biggest].second)
	{
		biggest = l;
	}

	if (r < size && numbs[r].second > numbs[biggest].second)
	{
		biggest = r;
	}

	if (biggest != i) {
		{
			std::swap(numbs[i], numbs[biggest]);
			heapify(numbs, size, biggest);
		}

	}
}

void buildHeap(std::vector<std::pair<std::string, unsigned int>>& arr, int n)
{
	if (n > 1) {
	for (int i = n / 2; i >= 0; i--) {
		std::swap(arr[0], arr[i]);
		heapify(arr, n, i);
	}
}
}

void heapSort(std::vector<std::pair<std::string, unsigned int>>& numbs, int size) {
	if (size > 1) {
		for (int i = size / 2 - 1; i >= 0; i--)
			heapify(numbs, size, i);

		for (int i = size - 1; i > 0; i--) {
			std::swap(numbs[0], numbs[i]);
			heapify(numbs, i, 0);
		}
	}
}


std::pair<std::string, unsigned int> deQueue(std::vector<std::pair<std::string, unsigned int>>& original) {
	std::pair<std::string, unsigned int> first = original[0];
	for (int i = 0; i < original.size() - 1; i++) {
		original[i] = original[i + 1];
	}
	original.pop_back();
	buildHeap(original, original.size());
	heapSort(original, original.size());
	return first;
}

std::pair<std::string, unsigned int> inQueue(std::vector<std::pair<std::string, unsigned int>>& original, std::pair<std::string, unsigned int> a, std::pair<std::string, unsigned int> b) {
	std::string text = a.first + b.first;
	int number = a.second + b.second;
	original.push_back(std::make_pair(text, number));
	buildHeap(original, original.size());
	heapSort(original, original.size());
	return std::make_pair(text,number);
}


std::vector<std::pair<std::string, unsigned int>> reform(std::vector<std::pair<std::string, unsigned int>>& numbs) {
	std::vector<std::pair<std::string, unsigned int>> hi;

		std::pair<std::string, unsigned int> a = deQueue(numbs);
		std::pair<std::string, unsigned int> b = deQueue(numbs);
		std::pair<std::string, unsigned int> ab = inQueue(numbs, a, b);
		hi.push_back(ab);
		hi.push_back(a);
		hi.push_back(b);

	return hi;
}

std::string getBinary(BigArray bigarray, std::string character, int k) {

	std::pair<std::string, unsigned int> ab;
	std::string text;
	for (int j = k; j < bigarray.getSize(); j++) {
		ab = bigarray.getPair(j, 0);
		text = ab.first;
		for (int i = 0; i < text.length(); i++) {
			if (text[i] == character[0]) {
				if (i < text.length() / 2) { return "1" + getBinary(bigarray, character, j + 1); }
				return "0" + getBinary(bigarray, character, j + 1);

			}
		}
	}
	return "";

}

void huffmanMethod(std::string path) {
	std::string text = retrieveText(path);
	std::vector<std::pair<std::string, unsigned int>> hi = makeVector(text);
	std::vector<std::pair<std::string, unsigned int>> copy = hi;
	std::vector<std::string> values;
	std::string argument = "";
	std::string numbers = "";
	BigArray bigarray;
	buildHeap(hi, hi.size());
	heapSort(hi, hi.size());


	while (hi.size() > 1) {
		bigarray.insertTree(reform(hi));
	}
	bigarray.invertTable();
	
	
	for (int i = 0; i < copy.size(); i++) {
		values.push_back(getBinary(bigarray, copy[i].first, 0));
		std::cout << "\nFor "<<copy[i].first<<" it's:"<<values[i];
		argument += copy[i].first + ":" + values[i]+'\n';
	}

	for (int i = 0; i < text.length(); i++) {
		for (int j = 0; j < values.size(); j++) {
			if (copy[j].first[0] == text[i]) {
				numbers += values[j];
			}
		}
	}
	std::vector<int> changed = toDecimal(numbers);
	for (int i = 0; i < changed.size(); i++) {
		argument+=(char)changed[i];
	}
	saveText("output.txt", argument);
}

int main()
{
	std::string data = retrieveText("input.txt");
	huffmanMethod(data);
}
