/*
https://www.youtube.com/watch?v=N9RUqGYuGfw
https://notes.eatonphil.com/writing-a-simple-json-parser.html
https://stackoverflow.com/questions/17063690/how-to-write-a-basic-json-parsing-class
https://fsharpforfunandprofit.com/posts/understanding-parser-combinators-4/
https://abhinavsarkar.net/posts/json-parsing-from-scratch-in-haskell/
https://lihautan.com/json-parser-with-javascript/
*/

// TODO:
// * Mudar o nome de várias coisas que tão meio estranhas e erradas (não sei por que mas não to conseguindo pensa em uns nomes boms)
// * Talvez eu tenha que mudar o tipo (que agora ta como long) que representa um "Number"
// * Se pa que vai precisar de um sistema mais complexo de getters, pra poder pegar values de objects específicos
// * O ideal é primeiro eu dar um jeito de "parsar" os arquivos em tokes e só depois que isso tiver funcionando certinho eu penso em
//   como eu vo faze o sistema de identificar o que é o que
// * Fazer um sistema de checagem de erro com uma função tipo bjson_expect(const char *tokens). Dai, por exemplo, depois de encontrar
//   o começo de um objeto '{' teria que ter um bjson_expect(""}"), ou seja, ele esperaria que tivesse uma '"' (significando o
//   começo de uma string), ou um '}' (significando o final do objeto, que no caso seria um objeto vazio), os espaços em branco ' ' 
//   a função vai ignorar. Como exemplo ver aquele site que checa se o JSON ta certo
// * Precisa urgentemente dar uma refatorada, principalmente na parte de parsing, pra diminuir a montanha de código repetido inútil que
//   tem e deixar mais bonito
// * Vou ter que colocar { } em cada case do switch do parser porque todo o código dentro do switch fica no mesmo contexto. Na real que
//   acho que a melhor coisa a fazer é trocar de switch-case pra if-else porque dai também daria uma facilitada na refatoração do código 
// * Já que vão ter varios getters e setters da pra colocar o bjson_object, o bjson_array e o bjson_thing em arquivos separados
// * Aparentemente agora não precisa mais ser só object ou array no parent do arquivo de json, pode ser qualquer tipo de value. Mas claro
//   que se não for um object ou array só vai ter um value no arquivo inteiro
// * Desistir de separar em arquivos diferentes e fazer tudo no mesmo arquivo header-only
// * checar se todos os prefixo dz_ estão devidamente trocados pelo bjson_
// * Talvez tirar o _ de todos os nomes de coisas em que ele aparece
// * Dar uma documentada boa em tudo
// * Fazer uns benchmarks pra procurar lugares que daria pra dar uma otimizada melhor
// * Criar um repo no github e colocar uma foto de um Blue Jay, também colocar alguma referência a Blue Jay Way. Talvez eu tenha que colocar
//   créditos das imagens que eu usar
// * Talvez mudar o nome do projeto pra BlueJSON, já que assim ele vai continuar carregando as referencias ao Gaio Azul e a Blue Jay Way só
//   que mais sutilmente. Também vai deixar o nome menorzin e mais facil de entender, e dai eu poderia mudar todos os prefixos bjson_ por b
//   e tirar o _json_ do meio dos nomes
// * Reduzir ao máximo o número de #includes
// * Talvez fazer todas as funções de getter (menos as que retornam um objeto, array ou thing) retornarem um código de erro, e mandar o valor
//   que teria retornado por meio de um pointer passado nos parametros (estilo scanf)
// * Adicionar uma licença no começo desse arquivo

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
void bjson_thing_destroy(bjson_thing *thing);

char *bjson_thing_get_name(bjson_thing *thing, char *buffer, unsigned int size);
bjson_value_type bjson_thing_get_value_type(bjson_thing *thing);
// bjson_value bjson_thing_get_value(bjson_thing *thing); // TODO: Se pa que é melhor não deixar o user mexer com o value direto na mão

char *bjson_thing_get_string(bjson_thing *thing, char *buffer, unsigned int size);
long bjson_thing_get_number(bjson_thing *thing);
bjson_object *bjson_thing_get_object(bjson_thing *thing);
bjson_array *bjson_thing_get_array(bjson_thing *thing);
int bjson_thing_is_true(bjson_thing *thing);
int bjson_thing_is_false(bjson_thing *thing);
int bjson_thing_is_null(bjson_thing *thing);

void bjson_thing_set_name(bjson_thing *thing, const char *name);
// void bjson_thing_set_value_type(bjson_thing *thing, bjson_value_type type); // TODO: Se pa que é melhor não deixar o user mexer com o value direto na mão
// void bjson_thing_set_value(bjson_thing *thing, bjson_value value); // TODO: Se pa que é melhor não deixar o user mexer com o value direto na mão

void bjson_thing_set_string(bjson_thing *thing, const char *string);
void bjson_thing_set_number(bjson_thing *thing, long number);
void bjson_thing_set_object(bjson_thing *thing, bjson_object *object);
void bjson_thing_set_array(bjson_thing *thing, bjson_array *array);
void bjson_thing_set_true(bjson_thing *thing);
void bjson_thing_set_false(bjson_thing *thing);
void bjson_thing_set_null(bjson_thing *thing);

bjson_object *bjson_object_create();
void bjson_object_destroy(bjson_object *object);

bjson_thing *bjson_object_get_thing(bjson_object *object, const char *name);
char *bjson_object_get_string(bjson_object *object, const char *name, char *buffer, unsigned int size);
long bjson_object_get_number(bjson_object *object, const char *name);
bjson_object *bjson_object_get_object(bjson_object *object, const char *name);
bjson_array *bjson_object_get_array(bjson_object *object, const char *name);
int bjson_object_is_true(bjson_object *object, const char *name);
int bjson_object_is_false(bjson_object *object, const char *name);
int bjson_object_is_null(bjson_object *object, const char *name);

// bjson_object_set_...();
// ...

bjson_array *bjson_array_create();
void bjson_array_destroy(bjson_array *array);

bjson_thing *bjson_array_get_thing(bjson_array *array, unsigned int index);
char *bjson_array_get_string(bjson_array *array, unsigned int index, char *buffer, unsigned int size);
long bjson_array_get_number(bjson_array *array, unsigned int index);
bjson_object *bjson_array_get_object(bjson_array *array, unsigned int index);
bjson_array *bjson_array_get_array(bjson_array *array, unsigned int index);
int bjson_array_is_true(bjson_array *array, unsigned int index);
int bjson_array_is_false(bjson_array *array, unsigned int index);
int bjson_array_is_null(bjson_array *array, unsigned int index);

// bjson_array_set_...();
// ...

// Parses the specified n strings
// Returns the outer-most parent bjson_thing, or NULL on error
bjson_thing *bjson_read_strings(const char *strs[], unsigned int n);

// Parses the specified string
// Returns the outer-most parent bjson_thing, or NULL on error
bjson_thing *bjson_read_string(const char *str);

// Parses the entire JSON file specified by the file path
// Returns the outer-most parent bjson_thing, or NULL on error
bjson_thing *bjson_read_file(const char *path);

#endif

#define BJSON_IMPLEMENTATION
#ifdef BJSON_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BJSON_TOKEN_TRUE "true"
#define BJSON_TOKEN_FALSE "false"
#define BJSON_TOKEN_NULL "null"

#define BJSON_FILE_MAX_LINE_SIZE 256
#define BJSON_FILE_MAX_LINES 1024

struct bjson_thing
{
    char *name; // NOTE: Since only objects have named tokens: if used as an array token, name is NULL
    bjson_value_type type;
    bjson_value value;
}; // TODO: Achar um termo mais apropriado do que "thing" (na verdade não hehe thing é bem único e legal)

bjson_thing *bjson_thing_create()
{
    bjson_thing *thing = malloc(sizeof(bjson_thing));
    thing->name = NULL;
    thing->type = -1; // TODO: Talvez criar um bjson_value_type chamado BJSON_NOTHING ou algo assim, pra facilitar a inicialização

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

char *bjson_thing_get_name(bjson_thing *thing, char *buffer, unsigned int size)
{
    return strncpy(buffer, thing->name, size);
}

bjson_value_type bjson_thing_get_value_type(bjson_thing *thing)
{
    return thing->type;
}

char *bjson_thing_get_string(bjson_thing *thing, char *buffer, unsigned int size)
{
    return strncpy(buffer, thing->value.string, size);
}

long bjson_thing_get_number(bjson_thing *thing)
{
    return thing->value.number;
}

bjson_object *bjson_thing_get_object(bjson_thing *thing)
{
    return (thing == NULL || thing->type != BJSON_OBJECT) ? NULL : thing->value.object; // TODO: Adicionar mais checks de NULL em todas as funcs
}

bjson_array *bjson_thing_get_array(bjson_thing *thing)
{
    return (thing == NULL || thing->type != BJSON_ARRAY) ? NULL : thing->value.array; // TODO: Adicionar mais checks de NULL em todas as funcs
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

void bjson_thing_set_string(bjson_thing *thing, const char *string)
{
    // TODO: Precisa adicionar mais checks pra não sobreescrever coisas que ja estejam setadas. Talvez o ideal seria só deixar setar coisas caso 
    //       a thing estja totalmente "limpa", pq se for para pra pensa (caso a thing tenha sido extraida de um json) tem chance de ela ser, por
    //       exemplo, um bjson_object, ai tu vai setar por cima desse objeto e ele vai ficar perdido na memória. Tem q pelo menos dar um jeito de
    //       de chamr uma função bjson_thing_clear_value() ou algo do tipo pra dar free caso precise dar free em algo (e isso não só aqui, isso
    //       precisa ser feito em todos os setters tanto da thing como do object e do array)

    thing->type = BJSON_STRING;
    thing->value.string = malloc(strlen(string) + 1);
    strcpy(thing->name, string);
}

void bjson_thing_set_number(bjson_thing *thing, long number)
{
    thing->type = BJSON_NUMBER;
    thing->value.number = number;
}

void bjson_thing_set_object(bjson_thing *thing, bjson_object *object)
{
    thing->type = BJSON_OBJECT;
    thing->value.object = object;
}

void bjson_thing_set_array(bjson_thing *thing, bjson_array *array)
{
    thing->type = BJSON_ARRAY;
    thing->value.array = array;
}

void bjson_thing_set_true(bjson_thing *thing)
{
    thing->type = BJSON_TRUE;
}

void bjson_thing_set_false(bjson_thing *thing)
{
    thing->type = BJSON_FALSE;
}

void bjson_thing_set_null(bjson_thing *thing)
{
    thing->type = BJSON_NULL;
}

typedef struct bjson_thing_list_node bjson_thing_list_node;
struct bjson_thing_list_node
{
    bjson_thing *thing;
    bjson_thing_list_node *next;
};

typedef struct
{
    // unsigned int length;
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

// TODO: Pra ficar mais eficiente: se o index for maior que a metade da length da list, chegar no node indo de trás pra frente
// TODO: Adicionar uns checkings de erro, pra evitar que o index seja maior que a quantidade de itens na lista, pra não dar ruim com coisas NULL
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

typedef struct bjson_thing_stack_node bjson_thing_stack_node;
struct bjson_thing_stack_node
{
    bjson_thing *thing;
    bjson_thing_stack_node *link;
};

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
    while (node != NULL) // !bjson_thing_stack_is_empty(stack)
    {
        // bjson_thing_stack_pop(stack);
        
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
    // NOTE: Preciso chamar o destructor de todas as things da lista antes de destruir a lista. Talvez
    //       fazer desse jeito aqui vai ser meio ineficiente, já que que a lista vai ser toda percorrida
    //       aqui e depois toda percorrida novamente no bjson_thing_list_destroy();
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
    // NOTE: Preciso chamar o destructor de todas as things da lista antes de destruir a lista. Talvez
    //       fazer desse jeito aqui vai ser meio ineficiente, já que que a lista vai ser toda percorrida
    //       aqui e depois toda percorrida novamente no bjson_thing_list_destroy();
    for (bjson_thing_list_node *node = array->things->start; node != NULL; node = node->next)
        bjson_thing_destroy(node->thing);

    bjson_thing_list_destroy(array->things);   
    free(array);
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

bjson_object *bjson_array_get_object(bjson_array *array, unsigned int index)
{
    bjson_thing *thing = bjson_thing_list_get_at(array->things, index);

    return (thing == NULL || thing->type != BJSON_OBJECT) ? NULL : thing->value.object;
}

size_t bjson_string_len(const char *str)
{
    return strchr(str + 1, '"') - (str + 1);
}

#ifdef BJSON_NO_ERROR_CHECKING

int bjson_expect() 
{ 
    return 1;
}

#else

int bjson_expect() 
{
     
}

#endif

bjson_thing *bjson_read_strings(const char *strs[], unsigned int n)
{
    // TODO: Talvez retirar essa variável pq daria pra retornar a parent thing direto pelo stack de nested things
    bjson_thing *parent_thing = NULL; // TODO: Talvez mudar o nome pra root_thing

    // int nested_thing_depth = 0; // How deep the parser is inside the nested JSON things
    bjson_thing_stack *nested_things = bjson_thing_stack_create();

    // TODO: Talvez quando eu refatorar e tirar a geração de thing pra mais pra fora não precise mais dessa var.
    //       Isso vai deixar o código mais legível e mais eficiênte, já que, quando necessário. o nome vai estar 
    //       sendo copiado direto pra dentro da thing, e não pra essa variável temporária pra depois ser recopiado 
    //       pra thing
    bjson_thing *thing = NULL;
    char *current_thing_name = NULL; // TODO: Talvez tirar o "current_" do nome dessa var
    
    for (int current_str = 0; current_str < n; current_str++)
    {
        const char *line = strs[current_str]; // TODO: Talvez mudar o nome dessa var
        for (int i = 0; i < strlen(line); i++)
        {
            // TODO: Nas strings, checar quando tiver caracateres de escape e especiais, tipo '\"' e '\n', etc...
            if (line[i] == '"')
            {
                size_t string_len = bjson_string_len(line + i);

                if (current_thing_name == NULL && !bjson_thing_stack_is_empty(nested_things) && bjson_thing_stack_peek(nested_things)->type != BJSON_ARRAY)
                {
                    current_thing_name = malloc(string_len + 1);
                    strncpy(current_thing_name, line + i + 1, string_len);
                    current_thing_name[string_len] = '\0';
                    
                    i += string_len + 1;

                    continue;
                }
                else
                {
                    thing = bjson_thing_create();
                    thing->type = BJSON_STRING;

                    thing->value.string = malloc(string_len + 1);
                    strncpy(thing->value.string, line + i + 1, string_len);
                    thing->value.string[string_len] = '\0';

                    i += string_len + 1;
                }
            }
            else if (line[i] == '{')
            {
                thing = bjson_thing_create();
                thing->type = BJSON_OBJECT;
                thing->value.object = bjson_object_create();
            }
            else if (line[i] == '}')
            {
                bjson_thing_stack_pop(nested_things);

                continue;
            }
            else if (line[i] == '[')
            {
                thing = bjson_thing_create();
                thing->type = BJSON_ARRAY;
                thing->value.array = bjson_array_create();
            }
            else if (line[i] == ']')
            {
                bjson_thing_stack_pop(nested_things);

                continue;
            }
            else if (line[i] == 't') // true
            {
                if (strncmp(line + i, BJSON_TOKEN_TRUE, strlen(BJSON_TOKEN_TRUE)) == 0)
                {
                    thing = bjson_thing_create();
                    thing->type = BJSON_TRUE;

                    i += strlen(BJSON_TOKEN_TRUE) - 1;
                }
                else
                    continue;
            }
            else if (line[i] == 'f') // false
            {
                if (strncmp(line + i, BJSON_TOKEN_FALSE, strlen(BJSON_TOKEN_FALSE)) == 0)
                {
                    thing = bjson_thing_create();
                    thing->type = BJSON_FALSE;
                    
                    i += strlen(BJSON_TOKEN_FALSE) - 1;                    
                }
                else
                    continue;
            }
            else if (line[i] == 'n') // null
            {
                if (strncmp(line + i, BJSON_TOKEN_NULL, strlen(BJSON_TOKEN_NULL)) == 0)
                {
                    thing = bjson_thing_create();
                    thing->type = BJSON_NULL;

                    i += strlen(BJSON_TOKEN_NULL) - 1;
                }
                else
                    continue;
            }
            else if (isdigit(line[i]) || line[i] == '-') // TODO: Parsar outros tipos de números que o JSON suporta, tipo float, double, etc... Mas implementar meu proprio parser, sem usa sscanf
            {   
                thing = bjson_thing_create();
                thing->type = BJSON_NUMBER;
                
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

#endif