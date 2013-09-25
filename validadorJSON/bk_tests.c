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

#define TEST(A) printf("%-72s-",#A);              \
                if(A){puts(" OK");tests_passed++;} \
                else{puts(" FAIL");tests_failed++;}
#define STREQ(A, B) (A && B ? strcmp(A, B) == 0 : 0)

void own_test_suite(void);
void test_suite_1(void);
void test_suite_2(void);
void test_suite_3(void);

void print_commits_info(const char *username, const char *repo);
void validateJsonImput(const char *filename, char *error);

static int tests_passed;
static int tests_failed;

int main() {

    const char *filename = "tests/modelo.txt";
    char error[100];
    /* retorna 0 error 1 valido, filename nombre del archivo, string del error */
    validateJsonImput(filename, error);
    printf("%s\n",error);

    /*pushNodesInMemory(filename);*/

    /*getNodeById(id);*/

    return 0;
}

void validateJsonImput(const char *filename, char *error) {
    JSON_Value  *root_value;/*file*/
    JSON_Object *object, *objectInArray;
    JSON_Array  *array,  *array2;

    size_t i;


    /*validate json*/
    root_value = json_parse_file(filename);
    if (!root_value)
    {
	strcpy(error, "ERROR: Estructura de Json no valida");
	return;
    }

    /*validate nature of json*/
    if (json_value_get_type(root_value) == JSONArray)
    {
	strcpy(error, "ERROR: Estructura de Json no valida");
	return;
    }

    if (json_value_get_type(root_value) != JSONObject)
    {
	strcpy(error, "ERROR: Estructura de Json no valida");
	return;
    }

    /*object = json_value_get_object(root_value);
    TEST(STREQ(json_object_dotget_string(object, "tgs.nombre"), "Sistema de prueba"));
    TEST(STREQ(json_object_dotget_string(object, "tgs.sistema"), "Discreto"));*/

    
    array = json_object_dotget_array(object, "transformacion.colas");
    /*if (array != NULL) {
        for (i = 0; i < json_array_get_count(array); i++) {
            //TEST(json_array_get_number(array, i) < 2);
	    objectInArray = json_array_get_object(array, i);
            printf("%f\n", json_object_get_number(objectInArray, "resources") );
        }
    }*/
    
    /*Validaciones Cola*/

    if (array != NULL) { /* array de colas*/
	/*printf("Nro de colas %zu\n",json_array_get_count(array));*/
	printf("%lu\n", (unsigned long)json_array_get_count(array));
        for (i = 0; i < json_array_get_count(array); i++) {
            /*TEST(json_array_get_number(array, i) < 2);*/
	    objectInArray = json_array_get_object(array, i);
	    printf("Cola %f:",json_object_get_number(objectInArray, "idnodo"));
	    array2 = json_object_dotget_array(objectInArray, "Sucesor");
	    if (array2 != NULL) { /* array de sucesores de una cola */
		/*for (i = 0; i < json_array_get_count(array2); i++) {
		     printf("%f\t", json_array_get_number(array2, i) );
		}*/
		if(json_array_get_count(array2)==0){
			printf("la cola no tiene sucesores\n");
		}
	    }
	    else{
		printf("la cola no tiene sucesores\n");
	    }
	    printf("\n");
        }
    }

    
    if (root_value) { json_value_free(root_value); }
}

/* 3 test files from json.org */
void test_suite_1(void) {
    JSON_Value *val;
    TEST((val = json_parse_file("tests/test_1_1.txt")) != NULL);
    if (val) { json_value_free(val); }
    TEST((val = json_parse_file("tests/test_1_2.txt")) != NULL);
    if (val) { json_value_free(val); }
    TEST((val = json_parse_file("tests/test_1_3.txt")) != NULL);
    if (val) { json_value_free(val); }
}

/* Testing correctness of parsed values */
void test_suite_2(void) {
    JSON_Value *root_value;
    JSON_Object *object;
    JSON_Array *array;
    size_t i;
    const char *filename = "tests/test_2.txt";
    printf("Testing %s:\n", filename);
    root_value = json_parse_file(filename);
    TEST(root_value);
    TEST(json_value_get_type(root_value) == JSONObject);
    object = json_value_get_object(root_value);
    TEST(STREQ(json_object_get_string(object, "string"), "lorem ipsum"));
    TEST(STREQ(json_object_get_string(object, "utf string"), "lorem ipsum"));
    TEST(STREQ(json_object_get_string(object, "utf-8 string"), "あいうえお"));
    TEST(json_object_get_number(object, "positive one") == 1.0);
    TEST(json_object_get_number(object, "negative one") == -1.0);
    TEST(json_object_get_number(object, "hard to parse number") == -0.000314);
    TEST(json_object_get_boolean(object, "boolean true") == 1);
    TEST(json_object_get_boolean(object, "boolean false") == 0);
    TEST(json_value_get_type(json_object_get_value(object, "null")) == JSONNull);
    
    array = json_object_get_array(object, "string array");
    if (array != NULL && json_array_get_count(array) > 1) {
        TEST(STREQ(json_array_get_string(array, 0), "lorem"));
        TEST(STREQ(json_array_get_string(array, 1), "ipsum"));
    } else {
        tests_failed++;
    }
    
    array = json_object_get_array(object, "x^2 array");
    if (array != NULL) {
        for (i = 0; i < json_array_get_count(array); i++) {
            TEST(json_array_get_number(array, i) == (i * i));
        }
    } else {
        tests_failed++;
    }
    
    TEST(json_object_get_array(object, "non existent array") == NULL);
    TEST(STREQ(json_object_dotget_string(object, "object.nested string"), "str"));
    TEST(json_object_dotget_boolean(object, "object.nested true") == 1);
    TEST(json_object_dotget_boolean(object, "object.nested false") == 0);
    TEST(json_object_dotget_value(object, "object.nested null") != NULL);
    TEST(json_object_dotget_number(object, "object.nested number") == 123);
    
    TEST(json_object_dotget_value(object, "should.be.null") == NULL);
    TEST(json_object_dotget_value(object, "should.be.null.") == NULL);
    TEST(json_object_dotget_value(object, ".") == NULL);
    TEST(json_object_dotget_value(object, "") == NULL);
    
    array = json_object_dotget_array(object, "object.nested array");
    if (array != NULL && json_array_get_count(array) > 1) {
        TEST(STREQ(json_array_get_string(array, 0), "lorem"));
        TEST(STREQ(json_array_get_string(array, 1), "ipsum"));
    } else {
        tests_failed++;
    }
    TEST(json_object_dotget_boolean(object, "nested true"));
    json_value_free(root_value);
}

/* Testing values, on which parsing should fail */
void test_suite_3(void) {
    char nested_20x[] = "[[[[[[[[[[[[[[[[[[[[\"hi\"]]]]]]]]]]]]]]]]]]]]";
    puts("Testing invalid strings:");
    TEST(json_parse_string(NULL) == NULL);
    TEST(json_parse_string("") == NULL); /* empty string */
    TEST(json_parse_string("[\"lorem\",]") == NULL);
    TEST(json_parse_string("[,]") == NULL);
    TEST(json_parse_string("[,") == NULL);
    TEST(json_parse_string("[") == NULL);
    TEST(json_parse_string("]") == NULL);
    TEST(json_parse_string("{\"a\":0,\"a\":0}") == NULL); /* duplicate keys */
    TEST(json_parse_string("{:,}") == NULL);
    TEST(json_parse_string("{,}") == NULL);
    TEST(json_parse_string("{,") == NULL);
    TEST(json_parse_string("{:") == NULL);
    TEST(json_parse_string("{") == NULL);
    TEST(json_parse_string("}") == NULL);
    TEST(json_parse_string("x") == NULL);
    TEST(json_parse_string("\"string\"") == NULL);
    TEST(json_parse_string("{:\"no name\"}") == NULL);
    TEST(json_parse_string("[,\"no first value\"]") == NULL);
    TEST(json_parse_string("[\"\\u00zz\"]") == NULL); /* invalid utf value */
    TEST(json_parse_string("[\"\\u00\"]") == NULL); /* invalid utf value */
    TEST(json_parse_string("[\"\\u\"]") == NULL); /* invalid utf value */
    TEST(json_parse_string("[\"\\\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\"\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\0\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\a\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\b\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\t\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\n\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\f\"]") == NULL); /* control character */
    TEST(json_parse_string("[\"\r\"]") == NULL); /* control character */
    TEST(json_parse_string(nested_20x) == NULL); /* too deep */
    TEST(json_parse_string("[0x2]") == NULL);    /* hex */
    TEST(json_parse_string("[0X2]") == NULL);    /* HEX */
    TEST(json_parse_string("[07]") == NULL);     /* octals */
    TEST(json_parse_string("[0070]") == NULL);
    TEST(json_parse_string("[07.0]") == NULL);
    TEST(json_parse_string("[-07]") == NULL);
    TEST(json_parse_string("[-007]") == NULL);
    TEST(json_parse_string("[-07.0]") == NULL);
}

void print_commits_info(const char *username, const char *repo) {
    JSON_Value *root_value;
    JSON_Array *commits;
    JSON_Object *commit;
    size_t i;
    
    char curl_command[512];
    char cleanup_command[256];
    char output_filename[] = "commits.json";
    
    /* it ain't pretty, but it's not a libcurl tutorial */
    sprintf(curl_command, 
        "curl -s \"https://api.github.com/repos/%s/%s/commits\" > %s",
        username, repo, output_filename);
    sprintf(cleanup_command, "rm -f %s", output_filename);
    system(curl_command);
    
    /* parsing json and validating output */
    root_value = json_parse_file(output_filename);
    if (json_value_get_type(root_value) != JSONArray) {
        system(cleanup_command);
        return;
    }
    
    /* getting array from root value and printing commit info */
    commits = json_value_get_array(root_value);
    printf("%-10.10s %-10.10s %s\n", "Date", "SHA", "Author");
    for (i = 0; i < json_array_get_count(commits); i++) {
        commit = json_array_get_object(commits, i);
        printf("%.10s %.10s %s\n",
               json_object_dotget_string(commit, "commit.author.date"),
               json_object_get_string(commit, "sha"),
               json_object_dotget_string(commit, "commit.author.name"));
    }
    
    /* cleanup code */
    json_value_free(root_value);
    system(cleanup_command);
}
