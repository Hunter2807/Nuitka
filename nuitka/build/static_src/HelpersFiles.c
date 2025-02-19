//     Copyright 2021, Kay Hayen, mailto:kay.hayen@gmail.com
//
//     Part of "Nuitka", an optimizing Python compiler that is compatible and
//     integrates with CPython, but also works on its own.
//
//     Licensed under the Apache License, Version 2.0 (the "License");
//     you may not use this file except in compliance with the License.
//     You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//     Unless required by applicable law or agreed to in writing, software
//     distributed under the License is distributed on an "AS IS" BASIS,
//     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//     See the License for the specific language governing permissions and
//     limitations under the License.
//
/* Small helpers to access files and their contents */

// This file is included from another C file, help IDEs to still parse it on
// its own.
#ifdef __IDE_ONLY__
#include "nuitka/prelude.h"
#endif

// Small helper to open files with few arguments.
PyObject *BUILTIN_OPEN_SIMPLE(PyObject *filename, char const *mode, bool buffering) {
    PyObject *mode_obj = Nuitka_String_FromString(mode);

    PyObject *buffering_obj = buffering ? const_int_pos_1 : const_int_0;

#if PYTHON_VERSION < 0x300
    PyObject *result = BUILTIN_OPEN(filename, mode_obj, buffering_obj);
#else
    PyObject *result = BUILTIN_OPEN(filename, mode_obj, buffering_obj, NULL, NULL, NULL, NULL, NULL);
#endif

    Py_DECREF(mode_obj);

    return result;
}

PyObject *GET_FILE_BYTES(PyObject *filename) {
    PyObject *result;

    if (TRACE_FILE_OPEN(filename, &result)) {
        return result;
    }

    PyObject *data_file = BUILTIN_OPEN_SIMPLE(filename, "rb", false);

    if (unlikely(data_file == NULL)) {
        // TODO: Issue a runtime warning maybe.
        return NULL;
    }

    PyObject *read_method = PyObject_GetAttr(data_file, const_str_plain_read);
    Py_DECREF(data_file);

    if (unlikely(read_method == NULL)) {
        return NULL;
    }

    result = CALL_FUNCTION_NO_ARGS(read_method);
    Py_DECREF(read_method);
    return result;
}
