#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * print_python_int - prints a Python int of any size in base 10
 * @p: PyObject pointer
 */
void print_python_int(PyObject *p)
{
        PyLongObject *lob = NULL;
        digit *dig = NULL;
        uint32_t *digits = NULL;
        char *buffer = NULL;
        int negative = 0, buf_idx = 0, len = 0;
        Py_ssize_t i = 0, size = 0;

#if PY_VERSION_HEX >= 0x030C0000
        uintptr_t *tag_ptr = NULL;
        uintptr_t tag = 0;
#else
        Py_ssize_t raw_size = 0;
#endif

        if (!p || !PyLong_Check(p))
        {
                printf("Invalid Int Object\n");
                return;
        }

        lob = (PyLongObject *)p;

#if PY_VERSION_HEX >= 0x030C0000
        tag_ptr = (uintptr_t *)((char *)lob + sizeof(PyObject));
        tag = *tag_ptr;
        len = (int)(tag >> 3);
        dig = (digit *)((char *)tag_ptr + sizeof(uintptr_t));
        if ((tag & 3) == 2)
                negative = 1;
        if ((tag & 3) == 1)
                len = 0;
#else
        raw_size = *(Py_ssize_t *)((char *)lob + sizeof(PyObject));
        if (raw_size < 0)
        {
                negative = 1;
                raw_size = -raw_size;
        }
        len = (int)raw_size;
        dig = (digit *)((char *)lob + sizeof(PyObject) + sizeof(Py_ssize_t));
#endif

        if (len == 0)
        {
                printf("0\n");
                return;
        }

        size = len;
        digits = malloc(size * sizeof(uint32_t));
        if (!digits)
                return;
        for (i = 0; i < size; i++)
                digits[i] = (uint32_t)dig[i];

        buffer = malloc((size_t)(size * 10 + 2));
        if (!buffer)
        {
                free(digits);
                return;
        }

        while (size > 0)
        {
                uint64_t remainder = 0;

                for (i = size - 1; i >= 0; i--)
                {
                        uint64_t temp = ((uint64_t)remainder << PyLong_SHIFT) + digits[i];

                        digits[i] = (uint32_t)(temp / 10);
                        remainder = temp % 10;
                }
                buffer[buf_idx++] = (char)(remainder + '0');

                while (size > 0 && digits[size - 1] == 0)
                        size--;
        }

        if (negative)
                putchar('-');

        for (i = buf_idx - 1; i >= 0; i--)
                putchar(buffer[i]);
        putchar('\n');

        free(digits);
        free(buffer);
}
