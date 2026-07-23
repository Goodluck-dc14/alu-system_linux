#include <Python.h>
#include <limits.h>
#include <stdint.h>

/**
 * print_python_int - prints info about a Python int object
 * @p: PyObject pointer
 */
void print_python_int(PyObject *p)
{
        char sign = 0;
        unsigned long int n = 0, pw = 0, prod = 0;
        PyLongObject *lob = NULL;
        digit *dig = NULL;

#if PY_VERSION_HEX >= 0x030C0000
        uintptr_t *tag_ptr = NULL;
        uintptr_t tag = 0;
        int len = 0, i = 0;
#else
        Py_ssize_t len = 0, i = 0;
#endif

        if (!PyLong_Check(p))
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
                sign = '-';
        if ((tag & 3) == 1)
                len = 0;
#else
        len = *(Py_ssize_t *)((char *)lob + sizeof(PyObject));
        if (len < 0)
        {
                len *= -1;
                sign = '-';
        }
        dig = (digit *)((char *)lob + sizeof(PyObject) + sizeof(Py_ssize_t));
#endif
        if (!len)
                n = 0;
        else
        {
                for (i = 0; i < len; i++)
                {
                        pw = (1UL << (PyLong_SHIFT * i));
                        prod = (unsigned long)dig[i] * pw;
                        if ((dig[i] && (pw > ULONG_MAX / dig[i])) ||
                                (prod > (ULONG_MAX - n)))
                        {
                                printf("C unsigned long int overflow\n");
                                return;
                        }
                        n += prod;
                }
        }
        if (sign)
                printf("%c", sign);
        printf("%lu\n", n);
}
