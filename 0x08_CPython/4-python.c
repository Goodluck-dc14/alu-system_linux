#include <Python.h>

/**
 * print_python_string - prints info about a Python string object
 * @p: PyObject pointer
 */
void print_python_string(PyObject *p)
{
        PyGILState_STATE gstate;
        PyObject *utf8_bytes = NULL;

        gstate = PyGILState_Ensure();

        printf("[.] string object info\n");
        if (!PyUnicode_Check(p))
        {
                printf("  [ERROR] Invalid String Object\n");
                PyGILState_Release(gstate);
                return;
        }
        printf("  type: %s\n",
                PyUnicode_IS_COMPACT_ASCII(p) ? "compact ascii" : "compact unicode object");
        printf("  length: %zd\n", PyUnicode_GET_LENGTH(p));

        utf8_bytes = PyUnicode_AsUTF8String(p);
        if (utf8_bytes == NULL)
        {
                printf("  value: [ERROR] Could not encode string\n");
                PyErr_Clear();
                PyGILState_Release(gstate);
                return;
        }
        printf("  value: %s\n", PyBytes_AsString(utf8_bytes));
        Py_DECREF(utf8_bytes);

        PyGILState_Release(gstate);
}
