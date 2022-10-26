#if !defined(DTYPE_H_INCL)
#define DTYPE_H_INCL

#include <stdbool.h>
#include <stddef.h>

/// @brief enum containing the type which can be represented in dtype
enum DTYPE_TYPES {
    DTYPE_NONE,
    /// @brief dtype_type indicating the type is boolean
    DTYPE_BOOL,
    /// @brief dtype_type indicating the type is character
    DTYPE_CHAR,
    /// @brief dtype_type indicating the type is short
    DTYPE_SHORT,
    /// @brief dtype_type indicating the type is unsigned short
    DTYPE_USHORT,
    /// @brief dtype_type indicating the type is int
    DTYPE_INT,
    /// @brief dtype_type indicating the type is unsigned int
    DTYPE_UINT,
    /// @brief dtype_type indicating the type is long
    DTYPE_LONG,
    /// @brief dtype_type indicating the type is unsigned long
    DTYPE_ULONG,
    /// @brief dtype_type indicating the type is float
    DTYPE_FLOAT,
    /// @brief dtype_type indicating the type is double
    DTYPE_DOUBLE,
    /// @brief dtype_type indicating the type is string
    DTYPE_STRING,
    /// @brief dtype_type indicating the type is a custom type
    DTYPE_CUSTOM
};


/// @brief the actual dtype definition
typedef struct dtype {
    /// @brief memory where the data is stored.
    void * mem;
    /// @brief current size of allocated memory
    size_t size;
    /// @brief curremt type of data stored in dtype
    enum DTYPE_TYPES type;
} dtype;

enum DTYPE_ERRORS {
    /// @brief dtype_error indicating no error
    DTYPE_NO_ERROR,
    /// @brief dtype_error indicating memory error
    DTYPE_MEMORY_ERROR,
    /// @brief dtype error indicating warning error [meaning that warn is equal to error & warning was produced]
    DTYPE_WARN_ERROR,
    /// @brief dtype error indicating type error.
    DTYPE_TYPE_ERROR,
    /// @brief dtype_error indicating unknown error
    DTYPE_UNKNOWN_ERROR
};


// ------------------------------ Function Definitions -----------------------------------

/// @brief get the string representation of type
/// @param var variable to get type from
/// @return the type of variable as string
const char * dtype_get_str_type(dtype var);

/// @brief Returns a null, but initialized dtype variable
/// @return initialized dtype variable
dtype dtype_default();

/// @brief prints the content of dtype as necessary
/// @param var the dtype var to print content
/// @return the value returned by printf when printing the content.
int dtype_print(dtype var);

/// @brief debug print of dtype variable
/// @param var the variable to debug print
/// @return the value returned by dtype_print when printing the content
int dtype_debug_print(dtype var);

/// @brief Changes size of the dtype variabled [ doesn't affect content as long as it fits ]
/// @param var the variable to change size.
/// @param size the size to change to. [ can't be zero ]
/// @return the dtype of given size.
dtype dtype_change_size(dtype var, size_t size);

// ----------- Set Functions ------------

/// @brief set the value to boolean
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_bool(dtype var, bool val);

/// @brief set the value to char
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_char(dtype var, char val);

/// @brief set the value to short
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_short(dtype var, short val);

/// @brief set the value to unsigned short
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_ushort(dtype var, unsigned short val);

/// @brief set the value to int
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_int(dtype var, int val);

/// @brief set the value to unsigned int
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_uint(dtype var, unsigned int val);

/// @brief set the value to long
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_long(dtype var, long val);

/// @brief set the value to unsigned long
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_ulong(dtype var, unsigned long val);

/// @brief set the value to float
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_float(dtype var, float val);

/// @brief set the value to double
/// @param var the dtype variable to set to
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_double(dtype var, double val);

/// @brief set the value to string
/// @param var the dtype variable to set dtype_clearto
/// @param val the value to set to
/// @return the dtype variable with value as given
dtype dtype_set_string(dtype var, char * val);

/// @brief clear the variable, i.e set it to none
/// @param var the variable to clear
/// @return the cleared variable
dtype dtype_clear(dtype var);

/// @brief set the value to custom type value
/// @param var the dtype variable to set to
/// @param valPointer the pointer of the custom type value
/// @param size the size of the custom type variable 
/// @return the dtype variable with value of custom type
dtype dtype_set_custom(dtype var, void * valPointer, size_t size);

// ----------- Get Functions ------------

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a boolean
bool dtype_get_bool(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a char
char dtype_get_char(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a short
short dtype_get_short(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as unsigned short
unsigned short dtype_get_ushort(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as int
int dtype_get_int(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as unsigned int
unsigned int dtype_get_uint(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as long
long dtype_get_long(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as unsigned long
unsigned long dtype_get_ulong(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a float
float dtype_get_float(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a double
double dtype_get_double(dtype var);

/// @brief get the value of dtype variable
/// @param var the dtype variable to get from
/// @return the value of given dtype variable as a string
char * dtype_get_string(dtype var);

#endif // DTYPE_H_INCL