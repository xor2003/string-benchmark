
#include "config.hpp"

/**
 * Generic implementation.
 */
template<typename T>
unsigned long build(benchmark::input& input)
{
    unsigned long res;

    BENCHMARK_FOREACH(s)
    {
        T str(s);
        res += str.size();
    }

    return res;
}

#ifdef USE_PYTHON_STRING
/**
 * Python String Object specialization.
 */
template<>
unsigned long build<PyStringObject>(benchmark::input& input)
{
    unsigned long res;

    BENCHMARK_FOREACH(s)
    {
        PyObject* str = PyString_FromString(s);
        res += PyString_Size(str);
        Py_DECREF(str);
    }
    return res;
}
#endif // USE_PYTHON_STRING

#ifdef USE_GC_CORD
/**
 * GC CORD specialization.
 */
template<>
unsigned long build<CORD>(benchmark::input& input)
{
    unsigned long res;

    BENCHMARK_FOREACH(s)
    {
        CORD str = CORD_from_char_star(s);
        res += CORD_len(str);
    }

    return res;
}
#endif // USE_GC_CORD

int main(int argc, char* argv[])
{
    BENCHMARK_GET_ITERATIONS(iterations);
    BENCHMARK_ACQUIRE_INPUT(input);
    BENCHMARK_ITERATE(input, iterations)
    {
        printf( "build: %lu bytes.\n", build<STR>(input));
    }
    return 0;
}
