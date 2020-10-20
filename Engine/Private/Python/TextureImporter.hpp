#ifndef DUSK_PY_TEXTURE_IMPORTER_HPP
#define DUSK_PY_TEXTURE_IMPORTER_HPP

#include <Dusk/Graphics/TextureImporter.hpp>

#include <Python.h>
#include <structmember.h>

namespace Dusk::Python {

class BaseTextureData : public Dusk::TextureData
{
public:

    BaseTextureData(PyObject * self);

    virtual ~BaseTextureData();

    uint8_t * GetData() override;

    uvec2 GetSize() override;

    int GetComponents() override;

    DataType GetDataType() override;

private:

    PyObject * _self;

    Py_buffer _buffer;

};

PyObject * New_TextureData(TextureData * data);

class BaseTextureImporter : public Dusk::TextureImporter
{
public:

    std::unique_ptr<Dusk::TextureData> LoadFromFile(const std::string& filename) override;

    std::unique_ptr<Dusk::TextureData> LoadFromMemory(uint8_t * buffer, size_t length) override;

};

PyObject * New_TextureImporter(TextureImporter * importer);

PyObject * TextureImporter_LoadFromFile(PyObject * self, PyObject * args);

PyObject * TextureImporter_LoadFromMemory(PyObject * self, PyObject * args);

bool PyInit_TextureImporter(PyObject * module);

} // namespace Dusk::Python

#endif // DUSK_PY_TEXTURE_IMPORTER_HPP