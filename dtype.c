#include <stdio.h>
#include <dtype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/// @brief array of strings mapping to string versions of typecode
const char * DTYPE_STR_TYPES[] = {
    "none",
    "boolean",
    "character",
    "short",
    "unsigned short",
    "int",
    "unsigned int",
    "long",
    "unsigned long",
    "float",
    "double",
    "string",
    "other (custom type)"
};

// -------------------------------- Flags ----------------------------------------------

/// @brief throw / show errors or not.
bool DTYPE_ERROR_THROW = true;

/// @brief throw / show warnings or not.
bool DTYPE_WARN_THROW = true;

/// @brief if warning should be taken as error
bool DTYPE_WARN_EQ_ERROR = false;

/// @brief exit on error.
bool DTYPE_EXIT_ON_ERROR = false;

/// @brief function to toggle DTYPE_WARN_THROW flag [ not accessible directly ].
/// @param val the value to set (in boolean)
void dtype_warn_throw(bool val) { DTYPE_WARN_THROW = val; }

/// @brief function to toggle DTYPE_ERROR_THROW flag [ not accessible directly ].
/// @param val the value to set (in boolean)
void dtype_error_throw(bool val) { DTYPE_ERROR_THROW = val; }

/// @brief function to toggle DTYPE_WARN_EQ_ERROR flag [ not accessible directly ].
/// @param val the value to set (in boolean)
void dtype_warn_eq_error(bool val) { DTYPE_WARN_EQ_ERROR = val; }

/// @brief function to toggle DTYPE_EXIT_ON_ERROR flag [ not accessible directly ].
/// @param val the value to set (in boolean)
void dtype_exit_on_error(bool val) { DTYPE_EXIT_ON_ERROR = val; }

// -------------------------------- Internal Functions ----------------------------------------------

/// @brief Error raising function for internal use.
/// @param func the function in which error occured.
/// @param msg the error message
/// @param errcode the errorcode
/// @return true if error is displayed, false if not. 
/// [ Note: Program directly exits with `errcode` as return value if DTYPE_EXIT_ON_ERROR is set ]
bool dtype__raise(const char * func, const char * msg, enum DTYPE_ERRORS errcode)
{
    // if error throw is set and there is  a valid errcode
    if ( DTYPE_ERROR_THROW && (errcode > DTYPE_NO_ERROR && errcode <= DTYPE_UNKNOWN_ERROR ) ) {
        // throw the error to stderr
        fprintf(
            stderr,
            "\n----- Dtype Error -----\nErrcode: %d\nin function `%s`: %s", errcode, func, msg
        );
        if ( DTYPE_EXIT_ON_ERROR ) { exit(errcode); }
        return true;
    }
    return false;
}

/// @brief memory error raising for internal use
/// @param size size for which memory was to be allocated.
/// @param func function which caused the memory allocation to happen.
void dtype__mem_error(size_t size, const char * func)
{
    if ( dtype__raise(func, "Couldn't allocate memory for size", DTYPE_MEMORY_ERROR))
    {
        fprintf(stderr, ": %lu\n", size);
    }
}

/// @brief dtype__typecheck function for internal use
/// @param var the dtype variable to check
/// @param type type to check
/// @return true if warning was displayed, else false.
bool dtype__typecheck (dtype var, enum DTYPE_TYPES type) {
    // if the type is not within type range
    if ( var.type < DTYPE_NONE || var.type > DTYPE_CUSTOM ) {
        if ( dtype__raise("dtype_typecheck", "Invalid type, type is corrupted.", DTYPE_TYPE_ERROR) ) {
            fprintf(stderr, " type `%d` is not within ( %d >= type >= %d )\n", var.type, DTYPE_NONE, DTYPE_CUSTOM);
        }
        return true;
    }
    // if type are same or if warn throw is not set, return false
    if (!DTYPE_WARN_THROW || var.type == type) {
        return false;
    }
    // output the warning to stderr
    fprintf(
        stderr,
        "\n----- Dtype Warning ------\n %s : `%s` [typecode : %d ] from `%s` [typecode : %d ]\n",
        "Type mismatch while getting", DTYPE_STR_TYPES[type], type, dtype_get_str_type(var), var.type
    );
    return true;
}

/// @brief memory allocator for internal use
/// @param size size to allocate
/// @return pointer to the calloc'd memory.
void * dtype__mem_alloc(size_t size)
{
    return calloc(size, 1);
}

/// @brief memory refresher for internal use
/// @param var variable to refresh memory
/// @param size new size of memory
/// @param func function name which is requesting to refresh
/// @return the dtype variable with refreshed memory.
dtype dtype__mem_refresh(dtype var, size_t size, const char * func)
{
    if (var.mem!=NULL) {
        free(var.mem);
    }
    var.mem = size ? dtype__mem_alloc(size): NULL;
    var.size = (var.mem != NULL) ? size : 0;
    var.type = DTYPE_NONE;
    if(!var.size && size) {
        dtype__mem_error(size, func);
    }
    return var;
}

// -------------------------------- External Functions ----------------------------------------------

/// @brief get the string representation of type
/// @param var variable to get type from
/// @return the type of variable as string
const char * dtype_get_str_type(dtype var)
{
    return DTYPE_STR_TYPES[var.type];
}

/// @brief Returns a null, but initialized dtype variable
/// @return initialized dtype variable
dtype dtype_default() {
    dtype var;
    var.size = 0;
    var.mem = NULL;
    var.type = DTYPE_NONE;
    return var;
}

/// @brief Changes size of the dtype variabled [ doesn't affect content as long as it fits ]
/// @param var the variable to change size.
/// @param size the size to change to. [ can't be zero ]
/// @return the dtype of given size.
dtype dtype_change_size(dtype var, size_t size)
{
    var.mem = realloc(var.mem, size);
    return var;
}

// ----------------- Set Functions ----------------

/// @brief set the value to boolean
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_bool(dtype var, bool val)
{
    var = dtype__mem_refresh(var, sizeof(bool), "dtype_set_bool");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(bool)) : 0;
    var.type = DTYPE_BOOL;
    return var;
}

/// @brief set the value to char
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_char(dtype var, char val)
{
    var = dtype__mem_refresh(var, sizeof(char), "dtype_set_char");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(char)) : 0;
    var.type = DTYPE_CHAR;
    return var;
}

/// @brief set the value to short
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_short(dtype var, short val)
{
    var = dtype__mem_refresh(var, sizeof(short), "dtype_set_short");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(short)) : 0;
    var.type = DTYPE_SHORT;
    return var;
}

/// @brief set the value to unsigned short
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_ushort(dtype var, unsigned short val)
{
    var = dtype__mem_refresh(var, sizeof(unsigned short), "dtype_set_ushort");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(unsigned short)) : 0;
    var.type = DTYPE_USHORT;
    return var;
}

/// @brief set the value to int
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_int(dtype var, int val)
{
    var = dtype__mem_refresh(var, sizeof(int), "dtype_set_int");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(int)) : 0;
    var.type = DTYPE_INT;
    return var;
}

/// @brief set the value to unsigned int
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_uint(dtype var, unsigned int val)
{
    var = dtype__mem_refresh(var, sizeof(unsigned int), "dtype_set_uint");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(unsigned int)) : 0;
    var.type = DTYPE_UINT;
    return var;
}

/// @brief set the value to long
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_long(dtype var, long val)
{
    var = dtype__mem_refresh(var, sizeof(long), "dtype_set_long");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(long)) : 0;
    var.type = DTYPE_LONG;
    return var;
}

/// @brief set the value to unsigned long
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_ulong(dtype var, unsigned long val)
{
    var = dtype__mem_refresh(var, sizeof(unsigned long), "dtype_set_ulong");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(unsigned long)) : 0;
    var.type = DTYPE_ULONG;
    return var;
}

/// @brief set the value to float
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_float(dtype var, float val)
{
    var = dtype__mem_refresh(var, sizeof(float), "dtype_set_float");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(float)) : 0;
    var.type = DTYPE_FLOAT;
    return var;
}

/// @brief set the value to double
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_double(dtype var, double val)
{
    var = dtype__mem_refresh(var, sizeof(int), "dtype_set_double");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, &val, sizeof(int)) : 0;
    var.type = DTYPE_DOUBLE;
    return var;
}

/// @brief set the value to string
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_string(dtype var, char * val)
{
    var = dtype__mem_refresh(var, strlen(val)+1, "dtype_set_string");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, val, strlen(val)+1) : 0;
    var.type = DTYPE_STRING;
    return var;
}

/// @brief clear the variable, i.e set it to none
/// @param var the variable to clear
/// @return the cleared variable
dtype dtype_clear(dtype var)
{
    var = dtype__mem_refresh(var, 0, "dtype_clear");
    var.type = DTYPE_NONE;
    return var;
}

/// @brief set the value to custom type value
/// @param var the dtype variable to set to
/// @param valPointer the pointer of the custom type value
/// @param size the size of the custom type variable 
/// @return the dtype variable with value of custom type
dtype dtype_set_custom(dtype var, void * valPointer, size_t size) {
    var = dtype__mem_refresh(var, size, "dtype_set_custom");
    // copy only if size of var is not zero else do nothing
    var.size ? memcpy(var.mem, valPointer, size) : 0;
    return var;
}

//----------------- Get Functions ----------------

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a boolean
bool dtype_get_bool(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_BOOL) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_bool", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (bool*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a char
char dtype_get_char(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_CHAR) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_char", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (char*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a short
short dtype_get_short(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_SHORT) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_short", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (short*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as unsigned short
unsigned short dtype_get_ushort(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_USHORT) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_ushort", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (unsigned short*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as int
int dtype_get_int(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_INT) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_int", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (int*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as unsigned int
unsigned int dtype_get_uint(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_UINT) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_uint", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (unsigned int*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as long
long dtype_get_long(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_LONG) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_long", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (long*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as unsigned long
unsigned long dtype_get_ulong(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_ULONG) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_ulong", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (unsigned long*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a float
float dtype_get_float(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_BOOL) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_float", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (float*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a double
double dtype_get_double(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_DOUBLE) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_double", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return *( (double*) var.mem );
}

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a string
char * dtype_get_string(dtype var)
{
    // type mismatch check
    if ( dtype__typecheck(var, DTYPE_STRING) && DTYPE_WARN_EQ_ERROR ) {
        dtype__raise(
            "dtype_get_string", "All warnings treated as errors, Error produced due to type mismatch.",
            DTYPE_WARN_ERROR
        );
    }
    // return the value
    return ( char * )( var.mem );
}

/// @brief prints the content of dtype as necessary
/// @param var the dtype var to print content
/// @return the value returned by printf when printing the content.
int dtype_print(dtype var)
{
    switch ( var.type )
    {
        case DTYPE_NONE: return printf("none");
        case DTYPE_BOOL: return printf("%s", dtype_get_bool(var) ? "true" : "false");
        case DTYPE_CHAR: return printf("%c", dtype_get_char(var));
        case DTYPE_SHORT: return printf("%hi", dtype_get_short(var));
        case DTYPE_USHORT: return printf("%hu", dtype_get_ushort(var));
        case DTYPE_INT: return printf("%d", dtype_get_int(var));
        case DTYPE_UINT: return printf("%u", dtype_get_uint(var));
        case DTYPE_LONG: return printf("%ld", dtype_get_long(var));
        case DTYPE_ULONG: return printf("%lu", dtype_get_ulong(var));
        case DTYPE_FLOAT: return printf("%f", dtype_get_float(var));
        case DTYPE_DOUBLE: return printf("%lf", dtype_get_double(var));
        case DTYPE_STRING: return printf("%s", dtype_get_string(var));
        case DTYPE_CUSTOM: return printf("dtype_custom_variable");
        default: return dtype__raise("dtype_print", "Invalid type to print.", DTYPE_TYPE_ERROR);
    }
}

/// @brief debug print of dtype variable
/// @param var the variable to debug print
/// @return the value returned by dtype_print when printing the content
int dtype_debug_print(dtype var)
{
    if ( var.type < DTYPE_NONE || var.type > DTYPE_CUSTOM ) {
        return dtype__raise("dtype_debug_print", "Invalid type to print.", DTYPE_TYPE_ERROR);
    }
    printf("\n{ typecode = %d, type = `%s`, size = `%lu`, content = `", var.type, dtype_get_str_type(var), var.size);
    int ret = dtype_print(var);
    printf("` }\n");
    return ret;
}