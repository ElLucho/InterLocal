#pragma once

#include "sqlite3.h"
#include <tchar.h>
#include <Windows.h>
#include <iostream>
using namespace std;

//Cantidad de opciones que representan el mismo comando
#define _CMD_MULT 2
#define _CANT_CMD 1 * _CMD_MULT

#define ARG_RUTA_BASE_DATOS 1

const static _TCHAR listacmd[_CANT_CMD][20] = {
	L"--ruta-base-datos", L"-b"
};

class ABD
{
public:
	int ModoAdministracion(int argc, _TCHAR **argv);
	ABD();
	~ABD();
private:
	sqlite3 *cbd; //Conexión a la base de datos.

	_TCHAR rutabasedatos[500];
	_TCHAR *nombrebase;

	//int callbackAdmin(void* Fuente, int Columnas, char **Resultados, char **NombreCol);
	bool primerCb;

	typedef int(*SQLITE3CALLBACK)(void*, int, char**, char**);

	enum cbFuentes{ ABD_ADMIN = 1 };

};

