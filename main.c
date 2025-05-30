#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Declare the external function from the assembly file
size_t ft_strlen(const char *s);

// Function to compare results and print
void test_string(const char *str) {
    size_t lib_result = strlen(str);
    size_t asm_result = ft_strlen(str);

    printf("Testing: \"%s\"\n", str);
    printf("  libc strlen: %zu\n", lib_result);
    printf("   asm strlen: %zu\n", asm_result);
    printf("       Result: %s\n\n",
           (lib_result == asm_result) ? "✅ PASS" : "❌ FAIL");
}

int main(void) {
    printf("=== ft_strlen Testing ===\n\n");

    // Test cases
    test_string("Hello, friend");
    test_string("");
    test_string("Special chars: !@#$%^&*()");

    // Test with allocated memory
    char *dynamic_str = malloc(1000);
    if (dynamic_str) {
        for (int i = 0; i < 1000; i++)
            dynamic_str[i] = 'A' + (i % 26);
        dynamic_str[1000] = '\0';
        test_string(dynamic_str);
        free(dynamic_str);
    }

    return 0;
}
