#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Declare the external functions from the assembly files
size_t  ft_strlen(const char *s);
char    *ft_strcpy(char *dest, const char *src);
int     ft_strcmp(const char *s1, const char *s2);

void test_strlen(const char *str) {
    size_t libc_result = strlen(str);
    size_t asm_result = ft_strlen(str);

    printf("Testing: \"%s\"\n", str);
    printf("  libc strlen: %zu\n", libc_result);
    printf("  asm strlen: %zu\n", asm_result);
    printf("  Result: %s\n\n",
           (libc_result == asm_result) ? "✅ PASS" : "❌ FAIL");
}

void test_strcpy(const char *src) {
    char libc_dest[1024] = {0};
    char asm_dest[1024] = {0};

    // Perform both copies
    char *libc_result = strcpy(libc_dest, src);
    char *asm_result = ft_strcpy(asm_dest, src);

    // Check if both return the correct pointer
    int return_match = (libc_result == libc_dest) && (asm_result == asm_dest);

    // Check if the strings match
    int content_match = strcmp(libc_dest, asm_dest) == 0;

    printf("Testing strcpy: \"%s\"\n", src);
    printf("  libc strcpy: \"%s\"\n", libc_dest);
    printf("  asm strcpy: \"%s\"\n", asm_dest);
    printf("  Return value: %s\n", return_match ? "✅ PASS" : "❌ FAIL");
    printf("  String content: %s\n\n", content_match ? "✅ PASS" : "❌ FAIL");
}

void test_strcmp(const char *s1, const char *s2) {
    int libc_result = strcmp(s1, s2);
    int asm_result = ft_strcmp(s1, s2);

    // Check for exact value match
    int exact_match = (libc_result == asm_result);

    printf("Testing strcmp: \"%s\" vs \"%s\"\n", s1, s2);
    printf("  libc strcmp: %d\n", libc_result);
    printf("  asm strcmp: %d\n", asm_result);

    if (exact_match) {
        printf("  Result: ✅ PASS\n\n");
    } else {
        printf("  Result: ❌ FAIL\n\n");
    }
}

int main(void) {
    printf("=== ft_strlen Testing ===\n\n");

    // Test cases
    test_strlen("Hello, friend");
    test_strlen("");
    test_strlen("Special chars: !@#$%^&*()");

    // Test with allocated memory
    char *dynamic_str = malloc(1000);
    if (dynamic_str) {
        for (int i = 0; i < 1000; i++)
            dynamic_str[i] = 'A' + (i % 26);
        dynamic_str[1000] = '\0';
        test_strlen(dynamic_str);
        free(dynamic_str);
    }

    printf("\n=== ft_strcpy Testing ===\n\n");

    // Test cases for strcpy
    test_strcpy("Hello, world!");
    test_strcpy("");
    test_strcpy("Special chars: !@#$%^&*()");

    // Test with allocated memory
    dynamic_str = malloc(1000);
    if (dynamic_str) {
        for (int i = 0; i < 999; i++)
            dynamic_str[i] = 'A' + (i % 26);
        dynamic_str[999] = '\0';
        test_strcpy(dynamic_str);
        free(dynamic_str);
    }

    printf("\n=== ft_strcmp Testing ===\n\n");

    // Empty string tests
    test_strcmp("", "");                    // Two empty strings
    test_strcmp("", "Hello");               // First empty, second not
    test_strcmp("Hello", "");               // Second empty, first not

    // Equal strings
    test_strcmp("identical", "identical");

    // Different strings and order switching
    test_strcmp("apple", "banana");         // First < second
    test_strcmp("banana", "apple");         // First > second (switched)

    // Prefix strings
    test_strcmp("test", "testing");         // First is prefix of second
    test_strcmp("testing", "test");         // Second is prefix of first

    return 0;
}
