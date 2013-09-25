/*
 Parson ( http://kgabis.github.com/parson/ )
 Copyright (c) 2012 Krzysztof Gabis
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include "parson.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STREQ(A, B) (A && B ? strcmp(A, B) == 0 : 0)

int validateJsonImput(const char *filename, char *error);

int main() {

    const char *filename = "tests/modelo.txt";
    char error[200];
    /* retorna 0 error 1 valido, filename nombre del archivo, string del error */
    if (validateJsonImput(filename, error)==1)
    	printf("TODO BIEN: %s\n",error);
    else
    	printf("TODO MAL: %s\n",error);

    /*pushNodesInMemory(filename);*/

    /*getNodeById(id);*/

    return 0;
}

int validateJsonImput(const char *filename, char *error) {
    JSON_Value  *root_value;
    JSON_Object *object, *objectInArray, *object2;
    JSON_Array  *array,  *array2;
    size_t i,j,k;
    double idAntecesor;

    /*validate json*/
    root_value = json_parse_file(filename);
    if (!root_value)
    {
	strcpy(error, "ERROR: Estructura de Json no valida");
	return 0;
    }

    /*validate nature of json*/
    if (json_value_get_type(root_value) == JSONArray)
    {
	strcpy(error, "ERROR: Estructura de Json no valida");
	return 0;
    }

    if (json_value_get_type(root_value) != JSONObject)
    {
	strcpy(error, "ERROR: Estructura de Json no valida");
	return 0;
    }
    object = json_value_get_object(root_value);

    /*TODO: Validacion diferente nro de id*/

    /*TODO: Validacion referencia a nodos existentes*/

    /*Validaciones COLA*/
    array = json_object_dotget_array(object, "transformacion.colas");

    /*ANTESESOR: No pueden ser Colas.*/
    if (array != NULL) /*colas*/
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Antecesor");
	    if (array2 != NULL) /*antecesores*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idAntecesor = json_array_get_number(array2, j);
		   for (k = 0; k < json_array_get_count(array); k++)
		   {
			object2 = json_array_get_object(array, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COLA %.0f tiene como antecesor a la COLA %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }
    /*TODO: SUCESOR: Solo pueden ser combis.*/
    

    /*Validaciones COMBI*/
    array = json_object_dotget_array(object, "transformacion.combis");
    /*TODO: ANTESESOR: Solo pueden ser nodos Cola.*/
    
    /*ANTESESOR: Se admite cualquier número de entradas, excepto 0 (cero).*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Antecesor");
	    if (array2 != NULL)
	    {
		if(json_array_get_count(array2)==0)
		{
		    sprintf(error,"ERROR: La COMBI %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		    return 0;
		}
	    }
	    else
	    {
		sprintf(error,"ERROR: La COMBI %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		return 0;
	    }
        }
    }
    /*TODO: SUCESOR: No pueden ser Combis. Es decir, pueden ser Normales, Colas, funciones o Contadores.*/
    /*TODO: SUCESOR: Se admite cualquier número de salidas, excepto 0.*/

    /*Validaciones Normal*/
    /*TODO: ANTESESOR: Se admite cualquier número de entradas, excepto 0.*/
    /*TODO: ANTESESOR: No pueden ser colas, es decir, puede anteceder de una  Normal, Combi, Contador y/o funciones.*/
    /*TODO: SUCESOR: Se admite cualquier número de salidas, excepto 0.*/
    /*TODO: SUCESOR: No pueden ser Combis. Es decir, pueden ser Normales, Colas, funciones y/o Contador.*/

    /*Validaciones Funnción*/
    /*TODO: ANTESESOR: Se admite cualquier número de entradas, excepto 0.*/
    /*TODO: ANTESESOR: No pueden ser colas. Es decir, pueden ser Normales, Combis, Contadores y otra Función.*/
    /*TODO: SUCESOR: Se admite cualquier número de salidas, excepto 0.*/
    /*TODO: SUCESOR: No pueden ser Combis. Es decir, pueden ser Normales, Colas, Funciones o Contadores.*/

    /*Validaciones Acumulador*/
    /*TODO: ANTESESOR: Se admite cualquier número de entradas, excepto 0.*/
    /*TODO: ANTESESOR: No pueden ser colas ni otro contador. Es decir, pueden ser Normales, combis, y funciones.*/
    /*TODO: SUCESOR: Se admite cualquier número de salidas.*/
    /*TODO: SUCESOR: No pueden ser combis, ni contadores. Es decir, pueden ser Normales, colas o funciones.*/

    strcpy(error, "OK");
    json_value_free(root_value);
    return 1;
}

