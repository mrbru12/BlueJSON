// TODOs:
// * Talvez usar C++ meio moderno em tudo, usar std::string ao invés de c-strings por exemplo
// * Deixar de lado essa versão pra C++ e voltar a focar na de C, porque eu descobri que eu 
//   não suporto programar em C++ por muito tempo

#ifndef BLUEJSON_BLUEJSON_H
#define BLUEJSON_BLUEJSON_H

#include <string>
#include <forward_list>

namespace BlueJSON
{
    enum class ValueType
    {
        NOTHING, 
        JSON_STRING, 
        JSON_NUMBER_INT, 
        JSON_NUMBER_FLOAT, 
        JSON_NUMBER_DOUBLE, 
        JSON_OBJECT, 
        JSON_ARRAY, 
        JSON_TRUE, 
        JSON_FALSE, 
        JSON_NULL
    };
    
    typedef union
    {
        std::string string;
        int number_int;
        float number_float;
        double number_double;
        Object* object;
        Array* array;
    } Value;
    
    class Thing
    {
        std::string name;
        ValueType type;
        Value value;

    public:
        Thing();
        ~Thing();

        // ===== GETTERS =====

        std::string getName();
        // ValueType getValueType();

        const Thing& operator [] (std::string name) const;
        const Thing& operator [] (unsigned int index) const;

        std::string getAsString();
        int         getAsInt();
        float       getAsFloat();
        double      getAsDouble();
        Object*     getAsObject();
        Array*      getAsArray();
        bool        isTrue();
        bool        isFalse();
        bool        isNull();

        // ===== SETTERS =====

        void setName(std::string name);

        Thing& operator [] (std::string name);
        Thing& operator [] (unsigned int index);

        void setAsString(std::string string);
        void setAsInt(int number);
        void setAsFloat(float number);
        void setAsDouble(double number);
        void setAsObject(Object* object);
        void setAsArray(Array* array);
        void setAsTrue();
        void setAsFalse();
        void setAsNull();

        // ===== INPUT/OUTPUT =====
        
        static Thing readStrings(std::string strings[], unsigned int count);
        static Thing readString(std::string string);
        static Thing readFile(std::string path);
    };

    class Object
    {
        std::forward_list<Thing> things; // std::forward_list<std::pair<std::string, Thing*>> things;

    public:
        Object();
        ~Object();

        void pushThing(Thing& thing);
    };

    class Array
    {
        std::forward_list<Thing> things;

    public:
        Array();
        ~Array();

        void pushThing(Thing& thing);
    };
}

#endif

#define BLUEJSON_IMPLEMENTATION
#ifdef BLUEJSON_IMPLEMENTATION

#include <stack>

namespace BlueJSON
{
    
}

#endif