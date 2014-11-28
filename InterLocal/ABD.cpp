#include "ABD.h"

bool primerCb;

void Tdpn(const char* fmt, ...)
{
	char buffer[512];
	va_list args;
	va_start(fmt, args);
	vsprintf_s(buffer, 512, fmt, args);
	std::cout << buffer;
}

ABD::ABD()
{
	cbd = NULL;
	rutabasedatos[0] = 0;
	nombrebase = 0;
}

int callbackAdmin(void* Fuente, int Columnas, char **Resultados, char **NombreCol)
{
	if (primerCb)
	{
		for (int i = 0; i < Columnas; i++)
		{
			if (i == (Columnas - 1))
				cout << NombreCol[i] << endl;
			else
				cout << NombreCol[i] << " | ";
		}
	}
	for (int i = 0; i < Columnas; i++)
	{
		if (Resultados[i])
		{
			if (i == (Columnas - 1))
				cout << Resultados[i] << endl;
			else
				cout << Resultados[i] << " | ";
		}
	}
	return 0;
}

///ModoAdministración: Administra la base de datos, solo disponible en aplicaciones de consola.
int ABD::ModoAdministracion(int argc, _TCHAR **argv)
{
	if (argc > 2)
	{
		for (int i = 2; i < argc; i++)
		{
			if (argv[i][0] != L'-')
			{
				//Error al analizar argumentos.
				cout << "Argumento desconocido \"" << argv[i] << "\"" << endl;
				return ERROR_INVALID_PARAMETER;
			}
			for (int k = 0; k < _CANT_CMD; k += 2)
			{
				if ((_wcsicmp(argv[i], listacmd[k]) == 0) || (_wcsicmp(argv[i], listacmd[k]) == 0))
				{
					switch (k)
					{
					case ARG_RUTA_BASE_DATOS: //-b
						i++;
						wcscpy_s(rutabasedatos, 500, argv[i]);
						break;
					}
				}
			}
		}
	}

	if (rutabasedatos[0] == 0) //No se especificó una ruta a la base de datos, se abre la predeterminada.
		wcscpy_s(rutabasedatos, 500, L"diccionario.bdi");

	if (sqlite3_open16(rutabasedatos, &cbd))
	{
		cout << "No se pudo abrir la conexión a la base de datos" << endl;
		cout << "SQLite dice: " << sqlite3_errmsg(cbd) << endl;
		return ERROR_UNIDENTIFIED_ERROR;
	}

	for (int i = wcslen(rutabasedatos); i > 0; i--)
	{
		if (rutabasedatos[i] == '\\')
			nombrebase = &(rutabasedatos[i]);
	}
	if (!nombrebase)
		nombrebase = (_TCHAR*)&rutabasedatos;

	//Base iniciada.

	cout << "Base de datos abierta exitosamente" << endl;

	cout << "Ingrese un stmt SQL para ejecutar en la base de datos o '--<nombre de archivo>'\r\n"
		"para ejecutar un script donde cada linea es un stmt (maximo 1000 caracteres)" << endl;
	cout << endl;

	char consbuffer[512];

	while (true)
	{
		consbuffer[0] = 0;
		wcout << nombrebase << L" > ";
		cin.getline(consbuffer, 512);
		if (consbuffer[0] == 0)
		{
			cin.clear();
			continue;
		}
		if ((consbuffer[0] == '-') && (consbuffer[1] == '-'))
		{
			if (_strcmpi(&consbuffer[2], "salir") == 0)
			{
				return 0;
			}
		}
		//Ejecutar stmt.
		primerCb = true;
		if (sqlite3_exec(cbd, consbuffer, &(callbackAdmin), (void*)ABD_ADMIN, NULL))
		{
			cout << "Se produjo un error al procesar el comando." << endl;
			cout << "SQLITE dice: " << sqlite3_errmsg(cbd) << endl;
		}
	}

	return 0;
}

ABD::~ABD()
{
	sqlite3_close(cbd);
}
