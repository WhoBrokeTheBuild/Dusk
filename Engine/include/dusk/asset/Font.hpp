#ifndef DUSK_FONT_HPP
#define DUSK_FONT_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/core/Math.hpp>

#include <memory>
#include <string>
#include <vector>

namespace dusk
{

struct stbtt_fontinfo;

class Font
{
public:

    Font(const std::string& filename, unsigned int size);
    virtual ~Font() = default;

    std::string GetFilename() const { return _filename; }

    unsigned int GetSize() const { return _size; }

    stbtt_fontinfo * GetSTBFontInfo() { return _stbFontInfo.get(); }

    const stbtt_fontinfo * GetSTBFontInfo() const { return _stbFontInfo.get(); }

private:

    std::string _filename;

    unsigned int _size;

    std::vector<std::uint8_t> _buffer;

    std::unique_ptr<stbtt_fontinfo> _stbFontInfo = nullptr;

}; // class Font

class Text
{
public:

    Text(const std::string& text, std::shared_ptr<Font> font, Shader * shader = nullptr);
    virtual ~Text();

    void SetText(const std::string& text);

    void SetBaseTransform(const glm::mat4& baseTransform);

    void SetPosition(const glm::vec3& pos);
    inline glm::vec3 GetPosition() const { return _position; }

    void SetRotation(const glm::vec3& rot);
    inline glm::vec3 GetRotation() const { return _rotation; }

    void SetScale(const glm::vec3& scale);
    inline glm::vec3 GetScale() const { return _scale; }

    void SetColor(const glm::vec4& color);
    inline glm::vec4 GetColor() const { return _color; }

    void Render();

private:

    const int TEXTURE_ID     = 0;
    const int ATTR_ID        = 0;
    const int TEXTURE_WIDTH  = 1024;
    const int TEXTURE_HEIGHT = 1024;

    bool _invalid = true;

    Shader * _shader;

    glm::mat4 _baseTransform;

    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    glm::vec4 _color;

    std::string _text;

    std::shared_ptr<Font> _font;

    int _glTexture;
    int _glVAO;
    int _glVBOs[2];

}; // class Text

} // namespace dusk

#endif // DUSK_FONT_HPP
