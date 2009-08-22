
class Texture {
  private:
    bool valid;
    GLuint textureNumber;
  
  public:
    Texture();
    Texture(int texNum);
    Texture(SDL_Surface* surface);
    Texture(SDL_Surface* surface, GLenum format);
    Texture(const Texture& t);
    ~Texture();
    
    void applyTexture() const;
    static void cancelTextures();
    bool isValid() const;
    GLuint getTextureIndex();
};

Texture texMakeCheckerboard();
Texture texMakeCurveBorder(int width, int height, bool alpha=true);
Texture texLoadTexture(const string& fileName);
Texture texLoadCustomTexture(const string& fileName, GLenum format);

