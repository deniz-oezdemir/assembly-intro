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
ssize_t ft_read(int fd, void *buf, size_t count);
char    *ft_strdup(const char *s);

// Helper functions
void verify_file_content(const char *filename);
void create_test_file(const char *filename, const char *content);

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

    // Check for sign match
    int sign_match = (libc_result < 0 && asm_result < 0) ||
                   (libc_result == 0 && asm_result == 0) ||
                   (libc_result > 0 && asm_result > 0);

    printf("Testing strcmp: \"%s\" vs \"%s\"\n", s1, s2);
    printf("  libc strcmp: %d\n", libc_result);
    printf("  asm strcmp: %d\n", asm_result);
    printf("  Result: %s\n\n", sign_match ? "✅ PASS" : "❌ FAIL");
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

void test_read(int fd, size_t size, const char *test_name) {
    // Buffers to store read data
    char libc_buffer[1024] = {0};
    char asm_buffer[1024] = {0};

    // Save original errno and file position
    int original_errno = errno;
    off_t original_pos = -1;

    // If this is a regular file, save position for repositioning
    if (fd > 0) {
        original_pos = lseek(fd, 0, SEEK_CUR);
    }

    errno = 0;

    // Test libc read
    ssize_t libc_result = read(fd, libc_buffer, size);
    int libc_errno = errno;

    // Reset file position if applicable
    if (original_pos >= 0) {
        lseek(fd, original_pos, SEEK_SET);
    }

    // Reset errno
    errno = 0;

    // Test ft_read
    ssize_t asm_result = ft_read(fd, asm_buffer, size);
    int asm_errno = errno;

    // Restore original errno
    errno = original_errno;

    // Check if results match
    int result_match = (libc_result == asm_result);
    int errno_match = (libc_errno == asm_errno);

    // Only compare content if both reads succeeded
    int content_match = 0;
    if (libc_result > 0 && asm_result > 0) {
        content_match = (memcmp(libc_buffer, asm_buffer,
                              (libc_result < asm_result) ? libc_result : asm_result) == 0);
    }

    printf("Testing read: %s\n", test_name);
    printf("  Size requested: %zu bytes\n", size);
    printf("  libc read result: %zd bytes\n", libc_result);
    printf("  asm read result: %zd bytes\n", asm_result);

    if (libc_result > 0) {
        printf("  libc read content: \"%.*s\"\n", (int)libc_result, libc_buffer);
    }

    if (asm_result > 0) {
        printf("  asm read content: \"%.*s\"\n", (int)asm_result, asm_buffer);
    }

    if (libc_result < 0)
        printf("  libc errno: %d (%s)\n", libc_errno, strerror(libc_errno));
    if (asm_result < 0)
        printf("  asm errno: %d (%s)\n", asm_errno, strerror(asm_errno));

    printf("  Return value match: %s\n", result_match ? "✅ PASS" : "❌ FAIL");

    if (libc_result > 0 && asm_result > 0)
        printf("  Content match: %s\n", content_match ? "✅ PASS" : "❌ FAIL");

    if (libc_result < 0 || asm_result < 0)
        printf("  Errno match: %s\n\n", errno_match ? "✅ PASS" : "❌ FAIL");
    else
        printf("\n");
}

void test_strdup(const char *str) {
    // Perform both duplications
    char *libc_result = strdup(str);
    char *asm_result = ft_strdup(str);

    // Check if both allocations succeeded
    int alloc_match = ((libc_result == NULL) == (asm_result == NULL));

    // Check if the strings match (only if both allocations succeeded)
    int content_match = 0;
    if (libc_result != NULL && asm_result != NULL) {
        content_match = (strcmp(libc_result, asm_result) == 0);
    }

    printf("Testing strdup: \"%s\"\n", str);
    if (libc_result) {
        printf("  libc strdup: \"%s\"\n", libc_result);
    } else {
        printf("  libc strdup: NULL\n");
    }

    if (asm_result) {
        printf("  asm strdup: \"%s\"\n", asm_result);
    } else {
        printf("  asm strdup: NULL\n");
    }

    printf("  Allocation result: %s\n", alloc_match ? "✅ PASS" : "❌ FAIL");
    // Test case pairs (s1, s2)

    if (libc_result != NULL && asm_result != NULL) {
        printf("  String content: %s\n\n", content_match ? "✅ PASS" : "❌ FAIL");
    } else {
        printf("\n");
    }

    // Free the allocated memory
    if (libc_result) free(libc_result);
    if (asm_result) free(asm_result);
}

void run_strlen_tests(void) {
    printf("=== ft_strlen Testing ===\n\n");

    // Basic test cases
    const char *basic_tests[] = {
        "Hello, friend",
        "",
        "Special chars: !@#$%^&*()"
    };

    for (size_t i = 0; i < sizeof(basic_tests) / sizeof(basic_tests[0]); i++) {
        test_strlen(basic_tests[i]);
    }

    // Test with allocated memory
    char *dynamic_str = malloc(1000);
    if (dynamic_str) {
        for (int i = 0; i < 999; i++)
            dynamic_str[i] = 'A' + (i % 26);
        dynamic_str[999] = '\0';
        test_strlen(dynamic_str);
        free(dynamic_str);
    }
}

void run_strcpy_tests(void) {
    printf("\n=== ft_strcpy Testing ===\n\n");

    // Basic test cases
    const char *basic_tests[] = {
        "Hello, world!",
        "",
        "Special chars: !@#$%^&*()"
    };

    for (size_t i = 0; i < sizeof(basic_tests) / sizeof(basic_tests[0]); i++) {
        test_strcpy(basic_tests[i]);
    }

    // Test with allocated memory
    char *dynamic_str = malloc(1000);
    if (dynamic_str) {
        for (int i = 0; i < 999; i++)
            dynamic_str[i] = 'A' + (i % 26);
        dynamic_str[999] = '\0';
        test_strcpy(dynamic_str);
        free(dynamic_str);
    }
}

void run_strcmp_tests(void) {
    printf("\n=== ft_strcmp Testing ===\n\n");

    // Test case pairs (s1, s2)
    const char *test_pairs[][2] = {
        {"", ""},                    // Two empty strings
        {"", "Hello"},               // First empty, second not
        {"Hello", ""},               // Second empty, first not
        {"identical", "identical"},  // Equal strings
        {"apple", "banana"},         // First < second
        {"banana", "apple"},         // First > second
        {"test", "testing"},         // First is prefix of second
        {"testing", "test"}          // Second is prefix of first
    };

    for (size_t i = 0; i < sizeof(test_pairs) / sizeof(test_pairs[0]); i++) {
        test_strcmp(test_pairs[i][0], test_pairs[i][1]);
    }
}

void run_write_tests(void) {
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
        verify_file_content("test_write.txt");
    } else {
        printf("Failed to create test file\n\n");
    }

    // Test error cases
    test_write(-1, "This should fail", 15, "Invalid file descriptor");
    test_write(1, NULL, 10, "NULL buffer");
}

void verify_file_content(const char *filename) {
    char buffer[100];
    int fd = open(filename, O_RDONLY);
    if (fd >= 0) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("File content verification:\n\"%s\"\n\n", buffer);
        }
        close(fd);
    }
}

void run_read_tests(void) {
    printf("\n=== ft_read Testing ===\n\n");

    // Create a test file
    const char *test_content = "This is test content for reading.\nSecond line of the file.\n";
    create_test_file("test_read.txt", test_content);

    // Test reading from file
    int fd = open("test_read.txt", O_RDONLY);
    if (fd >= 0) {
        test_read(fd, 100, "File input");
        close(fd);
    } else {
        printf("Failed to open test file for reading\n\n");
    }

    // Test stdin and invalid FD
    printf("For the stdin test, please type same text and press Enter twice:\n");
    test_read(STDIN_FILENO, 50, "Standard input");
    test_read(-1, 10, "Invalid file descriptor");
}

void create_test_file(const char *filename, const char *content) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, content, strlen(content));
        close(fd);
    } else {
        printf("Failed to create test file %s\n\n", filename);
    }
}

void run_strdup_tests(void) {
    printf("\n=== ft_strdup Testing ===\n\n");

    // Basic test cases
    const char *basic_tests[] = {
        "",
        "Hello, world!",
        "This is a test string with some special chars: !@#$%^&*()"
    };

    for (size_t i = 0; i < sizeof(basic_tests) / sizeof(basic_tests[0]); i++) {
        test_strdup(basic_tests[i]);
    }

    // Test with very long string
    char *long_str = malloc(1000);
    if (long_str) {
        for (int i = 0; i < 999; i++)
            long_str[i] = 'A' + (i % 26);
        long_str[999] = '\0';
        test_strdup(long_str);
        free(long_str);
    }
}

int main(void) {
    run_strlen_tests();
    run_strcpy_tests();
    run_strcmp_tests();
    run_write_tests();
    run_read_tests();
    run_strdup_tests();
    return 0;
}
