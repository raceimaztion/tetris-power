
class Texture {
  private:
    bool valid;
    GLuint textureNumber;
  
  public:
    Texture();
    Texture(SDL_Surface* surface);
    Texture(const Texture& t);
    ~Texture();
    
    void applyTexture() const;
    static void cancelTextures();
    bool isValid() const;
    GLuint getTextureIndex();
};

