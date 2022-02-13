/*
https://www.youtube.com/watch?v=N9RUqGYuGfw
https://notes.eatonphil.com/writing-a-simple-json-parser.html
https://stackoverflow.com/questions/17063690/how-to-write-a-basic-json-parsing-class
https://fsharpforfunandprofit.com/posts/understanding-parser-combinators-4/
https://abhinavsarkar.net/posts/json-parsing-from-scratch-in-haskell/
https://lihautan.com/json-parser-with-javascript/
*/

// TODOs:
// * Talvez eu tenha que mudar o tipo (que agora ta como long) que representa um "Number"
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
// * Talvez voltar pra C++ e refazer o BlueJSON lá, que dai eu posso deixar mais bonito usando operator overloading e polimorfismo. Daria pra
//   fazer por exemplo: parent_thing = BlueJSON::readFile("teste.json"); camiseta = parent_thing["arrayzin"][5]["Pessoa"]["Roupas"][1]
// * Talvez no Array e no Object mudar os nomes dos getters e setters pra ..._find_get_... e ..._find_set_...

// TODOs URGENTES:
// !!! Talvez só mecher com bjson_thing no high-level e deixar os objects e arrays por baixo dos panos, por ex: ao invés de bjson_thing_get_object()
//     retornar um bjson_object* ela vai rotornar uma bjson_thing* ai a função bjson_object_get_string(bjson_thing *object, ...) vai aceitar a thing 
//     como classe parametro, o que deixaria tudo mais organizado pq provavelmente reduziria mto a redunância dos getters e setters 
// !!! Talvez tirar todos os setters e ao invés deles o user vai precisar chamar funções específicas tipo bjson_thing_create_string(const char *string)
//     que vão retornar um pointer pra bjson_thing criada. Dessa forma se o user quiser mecher em um json que já esteja parsado, ele vai ter que primeiro
//     destruir a thing que ele quer mudar e alocar a nova no lugar da antiga

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
    long number;
    bjson_object *object;
    bjson_array *array;
} bjson_value;

bjson_thing *bjson_thing_create();
void bjson_thing_destroy(bjson_thing *thing); // TODO: Aparentemente tem algo de errado nessa função, se pá q ela não tá limpando direito, dar uma testada melhor

void bjson_thing_print(bjson_thing *thing); // TODO: Não sei se eu gosto muito dessa função existir :/

char *bjson_thing_get_name(bjson_thing *thing, char *buffer, unsigned int size);
bjson_value_type bjson_thing_get_value_type(bjson_thing *thing);

char *bjson_thing_get_as_string(bjson_thing *thing, char *buffer, unsigned int size);
long bjson_thing_get_as_number(bjson_thing *thing);
// int bjson_thing_get_as_int(bjson_thing *thing);
// float bjson_thing_get_as_float(bjson_thing *thing);
// double bjson_thing_get_as_double(bjson_thing *thing);
bjson_object *bjson_thing_get_as_object(bjson_thing *thing);
bjson_array *bjson_thing_get_as_array(bjson_thing *thing);
int bjson_thing_is_true(bjson_thing *thing);
int bjson_thing_is_false(bjson_thing *thing);
int bjson_thing_is_null(bjson_thing *thing);

void bjson_thing_set_name(bjson_thing *thing, const char *name);

void bjson_thing_set_as_string(bjson_thing *thing, const char *string);
void bjson_thing_set_as_number(bjson_thing *thing, long number);
// void bjson_thing_set_as_int(bjson_thing *thing, int number);
// void bjson_thing_set_as_float(bjson_thing *thing, float number);
// void json_thing_set_as_double(bjson_thing *thing, double number);
void bjson_thing_set_as_object(bjson_thing *thing, bjson_object *object);
void bjson_thing_set_as_array(bjson_thing *thing, bjson_array *array);
void bjson_thing_set_as_true(bjson_thing *thing);
void bjson_thing_set_as_false(bjson_thing *thing);
void bjson_thing_set_as_null(bjson_thing *thing);

bjson_object *bjson_object_create();
void bjson_object_destroy(bjson_object *object);

bjson_thing *bjson_object_get_thing(bjson_object *object, const char *name);
char *bjson_object_get_string(bjson_object *object, const char *name, char *buffer, unsigned int size);
long bjson_object_get_number(bjson_object *object, const char *name);
// int bjson_object_get_int(bjson_object *object);
// float bjson_object_get_float(bjson_object *object);
// double bjson_object_get_double(bjson_object *object);
bjson_object *bjson_object_get_object(bjson_object *object, const char *name);
bjson_array *bjson_object_get_array(bjson_object *object, const char *name);
int bjson_object_is_true(bjson_object *object, const char *name);
int bjson_object_is_false(bjson_object *object, const char *name);
int bjson_object_is_null(bjson_object *object, const char *name);

// Appends the specified thing to the end of the object things
void bjson_object_push_thing(bjson_object *object, bjson_thing *thing);

bjson_array *bjson_array_create();
void bjson_array_destroy(bjson_array *array);

bjson_thing *bjson_array_get_thing(bjson_array *array, unsigned int index);
char *bjson_array_get_string(bjson_array *array, unsigned int index, char *buffer, unsigned int size);
long bjson_array_get_number(bjson_array *array, unsigned int index);
// int bjson_array_get_int(bjson_array *array);
// float bjson_array_get_float(bjson_array *array);
// double bjson_array_get_double(bjson_array *array);
bjson_object *bjson_array_get_object(bjson_array *array, unsigned int index);
bjson_array *bjson_array_get_array(bjson_array *array, unsigned int index);
int bjson_array_is_true(bjson_array *array, unsigned int index);
int bjson_array_is_false(bjson_array *array, unsigned int index);
int bjson_array_is_null(bjson_array *array, unsigned int index);

// Appends the specified thing to the end of the array things
void bjson_array_push_thing(bjson_array *array, bjson_thing *thing);

// Parses the specified n strings
// Returns the outer-most parent bjson_thing, or NULL on error
bjson_thing *bjson_read_strings(const char *strs[], unsigned int n);

// Parses the specified string
// Returns the outer-most parent bjson_thing, or NULL on error
bjson_thing *bjson_read_string(const char *str);

// Parses the entire JSON file specified by the file path
// Returns the outer-most parent bjson_thing, or NULL on error
bjson_thing *bjson_read_file(const char *path);

// void bjson_write_file(bjson_thing *thing, const char *path);

// TODO: Essa função tá meio gambiarra por enquanto, dar uma melhorada nela futuramente
void bjson_print(bjson_thing *thing);

#endif

#define BJSON_IMPLEMENTATION
#ifdef BJSON_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BJSON_FILE_MAX_LINE_SIZE 256
#define BJSON_FILE_MAX_LINES 1024

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

void bjson_thing_print(bjson_thing *thing)
{
    switch (thing->type)
    {
    case BJSON_STRING:
        printf("%s\n", thing->value.string);
        break;

    case BJSON_NUMBER:
        printf("%d\n", (int)thing->value.number);
        break;

    case BJSON_TRUE:
        printf("true\n");
        break;

    case BJSON_FALSE:
        printf("false\n");
        break;
    
    case BJSON_NULL:
        printf("null\n");
        break;

    default:
        break;
    }
}

char *bjson_thing_get_name(bjson_thing *thing, char *buffer, unsigned int size)
{
    strncpy(buffer, thing->name, size);
    buffer[size - 1] = '\0';

    return buffer;
}

bjson_value_type bjson_thing_get_value_type(bjson_thing *thing)
{
    return thing->type;
}

char *bjson_thing_get_as_string(bjson_thing *thing, char *buffer, unsigned int size)
{
    strncpy(buffer, thing->value.string, size);
    buffer[size - 1] = '\0';

    return buffer;
}

long bjson_thing_get_as_number(bjson_thing *thing)
{
    return thing->value.number;
}

bjson_object *bjson_thing_get_as_object(bjson_thing *thing)
{
    return (thing == NULL || thing->type != BJSON_OBJECT) ? NULL : thing->value.object;
}

bjson_array *bjson_thing_get_as_array(bjson_thing *thing)
{
    return (thing == NULL || thing->type != BJSON_ARRAY) ? NULL : thing->value.array;
}

int bjson_thing_is_true(bjson_thing *thing)
{
    return thing->type == BJSON_TRUE;
}

int bjson_thing_is_false(bjson_thing *thing)
{
    return thing->type == BJSON_FALSE;
}

int bjson_thing_is_null(bjson_thing *thing)
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

void bjson_thing_set_as_number(bjson_thing *thing, long number)
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

bjson_thing *bjson_object_get_thing(bjson_object *object, const char *name)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);

    return thing;
}

char *bjson_object_get_string(bjson_object *object, const char *name, char *buffer, unsigned int size)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);
    
    if (thing != NULL && thing->type == BJSON_STRING)
    {
        strncpy(buffer, thing->value.string, size);
        buffer[size - 1] = '\0';

        return buffer;
    }

    return NULL;
}

long bjson_object_get_number(bjson_object *object, const char *name)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);

    return thing->value.number;
}

bjson_object *bjson_object_get_object(bjson_object *object, const char *name)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);

    return (thing == NULL || thing->type != BJSON_OBJECT) ? NULL : thing->value.object;
}

bjson_array *bjson_object_get_array(bjson_object *object, const char *name)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);

    return (thing == NULL || thing->type != BJSON_ARRAY) ? NULL : thing->value.array;
}

int bjson_object_is_true(bjson_object *object, const char *name)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);

    return thing->type == BJSON_TRUE;
}

int bjson_object_is_false(bjson_object *object, const char *name)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);

    return thing->type == BJSON_FALSE;
}

int bjson_object_is_null(bjson_object *object, const char *name)
{
    bjson_thing *thing = bjson_thing_list_get_by_name(object->things, name);

    return thing->type == BJSON_NULL;
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

bjson_thing *bjson_array_get_thing(bjson_array *array, unsigned int index)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return thing;
}

char *bjson_array_get_string(bjson_array *array, unsigned int index, char *buffer, unsigned int size)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);
    
    if (thing != NULL && thing->type == BJSON_STRING)
    {
        strncpy(buffer, thing->value.string, size);
        buffer[size - 1] = '\0';

        return buffer;
    }

    return NULL;
}

long bjson_array_get_number(bjson_array *array, unsigned int index)
{   
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return thing->value.number;
}

bjson_object *bjson_array_get_object(bjson_array *array, unsigned int index)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return (thing == NULL || thing->type != BJSON_OBJECT) ? NULL : thing->value.object;
}

bjson_array *bjson_array_get_array(bjson_array *array, unsigned int index)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return (thing == NULL || thing->type != BJSON_ARRAY) ? NULL : thing->value.array;
}

int bjson_array_is_true(bjson_array *array, unsigned int index)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return thing->type == BJSON_TRUE;
}

int bjson_array_is_false(bjson_array *array, unsigned int index)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return thing->type == BJSON_FALSE;
}

int bjson_array_is_null(bjson_array *array, unsigned int index)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return thing->type == BJSON_NULL;
}

void bjson_array_push_thing(bjson_array *array, bjson_thing *thing)
{
    bjson_thing_list_append(array->things, thing);
}

size_t bjson_string_len(const char *str)
{
    return strchr(str + 1, '"') - (str + 1);
}

#ifdef BJSON_NO_ERROR_CHECKING

int bjson_expect(const char *tokens) 
{
    return 1;
}

#else

int bjson_expect(char c, const char *tokens)
{
    for (int i = 0; i < strlen(tokens); i++)
    {
        if (c == tokens[i])
            return 1;
    }

    return 0;
}

#endif

bjson_thing *bjson_read_strings(const char *strs[], unsigned int n)
{
    // TODO: Talvez retirar essa variável pq daria pra retornar a parent thing direto pelo stack de nested things
    bjson_thing *parent_thing = NULL; // TODO: Talvez mudar o nome pra root_thing

    bjson_thing_stack *nested_things = bjson_thing_stack_create();

    // TODO: Talvez quando eu refatorar e tirar a geração de thing pra mais pra fora não precise mais dessa var.
    //       Isso vai deixar o código mais legível e mais eficiênte, já que, quando necessário. o nome vai estar 
    //       sendo copiado direto pra dentro da thing, e não pra essa variável temporária pra depois ser recopiado 
    //       pra thing
    bjson_thing *thing = NULL;
    char *current_thing_name = NULL;
    
    for (int current_str = 0; current_str < n; current_str++)
    {
        const char *line = strs[current_str];
        for (int i = 0; i < strlen(line); i++)
        {
            // TODO: Nas strings, checar quando tiver caracateres de escape e especiais, tipo '\"' e '\n', etc...
            if (line[i] == '"') 
            {
                size_t string_len = bjson_string_len(line + i);

                if (current_thing_name == NULL && 
                    !bjson_thing_stack_is_empty(nested_things) && 
                    bjson_thing_stack_peek(nested_things)->type != BJSON_ARRAY) // Name
                {
                    current_thing_name = malloc(string_len + 1);
                    strncpy(current_thing_name, line + i + 1, string_len);
                    current_thing_name[string_len] = '\0';
                    
                    i += string_len + 1;

                    continue;
                }
                else // String
                {
                    // TODO: Talvez mudar isso pra bjson_thing_set_string(thing, current_thing_name). Também mudar as outras partes pros setters correspondentes,
                    //       como quando o parser encontrar um objeto mudar para bjson_thing_set_object(thing, bjson_object_create()). Agora faria bem mais sentido
                    //       só deixar usar um setter em uma thing caso o value_type dela seja BJSON_NOTHING (ou algo do tipo) e ela esteja totalmente "limpa"

                    thing = bjson_thing_create();
                    thing->type = BJSON_STRING; 

                    thing->value.string = malloc(string_len + 1);
                    strncpy(thing->value.string, line + i + 1, string_len);
                    thing->value.string[string_len] = '\0';

                    i += string_len + 1;
                }
            }
            else if (line[i] == '{') // Object begin
            {
                thing = bjson_thing_create();
                thing->type = BJSON_OBJECT;
                thing->value.object = bjson_object_create();
            }
            else if (line[i] == '}') // Object end
            {
                bjson_thing_stack_pop(nested_things);

                continue;
            }
            else if (line[i] == '[') // Array begin
            {
                thing = bjson_thing_create();
                thing->type = BJSON_ARRAY;
                thing->value.array = bjson_array_create();
            }
            else if (line[i] == ']') // Array end
            {
                bjson_thing_stack_pop(nested_things);

                continue;
            }
            else if (strncmp(line + i, "true", 4) == 0) // True
            {
                thing = bjson_thing_create();
                thing->type = BJSON_TRUE;

                i += 3;
            }
            else if (strncmp(line + i, "false", 5) == 0) // False
            {
                thing = bjson_thing_create();
                thing->type = BJSON_FALSE;
                
                i += 4;      
            }
            else if (strncmp(line + i, "null", 4) == 0) // Null
            {
                thing = bjson_thing_create();
                thing->type = BJSON_NULL;

                i += 3;
            }
            else if (isdigit(line[i]) || line[i] == '-') // Number
            {
                thing = bjson_thing_create();
                thing->type = BJSON_NUMBER;
                
                // TODO: Parsar outros tipos de números que o JSON suporta, tipo float, double, etc... Mas implementar meu proprio parser, sem usa sscanf
                sscanf(line + i, "%ld", &thing->value.number);

                // TODO: Talvez esse método do log10 não funcione pra floats. Lembrando que sempre que for parsar algo que seja mais
                //       longo que um caracter precisa incrementar o i pra não bugar e ficar tentando ler coisa que já foi parsada
                i += (int)log10(thing->value.number);
            }
            else // Ignorable characters
                continue;
            
            // If it is the outer-most thing on the JSON
            if (bjson_thing_stack_is_empty(nested_things))
            {
                // If the thing is not "nestable", we can just return it, since the JSON will not be able to have anything else
                if (thing->type != BJSON_OBJECT && thing->type != BJSON_ARRAY)
                    return thing;

                // Set it as the parent thing and ignore the name setting, since it should not be named
                parent_thing = thing;
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

    return parent_thing;
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
    
    bjson_thing *parent_thing = bjson_read_strings((const char **)lines, n);
    
    for (int i = 0; i < n + 1; i++)
        free(lines[i]);

    fclose(file);

    return parent_thing;
}

void bjson_print(bjson_thing *root) // TODO: Talvez usar o nome root ao invés de parent em tudo
{
    static int nested_depth = 0;

    if (root->type == BJSON_OBJECT || root->type == BJSON_ARRAY)
    {
        for (bjson_thing_list_node *node = (root->type == BJSON_OBJECT) ? root->value.object->things->start : root->value.array->things->start; node != NULL; node = node->next)
        {
            bjson_thing *thing = node->thing;

            if (thing->type == BJSON_OBJECT || thing->type == BJSON_ARRAY)
            {
                nested_depth++;
                bjson_print(thing);
            }
            else
            {
                for (int i = 0; i < nested_depth; i++)
                    printf("  ");
                bjson_thing_print(thing);
            }
        }

        nested_depth--;
    }
    else
        bjson_thing_print(root);
}

#endif