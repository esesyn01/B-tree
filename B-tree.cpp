#include<iostream>
#include<string>
using namespace std;
struct Node
{
	int t, n;
	int* keys;
	bool l;
	Node** children;
};
void init(Node** newn, int t, bool l)  // poczatkowa inicjalizacja node'a
{
	(*newn)->t = t;
	(*newn)->l = l;
	(*newn)->children = new Node * [2 * t];
	(*newn)->keys = new int[2 * t - 1];
	for (int i = 0; i < 2 * t - 1; i++)
	{
		(*newn)->keys[i] = -1;
	}
	(*newn)->n = 0;
}
void split(Node** oldnode, Node** newnode, int t)  // dzielenie drzewa
{
	Node* newright = new Node;
	init(&newright, t, (*oldnode)->l); // alokacja nowego node'a (ten na prawą część t-1 kluczy i dzieci)
	(*oldnode)->n = t - 1;
	newright->n = t - 1;
	int n = (*oldnode)->keys[t - 1];
	int p = -1;
	for (int i = 0; i < 2 * t - 1; i++) // ustalenie, gdzie wyląduje klucz z dzielonego node'a
	{
		if ((*newnode)->keys[i] == -1 || (*newnode)->keys[i] > n)
		{
			p = i;
			break;
		}
	}
	int j = (*newnode)->n;
	while (j >= p + 1)  // przesunięcie kluczy i dzieci w prawo do miejsca gdzie wstawimy przenoszony klucz
	{
		(*newnode)->children[j + 1] = (*newnode)->children[j];
		(*newnode)->keys[j + 1] = (*newnode)->keys[j];
		j--;
	}
	(*newnode)->keys[j + 1] = (*newnode)->keys[j];
	for (int x = t; x < 2 * t-1; x++)  // ustawienie kluczy w prawym dziecku
	{
		newright->keys[x - t] = (*oldnode)->keys[x];
	}
	if (!(*oldnode)->l) // ustawienie dzieci prawego dziecka (jesli nie jest lisciem)
	{
		for (int x = t; x < 2 * t; x++)
		{
			newright->children[x - t] = (*oldnode)->children[x];
		}
	}
	(*newnode)->children[p + 1] = newright; // ustawienie przenoszonego klucza i prawego dziecka
	(*newnode)->keys[p] = n;
	(*newnode)->n++;
}
void insertinnode(Node** node, int k, int t)
{
	if (!(*node)->l) 
	{
		int j = (*node)->n - 1;
		while (k < (*node)->keys[j] && j >= 0) // ustalenie do ktorego dziecka nalezy przejsc
		{
			j--;
		}
		if ((*node)->children[j + 1]->n == 2 * t - 1)
		{
			split(&(*node)->children[j + 1], node, t); // jesli dziecko jest pelne, podziel je
			if ((*node)->keys[j + 1] < k) // ustalenie w ktorym dziecku wpisujemy klucz
			{
				j++;
			}
		}
		insertinnode(&(*node)->children[j + 1], k, t);
	}
	else
	{
		int j = (*node)->n-1;
		while (k < (*node)->keys[j] && j>=0) // jesli node jest lisciem ustal gdzie wpisany będzie klucz
		{                                    // i przesun klucze na prawo
			(*node)->keys[j + 1] = (*node)->keys[j];
			j--;
		}
		(*node)->keys[j+1] = k;
		(*node)->n++;
	}
}
void insert(Node** node, int k, int t)
{
	if ((*node)->n == 2 * t - 1) // jesli node jest pełny, podziel go uzywajac nowego node'a
	{
		Node* newnode = new Node;
		init(&newnode, t, 0);
		newnode->children[0] = *node;
		split(node, &newnode, t);
		if (newnode->keys[0] < k) // sprawdz, po ktorej stronie trzeba dodac nowy klucz
		{
			insertinnode(&newnode->children[1], k, t);
		}
		else
		{
			insertinnode(&newnode->children[0], k, t);
		}
		*node=newnode; // nowy node staje sie starszy
	}
	else
	{
		insertinnode(node, k, t);
	}
}
Node* search(Node* root, int k)
{
	if (root != nullptr)
	{
		int j = 0;
		while (j < root->n && root->keys[j] < k) // ustal miejsce, gdzie powinien byc szukany klucz
		{
			j++;
		}
		if (k == root->keys[j])
		{
			return root;
		}
		if (root->l)
		{
			return NULL;
		}
		return search(root->children[j], k);
	}
	else
	{
		return NULL;
	}
}
void print(Node* node)
{
	if (node != nullptr)
	{
		int j = 0;
		for (j = 0; j < node->n; j++)
		{
			if(!node->l) // jesli to nie lisc, wywolaj rekurencyjnie funckje dla dziecka
			print(node->children[j]);
			cout << node->keys[j] << " "; // wypisz klucze
		}
		if (!node->l)
		{
			print(node->children[j]); // wywolaj funkcje dla ostaniego dziecka
		}
	}
}
int stringtonum(string a) // zamiana stringa na liczbe
{
	int n = 0;
	for (int i = 0; i < a.size(); i++)
	{
		n *= 10;
		n += (int)a[i]-48;
	}
	return n;
}
void load(int t, Node** root, string a,int* i)
{
	string temp;
	temp.clear();
	while (*(i) < a.size()-1)
	{
		if (a[*(i)] == '(')    // stwórz dziecko i rekurencyjnie wywołaj funkjcę
		{
			Node* newnode = new Node;
			init(&newnode, t, 1);
			(*root)->l = 0;
			int n = (*root)->n;
			(*root)->children[n] = newnode;
			(*i)++;
			load(t, &(*root)->children[n], a, i);
		}
		else
		{
			if (a[*(i)] == ')')  // wyjdz z rekurencji (koniec node'a)
			{
				return;
			}
			else
			{
				if (a[*(i)] >= 48 && a[*(i)] <= 57) // jesli to liczba, wpisz ją
				{
					temp.clear();
					while (a[*(i)] >= 48 && a[*(i)] <= 57)
					{
						temp+= a[*(i)];
						(*i)++;
					}
					int p = stringtonum(temp);
					int n = (*root)->n;
					(*root)->keys[n] = p;
					(*root)->n++;
				}
			}
		}
		(*i)++;
	}
}
void save(Node* node)
{
	if (node != nullptr)
	{
		cout << "( "; // na poczatku otworz nowego node'a
		int j = 0;
		for (j = 0; j < node->n; j++)
		{
			if (!node->l)  // funkcja ta sama jak wypisanie
				save(node->children[j]);
			cout << node->keys[j] << " ";
		}
		if (!node->l)
		{
			save(node->children[j]);
		}
		cout << ") ";  // zamnknij node'a
	}
}
void freemem(Node** root) // zwalnianie pamięci rekurencyjnie
{
	if ((*root)->l)
	{
		return;
	}
	if (root != nullptr)
	{
		int j = 0;
		while (j <= (*root)->n)
		{
			if (!(*root)->l)
			{
				freemem(&(*root)->children[j]);
			}
			delete (*root)->children[j];
			j++;
		}
	}
}
int main()
{
	Node* root = new Node;
	int t, n;
	int h;
	char cmd;
	bool end = 0;
	string a;
	while (cin >> cmd)
	{
		switch (cmd)
		{
		case 'A':
			cin >> n;
			insert(&root, n, t);
			break;
		case 'I':
			cin >> t;
			init(&root, t, 1);
			break;
		case '?':
			cin >> n;
			if (search(root, n) != NULL)
			{
				cout << n << " +\n";
			}
			else
			{
				cout << n << " -\n";
			}
			break;
		case 'P':
			print(root);
			break;
		case 'L':
			cin >> t;
			a.clear();
			std::getline(cin, a);
			a.clear();
			std::getline(cin, a);
			init(&root, t, 1);
			h = 1;
			load(t, &root, a,&h);
			break;
		case 'S':
			cout << t << "\n";
			save(root);
			cout << "\n";
			break;
		case 'X':
			end = 1;
		}
		if (end)
		{
			break;
		}
	}
	freemem(&root);
	delete root;
}