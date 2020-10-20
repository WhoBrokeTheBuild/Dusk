#include <Python/TextureImporter.hpp>

namespace Dusk::Python {

BaseTextureData::BaseTextureData(PyObject * self)
    : _self(self)
{
    PyObject * pyEnumModule = PyImport_ImportModule("enum");
    PyObject * pyEnumDict = PyModule_GetDict(pyEnumModule);
    PyObject * pyEnumType = PyDict_GetItemString(pyEnumDict, "Enum");
    

}

BaseTextureData::~BaseTextureData()
{
    PyBuffer_Release(&_buffer);
}

uint8_t * BaseTextureData::GetData()
{
    PyObject * pyGetData = PyDict_GetItemString(_self, "GetData");
    if (!PyCallable_Check(pyGetData)) {
        return NULL;
    }

    PyObject * result = PyObject_CallFunction(pyGetData, NULL);
    if (!PyObject_CheckBuffer(result)) {
        return NULL;
    }

    if (PyObject_GetBuffer(result, &_buffer, 0) < 0) {
        return NULL;
    }

    if (!PyBuffer_IsContiguous(&_buffer, 'C')) {
        return NULL;
    }

    

}

typedef struct 
{
    PyObject_HEAD
    BaseTextureData * Pointer;

} TextureData;

static PyMethodDef TextureData_methods[] = {
    // { "AddListener",            PyScriptEvent_AddListener,          METH_VARARGS,    nullptr },
    // { "RemoveAllListeners",     PyScriptEvent_RemoveAllListeners,   METH_VARARGS,    nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static PyMemberDef TextureData_members[] = {

    // { "WindowResizedEvent", T_OBJECT, offsetof(PyGraphicsDriver, WindowResizedEvent), READONLY, nullptr },
    { nullptr, 0, 0, 0, nullptr },
};

PyObject * TextureData_alloc(PyTypeObject *self, Py_ssize_t nitems)
{
    PyObject * obj = PyType_GenericAlloc(&TextureData_type, 1);
    
    TextureData * data = (TextureData *)obj;
    data->Pointer = new BaseTextureData(obj);

    return obj;
}

void TextureData_free(TextureData * obj)
{
    delete obj->Pointer;
    obj->Pointer = NULL;
    
    PyObject_Free(obj);
}

static PyTypeObject TextureData_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Dusk.TextureData",                         // tp_name 
    sizeof(TextureData),                        // tp_basicsize
    0,                                          // tp_itemsize
    0,                                          // tp_dealloc
    0,                                          // tp_vectorcall_offset
    0,                                          // tp_getattr
    0,                                          // tp_setattr
    0,                                          // tp_as_async
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
    Py_TPFLAGS_DEFAULT,                         // tp_flags
    "Dusk Texture Data",                        // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    TextureData_methods,                        // tp_methods
    TextureData_members,                        // tp_members
    0,                                          // tp_getset
    0,                                          // tp_base
    0,                                          // tp_dict
    0,                                          // tp_descr_get
    0,                                          // tp_descr_set
    0,                                          // tp_dictoffset
    0,                                          // tp_init
    TextureData_alloc,                          // tp_alloc
    0,                                          // tp_new
    TextureData_free,                           // tp_free
    0,                                          // tp_is_gc
    0,                                          // tp_bases
    0,                                          // tp_mro
    0,                                          // tp_cache
    0,                                          // tp_subclasses
    0,                                          // tp_weaklist
    0,                                          // tp_del
    0,                                          // tp_version_tag
    0,                                          // tp_finalize
    0,                                          // tp_vectorcall
};


typedef struct
{
    PyObject_HEAD
    BaseTextureImporter * Pointer;
    
} TextureImporter;

static PyMethodDef TextureImporter_methods[] = {
    // { "AddListener",            PyScriptEvent_AddListener,          METH_VARARGS,    nullptr },
    // { "RemoveAllListeners",     PyScriptEvent_RemoveAllListeners,   METH_VARARGS,    nullptr },
    { nullptr, nullptr, 0, nullptr },
};

static PyMemberDef TextureImporter_members[] = {
    { "WindowResizedEvent", T_OBJECT, offsetof(PyGraphicsDriver, WindowResizedEvent), READONLY, nullptr },
    { nullptr, 0, 0, 0, nullptr },
};

static PyTypeObject TextureImporter_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Dusk.TextureImporter",                     // tp_name 
    sizeof(TextureImporter),                    // tp_basicsize
    0,                                          // tp_itemsize
    0,                                          // tp_dealloc
    0,                                          // tp_vectorcall_offset
    0,                                          // tp_getattr
    0,                                          // tp_setattr
    0,                                          // tp_as_async
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
    Py_TPFLAGS_DEFAULT,                         // tp_flags
    "Dusk Script Event",                        // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    TextureImporter_methods,                    // tp_methods
    TextureImporter_members,                    // tp_members
    0,                                          // tp_getset
    0,                                          // tp_base
    0,                                          // tp_dict
    0,                                          // tp_descr_get
    0,                                          // tp_descr_set
    0,                                          // tp_dictoffset
    0,                                          // tp_init
    0,                                          // tp_alloc
    0,                                          // tp_new
    0,                                          // tp_free
    0,                                          // tp_is_gc
    0,                                          // tp_bases
    0,                                          // tp_mro
    0,                                          // tp_cache
    0,                                          // tp_subclasses
    0,                                          // tp_weaklist
    0,                                          // tp_del
    0,                                          // tp_version_tag
    0,                                          // tp_finalize
    0,                                          // tp_vectorcall
};

PyObject * New_TextureImporter(TextureImporter * importer)
{

}

PyObject * TextureImporter_LoadFromFile(PyObject * self, PyObject * args)
{

}

PyObject * TextureImporter_LoadFromMemory(PyObject * self, PyObject * args)
{

}

bool PyInit_TextureImporter(PyObject * module)
{

}

}