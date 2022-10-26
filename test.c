#include <dtype.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
} custom;

int main()
{
    dtype var = dtype_default();
    // working with custom types

    // 1: creating custom type

    // change size as required.
    var = dtype_change_size(var, sizeof(custom));
    // set the properties.
    ((custom *) var.mem ) -> x = 10;
    ((custom *) var.mem ) -> y = 20;
    // print the properties
    printf("x = %d, y = %d\n", ((custom*) var.mem) -> x, ((custom*) var.mem) -> y);
    // debug info
    dtype_debug_print(var);
    printf("\n");

    // 2: copying custom type, can also directly copy through creating and accessing
    // but for easier method.

    // create a custom type to copy from
    custom c1 = {15, 16};
    // copy the custom type
    var = dtype_set_custom(var, &c1, sizeof(custom));
    // clear the original to verify the copy
    c1.x = 0; c1.y = 0;
    // print the properties
    printf("x = %d, y = %d\n", ((custom*) var.mem) -> x, ((custom*) var.mem) -> y);
    // debug info
    dtype_debug_print(var);
    printf("\n");
}