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
int getArrayId(const JSON_Array* array, int** idPickUpteds, int* amount,char* subKey);

/*int* idQueueNodes;
int* idQueueFollowsNodes;
int idQueueNodesAmount = rockNrollNENE2( array, &idQueueNodes , "idnodo");
int idQueueNodesFollowsAmount = rockNrollNENE2( array, &idQueueFollowsNodes , "Sucesor");*/

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
    JSON_Array  *array,  *array2 , *array3;
    size_t i,j,k;
    double idAntecesor,idSucesor,idComparar;

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

    /*TODO: Que contengan todos los elementos. Los demas se ignoran*/

    /*Validacion diferente nro de id*/
    array = json_object_dotget_array(object, "transformacion.colas");/*colas,contadores,funciones,normales,combis*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    idComparar = json_object_dotget_number(objectInArray,"idnodo");

	    /* comparo con la misma lista de COLAS */
	    for (k = i+1; k < json_array_get_count(array); k++)
	    {
		object2 = json_array_get_object(array, k);
		if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La COLA %.0f está repetida en la lista de COLAS",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		}
	    }

	    /* comparo con la misma lista de CONTADORES */
	    array2 = json_object_dotget_array(object, "transformacion.contadores");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La COLA %.0f está repetida en la lista de CONTADORES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }

	    /* comparo con la misma lista de FUNCIONES */
	    array2 = json_object_dotget_array(object, "transformacion.funciones");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La COLA %.0f está repetida en la lista de FUNCIONES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }

	    /* comparo con la misma lista de NORMALES */
	    array2 = json_object_dotget_array(object, "transformacion.normales");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La COLA %.0f está repetida en la lista de NORMALES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }

	    /* comparo con la misma lista de COMBIS */
	    array2 = json_object_dotget_array(object, "transformacion.combis");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La COLA %.0f está repetida en la lista de COMBIS",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }
        }
    }

   array = json_object_dotget_array(object, "transformacion.contadores");/*colas,contadores,funciones,normales,combis*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    idComparar = json_object_dotget_number(objectInArray,"idnodo");

	    /* comparo con la misma lista de CONTADORES */
	    for (k = i+1; k < json_array_get_count(array); k++)
	    {
		object2 = json_array_get_object(array, k);
		if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: El CONTADOR %.0f está repetido en la lista de CONTADORES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		}
	    }

	    /* comparo con la misma lista de FUNCIONES */
	    array2 = json_object_dotget_array(object, "transformacion.funciones");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: El CONTADOR %.0f está repetido en la lista de FUNCIONES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }

	    /* comparo con la misma lista de NORMALES */
	    array2 = json_object_dotget_array(object, "transformacion.normales");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: El CONTADOR %.0f está repetido en la lista de NORMALES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }

	    /* comparo con la misma lista de COMBIS */
	    array2 = json_object_dotget_array(object, "transformacion.combis");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: El CONTADOR %.0f está repetido en la lista de COMBIS",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }
        }
    }

    array = json_object_dotget_array(object, "transformacion.funciones");/*colas,contadores,funciones,normales,combis*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    idComparar = json_object_dotget_number(objectInArray,"idnodo");

	    /* comparo con la misma lista de FUNCIONES */
	    for (k = i+1; k < json_array_get_count(array); k++)
	    {
		object2 = json_array_get_object(array, k);
		if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La FUNCION %.0f está repetida en la lista de FUNCIONES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		}
	    }

	    /* comparo con la misma lista de NORMALES */
	    array2 = json_object_dotget_array(object, "transformacion.normales");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La FUNCION %.0f está repetida en la lista de NORMALES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }

	    /* comparo con la misma lista de COMBIS */
	    array2 = json_object_dotget_array(object, "transformacion.combis");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La FUNCION %.0f está repetida en la lista de COMBIS",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }
        }
    }

    array = json_object_dotget_array(object, "transformacion.normales");/*colas,contadores,funciones,normales,combis*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    idComparar = json_object_dotget_number(objectInArray,"idnodo");

	    /* comparo con la misma lista de NORMALES */
	    for (k = i+1; k < json_array_get_count(array); k++)
	    {
		object2 = json_array_get_object(array, k);
		if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La NORMAL %.0f está repetida en la lista de NORMALES",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		}
	    }

	    /* comparo con la misma lista de COMBIS */
	    array2 = json_object_dotget_array(object, "transformacion.combis");/*colas,contadores,funciones,normales,combis*/
	    if (array2 != NULL)
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   object2 = json_array_get_object(array2, j);
		   if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La NORMAL %.0f está repetida en la lista de COMBIS",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		   }
		}
	    }
        }
    }

    array = json_object_dotget_array(object, "transformacion.combis");/*colas,contadores,funciones,normales,combis*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    idComparar = json_object_dotget_number(objectInArray,"idnodo");

	    /* comparo con la misma lista de NORMALES */
	    for (k = i+1; k < json_array_get_count(array); k++)
	    {
		object2 = json_array_get_object(array, k);
		if (idComparar == json_object_dotget_number(object2,"idnodo")){
		     sprintf(error,"ERROR: La COMBI %.0f está repetida en la lista de COMBIS",json_object_dotget_number(objectInArray,"idnodo"));
		     return 0;
		}
	    }
        }
    }


    /*TODO: Validacion referencia a nodos existentes*/

    /*Validaciones COLA*/
    array = json_object_dotget_array(object, "transformacion.colas");

    /*ANTESESOR: No pueden ser Colas.*/
    if (array != NULL)
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

    /*COLA: SUCESOR: Solo pueden ser combis.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL) /*Sucesores*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idSucesor = json_array_get_number(array2, j);
		   array3 = json_object_dotget_array(object, "transformacion.contadores");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idSucesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COLA %.0f tiene como Sucesor al CONTADOR %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		   array3 = json_object_dotget_array(object, "transformacion.funciones");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idSucesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COLA %.0f tiene como Sucesor la FUNCION %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		   array3 = json_object_dotget_array(object, "transformacion.normales");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idSucesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COLA %.0f tiene como Sucesor la NORMAL %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		   array3 = json_object_dotget_array(object, "transformacion.colas");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idSucesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COLA %.0f tiene como Sucesor la COLA %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }

    /*Validaciones COMBI*/
    array = json_object_dotget_array(object, "transformacion.combis");
    /*ANTESESOR: Solo pueden ser nodos Cola.*/
    if (array != NULL)
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
		   array3 = json_object_dotget_array(object, "transformacion.contadores");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COMBI %.0f tiene como antecesor al CONTADOR %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		   array3 = json_object_dotget_array(object, "transformacion.funciones");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COMBI %.0f tiene como antecesor la FUNCION %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		   array3 = json_object_dotget_array(object, "transformacion.normales");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COMBI %.0f tiene como antecesor la NORMAL %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		   array3 = json_object_dotget_array(object, "transformacion.combis");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COMBI %.0f tiene como antecesor la COMBI %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }

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
    /*SUCESOR: No pueden ser Combis.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL) /*sucesor*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idSucesor = json_array_get_number(array2, j);
		   for (k = 0; k < json_array_get_count(array); k++)
		   {
			object2 = json_array_get_object(array, k);
			if (idSucesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La COMBI %.0f tiene como Sucesor a la COMBI %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }
    /*COMBI: SUCESOR: Se admite cualquier número de salidas, excepto 0.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL)
	    {
		if(json_array_get_count(array2)==0)
		{
		    sprintf(error,"ERROR: La COMBI %.0f no tiene Sucesores",json_object_dotget_number(objectInArray,"idnodo"));
		    return 0;
		}
	    }
	    else
	    {
		sprintf(error,"ERROR: La COMBI %.0f no tiene Sucesores",json_object_dotget_number(objectInArray,"idnodo"));
		return 0;
	    }
        }
    }

    /*Validaciones Normal*/
    array = json_object_dotget_array(object, "transformacion.normales");

    /*NORMAL: ANTESESOR: Se admite cualquier número de entradas, excepto 0.*/
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
		    sprintf(error,"ERROR: La NORMAL %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		    return 0;
		}
	    }
	    else
	    {
		sprintf(error,"ERROR: La NORMAL %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		return 0;
	    }
        }
    }

    /*NORMAL: ANTESESOR: No pueden ser colas*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Antecesor");
	    if (array2 != NULL) /*antecesor*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idAntecesor = json_array_get_number(array2, j);
		   array3 = json_object_dotget_array(object, "transformacion.colas");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La NORMAL %.0f tiene como Antecesor a la COLA %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }
    /*NORMAL: SUCESOR: Se admite cualquier número de salidas, excepto 0.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL)
	    {
		if(json_array_get_count(array2)==0)
		{
		    sprintf(error,"ERROR: La NORMAL %.0f no tiene Sucesores",json_object_dotget_number(objectInArray,"idnodo"));
		    return 0;
		}
	    }
	    else
	    {
		sprintf(error,"ERROR: La NORMAL %.0f no tiene Sucesores",json_object_dotget_number(objectInArray,"idnodo"));
		return 0;
	    }
        }
    }

    /*NORMAL: SUCESOR: No pueden ser COMBIS.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL) /*sucesor*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idAntecesor = json_array_get_number(array2, j);
		   array3 = json_object_dotget_array(object, "transformacion.combis");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: la NORMAL %.0f tiene como Sucesor a la COMBI %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }

    /*Validaciones FUNCION*/
    array = json_object_dotget_array(object, "transformacion.funciones");
    /*FUNCION: ANTESESOR: Se admite cualquier número de entradas, excepto 0.*/
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
		    sprintf(error,"ERROR: La FUNCION %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		    return 0;
		}
	    }
	    else
	    {
		sprintf(error,"ERROR: La FUNCION %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		return 0;
	    }
        }
    }

    /*FUNCION: ANTESESOR: No pueden ser colas.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Antecesor");
	    if (array2 != NULL) /*antecesor*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idAntecesor = json_array_get_number(array2, j);
		   array3 = json_object_dotget_array(object, "transformacion.colas");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: La FUNCION %.0f tiene como Antecesor a la COLA %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }
    /*FUNCION: SUCESOR: Se admite cualquier número de salidas, excepto 0.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL)
	    {
		if(json_array_get_count(array2)==0)
		{
		    sprintf(error,"ERROR: La FUNCION %.0f no tiene Sucesores",json_object_dotget_number(objectInArray,"idnodo"));
		    return 0;
		}
	    }
	    else
	    {
		sprintf(error,"ERROR: La FUNCION %.0f no tiene Sucesores",json_object_dotget_number(objectInArray,"idnodo"));
		return 0;
	    }
        }
    }

    /*FUNCION: SUCESOR: No pueden ser Combis*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL) /*sucesor*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idAntecesor = json_array_get_number(array2, j);
		   array3 = json_object_dotget_array(object, "transformacion.combis");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: la FUNCION %.0f tiene como Sucesor a la COMBI %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }
    /*Validaciones CONTADOR*/
    array = json_object_dotget_array(object, "transformacion.contadores");

    /*CONTADOR: ANTESESOR: Se admite cualquier número de entradas, excepto 0.*/
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
		    sprintf(error,"ERROR: El CONTADOR %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		    return 0;
		}
	    }
	    else
	    {
		sprintf(error,"ERROR: El CONTADOR %.0f no tiene Antecesores",json_object_dotget_number(objectInArray,"idnodo"));
		return 0;
	    }
        }
    }

    /*CONTADOR: ANTESESOR: No pueden ser colas ni otro contador.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Antecesor");
	    if (array2 != NULL) /*antecesor*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idAntecesor = json_array_get_number(array2, j);
		   array3 = json_object_dotget_array(object, "transformacion.colas");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: El CONTADOR %.0f tiene como Antecesor a la COLA %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }

		   for (k = 0; k < json_array_get_count(array); k++)
		   {
			object2 = json_array_get_object(array, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: El CONTADOR %.0f tiene como Antecesor a el CONTADOR %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }

    /*CONTADOR: SUCESOR: No pueden ser combis, ni contadores.*/
    if (array != NULL)
    {
        for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL) /*sucesor*/
	    {
		for (j = 0; j < json_array_get_count(array2); j++)
		{
		   idAntecesor = json_array_get_number(array2, j);
		   array3 = json_object_dotget_array(object, "transformacion.combis");
		   for (k = 0; k < json_array_get_count(array3); k++)
		   {
			object2 = json_array_get_object(array3, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: El CONTADOR %.0f tiene como Sucesor a la COMBI %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }

		   for (k = 0; k < json_array_get_count(array); k++)
		   {
			object2 = json_array_get_object(array, k);
			if (idAntecesor == json_object_dotget_number(object2,"idnodo")){
			     sprintf(error,"ERROR: El CONTADOR %.0f tiene como Sucesor a el CONTADOR %.0f",json_object_dotget_number(objectInArray,"idnodo"), json_object_dotget_number(object2,"idnodo"));
			     return 0;
			}
		   }
		}
	    }
        }
    }

    strcpy(error, "OK");
    json_value_free(root_value);
    return 1;
}



