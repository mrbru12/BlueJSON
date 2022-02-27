/*
https://www.youtube.com/watch?v=N9RUqGYuGfw
https://notes.eatonphil.com/writing-a-simple-json-parser.html
https://stackoverflow.com/questions/17063690/how-to-write-a-basic-json-parsing-class
https://fsharpforfunandprofit.com/posts/understanding-parser-combinators-4/
https://abhinavsarkar.net/posts/json-parsing-from-scratch-in-haskell/
https://lihautan.com/json-parser-with-javascript/
*/

// TODOs:
// * Fazer um sistema de checagem de erro com uma função tipo bjson_expect(const char *tokens). Dai, por exemplo, depois de encontrar
//   o começo de um objeto '{' teria que ter um bjson_expect(""}"), ou seja, ele esperaria que tivesse uma '"' (significando o
//   começo de uma string), ou um '}' (significando o final do objeto, que no caso seria um objeto vazio), os espaços em branco ' '
//   a função vai ignorar. Como exemplo ver aquele site que checa se o JSON ta certo
// * Precisa urgentemente dar uma refatorada em praticamente tudo
// * Dar uma documentada boa em tudo
// * Fazer uns benchmarks pra procurar lugares que daria pra dar uma otimizada melhor
// * Criar um repo no github e colocar uma foto de um Blue Jay, também colocar alguma referência a Blue Jay Way. Talvez eu tenha que colocar
//   créditos das imagens que eu usar
// * Reduzir ao máximo o número de #includes
// * Adicionar uma licença no começo desse arquivo
// * Talvez no Array e no Object mudar os nomes dos getters e setters pra ..._find_get_...() e ..._find_set_...()
// * Talvez olhar nos commits antigos no Github e pegar de volta alguns TODOs que eu tirei pq pensei que n ia mais precisar
// * Seria interessante se os setters retornassem o objeto pra poder fazer várias operações encadeadas em uma única linha
// * Seria legal mudar o design do projeto inteiro e fazer tudo estilo funções C, teria tipo fprintjson(FILE *file, ...)
// * Talvez mudar de bjson_read_...() pra bjson_load_...() porque se pa que faz mais sentido
// * Talvez desencanar de tudo e deixar o BlueJSON como sendo simplesmente um parser de JSON, sem função pra dar write e sem checagem de erro

#ifndef BJSON_BLUEJSON_H
#define BJSON_BLUEJSON_H

// A thing can be any JSON value, having a name if it belongs to an object
typedef struct bjson_thing bjson_thing;

// In JSON, objects are collections of tuples with (name, value)
typedef struct bjson_object bjson_object;

// In JSON, arrays are collections of values
typedef struct bjson_array bjson_array;

typedef enum
{
    BJSON_NOTHING,
    BJSON_STRING,
    BJSON_NUMBER,
    BJSON_OBJECT,
    BJSON_ARRAY,
    BJSON_TRUE,
    BJSON_FALSE,
    BJSON_NULL,
} bjson_value_type;

typedef union
{
    char *string;
    double number;
    bjson_object *object;
    bjson_array *array;
} bjson_value;

bjson_thing *bjson_thing_create();
void bjson_thing_destroy(bjson_thing *thing);

char *bjson_thing_get_name(const bjson_thing *thing, char *buffer, unsigned int size);
bjson_value_type bjson_thing_get_value_type(const bjson_thing *thing);
// TODO: bjson_value bjson_thing_get_value(const bjson_thing *thing);

// TODO: Fazer todos os getters tipo esses: com const
char *bjson_thing_get_as_string(const bjson_thing *thing, char *buffer, unsigned int size);
int bjson_thing_get_as_int(const bjson_thing *thing);
float bjson_thing_get_as_float(const bjson_thing *thing);
double bjson_thing_get_as_double(const bjson_thing *thing);
bjson_object *bjson_thing_get_as_object(const bjson_thing *thing);
bjson_array *bjson_thing_get_as_array(const bjson_thing *thing);
int bjson_thing_is_true(const bjson_thing *thing);
int bjson_thing_is_false(const bjson_thing *thing);
int bjson_thing_is_null(const bjson_thing *thing);

void bjson_thing_set_name(bjson_thing *thing, const char *name);

void bjson_thing_set_as_string(bjson_thing *thing, const char *string);
void bjson_thing_set_as_int(bjson_thing *thing, int number);
void bjson_thing_set_as_float(bjson_thing *thing, float number);
void bjson_thing_set_as_double(bjson_thing *thing, double number);
void bjson_thing_set_as_object(bjson_thing *thing, bjson_object *object);
void bjson_thing_set_as_array(bjson_thing *thing, bjson_array *array);
void bjson_thing_set_as_true(bjson_thing *thing);
void bjson_thing_set_as_false(bjson_thing *thing);
void bjson_thing_set_as_null(bjson_thing *thing);

bjson_object *bjson_object_create();
void bjson_object_destroy(bjson_object *object);

bjson_thing *bjson_object_get_thing(const bjson_object *object, const char *name);
char *bjson_object_get_string(const bjson_object *object, const char *name, char *buffer, unsigned int size);
int bjson_object_get_int(const bjson_object *object, const char *name);
float bjson_object_get_float(const bjson_object *object, const char *name);
double bjson_object_get_double(const bjson_object *object, const char *name);
bjson_object *bjson_object_get_object(const bjson_object *object, const char *name);
bjson_array *bjson_object_get_array(const bjson_object *object, const char *name);
int bjson_object_is_true(const bjson_object *object, const char *name);
int bjson_object_is_false(const bjson_object *object, const char *name);
int bjson_object_is_null(const bjson_object *object, const char *name);

// Appends the specified thing to the end of the object things
void bjson_object_push_thing(bjson_object *object, bjson_thing *thing);

bjson_array *bjson_array_create();
void bjson_array_destroy(bjson_array *array);

bjson_thing *bjson_array_get_thing(const bjson_array *array, unsigned int index);
char *bjson_array_get_string(const bjson_array *array, unsigned int index, char *buffer, unsigned int size);
int bjson_array_get_int(const bjson_array *array, unsigned int index);
float bjson_array_get_float(const bjson_array *array, unsigned int index);
double bjson_array_get_double(const bjson_array *array, unsigned int index);
bjson_object *bjson_array_get_object(const bjson_array *array, unsigned int index);
bjson_array *bjson_array_get_array(const bjson_array *array, unsigned int index);
int bjson_array_is_true(const bjson_array *array, unsigned int index);
int bjson_array_is_false(const bjson_array *array, unsigned int index);
int bjson_array_is_null(const bjson_array *array, unsigned int index);

// Appends the specified thing to the end of the array things
void bjson_array_push_thing(bjson_array *array, bjson_thing *thing);

// Parses the specified n strings
// Returns the outer-most root bjson_thing, or NULL on error
bjson_thing *bjson_read_strings(const char *strs[], unsigned int n);

// Parses the specified string
// Returns the outer-most root bjson_thing, or NULL on error
bjson_thing *bjson_read_string(const char *str);

// Parses the entire JSON file specified by the file path
// Returns the outer-most root bjson_thing, or NULL on error
bjson_thing *bjson_read_file(const char *path);

// Writes the JSON structure among the n strings. All buffers are expected to be the same size
void bjson_write_strings(bjson_thing *thing, char *buffers[], unsigned int size, unsigned int n);

void bjson_write_string(bjson_thing *thing, char *buffer, unsigned int size);

void bjson_write_file(bjson_thing *thing, const char *path);

#endif

#ifdef BJSON_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BJSON_FILE_MAX_LINE_SIZE 256
#define BJSON_FILE_MAX_LINES 1024

#define BJSON_DYNSTR_SIZE_AS_WRITE_BUFFER 256

#define BJSON_WRITE_TAB_SIZE 2

struct bjson_thing
{
    char *name;
    bjson_value_type type;
    bjson_value value;
};

bjson_thing *bjson_thing_create()
{
    bjson_thing *thing = malloc(sizeof(bjson_thing));
    thing->name = NULL;
    thing->type = BJSON_NOTHING;

    return thing;
}

void bjson_thing_destroy(bjson_thing *thing)
{
    if (thing->name != NULL)
        free(thing->name);

    if (thing->type == BJSON_STRING)
        free(thing->value.string);
    else if (thing->type == BJSON_OBJECT)
        bjson_object_destroy(thing->value.object);
    else if (thing->type == BJSON_ARRAY)
        bjson_array_destroy(thing->value.array);

    free(thing);
}

void bjson_thing_destroy_value(bjson_thing *thing)
{
    if (thing->type == BJSON_STRING)
        free(thing->value.string);
    else if (thing->type == BJSON_OBJECT)
        bjson_object_destroy(thing->value.object);
    else if (thing->type == BJSON_ARRAY)
        bjson_array_destroy(thing->value.array);

    thing->type = BJSON_NOTHING;
}

char *bjson_thing_get_name(const bjson_thing *thing, char *buffer, unsigned int size)
{
    strncpy(buffer, thing->name, size);
    buffer[size - 1] = '\0';

    return buffer;
}

bjson_value_type bjson_thing_get_value_type(const bjson_thing *thing)
{
    return thing->type;
}

char *bjson_thing_get_as_string(const bjson_thing *thing, char *buffer, unsigned int size)
{
    strncpy(buffer, thing->value.string, size);
    buffer[size - 1] = '\0';

    return buffer;
}

int bjson_thing_get_as_int(const bjson_thing *thing)
{
    return (int)(thing->value.number);
}

float bjson_thing_get_as_float(const bjson_thing *thing)
{
    return (float)(thing->value.number);
}

double bjson_thing_get_as_double(const bjson_thing *thing)
{
    return thing->value.number;
}

bjson_object *bjson_thing_get_as_object(const bjson_thing *thing)
{
    return (thing == NULL || thing->type != BJSON_OBJECT) ? NULL : thing->value.object;
}

bjson_array *bjson_thing_get_as_array(const bjson_thing *thing)
{
    return (thing == NULL || thing->type != BJSON_ARRAY) ? NULL : thing->value.array;
}

int bjson_thing_is_true(const bjson_thing *thing)
{
    return thing->type == BJSON_TRUE;
}

int bjson_thing_is_false(const bjson_thing *thing)
{
    return thing->type == BJSON_FALSE;
}

int bjson_thing_is_null(const bjson_thing *thing)
{
    return thing->type == BJSON_NULL;
}

void bjson_thing_set_name(bjson_thing *thing, const char *name)
{
    if (thing->name != NULL)
        free(thing->name);

    thing->name = malloc(strlen(name) + 1);
    strcpy(thing->name, name);
}

void bjson_thing_set_as_string(bjson_thing *thing, const char *string)
{
    if (thing->type != BJSON_NOTHING)
        bjson_thing_destroy_value(thing);

    thing->type = BJSON_STRING;
    thing->value.string = malloc(strlen(string) + 1);
    strcpy(thing->value.string, string);
}

void bjson_thing_set_as_int(bjson_thing *thing, int number)
{
    bjson_thing_set_as_double(thing, (double)number);
}

void bjson_thing_set_as_float(bjson_thing *thing, float number)
{
    bjson_thing_set_as_double(thing, (double)number);
}

void bjson_thing_set_as_double(bjson_thing *thing, double number)
{
    if (thing->type != BJSON_NOTHING)
        bjson_thing_destroy_value(thing);

    thing->type = BJSON_NUMBER;
    thing->value.number = number;
}

void bjson_thing_set_as_object(bjson_thing *thing, bjson_object *object)
{
    if (thing->type != BJSON_NOTHING)
        bjson_thing_destroy_value(thing);

    thing->type = BJSON_OBJECT;
    thing->value.object = object;
}

void bjson_thing_set_as_array(bjson_thing *thing, bjson_array *array)
{
    if (thing->type != BJSON_NOTHING)
        bjson_thing_destroy_value(thing);

    thing->type = BJSON_ARRAY;
    thing->value.array = array;
}

void bjson_thing_set_as_true(bjson_thing *thing)
{
    if (thing->type != BJSON_NOTHING)
        bjson_thing_destroy_value(thing);

    thing->type = BJSON_TRUE;
}

void bjson_thing_set_as_false(bjson_thing *thing)
{
    if (thing->type != BJSON_NOTHING)
        bjson_thing_destroy_value(thing);

    thing->type = BJSON_FALSE;
}

void bjson_thing_set_as_null(bjson_thing *thing)
{
    if (thing->type != BJSON_NOTHING)
        bjson_thing_destroy_value(thing);

    thing->type = BJSON_NULL;
}

typedef struct bjson_thing_list_node
{
    bjson_thing *thing;
    struct bjson_thing_list_node *next;
} bjson_thing_list_node;

typedef struct
{
    bjson_thing_list_node *start, *end;
} bjson_thing_list;

bjson_thing_list *bjson_thing_list_create()
{
    bjson_thing_list *list = malloc(sizeof(bjson_thing_list));
    list->start = NULL;
    list->end = NULL;

    return list;
}

void bjson_thing_list_destroy(bjson_thing_list *list)
{
    bjson_thing_list_node *node = list->start;
    while (node != NULL)
    {
        bjson_thing_list_node *temp_node = node;
        node = node->next;

        free(temp_node);
    }

    free(list);
}

int bjson_thing_list_is_empty(bjson_thing_list *list)
{
    return list->start == NULL && list->end == NULL;
}

void bjson_thing_list_append(bjson_thing_list *list, bjson_thing *thing)
{
    bjson_thing_list_node *node = malloc(sizeof(bjson_thing_list_node));
    node->thing = thing;
    node->next = NULL;

    if (bjson_thing_list_is_empty(list))
        list->start = node;
    else
        list->end->next = node;
    list->end = node;
}

// void bjson_thing_list_add(bjson_thing_list *list, unsigned int index)

// void bjson_thing_list_remove(bjson_thing_list *list, unsigned int index)

bjson_thing *bjson_thing_list_get_at(bjson_thing_list *list, unsigned int index)
{
    bjson_thing_list_node *node = list->start;
    for (int i = 0; i < index; i++)
    {
        if (node == NULL)
            return NULL;

        node = node->next;
    }

    return node->thing;
}

bjson_thing *bjson_thing_list_get_by_name(bjson_thing_list *list, const char *name)
{
    bjson_thing_list_node *node = list->start;
    while (node != NULL)
    {
        if (node->thing->name != NULL && strcmp(node->thing->name, name) == 0)
            return node->thing;

        node = node->next;
    }

    return NULL;
}

typedef struct bjson_thing_stack_node
{
    bjson_thing *thing;
    struct bjson_thing_stack_node *link;
} bjson_thing_stack_node;

typedef struct
{
    bjson_thing_stack_node *top;
} bjson_thing_stack;

bjson_thing_stack *bjson_thing_stack_create()
{
    bjson_thing_stack *stack = malloc(sizeof(bjson_thing_stack));
    stack->top = NULL;

    return stack;
}

void bjson_thing_stack_destroy(bjson_thing_stack *stack)
{
    bjson_thing_stack_node *node = stack->top;
    while (node != NULL)
    {
        bjson_thing_stack_node *temp_node = node;
        node = node->link;

        free(temp_node);
    }

    free(stack);
}

int bjson_thing_stack_is_empty(bjson_thing_stack *stack)
{
    return stack->top == NULL;
}

void bjson_thing_stack_push(bjson_thing_stack *stack, bjson_thing *thing)
{
    bjson_thing_stack_node *node = malloc(sizeof(bjson_thing_stack_node));
    node->thing = thing;
    node->link = stack->top;

    stack->top = node;
}

bjson_thing *bjson_thing_stack_pop(bjson_thing_stack *stack)
{
    bjson_thing_stack_node *node = stack->top;

    bjson_thing *thing = NULL;
    if (!bjson_thing_stack_is_empty(stack))
    {
        thing = node->thing;
        stack->top = node->link;

        free(node);
    }

    return thing;
}

bjson_thing *bjson_thing_stack_peek(bjson_thing_stack *stack)
{
    return stack->top->thing;
}

struct bjson_object
{
    bjson_thing_list *things;
};

bjson_object *bjson_object_create()
{
    bjson_object *object = malloc(sizeof(bjson_object));
    object->things = bjson_thing_list_create();

    return object;
}

void bjson_object_destroy(bjson_object *object)
{
    for (bjson_thing_list_node *node = object->things->start; node != NULL; node = node->next)
        bjson_thing_destroy(node->thing);

    bjson_thing_list_destroy(object->things);
    free(object);
}

bjson_thing *bjson_object_get_thing(const bjson_object *object, const char *name)
{
    return bjson_thing_list_get_by_name(object->things, name);
}

char *bjson_object_get_string(const bjson_object *object, const char *name, char *buffer, unsigned int size)
{
    return bjson_thing_get_as_string(bjson_object_get_thing(object, name), buffer, size);
}

int bjson_object_get_int(const bjson_object *object, const char *name)
{
    return (int)bjson_object_get_double(object, name);
}

float bjson_object_get_float(const bjson_object *object, const char *name)
{
    return (float)bjson_object_get_double(object, name);
}

double bjson_object_get_double(const bjson_object *object, const char *name)
{
    return bjson_thing_get_as_double(bjson_object_get_thing(object, name));
}

bjson_object *bjson_object_get_object(const bjson_object *object, const char *name)
{
    return bjson_thing_get_as_object(bjson_object_get_thing(object, name));
}

bjson_array *bjson_object_get_array(const bjson_object *object, const char *name)
{
    return bjson_thing_get_as_array(bjson_object_get_thing(object, name));
}

int bjson_object_is_true(const bjson_object *object, const char *name)
{
    return bjson_thing_is_true(bjson_object_get_thing(object, name));
}

int bjson_object_is_false(const bjson_object *object, const char *name)
{
    return bjson_thing_is_false(bjson_object_get_thing(object, name));
}

int bjson_object_is_null(const bjson_object *object, const char *name)
{
    return bjson_thing_is_null(bjson_object_get_thing(object, name));
}

void bjson_object_push_thing(bjson_object *object, bjson_thing *thing)
{
    bjson_thing_list_append(object->things, thing);
}

struct bjson_array
{
    bjson_thing_list *things;
};

bjson_array *bjson_array_create()
{
    bjson_array *array = malloc(sizeof(bjson_array));
    array->things = bjson_thing_list_create();

    return array;
}

void bjson_array_destroy(bjson_array *array)
{
    for (bjson_thing_list_node *node = array->things->start; node != NULL; node = node->next)
        bjson_thing_destroy(node->thing);

    bjson_thing_list_destroy(array->things);
    free(array);
}

bjson_thing *bjson_array_get_thing(const bjson_array *array, unsigned int index)
{
    return bjson_thing_list_get_at(array->things, index);
}

char *bjson_array_get_string(const bjson_array *array, unsigned int index, char *buffer, unsigned int size)
{
    return bjson_thing_get_as_string(bjson_array_get_thing(array, index), buffer, size);
}

int bjson_array_get_int(const bjson_array *array, unsigned int index)
{
    return (int)bjson_array_get_double(array, index);
}

float bjson_array_get_float(const bjson_array *array, unsigned int index)
{
    return (float)bjson_array_get_double(array, index);
}

double bjson_array_get_double(const bjson_array *array, unsigned int index)
{
    return bjson_thing_get_as_double(bjson_array_get_thing(array, index));
}

bjson_object *bjson_array_get_object(const bjson_array *array, unsigned int index)
{
    return bjson_thing_get_as_object(bjson_array_get_thing(array, index));
}

bjson_array *bjson_array_get_array(const bjson_array *array, unsigned int index)
{
    return bjson_thing_get_as_array(bjson_array_get_thing(array, index));
}

int bjson_array_is_true(const bjson_array *array, unsigned int index)
{
    return bjson_thing_is_true(bjson_array_get_thing(array, index));
}

int bjson_array_is_false(const bjson_array *array, unsigned int index)
{
    return bjson_thing_is_false(bjson_array_get_thing(array, index));
}

int bjson_array_is_null(const bjson_array *array, unsigned int index)
{
    return bjson_thing_is_null(bjson_array_get_thing(array, index));
}

void bjson_array_push_thing(bjson_array *array, bjson_thing *thing)
{
    bjson_thing_list_append(array->things, thing);
}

#ifdef BJSON_NO_ERROR_CHECKING

int bjson_expect_is_char(char c, const char *expected_chars, unsigned int count)
{
    return 1;
}

int bjson_expect_is_number(char c)
{
    return 1;
}

#else

int bjson_expect_is_char(char c, const char *expected_chars, unsigned int count)
{
    for (int i = 0; i < count; i++)
    {
        if (c == expected_chars[i])
            return 1;
    }

    return 0;
}

int bjson_expect_is_number(char c)
{
    return isdigit(c);
}

#endif

void bjson_panic(const char *msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(1);
}

size_t bjson_string_len(const char *string) // TODO: Talvez trocar string por str
{
    return strchr(string + 1, '"') - (string + 1);
}

int bjson_parse_string(const char *string, char *buffer, unsigned int size) // TODO: Talvez trocar string por string_str
{
    // TODO: Precisa de mais checks desse tipo (checar se não está no fim da linha, etc...) em outros lugares do parser
    int string_i = 1, buffer_i = 0;
    for (; string[string_i] != '"' && string[string_i] != '\0' && buffer_i < size - 1; string_i++, buffer_i++)
    {
        if (string[string_i] == '\\')
        {
            // TODO: Talvez abandonar a ideia de checar erros (ou pensar em outra maneira mais adequada) porque não da pra separar as funções direito
            if (!bjson_expect_is_char(string[++string_i], "\"\\/bfnrtu", 9)) // TODO: Talvez fazer if (bjson_expect_is_...(...)) e colocar o panic no else em tudo
                bjson_panic("Unexpected char somewhere!");
            else
            {
                switch (string[string_i])
                {
                case 'b':
                    buffer[buffer_i] = '\b';
                    break;

                case 'f':
                    buffer[buffer_i] = '\f';
                    break;

                case 'n':
                    buffer[buffer_i] = '\n';
                    break;

                case 'r':
                    buffer[buffer_i] = '\r';
                    break;

                case 't':
                    buffer[buffer_i] = '\t';
                    break;

                case 'u': // TODO: Isso representa um caracter unicode, então vai estar no formato \u seguido de 4 digitos que são o identificador do caractere
                    // TODO: buffer[i - 1] = '\u1234';
                    break;

                default:
                    buffer[buffer_i] = string[string_i]; // The cases that left are not any special code and can be just copied
                }
            }
        }
        else
            buffer[buffer_i] = string[string_i];
    }

    buffer[buffer_i] = '\0';

    return string_i + 1;
}

int bjson_parse_number(const char *number, double *buffer) // TODO: Talvez trocar number por number_str
{
    // TODO: Precisa de uma boa refatorada nessa parte de números pra deixar mais claro o que ta acontecendo, apesar de estar bonitin *-*
    // TODO: Precisa fazer mensagens de erro correspondentes pra cada tipo de erro e talvez mostrar mais infos nelas como a linha que deu erro

    int number_i = 0; // Number string iterator

    *buffer = 0.0;
    int is_number_negative = number[0] == '-'; // TODO: Talvez trocar isso pra: int number_signal = line[i] == '-' ? -1 : 1;

    if (is_number_negative)
    {
        if (!bjson_expect_is_number(number[++number_i])) // There must be at least a digit after the signal and before the fraction point '.'
            bjson_panic("Unexpected char somewhere!");
    }

    // Parse the integer part of the number
    int integer_start = number_i;
    for (; isdigit(number[number_i]); number_i++)
        *buffer += (double)(number[number_i] - 48) / pow(10.0, number_i - integer_start); // Generate the number after the floating point, this is needed to mirror it
    *buffer *= pow(10.0, number_i - integer_start - 1);                                   // Offset the mirrored number to the left side of the floating point

    // TODO: Se pa que precisa fazer nesse estilo aqui em todos. Deve ter uma forma melhor de fazer mais eficientemente
    if (!bjson_expect_is_char(number[number_i], ".eE", 3))
    {
        if (!bjson_expect_is_char(number[number_i], ",}] \n\0", 6))
            bjson_panic("Unexpected char somewhere!");
    }

    // Parse the fraction part of the number
    if (number[number_i] == '.')
    {
        if (!bjson_expect_is_number(number[++number_i])) // There must be at least a digit after the fraction point '.'
        {
            bjson_panic("Unexpected char somewhere!");
            // fprintf(stderr, "ERROR: Unexpected '%c' at line %d!\n", line[i], current_str + 1); // TODO: Exemplo de como seria um erro mais detalhado (que não funcionaria nessa func separada)
        }

        int fraction_start = number_i;
        for (; isdigit(number[number_i]); number_i++)
            *buffer += (number[number_i] - 48) / pow(10.0, number_i - fraction_start + 1); // Generate the fraction

        if (!bjson_expect_is_char(number[number_i], "eE", 2))
        {
            if (!bjson_expect_is_char(number[number_i], ",}] \n\0", 6))
                bjson_panic("Unexpected char somewhere!");
        }
    }

    // Parse the exponent part of the number
    if (number[number_i] == 'e' || number[number_i] == 'E')
    {
        double exponent = 0.0;
        int is_exponent_negative = 0; // TODO: Talvez trocar isso pra: int exponent_signal = line[i] == '-' ? -1 : 1;

        if (bjson_expect_is_char(number[++number_i], "+-", 2)) // The signal is optional after an 'e' or 'E'
            is_exponent_negative = number[number_i++] == '-';

        if (!bjson_expect_is_number(number[number_i])) // There must be at least a digit after 'e' or 'E'
            bjson_panic("Unexpected char somewhere!");

        int exponent_start = number_i;
        for (; isdigit(number[number_i]); number_i++)
            exponent += (double)(number[number_i] - 48) / pow(10.0, number_i - exponent_start); // Generate the exponent number after the floating point, this is needed to mirror it
        exponent *= pow(10.0, number_i - exponent_start - 1);                                   // Offset the mirrored exponent number to the left side of the floating point

        if (is_exponent_negative)
            exponent = -exponent;

        *buffer *= pow(10.0, (double)exponent); // Multiply the number by the exponent of 10
    }

    // TODO: Talvez esse expect possa ser generalizado pra todos os tokens tirando ele pra fora, lembrando que a ',' só
    //       pode ser expected caso a thing esteja dentro de um object ou array, e se tiver uma ',' precisa ter outra thing depois
    if (!bjson_expect_is_char(number[number_i], ",}] \n\0", 6))
        bjson_panic("Unexpected char somewhere!");

    if (is_number_negative) // TODO: Talvez seja legal eu começar a colocar ifs assim em 1 só linha, exemplo: if (is_number_negative) *buffer = -(*buffer);
        *buffer = -(*buffer);

    return number_i;
}

// TODO: Separar mais algumas partes dessa função em subfunções pra ficar um pouco mais organizado
bjson_thing *bjson_read_strings(const char *strs[], unsigned int n)
{
    // TODO: Talvez retirar essa variável pq daria pra retornar a root thing direto pelo stack de nested things
    bjson_thing *root_thing = NULL; // TODO: Talvez mudar o nome pra root_thing

    bjson_thing_stack *nested_things = bjson_thing_stack_create();

    // TODO: Talvez quando eu refatorar e tirar a geração de thing pra mais pra fora não precise mais dessa var.
    //       Isso vai deixar o código mais legível e mais eficiênte, já que, quando necessário. o nome vai estar
    //       sendo copiado direto pra dentro da thing, e não pra essa variável temporária pra depois ser recopiado
    //       pra thing
    bjson_thing *thing = NULL;
    char *current_thing_name = NULL;

    for (unsigned int current_str = 0; current_str < n; current_str++)
    {
        const char *line = strs[current_str];

        int i = 0;
        while (i < strlen(line))
        {
            // TODO: Nas strings, checar quando tiver caracateres de escape e especiais, tipo '\"' e '\n', etc...
            if (line[i] == '"')
            {
                size_t string_len = bjson_string_len(line + i);

                if (current_thing_name == NULL &&
                    !bjson_thing_stack_is_empty(nested_things) &&
                    bjson_thing_stack_peek(nested_things)->type != BJSON_ARRAY) // Name
                {
                    /*
                    current_thing_name = malloc(string_len + 1);
                    strncpy(current_thing_name, line + i + 1, string_len);
                    current_thing_name[string_len] = '\0';

                    i += string_len + 2;
                    */

                    current_thing_name = malloc(string_len + 1);
                    i += bjson_parse_string(line + i, current_thing_name, string_len + 1);

                    continue;
                }
                else // String
                {
                    // TODO: Talvez mudar isso pra bjson_thing_set_string(thing, current_thing_name). Também mudar as outras partes pros setters correspondentes,
                    //       como quando o parser encontrar um objeto mudar para bjson_thing_set_object(thing, bjson_object_create()). Agora faria bem mais sentido
                    //       só deixar usar um setter em uma thing caso o value_type dela seja BJSON_NOTHING (ou algo do tipo) e ela esteja totalmente "limpa"

                    /*
                    thing = bjson_thing_create();
                    thing->type = BJSON_STRING;

                    thing->value.string = malloc(string_len + 1);
                    strncpy(thing->value.string, line + i + 1, string_len);
                    thing->value.string[string_len] = '\0';

                    i += string_len + 2;
                    */

                    thing = bjson_thing_create();
                    thing->type = BJSON_STRING;
                    thing->value.string = malloc(string_len + 1);
                    i += bjson_parse_string(line + i, thing->value.string, string_len + 1);
                }
            }
            else if (line[i] == '{') // Object begin
            {
                thing = bjson_thing_create();
                thing->type = BJSON_OBJECT;
                thing->value.object = bjson_object_create();

                i++;
            }
            else if (line[i] == '}') // Object end
            {
                bjson_thing_stack_pop(nested_things);

                i++;
                continue;
            }
            else if (line[i] == '[') // Array begin
            {
                thing = bjson_thing_create();
                thing->type = BJSON_ARRAY;
                thing->value.array = bjson_array_create();

                i++;
            }
            else if (line[i] == ']') // Array end
            {
                bjson_thing_stack_pop(nested_things);

                i++;
                continue;
            }
            else if (strncmp(line + i, "true", 4) == 0) // True
            {
                thing = bjson_thing_create();
                thing->type = BJSON_TRUE;

                i += 4;
            }
            else if (strncmp(line + i, "false", 5) == 0) // False
            {
                thing = bjson_thing_create();
                thing->type = BJSON_FALSE;

                i += 5;
            }
            else if (strncmp(line + i, "null", 4) == 0) // Null
            {
                thing = bjson_thing_create();
                thing->type = BJSON_NULL;

                i += 4;
            }
            else if (isdigit(line[i]) || line[i] == '-') // Number
            {
                thing = bjson_thing_create();
                thing->type = BJSON_NUMBER;
                i += bjson_parse_number(line + i, &thing->value.number);
            }
            else // Ignorable characters
            {
                if (!bjson_expect_is_char(line[i], ":, \n\0", 5))
                    bjson_panic("Unexpected char somewhere!");

                i++;
                continue;
            }

            // If it is the outer-most thing on the JSON
            if (bjson_thing_stack_is_empty(nested_things))
            {
                // If the thing is not "nestable", we can just return it, since the JSON will not be able to have anything else
                if (thing->type != BJSON_OBJECT && thing->type != BJSON_ARRAY)
                    return thing;

                // Set it as the root thing and ignore the name setting, since it should not be named
                root_thing = thing;
            }
            else
            {
                // Set thing name (if the current scope is an object) and append it to the current scope
                if (bjson_thing_stack_peek(nested_things)->type == BJSON_OBJECT)
                {
                    bjson_thing_set_name(thing, current_thing_name);
                    free(current_thing_name);
                    current_thing_name = NULL;

                    bjson_thing_list_append(bjson_thing_stack_peek(nested_things)->value.object->things, thing);
                }
                else
                    bjson_thing_list_append(bjson_thing_stack_peek(nested_things)->value.array->things, thing);
            }

            // If the thing is "nestable", push it to the top of the nested things stack, entering a new scope
            if (thing->type == BJSON_OBJECT || thing->type == BJSON_ARRAY)
                bjson_thing_stack_push(nested_things, thing);
        }
    }

    return root_thing;
}

bjson_thing *bjson_read_string(const char *str)
{
    return bjson_read_strings(&str, 1);
}

bjson_thing *bjson_read_file(const char *path)
{
    FILE *file = fopen(path, "r");

    // Fuck matrices, they should obviously be an array of arrays, being a char**. But they aren't! They are
    // a fucking contiguous array, a char*, and the only diference from normal arrays is in our imagination
    char *lines[BJSON_FILE_MAX_LINES];
    unsigned int n = 0;
    while (n < BJSON_FILE_MAX_LINES)
    {
        lines[n] = malloc(BJSON_FILE_MAX_LINE_SIZE);

        if (fgets(lines[n], BJSON_FILE_MAX_LINE_SIZE, file) == NULL)
            break;

        n++;
    }

    bjson_thing *root_thing = bjson_read_strings((const char **)lines, n);

    for (int i = 0; i < n + 1; i++)
        free(lines[i]);

    fclose(file);

    return root_thing;
}

typedef struct
{
    char *start;       // The begining of the string
    unsigned int size; // The total size of the string, including unused space
    unsigned int len;  // The count of chars in the string until the terminating '\0'
} bjson_dynstr;        // Dynamic String

bjson_dynstr *bjson_dynstr_create(unsigned int size)
{
    bjson_dynstr *string = malloc(sizeof(bjson_dynstr));
    string->start = malloc((size > 0) ? size : 1);
    string->start[0] = '\0';
    string->size = size;
    string->len = 0;

    return string;
}

void bjson_dynstr_destroy(bjson_dynstr *string)
{
    free(string->start);
    free(string);
}

int bjson_dynstr_can_fit(bjson_dynstr *string, unsigned int size)
{
    return size <= string->size - string->len - 1;
}

void bjson_dynstr_extend_to_fit(bjson_dynstr *string, unsigned int size)
{
    do
    {
        string->size *= 2; // Double the size each time
    } while (!bjson_dynstr_can_fit(string, size));

    string->start = realloc(string->start, string->size);
}

// TODO: Talvez mudar o nome size pra buffer_size e o nome n pra buffer_count
// This function's logic is kinda crazy ^_^
void bjson_dynstr_dump_to_buffers(bjson_dynstr *string, int *current_buffer, int *buffer_iterator, char *buffers[], unsigned int size, unsigned int n)
{
    int total_chars = 0, chars_left = 0;
    int string_iterator = 0;
    while (*current_buffer < n)
    {
        total_chars = snprintf(buffers[*current_buffer] + *buffer_iterator, size - *buffer_iterator, "%s", string->start + string_iterator);
        chars_left = total_chars - (size - *buffer_iterator - 1);
        string_iterator += total_chars - chars_left;

        if (chars_left <= 0) // The data successfully fit in the buffer
        {
            *buffer_iterator = (total_chars + *buffer_iterator) % (size - 1); // Increment the buffer iterator with the amount dumped
            break;
        }
        else // The data didn't fit, so jump to the begining of the next buffer
        {
            (*current_buffer)++;
            *buffer_iterator = 0;
        }
    }
}

void bjson_dynstr_cat_str(bjson_dynstr *string, const char *str)
{
    if (!bjson_dynstr_can_fit(string, strlen(str)))
        bjson_dynstr_extend_to_fit(string, strlen(str));

    string->len = strlen(strcat(string->start + string->len, str)); // TODO: Esse string->start + string->len na strcat() é pra otimizar, fazer um benchmark depois pra ver como fica com e sem isso
}

void bjson_dynstr_cat_thing(bjson_dynstr *string, bjson_thing *thing)
{
    switch (thing->type)
    {
    case BJSON_NOTHING:
        break;

    case BJSON_STRING:
    {
        // char *format = malloc(strlen(thing->value.string) + 3); // +3 means ['"' + '"' + '\0']
        // sprintf(format, "\"%s\"", thing->value.string);
        // bjson_dynstr_cat_str(string, format);
        // free(format);

        bjson_dynstr_cat_str(string, "\"");
        bjson_dynstr_cat_str(string, thing->value.string);
        bjson_dynstr_cat_str(string, "\"");
    }
    break;

    case BJSON_NUMBER:
    {
        char format[256]; // It's kinda sad and disappointing having to do it this way... but at least any number should fit inside this buffer
        if (floor(thing->value.number) == thing->value.number)
            snprintf(format, sizeof(format), "%d", (int)thing->value.number);
        else
            snprintf(format, sizeof(format), "%lf", thing->value.number);

        bjson_dynstr_cat_str(string, format);
    }
    break;

    case BJSON_OBJECT:
        break;

    case BJSON_ARRAY:
        break;

    case BJSON_TRUE:
        bjson_dynstr_cat_str(string, "true");
        break;

    case BJSON_FALSE:
        bjson_dynstr_cat_str(string, "false");
        break;

    case BJSON_NULL:
        bjson_dynstr_cat_str(string, "null");
        break;
    }
}

// TODO: Se pa que da pra dar uma refatorada nessa func reduzindo um pouco a quatidade de código repetido
void bjson_write_strings(bjson_thing *thing, char *buffers[], unsigned int size, unsigned int n)
{
    int current_buffer = 0;
    int buffer_iterator = 0; // Iterator to walk in the current buffer

    if (thing->type == BJSON_OBJECT || thing->type == BJSON_ARRAY)
    {
        bjson_thing_stack *nested_things = bjson_thing_stack_create();
        int nested_depth = 0;

        // TODO: Procurar um jeito de tirar essa parte repetida de código que tem nesses dois blocos aqui em baixo, tanto o de stack quanto o de print

        // Push the root thing and it's iterator
        {
            bjson_thing *thing_iterator = bjson_thing_create(); // TODO: Iterator que vai guardar quantas things já foram writadas em cada Object ou Array: GAMBIARRA ^^
            bjson_thing_set_as_int(thing_iterator, 0);

            bjson_thing_stack_push(nested_things, thing);
            bjson_thing_stack_push(nested_things, thing_iterator);

            nested_depth++;
        }

        // Print the start of the root Object or Array
        {
            bjson_dynstr *string = bjson_dynstr_create(BJSON_DYNSTR_SIZE_AS_WRITE_BUFFER);

            // Add the opening of the Object or Array
            if (thing->type == BJSON_OBJECT)
                bjson_dynstr_cat_str(string, "{\n");
            else
                bjson_dynstr_cat_str(string, "[\n");

            bjson_dynstr_dump_to_buffers(string, &current_buffer, &buffer_iterator, buffers, size, n);
            
            bjson_dynstr_destroy(string);
        }

        while (!bjson_thing_stack_is_empty(nested_things))
        {
            bjson_thing *top_thing = nested_things->top->link->thing;
            bjson_thing *top_thing_iterator = bjson_thing_stack_peek(nested_things);

            // Step to the next thing in the list using the iterator
            bjson_thing_list_node *node = (top_thing->type == BJSON_OBJECT) ? top_thing->value.object->things->start : top_thing->value.array->things->start;
            for (int i = 0; node != NULL && i < bjson_thing_get_as_int(top_thing_iterator); i++)
                node = node->next;

            for (int i = 1; node != NULL; node = node->next, i++)
            {
                if (node->thing->type == BJSON_OBJECT || node->thing->type == BJSON_ARRAY)
                {
                    // Print the start of the Object or Array
                    {
                        bjson_dynstr *string = bjson_dynstr_create(BJSON_DYNSTR_SIZE_AS_WRITE_BUFFER);
                        
                        // Add the tabs
                        for (int i = 0; i < nested_depth; i++)
                        {
                            for (int j = 0; j < BJSON_WRITE_TAB_SIZE; j++)
                                bjson_dynstr_cat_str(string, " ");
                        }

                        // Add the name
                        if (top_thing->type == BJSON_OBJECT)
                        {
                            bjson_dynstr_cat_str(string, "\"");
                            bjson_dynstr_cat_str(string, node->thing->name);
                            bjson_dynstr_cat_str(string, "\": ");
                        }

                        // Add the opening of the Object or Array
                        if (node->thing->type == BJSON_OBJECT)
                            bjson_dynstr_cat_str(string, "{\n");
                        else
                            bjson_dynstr_cat_str(string, "[\n");

                        bjson_dynstr_dump_to_buffers(string, &current_buffer, &buffer_iterator, buffers, size, n);
                        
                        bjson_dynstr_destroy(string);
                    }

                    top_thing_iterator->value.number += (double)i; // Increment the current iterator with the amount walked

                    // Push the thing and it's iterator
                    {
                        bjson_thing *thing_iterator = bjson_thing_create();
                        bjson_thing_set_as_int(thing_iterator, 0);

                        bjson_thing_stack_push(nested_things, node->thing);
                        bjson_thing_stack_push(nested_things, thing_iterator);

                        nested_depth++;
                    }

                    break; // Break the loop to go inside the found nested thing
                }
                else // Print the Thing
                {
                    bjson_dynstr *string = bjson_dynstr_create(BJSON_DYNSTR_SIZE_AS_WRITE_BUFFER);

                    // Add the tabs
                    for (int i = 0; i < nested_depth; i++)
                    {
                        for (int j = 0; j < BJSON_WRITE_TAB_SIZE; j++)
                            bjson_dynstr_cat_str(string, " ");
                    }

                    // Add the name
                    if (top_thing->type == BJSON_OBJECT)
                    {
                        bjson_dynstr_cat_str(string, "\"");
                        bjson_dynstr_cat_str(string, node->thing->name);
                        bjson_dynstr_cat_str(string, "\": ");
                    }

                    bjson_dynstr_cat_thing(string, node->thing);
                    
                    // TODO: Falta checar se precisa adicinar virgula antes do \n
                    bjson_dynstr_cat_str(string, "\n");

                    bjson_dynstr_dump_to_buffers(string, &current_buffer, &buffer_iterator, buffers, size, n);
                    
                    bjson_dynstr_destroy(string);
                }
            }

            if (node == NULL)
            {
                nested_depth--;

                // Print the end of the Object or Array
                {
                    bjson_dynstr *string = bjson_dynstr_create(BJSON_DYNSTR_SIZE_AS_WRITE_BUFFER);

                    // Add the tabs
                    for (int i = 0; i < nested_depth; i++)
                    {
                        for (int j = 0; j < BJSON_WRITE_TAB_SIZE; j++)
                            bjson_dynstr_cat_str(string, " ");
                    }

                    // TODO: Falta checar se precisa adicinar virgula antes do \n
                    // Add the closing of the Object or Array
                    if (top_thing->type == BJSON_OBJECT)
                        bjson_dynstr_cat_str(string, "}\n");
                    else
                        bjson_dynstr_cat_str(string, "]\n");

                    bjson_dynstr_dump_to_buffers(string, &current_buffer, &buffer_iterator, buffers, size, n);
                    
                    bjson_dynstr_destroy(string);
                }

                bjson_thing_destroy(bjson_thing_stack_pop(nested_things)); // Pop and destroy the iterator
                bjson_thing_stack_pop(nested_things);                      // Pop the thing

                // continue;
            }
        }

        bjson_thing_stack_destroy(nested_things);
    }
    else // Print the Thing
    {
        // TODO: Talvez deixar as outras partes que printam juntinhas que nem ta aqui
        bjson_dynstr *string = bjson_dynstr_create(BJSON_DYNSTR_SIZE_AS_WRITE_BUFFER);
        bjson_dynstr_cat_thing(string, thing);
        bjson_dynstr_dump_to_buffers(string, &current_buffer, &buffer_iterator, buffers, size, n);
        bjson_dynstr_destroy(string);
    }
}

void bjson_write_string(bjson_thing *thing, char *buffer, unsigned int size)
{
    bjson_write_strings(thing, &buffer, size, 1);
}

void bjson_write_file(bjson_thing *thing, const char *path)
{
    // TODO
}

#endif