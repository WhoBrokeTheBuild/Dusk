import Dusk
import enum

#
# PyObject * enum = PyModule_Get("enum")
# PyObject * Enum = PyDict_Get("Enum")
#
#
#
3

class TextureData:

    class WrapType(enum.Enum):
        Repeat          = 0
        MirroredRepeat  = 1
        ClampToEdge     = 2
        ClampToBorder   = 3
    


# Dusk.TextureData = TextureData
Dusk.TextureData.WrapType.ClampToEdge

class TestTextureImporter(Dusk.TextureImporter):
    
    def LoadFromFile(filename):
        return None

    def LoadFromMemory(buffer, length):
        return None

Dusk.AddTextureImporter(TestTextureImporter())
