#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>

#include <Python.h>
#include <frameobject.h>

#include <cstdio>

namespace Dusk {

class Thing
{
public:

    inline Thing() {
        DuskLogInfo("Thing::Thing()");
    }

    virtual ~Thing() {
        DuskLogInfo("Thing::~Thing()");
    }

    virtual void SayHello() {
        DuskLogInfo("Hello, World!");
    }

private:

};

typedef struct {
    PyObject_HEAD
    Thing * _ptr;
} Thing_data;

static PyObject * Thing_SayHello(PyObject * obj, PyObject * args) {
    Thing * self = ((Thing_data *)obj)->_ptr;
    self->SayHello();
    Py_RETURN_NONE;
}

static PyMethodDef Thing_methods[] = {
    { "SayHello", Thing_SayHello, METH_NOARGS, nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static PyObject * Thing_new(PyTypeObject * subtype, PyObject * args, PyObject * kwargs) {
    PyObject * obj = subtype->tp_alloc(subtype, 1);
    ((Thing_data *)obj)->_ptr = new Thing();
    return obj;
}

static void Thing_free(void * obj) {
    Thing_data * data = (Thing_data *)obj;
    delete data->_ptr;
    PyObject_Free(obj);
}

static int Thing_init(PyObject * obj, PyObject * args, PyObject * kwargs) {
    //Thing_data * data = (Thing_data *)obj;
    printf("Thing.__init__()\n");
    return 0;
}

static PyTypeObject Thing_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Dusk.Thing",                               // tp_name
    sizeof(Thing_data),                         // tp_basicsize
    0,                                          // tp_itemsize
    0,                                          // tp_dealloc
    0,                                          // tp_print
    0,                                          // tp_getattr
    0,                                          // tp_setattr
    0,                                          // tp_compare
    0,                                          // tp_repr
    0,                                          // tp_as_number
    0,                                          // tp_as_sequence
    0,                                          // tp_as_mapping
    0,                                          // tp_hash
    0,                                          // tp_call
    0,                                          // tp_str
    0,                                          // tp_getattro
    0,                                          // tp_setattro
    0,                                          // tp_as_buffer
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   // tp_flags
    0,                                          // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    Thing_methods,                              // tp_methods
    0,                                          // tp_members
    0,                                          // tp_getset
    0,                                          // tp_base
    0,                                          // tp_dict
    0,                                          // tp_descr_get
    0,                                          // tp_descr_set
    0,                                          // tp_dictoffset
    Thing_init,                                 // tp_init
    0,                                          // tp_alloc
    Thing_new,                                  // tp_new
    Thing_free,                                 // tp_free
    0,                                          // tp_is_gc
    0,                                          // tp_bases
    0,                                          // tp_mro
    0,                                          // tp_cache
    0,                                          // tp_subclasses
    0,                                          // tp_weaklist
#if PYTHON_API_VERSION >= 1012
    0                                           // tp_del
#endif
};

static struct PyModuleDef DuskModule = {
    PyModuleDef_HEAD_INIT,
    "Dusk",                                     // m_name
    "Dusk",                                     // m_doc
    -1,                                         // m_size
    nullptr,                                    // m_methods
};

PyMODINIT_FUNC PyInit_Dusk()
{
    PyObject * module = nullptr;

    module = PyModule_Create(&DuskModule);
    if (!module) {
        return nullptr;
    }

    if (PyType_Ready(&Thing_type) == 0) {
        Py_INCREF(&Thing_type);
        if (PyModule_AddObject(module, "Thing", (PyObject *)&Thing_type) < 0) {
            Py_DECREF(&Thing_type);
            Py_DECREF(module);
            return nullptr;
        }
    }

    return module;
}

DUSK_CORE_API void Initialize(int argc, char ** argv) {
    PyImport_AppendInittab("Dusk", PyInit_Dusk);

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program) {
        Py_SetProgramName(program);
    }

    PyMem_RawFree(program);
    
    Py_Initialize();
    PyImport_ImportModule("Dusk");

    DuskLogVerbose("Dusk Version: %s", GetVersion().GetString());
    DuskLogVerbose("Application Name: %s", GetApplicationName());
    DuskLogVerbose("Application Version: %s", GetApplicationVersion().GetString());
}

DUSK_CORE_API void Terminate() {
    Py_Finalize();

    FreeModules();
}

static bool _Running = false;

DUSK_CORE_API void SetRunning(bool running) {
    _Running = running;
}

DUSK_CORE_API bool IsRunning() {
    return _Running;
}

DUSK_CORE_API void RunScript(const std::string& filename) {
    FILE * file = fopen(filename.c_str(), "rt");
    if (!file) {
        exit(1);
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_File(file, filename.c_str(), Py_file_input, pyMainDict, pyLocalDict);

    if (PyErr_Occurred()) {
        PyObject * pyType = nullptr;
        PyObject * pyValue = nullptr;
        PyObject * pyTrace = nullptr;

        PyErr_Fetch(&pyType, &pyValue, &pyTrace);
        PyErr_NormalizeException(&pyType, &pyValue, &pyTrace);

        PyObject * pyValueRepr = PyObject_Repr(pyValue);
        if (pyValueRepr) {
            PyObject * pyValueStr = PyUnicode_AsEncodedString(pyValueRepr, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyValueStr));

            Py_XDECREF(pyValueStr);
            Py_XDECREF(pyValueRepr);
        }
        else {
            PyObject * pyTypeName = PyObject_GetAttrString(pyType, "__name__");
            PyObject * pyTypeNameStr = PyUnicode_AsEncodedString(pyTypeName, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyTypeNameStr));

            Py_XDECREF(pyTypeNameStr);
            Py_XDECREF(pyTypeName);
        }
    }

    fclose(file);
}

static std::string _ApplicationName;

DUSK_CORE_API
void SetApplicationName(const std::string& name)
{
    _ApplicationName = name;
}

DUSK_CORE_API
std::string GetApplicationName()
{
    return _ApplicationName;
}

static Version _ApplicationVersion;

DUSK_CORE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch)
{
    _ApplicationVersion = Version(major, minor, patch);
}

DUSK_CORE_API
void SetApplicationVersion(const Version& version)
{
    _ApplicationVersion = version;
}

DUSK_CORE_API
Version GetApplicationVersion()
{
    return _ApplicationVersion;
}

}
