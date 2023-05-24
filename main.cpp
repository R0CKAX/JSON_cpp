#include <iostream>
#define ARDUINO_JSON_ENABLE_STD_STRING
#include "arduinojson.h"
#include "highscores.json"
#include "config.json"
#include <fstream>
#include <string>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;

void menu()
{
	cout << "1 - Opcje programu\n2 - Tabela wynikow\n3 - Nowa gra\n4 - Wyjscie\n";
}

void scores()
{
	fstream highscores;
	highscores.open("highscores.json", ios::in);
	StaticJsonDocument<1024> doc;

	string content;
	string line;
	while (getline(highscores, line))
	{
		//content += line;
		cout << line << "\n";
	}
	//cout << content << "\n";

	// Poprawiæ ¿eby wyœwietla³o sie ³adnie podczas wyswietlania tabeli wynikow bo wyswietla sie tylko 1 rzad od gory

	deserializeJson(doc, content);

	for (JsonPair keyValue : doc.as<JsonObject>())
	{
		cout << keyValue.key() << " : " << keyValue.value() << "\n";
	}

	highscores.close();
}

void display()
{
	// Funkcja wyswietlajaca aktualne wartosci ustawien z pliku config.json
	fstream config_d;
	config_d.open("config.json", ios::in);

	if (!config_d.good())
	{
		cout << "cannot open file!\n";
	}
	
	StaticJsonDocument<1024> doc;
	string settings;
	getline(config_d, settings);
	
	deserializeJson(doc, settings);

	for (JsonPair keyValue : doc.as<JsonObject>())
	{
		cout << keyValue.key() << " : " << keyValue.value() << "\n";
	}
	config_d.close();
}

void change()
{
	// Funkcja zmieniajaca wartosci ustawien w pliku konfiguracyjnym
	fstream config_c;
	config_c.open("config.json", ios::out);

	if (!config_c.good())
	{
		cout << "cannot open config file!\n";
	}

	string antialiasing;
	string multisampling;
	string shaders;
	string crowdDensity;
	string renderDistance;

	StaticJsonDocument<1024> doc;
	cout << "Podaj ilosc antyaliasingu [ 2 , 4 , 8 , 16 (X) ]: ";
	cin >> antialiasing;
	cout << "Podaj ilosc multisamplingu [ 2 , 4 , 8 , 16 (X) ]: ";
	cin >> multisampling;
	cout << "Podaj ilosc cieni [ low , medium , high , ultra ]: ";
	cin >> shaders;
	cout << "Podaj gestosc pojawiajacych sie ludzi [ 1 , 2 , 3 ]: ";
	cin >> crowdDensity;
	cout << "Podaj odleglosc renderowania terenu [ close , normal , further , far ]: ";
	cin >> renderDistance;

	doc["antialiasing"] = antialiasing;
	doc["multisampling"] = multisampling;
	doc["shaders"] = shaders;
	doc["crowdDensity"] = crowdDensity;
	doc["renderDistance"] = renderDistance;

	string wyjscie;
	serializeJson(doc, wyjscie);

	cout << wyjscie;
	config_c << wyjscie << endl;

	config_c.close();
}

void game()
{
	//tutaj bedzie znajdowac sie gra. po skonczonej grze zapisuje ona score i nazwe uzytkownika do pliku highscores.json
	string nick;
	fstream highscores;
	highscores.open("highscores.json", ios::app);

	cout << "Podaj swoj nick: ";
	cin >> nick;

	cout << "----- ZASADY GRY -----" << endl << "W momencie rozpoczecia gry bedziesz musial wpisac ile gwiazdek widzisz na ekranie." << endl;
	cout << "W przypadku podania poprawnej odpowiedzi iloœæ gwiazdek zostanie dodana do Twojego lacznego wyniku." << endl;
	cout << "W przypadku podania blednej odpowiedzi gra zakonczy sie natychmiastowo, a Twoj wynik pojawi sie w tabeli wynikow" << endl;
	cout << "Gra rozpocznie sie za 10 sekund..." << endl;
	Sleep(10000);
	
	int score = 0;	// Inicjalizacja poczatkowej wartosci wyniku
	srand(time(NULL));	// Dodanie losowosci podczas losowania ilosci gwiazdek

	const int MAX = 30;
	const int DZIELNIK = 3;	// Podanie zmiennej wartosci maksymalnej ilosci gwiazdek
	int ilosc;	// Zmienna ktora przechowuje losowana ilosc gwiazdek
	int guess;	// Zmienna przechowujaca wartosc jaka wpisal uzytkownik
	bool gameOver = true;	// Zmienna ktora odpowiada za wyswietlanie sie kolejnych poziomow (gra nie konczy sie na 1 poziomie)
	int counter = 0;	// Licznik ile poziomow udalo sie przejsc uzytkownikowi
	char input;	// Zmienna przechowujaca wartosc tego czy uzytkownik chce aby wpisac jego wynik do tabeli

	while (gameOver)
	{
		while (true)
		{
			ilosc = rand() % MAX + 1;
			if (ilosc % DZIELNIK == 0) break;
		}

		system("cls");
		cout << "---------- POZIOM " << counter + 1 << " ----------" << endl << endl;
		for (int i = 0; i < ilosc / DZIELNIK; i++)
		{
			for (int j = 0; j < DZIELNIK; j++)
			{
				cout << "* ";
			}
			cout << "\n";
		}

		cout << endl << "Nick: " << nick << endl;
		cout << "Score: " << score << endl;
		cout << "Ile widzisz gwiazdek: ";
		cin >> guess;

		if (guess == ilosc)
		{
			cout << "Brawo, to poprawna odpowiedz!" << endl;
			score += guess;
			counter++;
			Sleep(2000);
		}
		else
		{
			cout << "Niestety podana przez Ciebie odpowiedz nie jest poprawna." << endl;
			cout << "Twoj koncowy wynik to: " << score << endl;
			gameOver = false;
			Sleep(5000);
		}
	}
	cout << "Czy chcesz zapisac swoj wynik do tabeli wynikow? [ T / N ]: ";
	cin >> input;

	if (input == 'T' || input == 't')
	{
		// wpisywanie wyniku do tabeli
		StaticJsonDocument<1024> doc;
		string dane;

		//JsonObject nazwy = doc.createNestedObject("gracze");
		doc[nick] = score;
		serializeJson(doc, dane);

		highscores << dane << "\n";
	}
	cout << "Dzieki za zagranie w moja SUPER gre :)" << endl;
	highscores.close();
}

void options_menu()
{
	char input;
	//tutaj program bedzie pobieral wartosci podane przez uzytkownika i zapisywal je jako konfiguracje pliku .json
	while (true)
	{
		cout << "1 - Wyswietl aktualnie ustawione opcje w pliku konfiguracyjnym\n2 - Dokonaj zmiany opcji\n3 - Zapisz i wyjdz\n";
		cin >> input;
	
		system("cls");

		switch (input)
		{
		case '1':
			display();
			break;
		case '2':
			change();
			break;
		case '3':
			system("cls");
			cout << "Fajnie, ze wpadles. Do nastepnego razu ;)";
			break;
		default:
			cout << "Nie ma takiej opcji\n";
			break;
		}
	}
}

int main()
{
	// menu witajace uzytkownika i wyswietlajace mu mozliwosci pod numerami 1 (opcje), 2 (minigra) i 3 (wyjscie)
	char input;

	menu();
	cin >> input;

	system("cls");

	switch (input)
	{
		case '1':
			cout << "Wybrano opcje\n";
			options_menu();
			break;
		case '2':
			cout << "---------- TABELA WYNIKOW ----------\n";
			scores();
			break;
		case '3':
			cout << "Wybrano nowa gra\n";
			game();
			break;
		case '4':
			cout << "Wybrano wyjscie\n";
			break;
		default:
			cout << "Nie ma takiej opcji\n";
			break;
	}

	return 0;
}