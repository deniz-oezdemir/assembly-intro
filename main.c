#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// Declare the external functions from the assembly files
size_t  ft_strlen(const char *s);
char    *ft_strcpy(char *dest, const char *src);
int     ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fd, const void *buf, size_t count);

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

void test_write(int fd, const char *buffer, size_t size, const char *test_name) {
    // Save original errno
    int original_errno = errno;
    errno = 0;

    // Test libc write
    ssize_t libc_result = write(fd, buffer, size);
    int libc_errno = errno;

    // Reset errno
    errno = 0;

    // Test ft_write
    ssize_t asm_result = ft_write(fd, buffer, size);
    int asm_errno = errno;

    // Restore original errno
    errno = original_errno;

    // Check if results match
    int result_match = (libc_result == asm_result);
    int errno_match = (libc_errno == asm_errno);

    printf("Testing write: %s\n", test_name);
    // printf("  Buffer: \"%.*s\"\n", (int)size, buffer);
    printf("  Size: %zu bytes\n", size);
    printf("  libc write result: %zd\n", libc_result);
    printf("  asm write result: %zd\n", asm_result);

    if (libc_result < 0)
        printf("  libc errno: %d (%s)\n", libc_errno, strerror(libc_errno));
    if (asm_result < 0)
        printf("  asm errno: %d (%s)\n", asm_errno, strerror(asm_errno));

    printf("  Return value match: %s\n", result_match ? "✅ PASS" : "❌ FAIL");

    if (libc_result < 0 || asm_result < 0)
        printf("  Errno match: %s\n\n", errno_match ? "✅ PASS" : "❌ FAIL");
    else
        printf("\n");
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

    printf("\n=== ft_write Testing ===\n\n");

    // Test writing to stdout
    const char *stdout_msg = "Test message to stdout\n";
    test_write(STDOUT_FILENO, stdout_msg, strlen(stdout_msg), "Standard output");

    // Test writing to a file
    int fd = open("test_write.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        const char *file_msg = "Test message to file\n";
        test_write(fd, file_msg, strlen(file_msg), "File output");
        close(fd);

        // Read back and show the file content
        char buffer[100];
        fd = open("test_write.txt", O_RDONLY);
        if (fd >= 0) {
            ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("File content verification:\n\"%s\"\n\n", buffer);
            }
            close(fd);
        }
    } else {
        printf("Failed to create test file\n\n");
    }

    // Test with invalid file descriptor
    test_write(-1, "This should fail", 15, "Invalid file descriptor");

    // Test with NULL buffer
    test_write(1, NULL, 10, "NULL buffer");

    return 0;
}
