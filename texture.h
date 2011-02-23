/* ****************** *
 * The Texture module *
 * ****************** */

/* ********************************* *
 * The Texture class:                *
 *    Handles all texture operations *
 *      after the texture is loaded. *
 * ********************************* */
class Texture {
  private:
    bool valid;
    GLuint textureNumber;
    GLenum textureFormat;
  
  public:
    Texture();
    Texture(int texNum);
    Texture(SDL_Surface* surface);
//    Texture(SDL_Surface* surface, GLenum format);
    Texture(const Texture& t);
    ~Texture();
    
    void applyTexture() const;
    static void cancelTextures();
    bool isValid() const;
    GLuint getTextureIndex() const;
    GLenum getTextureFormat() const;
};

/* ************************************ *
 * Default texture generation functions *
 * ************************************ */
Texture texMakeCheckerboard();
Texture texMakeCurveBorder(int width, int height, bool alpha=true);
Texture texMakeCurveBackground(int width, int height, bool alpha=true);

/* ************************************* *
 * Functions that load images from files *
 * ************************************* */
Texture texLoadTexture(const string& fileName);
//Texture texLoadCustomTexture(const string& fileName, GLenum format);

