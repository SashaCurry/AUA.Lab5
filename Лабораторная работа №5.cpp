#include "iostream"
#include "vector"
#include "math.h"
#include "algorithm"
#include "set"
#include "string"
#include "map"
#include "iomanip"

using namespace std;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                               Построить идеалы полугруппы по таблице Кэли                                                           */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int findNumber(char word, char* el, int N) { //Поиск номера буквы в элементах полугруппы
	for (int i = 0; i < N; i++)
		if (el[i] == word)
			return i;
}


bool findElUnderset(char word, set <char> underset, int size) { //Поиск елемента в подмножестве
	for (char help : underset)
		if (help == word)
			return true;
	return false;
}


bool associativity(char** mas, int N, char* halfgroup) { //Проверка свойства ассоциативности
	for (int a = 0; a < N; a++)
		for (int b = a; b < N; b++)
			for (int c = 0; c < N; c++)
				if (mas[findNumber(mas[a][b], halfgroup, N)][c] != mas[a][findNumber(mas[b][c], halfgroup, N)])
					return false;
	return true;
}


void makeUnderset(set<set <char>>& undersets, set <char> underset, int d, char* halfgroup, int N) { //Строим подмножество
	if (underset.size() == d) {
		undersets.insert(underset);
		return;
	}

	for (int i = 0; i < N; i++) {
		set <char> newUnderset = underset;
		if (newUnderset.find(halfgroup[i]) == newUnderset.end()) {
			newUnderset.insert(halfgroup[i]);
			makeUnderset(undersets, newUnderset, d, halfgroup, N);
		}
	}
}


void makeIdeals() { //Строим идеалы
	cout << "\nВведите количество элементов полугруппы: ";
	int N;
	cin >> N;

	cout << "Введите полугруппу: ";
	char* halfgroup = new char[N];
	for (int i = 0; i < N; i++)
		cin >> halfgroup[i];

	cout << "Введите таблицу Кэли: \n";
	char** matrix = new char* [N];
	for (int i = 0; i < N; i++) {
		matrix[i] = new char[N];
		for (int j = 0; j < N; j++)
			cin >> matrix[i][j];
	}

	if (!associativity(matrix, N, halfgroup)) {
		cout << "\nНе выполняется ассоциативность полугруппы! \n";
		return;
	}

	set<set <char>> undersets;
	for (int d = 1; d <= N; d++) { //Строим все возможные подмножества
		set <char> underset;
		makeUnderset(undersets, underset, d, halfgroup, N);
	}

	vector <vector <char>> idealR;
	vector <vector <char>> idealL;
	vector <vector <char>> ideal;
	for (auto s : undersets) {
		bool flagR = true;
		bool flagL = true;
		for (char word : s) {
			for (int i = 0; i < N; i++) {
				if (!findElUnderset(matrix[findNumber(word, halfgroup, N)][i], s, s.size())) {
					flagR = false;
				}

				if (!findElUnderset(matrix[i][findNumber(word, halfgroup, N)], s, s.size())) {
					flagL = false;
				}

				if (!flagR && !flagL)
					break;
			}

			if (!flagR && !flagL)
				break;
		}

		if (flagR) {
			vector <char> res;
			for (char word : s)
				res.push_back(word);
			idealR.push_back(res);
		}

		if (flagL) {
			vector <char> res;
			for (char word : s)
				res.push_back(word);
			idealL.push_back(res);

			if (flagR)
				ideal.push_back(res);
		}
	}

	cout << "\nПравые идеалы: ";
	for (int i = 0; i < idealR.size(); i++) {
		cout << "{";
		for (int j = 0; j < idealR[i].size(); j++) {
			if (j == idealR[i].size() - 1)
				cout << char(idealR[i][j]);
			else
				cout << char(idealR[i][j]) << " ";
		}

		if (i == idealR.size() - 1)
			cout << "}";
		else
			cout << "}, ";
	}

	cout << "\nЛевые идеалы: ";
	for (int i = 0; i < idealL.size(); i++) {
		cout << "{";
		for (int j = 0; j < idealL[i].size(); j++) {
			if (j == idealL[i].size() - 1)
				cout << char(idealL[i][j]);
			else
				cout << char(idealL[i][j]) << " ";
		}
		
		if (i == idealL.size() - 1)
			cout << "}";
		else
			cout << "}, ";
	}

	cout << "\nДвусторонние идеалы: ";
	for (int i = 0; i < ideal.size(); i++) {
		cout << "{";
		for (int j = 0; j < ideal[i].size(); j++) {
			if (j == ideal[i].size() - 1)
				cout << char(ideal[i][j]);
			else
				cout << char(ideal[i][j]) << " ";
		}
		
		if (i == ideal.size() - 1)
			cout << "}";
		else
			cout << "}, ";
	}
	cout << endl;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                           Вычислить отношения Грина и построить "egg-box"-картину конечной полугруппы                                               */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
set <char> srez(char** matrix, int N, int num, string str) { //Срез по строке или столбцу матрицы
	set <char> res;
	if (str == "row")
		for (int i = 0; i < N; i++)
			res.insert(matrix[num][i]);
	else
		for (int i = 0; i < N; i++)
			res.insert(matrix[i][num]);

	return res;
}


bool entry(vector <char> A, vector <char> B) { //Проверяем, входит ли вектор В в вектор А
	int count = 0;
	for (int i = 0; i < A.size(); i++)
		for (int j = 0; j < B.size(); j++)
			if (A[i] == B[j])
				count++;

	if (count == B.size())
		return true;
	return false;
}



char generalEl(vector <char> A, vector <char> B) { //Находим общий элемент
	for (int i = 0; i < A.size(); i++)
		for (int j = 0; j < B.size(); j++)
			if (A[i] == B[j])
				return A[i];
}


void eggBox(char** matrix, char* halfgroup, int N, vector <pair <char, char>> R, vector <pair <char, char>> L, vector <pair <char, char>> D) { //Строим egg-box картину
	//создаём матрицу отношения R
	bool** matrixR = new bool*[N];
	for (int i = 0; i < N; i++) {
		matrixR[i] = new bool[N];
		for (int j = 0; j < N; j++)
			matrixR[i][j] = false;
	}
	for (int i = 0; i < R.size(); i++)
		matrixR[findNumber(R[i].first, halfgroup, N)][findNumber(R[i].second, halfgroup, N)] = true;

	cout << "\nМатрица отношения R: \n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << matrixR[i][j] << " ";
		cout << endl;
	}

	//создаём матрицу отношения L
	bool** matrixL = new bool* [N];
	for (int i = 0; i < N; i++) {
		matrixL[i] = new bool[N];
		for (int j = 0; j < N; j++)
			matrixL[i][j] = false;
	}
	for (int i = 0; i < L.size(); i++)
		matrixL[findNumber(L[i].first, halfgroup, N)][findNumber(L[i].second, halfgroup, N)] = true;

	cout << "\nМатрица отношения L: \n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << matrixL[i][j] << " ";
		cout << endl;
	}

	//создаём матрицу отношения D
	bool** matrixD = new bool* [N];
	for (int i = 0; i < N; i++) {
		matrixD[i] = new bool[N];
		for (int j = 0; j < N; j++)
			matrixD[i][j] = false;
	}
	for (int i = 0; i < D.size(); i++)
		matrixD[findNumber(D[i].first, halfgroup, N)][findNumber(D[i].second, halfgroup, N)] = true;

	cout << "\nМатрица отношения D: \n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << matrixD[i][j] << " ";
		cout << endl;
	}

	//Вычисляем классы эквивалентности отношения R
	vector <bool> used(N, false);
	vector <vector <char>> classesR;
	for (int i = 0; i < N; i++) {
		vector <char> clas;
		for (int j = 0; j < N; j++) {
			if (matrixR[i][j] && !used[j]) {
				clas.push_back(halfgroup[j]);
				used[j] = true;
			}
		}

		if (!clas.empty())
			classesR.push_back(clas);
	}

	cout << "\nКлассы эквивалентности отношения R: { ";
	int size = classesR.size();
	for (int i = 0; i < size; i++) {
		cout << "{";
		int iSize = classesR[i].size();
		for (int j = 0; j < iSize; j++) {
			if (j == iSize - 1)
				cout << classesR[i][j] << "}";
			else
				cout << classesR[i][j] << ", ";
		}

		if (i == size - 1)
			cout << " } \n";
		else
			cout << ", ";
	}

	//Вычисляем классы эквивалентности отношения L
	used.assign(N, false);
	vector <vector <char>> classesL;
	for (int i = 0; i < N; i++) {
		vector <char> clas;
		for (int j = 0; j < N; j++) {
			if (matrixL[i][j] && !used[j]) {
				clas.push_back(halfgroup[j]);
				used[j] = true;
			}
		}

		if (!clas.empty())
			classesL.push_back(clas);
	}

	cout << "Классы эквивалентности отношения L: { ";
	size = classesL.size();
	for (int i = 0; i < size; i++) {
		cout << "{";
		int iSize = classesL[i].size();
		for (int j = 0; j < iSize; j++) {
			if (j == iSize - 1)
				cout << classesL[i][j] << "}";
			else
				cout << classesL[i][j] << ", ";
		}

		if (i == size - 1)
			cout << " } \n";
		else
			cout << ", ";
	}

	//Вычисляем классы эквивалентности отношения D
	used.assign(N, false);
	vector <vector <char>> classesD;
	for (int i = 0; i < N; i++) {
		vector <char> clas;
		for (int j = 0; j < N; j++) {
			if (matrixD[i][j] && !used[j]) {
				clas.push_back(halfgroup[j]);
				used[j] = true;
			}
		}

		if (!clas.empty())
			classesD.push_back(clas);
	}

	cout << "Классы эквивалентности отношения D: { ";
	size = classesD.size();
	for (int i = 0; i < size; i++) {
		cout << "{";
		int iSize = classesD[i].size();
		for (int j = 0; j < iSize; j++) {
			if (j == iSize - 1)
				cout << classesD[i][j] << "}";
			else
				cout << classesD[i][j] << ", ";
		}

		if (i == size - 1)
			cout << " } \n";
		else
			cout << ", ";
	}

	//Строим egg-box картину
	cout << "\"egg-box\"-картины:";
	for (int n = 0; n < classesD.size(); n++) {
		vector <vector <char>> rows;
		for (int i = 0; i < classesR.size(); i++)
			if (entry(classesD[n], classesR[i]))
				rows.push_back(classesR[i]);

		vector <vector <char>> columns;
		for (int i = 0; i < classesL.size(); i++)
			if (entry(classesD[n], classesL[i]))
				columns.push_back(classesL[i]);

		//Выводим название столбцов
		cout << "\n       " << setw(10);
		size = columns.size();
		for (int i = 0; i < size; i++) {
			cout << "(";
			int iSize = columns[i].size();
			for (int j = 0; j < iSize; j++) {
				if (j == iSize - 1)
					cout << columns[i][j] << ")" << setw(10);
				else
					cout << columns[i][j] << ",";
			}	
		}

		//Выводим название строк и сами строки
		cout << " \n";
		size = rows.size();
		for (int i = 0; i < size; i++) {
			cout << "(";
			int iSize = rows[i].size();
			for (int j = 0; j < iSize; j++) {
				if (j == iSize - 1)
					cout << rows[i][j] << ")" << setw(12);
				else
					cout << rows[i][j] << ",";
			}

			iSize = columns.size();
			for (int j = 0; j < iSize; j++) {
				cout << generalEl(rows[i], columns[j]) << setw(12);
			}
			cout << " \n";
		}
	}
}


void makeRelsGreen() { //Вычисляем отношения Грина
	cout << "\nВведите количество элементов полугруппы: ";
	int N;
	cin >> N;

	cout << "Введите полугруппу: ";
	char* halfgroup = new char[N];
	for (int i = 0; i < N; i++)
		cin >> halfgroup[i];

	cout << "Введите таблицу Кэли: \n";
	char** matrix = new char* [N];
	for (int i = 0; i < N; i++) {
		matrix[i] = new char[N];
		for (int j = 0; j < N; j++)
			cin >> matrix[i][j];
	}

	if (!associativity(matrix, N, halfgroup)) {
		cout << "\nНе выполняется ассоциативность полугруппы! \n";
		return;
	}

	vector <pair <char, char>> R;
	vector <pair <char, char>> L;
	vector <pair <char, char>> J;
	vector <pair <char, char>> H;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			set <char> a = srez(matrix, N, i, "row");
			set <char> b = srez(matrix, N, j, "row");
			if (a == b)
				R.push_back(make_pair(halfgroup[i], halfgroup[j]));

			a = srez(matrix, N, i, "column");
			b = srez(matrix, N, j, "column");
			if (a == b)
				L.push_back(make_pair(halfgroup[i], halfgroup[j]));

			set <char> resA;
			a = srez(matrix, N, i, "column");
			for (char word : a) {
				set <char> help;
				help = srez(matrix, N, findNumber(word, halfgroup, N), "row");
				for (char ans : help)
					resA.insert(ans);
			}
			set <char> resB;
			b = srez(matrix, N, j, "column");
			for (char word : b) {
				set <char> help;
				help = srez(matrix, N, findNumber(word, halfgroup, N), "row");
				for (char ans : help)
					resB.insert(ans);
			}
			if (resA == resB)
				J.push_back(make_pair(halfgroup[i], halfgroup[j]));
		}
	for (int i = 0; i < R.size(); i++)
		for (int j = 0; j < L.size(); j++)
			if (R[i] == L[j])
				H.push_back(R[i]);

	cout << "\nОтношения Грина: \nR = {";
	int size = R.size();
	for (int i = 0; i < size; i++) {
		if (i == size - 1)
			cout << "(" << R[i].first << ", " << R[i].second << ")} \n";
		else
			cout << "(" << R[i].first << ", " << R[i].second << ") ";
	}

	cout << "L = {";
	size = L.size();
	for (int i = 0; i < size; i++) {
		if (i == size - 1)
			cout << "(" << L[i].first << ", " << L[i].second << ")} \n";
		else
			cout << "(" << L[i].first << ", " << L[i].second << ") ";
	}

	cout << "J = {";
	size = J.size();
	for (int i = 0; i < size; i++) {
		if (i == size - 1)
			cout << "(" << J[i].first << ", " << J[i].second << ")} \n";
		else
			cout << "(" << J[i].first << ", " << J[i].second << ") ";
	}

	cout << "H = {";
	size = H.size();
	for (int i = 0; i < size; i++) {
		if (i == size - 1)
			cout << "(" << H[i].first << ", " << H[i].second << ")} \n";
		else
			cout << "(" << H[i].first << ", " << H[i].second << ") ";
	}

	cout << "D = {";
	size = J.size();
	for (int i = 0; i < size; i++) {
		if (i == size - 1)
			cout << "(" << J[i].first << ", " << J[i].second << ")} \n";
		else
			cout << "(" << J[i].first << ", " << J[i].second << ") ";
	}

	eggBox(matrix, halfgroup, N, R, L, J);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                        Главная функция                                                                              */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	setlocale(LC_ALL, "ru");
	for (;;) {
		cout << "1 - Построить идеалы полугруппы по таблице Кэли \n2 - Вычислить отношения Грина и построить \"egg-box\"-картину конечной полугруппы \n3 - Выход \n";
		int x;
		cin >> x;
		switch (x) {
		case 1:
			makeIdeals();
			cout << endl;
			break;
		case 2:
			makeRelsGreen();
			cout << endl;
			break;
		case 3:
			return 0;
		default:
			cout << "Incorrect. Try again! \n";
		}
	}
}