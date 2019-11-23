#include <Dusk/Dusk.hpp>

#include <Dusk/Module.hpp>

#include <Python.h>
#include <frameobject.h>

#include <cstdio>

namespace Dusk {

class Thing
{
public:

    inline Thing() {
        printf("Thing::Thing()\n");
    }

    virtual ~Thing() {
        printf("Thing::~Thing()\n");
    }

    virtual void SayHello() {
        printf("Hello, World!\n");
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
    .tp_name        = "Dusk.Thing",
    .tp_doc         = "Dusk Thing",
    .tp_basicsize   = sizeof(Thing_data),
    .tp_itemsize    = 0,
    .tp_flags       = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new         = Thing_new,
    .tp_free        = Thing_free,
    .tp_init        = Thing_init,
    .tp_methods     = Thing_methods,
};

static struct PyModuleDef DuskModule = {
    PyModuleDef_HEAD_INIT,
    .m_name         = "Dusk",
    .m_doc          = nullptr,
    .m_size         = -1,
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

void Initialize(int argc, char ** argv) {
    PyImport_AppendInittab("Dusk", PyInit_Dusk);
    Py_Initialize();
    PyImport_ImportModule("Dusk");
}

void Terminate() {
    Py_Finalize();

    FreeModules();
}

void RunScript(const std::string& filename) {
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

}