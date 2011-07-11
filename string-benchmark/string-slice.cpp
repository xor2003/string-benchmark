
#include "config.hpp"

#include <algorithm>
#include <cstring>

/**
 * Generic implementation.
 */
template<typename T>
unsigned long slice()
{
    size_t total = 0, prev = 0, ante = 0;

    FOREACH_LINE
    {
        T str(s);

        size_t cur = str.size();
        if (cur != 0)
        {
            size_t from = prev % cur;
            size_t to   = ante % cur;

            if (from > to)
            {
                std::swap(from, to);
            }

            T sliced = str.substr(from, (to - from));

            total += sliced.size();
        }
        ante = prev;
        prev = cur;
    }

    return total;
}

#ifdef USE_PYTHON_STRING
/**
 * Python String Object specialization.
 */
template<>
unsigned long slice<PyStringObject>()
{
    size_t total = 0, prev = 0, ante = 0;

    FOREACH_LINE
    {
        PyObject *str = PyString_FromString(s);

        size_t cur = PyString_Size(str);
        if (cur != 0)
        {
            size_t from = prev % cur;
            size_t to   = ante % cur;

            if (from > to)
            {
                std::swap(from, to);
            }

            PyObject* sliced = PySequence_GetSlice(str, from, to);

            total += PyString_Size(sliced);

            Py_DECREF(sliced);
        }
        ante = prev;
        prev = cur;

        Py_DECREF(str);
    }

    return total;
}
#endif // USE_PYTHON_STRING

#ifdef USE_GC_CORD
/**
 * GC CORD specialization.
 */
template<>
unsigned long slice<CORD>()
{
    size_t total = 0, prev = 0, ante = 0;

    FOREACH_LINE
    {
        CORD str = CORD_from_char_star(s);

        size_t cur = CORD_len(str);
        if (cur != 0)
        {
            size_t from = prev % cur;
            size_t to   = ante % cur;

            if (from > to)
            {
                std::swap(from, to);
            }

            CORD sliced = CORD_substr(str, from, (to - from));

            total += CORD_len(sliced);
        }
        ante = prev;
        prev = cur;
    }

    return total;
}
#endif // USE_GC_CORD

int main()
{
    printf( "slice: %lu bytes.\n", slice<STR>());
    return 0;
}
