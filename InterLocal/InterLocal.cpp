// InterLocal.cpp: define el punto de entrada de la aplicación de consola.
//

#include "InterLocal.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ABD *abd = new ABD();

	if (argc > 1)
	{
		//Analizar primer argumento
		if ((_wcsicmp(argv[1], L"--administrar-bdi") == 0) || (_wcsicmp(argv[1], L"-a") == 0))
		{
			//Modo de administración de la base de datos.
			int rc = abd->ModoAdministracion(argc, argv);
			delete abd;
			return rc;
		}
	}
	return 0;
}

